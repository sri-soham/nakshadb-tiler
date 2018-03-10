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
#include <math.h>
#include <iostream>

#include "lat_lng.hpp"
#include "google_projection.hpp"

GoogleProjection::GoogleProjection(void) {
    int i, c;
    double e;

    c = 256;
    for (i=0; i <SIZE; ++i) {
        e = c/2;
        m_Bc[i] = c/360.0;
        m_Cc[i] = c/(2*M_PI);
        m_zc[i] = {e, e};
        m_Ac[i] = c;
        c *= 2;
    }
}

double GoogleProjection::minMax(double a, double b, double c)
{
    a = std::max(a, b);
    a = std::min(a, c);

    return a;
}

double GoogleProjection::degreesToRadians(double degrees)
{
    return (degrees * M_PI) / 180;
}

double GoogleProjection::radiansToDegrees(double radians)
{
    return (radians * 180) / M_PI;
}

LatLng GoogleProjection::fromLLToPixel(LatLng ll, int zoom)
{
    LatLng d, r;
    double e, f, g;

    d = m_zc[zoom];
    e = round(d.x * ll.x * m_Bc[zoom]);
    f = minMax(sin(degreesToRadians(ll.y)), -0.9999, 0.9999);
    g = round(d.y + 0.5*log((1+f)/(1-f)) * -m_Cc[zoom]);

    r.x = e;
    r.y = g;

    return r;
}

LatLng GoogleProjection::fromPixelToLL(LatLng px, int zoom)
{
    double f, g, h;
    LatLng e, r;

    e = m_zc[zoom];
    f = (px.x - e.x) / m_Bc[zoom];
    g = (px.y - e.y) / -m_Cc[zoom];
    h = radiansToDegrees(2 * atan(exp(g)) - 0.5 * M_PI);

    r.x = f;
    r.y = h;

    return r;
}

