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
#ifndef MAP_TILE_H
#define MAP_TILE_H

#include <iostream>
#include <mapnik/map.hpp>
#include "layer_data.hpp"

class MapTile {
    private:
        int m_max_zoom;
        std::string m_table;
        mapnik::Map m_map;
        std::string m_db_host;
        std::string m_db_user;
        std::string m_db_pass;
        std::string m_db_name;
        LayerData   m_layer_data;

        void setMap(void);
        void setStyles(void);
        mapnik::box2d<double> getBox(int x, int y, int z);
        void addLayer(void);

    public:
        static const int TILE_WIDTH = 256;
        static const int TILE_HEIGHT = 256;
        static const std::string SRS_MERC;
        static const std::string MAP_STYLE_XML;
        MapTile(std::string db_host, std::string db_user, std::string db_pass, std::string db_name, LayerData layer_data);
        void genTile(int x, int y, int z);
        void genGrid(int x, int y, int z, std::string callback);

};

#endif
