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
#ifndef TILE_REQUEST_H
#define TILE_REQUEST_H

#include <vector>
class TileRequest {
    private:
        long int m_x;
        long int m_y;
        long int m_z;
        std::string m_ext;
        std::string m_callback;
        bool m_is_valid;
        std::string m_uri;
        std::string m_hash;
        std::string m_update_hash;
        std::string m_error;
        std::string m_type;
        std::vector<std::string> split(const std::string &s, const std::string delim);

    public:
        TileRequest(std::string);
        static const std::string TYPE_LAYER;
        static const std::string TYPE_VISUALIZATION;
        void parseUri(void);
        bool isValid(void);
        long int getX(void);
        long int getY(void);
        long int getZ(void);
        std::string getExt(void);
        std::string getCallback(void);
        std::string getHash(void);
        std::string getUpdateHash(void);
        std::string getError(void);
        bool isJson(void);
        bool isLayerType(void);
};

#endif
