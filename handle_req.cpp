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

#include "fcgio.h"
#include <mapnik/datasource_cache.hpp>

#include "tile_request.hpp"
#include "map_tile.hpp"
#include "layer_data.hpp"
#include "config.hpp"

int main(int argc, char** argv)
{
    using namespace std;

    Config config = Config(argv[1]);
    if (!config.parse()) {
        cout << "config file parsing failed\n";
        return 1;
    }

    // backup stdio streambufs
    streambuf * cin_streambuf = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    mapnik::datasource_cache::instance().register_datasources(config.getMapnikInputPath());

    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        const char * uri = FCGX_GetParam("REQUEST_URI", request.envp);

        TileRequest tr = TileRequest(uri);
        tr.parseUri();
        if (tr.isValid()) {
            LayerData layer_data = LayerData(config.getDBConnectionString(), tr.getHash());
            layer_data.fetchData();
            if (layer_data.isValid(tr.getUpdateHash())) {
                MapTile map_tile = MapTile(
                    config.getDBHost(), config.getDBUser(), config.getDBPass(), config.getDBName(), layer_data
                );
                if (tr.isJson()) {
                    map_tile.genGrid(tr.getX(), tr.getY(), tr.getZ(), tr.getCallback());
                }
                else {
                    map_tile.genTile(tr.getX(), tr.getY(), tr.getZ());
                }
            }
            else {
                cout << "Content-Type: text/plain" << endl << endl;
                cout << "Invalid request: no ruch resource\n";
            }
        }
        else {
            cout << "Content-Type: text/plain" << endl << endl;
            cout << "invalid request\n";
        }
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}

