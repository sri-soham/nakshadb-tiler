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
#include <map>
#include <sstream>
#include <pqxx/pqxx>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>
#include "layer_data.hpp"

using namespace std;
using namespace pqxx;

LayerData::LayerData(void)
{
    m_conn_str = "";
    m_hash = "";
    m_update_hash = "";
}

LayerData::LayerData(string db_conn_str, string hash)
{
    m_conn_str = db_conn_str;
    m_hash = hash;
    m_update_hash = "";
}

void LayerData::fetchData(void)
{
    try {
        connection conn(m_conn_str);
        string pst_name = "layer_data";
        string sql = R"V0G0N(
    SELECT ml.id
         , mt.table_name
         , ml.geometry_column
         , ml.query
         , ml.update_hash
         , ml.style
         , ml.options
      FROM mstr_layer AS ml
INNER JOIN mstr_table AS mt
        ON ml.table_id = mt.id
     WHERE ml.hash = $1
)V0G0N";
        work w(conn);
        conn.prepare(pst_name, sql);
        result r = w.prepared(pst_name)(m_hash).exec();
        if (r.size() == 1) {
            m_is_valid = true;
            result::const_iterator row = r[0];
            m_table = row[1].as<string>();
            m_geometry_column = row[2].as<string>();
            m_query = row[3].as<string>();
            m_update_hash = row[4].as<string>();
            m_style = row[5].as<string>();
        }
        else {
            m_is_valid = false;
        }
        w.commit();
    }
    catch (const exception &e) {
        cerr << e.what() << endl;
    }
}

string LayerData::getTable(void)
{
    return m_table;
}

string LayerData::getGeometryColumn(void)
{
    return m_geometry_column;
}

string LayerData::getQuery(void)
{
    return m_query;
}

string LayerData::getStyle(void)
{
    return m_style;
}

/*
2017-07-29:
Not being used as the infowindow fields are fetched through query.
UtfGrid is requested on demand, not fetched by default.
We could have possibly included all the fields in the utfgrid but some
data types like timestamp are being returned as empty strings.
So, just naksha_id id enough which makes the parsing of infodetails
field unnecessary.
*/
void LayerData::parseInfoDetails(string infowindow)
{
    if (infowindow.length() == 0) {
        return;
    }

    string info_template;
    vector<string> info_fields;
    boost::property_tree::ptree root;
    stringstream ss;

    ss << infowindow;
    boost::property_tree::read_json(ss, root);
    boost::optional<boost::property_tree::ptree&> child = root.get_child_optional("template");
    boost::property_tree::ptree::const_assoc_iterator it = root.find("template");
    if (it != root.not_found())
    {
        info_template = root.get<string>("template");
    }
    for (boost::property_tree::ptree::value_type &field : root.get_child("fields"))
    {
        info_fields.push_back(field.second.data());
    }
}

vector<string> LayerData::getInfoFields()
{
    vector<string> fields;
    fields.push_back("naksha_id");

    return fields;
}

bool LayerData::isValid(string update_hash)
{
    return m_is_valid;
}

