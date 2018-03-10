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
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "config.hpp"

Config::Config(std::string file_path) {
    m_config_file_path = file_path;
}

bool Config::parse(void) {
    using boost::property_tree::ptree;
    std::ifstream f;
    bool is_valid;

    try {
        f.open(m_config_file_path);

        ptree pt;
        read_xml(f, pt);

        m_mapnik_input_path = pt.get<std::string>("naksha.mapnik_input_path");
        const ptree &v = pt.get_child("naksha.database");
        m_db_host = v.get<std::string>("host");
        m_db_name = v.get<std::string>("name");
        m_db_user = v.get<std::string>("user");
        m_db_pass = v.get<std::string>("pass");
        f.close();

        is_valid = (m_mapnik_input_path.length() > 0 &&
            m_db_host.length() > 0 &&
            m_db_name.length() > 0 &&
            m_db_user.length() > 0 &&
            m_db_pass.length() > 0); 
    }
    catch (const std::exception &e) {
        if (f.is_open()) {
            f.close();
        }
        is_valid = false;
    }

    return is_valid;
}

std::string Config::getDBHost(void) { return m_db_host; }
std::string Config::getDBName(void) { return m_db_name; }
std::string Config::getDBUser(void) { return m_db_user; }
std::string Config::getDBPass(void) { return m_db_pass; }
std::string Config::getMapnikInputPath(void) { return m_mapnik_input_path; }

std::string Config::getDBConnectionString(void) {
    return "dbname=" + m_db_name + " user=" + m_db_user + " password=" + m_db_pass + " hostaddr=" + m_db_host;
}


