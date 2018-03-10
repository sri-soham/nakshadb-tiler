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
#include <vector>
#include "tile_request.hpp"

const std::string TileRequest::TYPE_LAYER = "lyr";
const std::string TileRequest::TYPE_VISUALIZATION = "vis";

TileRequest::TileRequest(std::string uri) {
    m_uri = uri;
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_ext = "";
    m_callback = "";
    m_is_valid = false;
    m_hash = "";
    m_update_hash = "";
}

void TileRequest::parseUri() {
    std::string last;
    std::string tmp;

    std::vector<std::string> parts1 = split(m_uri, "/");
    // There should be at least 5 parts when the uri is split at /
    if (parts1.size() != 6) {
        m_error = "parts1.size is not equal to 6";
        return;
    }

    last = parts1.back();
    parts1.pop_back();

    std::vector<std::string> parts2 = split(last, "?");
    if (parts2.size() == 2) {
        tmp = parts2.back();
        last = parts2.front();

        std::vector<std::string> parts3 = split(tmp, "&");
        for (auto const& qsp : parts3) {
            std::vector<std::string> parts4 = split(qsp, "=");
            if (parts4.size() == 2 && parts4.front() == "callback") {
                m_callback = parts4.back();
                break;
            }
        }
    }
    else {
        last = parts2.front();
    }

    parts2 = split(last, ".");
    // there should be only 2 parts when the last part is split at "."
    if (parts2.size() != 2) {
        m_error = "parts2.size is less than 2";
        return;
    }

    m_ext = parts2.back();
    parts2.pop_back();

    try {
        m_y = std::stol(parts2.back());
        // only positive values should be there in the url
        if (m_y < 0) {
            m_error = "m_y is less than 0";
            return;
        }
    }
    catch (std::invalid_argument ex) {
        m_error = "invalid value for m_y";
        return;
    }

    try {
        m_x = std::stol(parts1.back());
        if (m_x <  0) {
            m_error = "m_x is less than 0";
            return;
        }
    }
    catch (std::invalid_argument ex) {
        m_error = "invalid value for m_x";
        return;
    }

    parts1.pop_back();
    try {
        m_z = std::stol(parts1.back());
        if (m_z < 0) {
            m_error = "m_z is less than 0";
            return;
        }
    }
    catch (std::invalid_argument ex) {
        m_error = "invalid value for m_z";
        return;
    }

    parts1.pop_back();
    tmp = parts1.back();
    parts2 = split(tmp, "-");
    if (parts2.size() != 2) {
        m_error = "Invalid hash value";
        return;
    }
    m_update_hash = parts2.back();
    m_hash = parts2.front();

    parts1.pop_back();
    m_type = parts1.back();
    if (m_type != TYPE_LAYER) {
        m_error = "invalid type: should be lyr";
        return;
    }

    m_is_valid = true;
}

bool TileRequest::isValid(void) { return m_is_valid; }
bool TileRequest::isJson(void) { return (m_ext.compare("json") == 0); }
bool TileRequest::isLayerType(void) { return m_type == TYPE_LAYER; }
long int TileRequest::getX(void) { return m_x; }
long int TileRequest::getY(void) { return m_y; }
long int TileRequest::getZ(void) { return m_z; }
std::string TileRequest::getExt(void) { return m_ext; }
std::string TileRequest::getCallback(void) { return m_callback; }
std::string TileRequest::getHash(void) { return m_hash; }
std::string TileRequest::getUpdateHash(void) { return m_update_hash; }
std::string TileRequest::getError(void) { return m_error; }

std::vector<std::string> TileRequest::split(const std::string &s, const std::string delim) {
    std::vector<std::string> elems;
    std::string ss, last;
    int length, prev, diff, count;
    size_t pos;
    
    length = s.length();
    if (length == 0) {
        return elems;
    }
    prev = 0;
    count = 0;

    while (prev < length) {
        ++count;
        pos = s.find(delim, prev);
        //if (pos < 0) { // Delimiter not found
        if (std::string::npos == pos) {
            ss = s.substr(prev);
            prev = length;
        }
        else {
            diff = pos - prev;
            ss = s.substr(prev, diff);
            prev = pos + 1;
        }
        elems.push_back(ss);
        if (count >= 10) break;
    }
    last = s.substr(length - delim.length());
    if (last.compare(delim) == 0) {
        elems.push_back("");
    }

    return elems;
}

