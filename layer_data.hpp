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
#ifndef LAYER_DATA_HPP
#define LAYER_DATA_HPP

#include <pqxx/pqxx>
#include <map>

using namespace std;

class LayerData {
    private:
        string m_conn_str;
        string m_hash;
        string m_table;
        string m_geometry_column;
        string m_query;
        string m_style;
        string m_update_hash;
        bool   m_is_valid;

        void parseInfoDetails(string);

    public:
        LayerData(void);
        LayerData(string db_conn_str, string hash);
        void fetchData(void);
        string getTable(void);
        string getGeometryColumn(void);
        string getQuery(void);
        string getStyle(void);
        vector<string> getInfoFields();
        bool isValid(string update_hash);
};

#endif
