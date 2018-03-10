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
#ifndef CONFIG_H
#define CONFIG_H

class Config {
    private:
        std::string m_db_host;
        std::string m_db_name;
        std::string m_db_user;
        std::string m_db_pass;
        std::string m_mapnik_input_path;
        std::string m_config_file_path;

    public:
        Config(std::string file_path);
        std::string getDBHost(void);
        std::string getDBName(void);
        std::string getDBUser(void);
        std::string getDBPass(void);
        std::string getDBConnectionString(void);
        std::string getMapnikInputPath(void);
        bool parse(void);
};

#endif
