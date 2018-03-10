/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2015 Artem Pavlenko
 * Modified work Copyright (C) 2018 Sridhar
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
#ifndef MAPNIK_PYTHON_BINDING_GRID_UTILS_INCLUDED
#define MAPNIK_PYTHON_BINDING_GRID_UTILS_INCLUDED

// mapnik
#include <map>
#include <mapnik/map.hpp>
#include <mapnik/grid/grid.hpp>

using grid_feature_data = std::map<std::string, std::map<std::string, std::string>>;
using grid_json_data = struct gsd {
    std::vector<std::string> keys;
    grid_feature_data data;
    std::vector<std::string> grid;
};

namespace mapnik {


template <typename T>
void grid2utf(T const& grid_type,
                     std::vector<std::string>& l,
                     std::vector<typename T::lookup_type>& key_order,
                     unsigned int resolution);


template <typename T>
void write_features(T const& grid_type,
                           grid_feature_data& feature_data,
                           std::vector<typename T::lookup_type> const& key_order);

template <typename T>
void grid_encode_utf(T const& grid_type,
                            grid_json_data & json,
                            bool add_features,
                            unsigned int resolution);

template <typename T>
grid_json_data grid_encode( T const& grid, std::string const& format, bool add_features, unsigned int resolution);

void render_layer_for_grid(const mapnik::Map& map,
                           mapnik::grid& grid,
                           unsigned layer_idx, // TODO - layer by name or index
                           std::vector<std::string> const& fields,
                           double scale_factor,
                           unsigned offset_x,
                           unsigned offset_y);


std::string get_json_string(grid_json_data json);
}
#endif // MAPNIK_PYTHON_BINDING_GRID_UTILS_INCLUDED
