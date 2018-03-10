/*****************************************************************************
 *
 * Copyright (C) 2018 Sridhar
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/
#include <iostream>
#include <sstream>

#include <mapnik/map.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/rule.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/text/placements/dummy.hpp>
#include <mapnik/text/text_properties.hpp>
#include <mapnik/text/formatting/text.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/expression.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/unicode.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/cairo_io.hpp>
#include <mapnik/projection.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/grid/grid.hpp>
#include <mapnik/grid/grid_renderer.hpp>
#include <boost/algorithm/string/replace.hpp>

#if defined(HAVE_CAIRO)
#include <mapnik/cairo/cairo_renderer.hpp>
#include <mapnik/cairo/cairo_image_util.hpp>
#endif

#include "google_projection.hpp"
#include "lat_lng.hpp"
#include "map_tile.hpp"
#include "python_grid_utils.hpp"
#include "layer_data.hpp"

using namespace mapnik;

const std::string MapTile::SRS_MERC = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0.0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs +over";

const std::string MapTile::MAP_STYLE_XML = R"V0G0N(
<Map>
  <Style name="{table}">
    {rules}
  </Style>
</Map>
)V0G0N";

MapTile::MapTile(std::string db_host, std::string db_user, std::string db_pass, std::string db_name, LayerData layer_data)
{
    m_db_host = db_host;
    m_db_user = db_user;
    m_db_pass = db_pass;
    m_db_name = db_name;
    m_layer_data = layer_data;
}
void MapTile::genTile(int x, int y, int z)
{
    setMap();
    box2d<double> box = getBox(x, y, z);
    m_map.zoom_to_box(box);

    image_rgba8 buf(m_map.width(), m_map.height());
    agg_renderer<image_rgba8> ren(m_map, buf);
    ren.apply();
    std::cout << "Content-type: image/png" << std::endl << std::endl;
    save_to_stream(buf, std::cout, "png");
}

void MapTile::genGrid(int x, int y, int z, std::string callback)
{
    try {
        setMap();
        std::vector<std::string> fields;
        box2d<double> box = getBox(x, y, z);
        m_map.zoom_to_box(box);
        std::string known_id_key = "naksha_id";
        mapnik::grid my_grid(m_map.width(), m_map.height(), known_id_key);

        // 2017-09-09:
        // Info fields are *not* included in the data.data of the json grid,
        // because timestamp columns are coming up empty. getInfoFields returns
        // only the 'naksha_id', which also happens to be the key. Additional
        // pay load with no additional benefit. Hence, passing empty vector in
        // place of fields.
        // render_layer_for_grid(m_map, my_grid, 0, m_layer_data.getInfoFields(), 1.0, 0, 0);

        render_layer_for_grid(m_map, my_grid, 0, fields, 1.0, 0, 0);

        grid_json_data json = grid_encode<mapnik::grid>(my_grid, "utf", true, 4);
        std::string json_str = get_json_string(json);
        if (callback.length() > 0) {
            json_str = callback + "(" + json_str + ")";
        }
        std::cout << "Content-type: application/json" << std::endl << std::endl;
        std::cout << json_str;
    }
    catch (const exception &e) {
        std::cerr << "genGrid exception: " << e.what() << std::endl;
    }
}

void MapTile::setMap(void)
{
    m_map = Map(TILE_WIDTH, TILE_HEIGHT);
    m_map.set_srs(SRS_MERC);
    setStyles();
    addLayer();
}

void MapTile::setStyles(void)
{
    m_map.set_background(color(255, 255, 255, 0));
    std::string style_rules = m_layer_data.getStyle();
    
    if (style_rules.length() > 0) {
        std::string style_str = std::string(MAP_STYLE_XML);
        boost::algorithm::replace_all(style_str, "{table}", m_layer_data.getTable());
        boost::algorithm::replace_all(style_str, "{rules}", style_rules);

        load_map_string(m_map, style_str, true, "");
    }
}

box2d<double> MapTile::getBox(int x, int y, int z)
{
    GoogleProjection gprj;
    LatLng i0, i1, p0, p1;
    i0.x = TILE_WIDTH*x;
    i0.y = TILE_HEIGHT*(y+1);
    i1.x = TILE_WIDTH*(x+1);
    i1.y = TILE_HEIGHT*y;

    p0 = gprj.fromPixelToLL(i0, z);
    p1 = gprj.fromPixelToLL(i1, z);
    
    projection proj = projection(SRS_MERC, false);
    proj.forward(p0.x, p0.y);
    proj.forward(p1.x, p1.y);

    box2d<double> res = box2d<double>(p0.x, p0.y, p1.x, p1.y);

    return res;
}

void MapTile::addLayer(void)
{
    const std::string table = m_layer_data.getTable();
    parameters p;
    p["type"] = "postgis";
    p["host"] = m_db_host;
    p["user"] = m_db_user;
    p["password"] = m_db_pass;
    p["dbname"] = m_db_name;
    p["table"] = "(" + m_layer_data.getQuery() + ") AS " + table;
    p["geometry_field"] = m_layer_data.getGeometryColumn();
    layer lyr(table);
    lyr.set_datasource(datasource_cache::instance().create(p));
    // Value of parameter table should match the 'name' attribute of the style tag.
    lyr.add_style(table);
    lyr.set_srs(SRS_MERC);
    m_map.add_layer(lyr);
}

