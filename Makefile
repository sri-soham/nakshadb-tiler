##############################################################################
#
# Copyright (C) 2018 Sridhar
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
##############################################################################
.RECIPEPREFIX = >
CC = g++
CFLAGS = -std=c++11 -Wall
UNAME_P = $(shell uname -p)
ifeq ($(UNAME_P),x86_64)
    CFLAGS += -DBIGINT
endif

config.o : config.cpp config.hpp
>   $(CC) $(CFLAGS) -c config.cpp
google_projection.o : google_projection.cpp google_projection.hpp lat_lng.hpp
>   $(CC) $(CFLAGS) -c google_projection.cpp

layer_data.o : layer_data.hpp layer_data.cpp
>   $(CC) $(CFLAGS) -lpq -lpqxx -c layer_data.cpp

python_grid_utils.o : python_grid_utils.hpp python_grid_utils.cpp
>   $(CC) $(CFLAGS) -DGRID_RENDERER -lmapnik -licuuc -c python_grid_utils.cpp

map_tile.o : map_tile.cpp map_tile.hpp python_grid_utils.hpp python_grid_utils.cpp layer_data.hpp layer_data.cpp
>   $(CC) $(CFLAGS) -DGRID_RENDERER -lmapnik -lpng -lcairo -lfreetype -licuuc -c map_tile.cpp

tile_request.o : tile_request.cpp tile_request.hpp
>   $(CC) $(CFLAGS) -DGRID_RENDERER -c tile_request.cpp

handle_req.o : config.hpp config.cpp tile_request.hpp tile_request.cpp map_tile.hpp map_tile.cpp handle_req.cpp
>   $(CC) $(CFLAGS) -DGRID_RENDERER -c handle_req.cpp

all : config.o google_projection.o layer_data.o map_tile.o python_grid_utils.o tile_request.o handle_req.o
>   $(CC) $(CFLAGS) -DGRID_RENDERER config.o google_projection.o layer_data.o python_grid_utils.o map_tile.o tile_request.o handle_req.o -lmapnik -lpng -lcairo -lfreetype -licuuc -lpq -lpqxx -lfcgi -lfcgi++ -o naksha_tiler.fcgi

clean:
>   rm -f *.o naksha_tiler.fcgi
