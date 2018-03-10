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
#ifndef GOOGLE_PROJECTION_H
#define GOOGLE_PROJECTION_H

#include "lat_lng.hpp"
class GoogleProjection {
    public:
        static const int SIZE = 20;
        GoogleProjection(void);
        LatLng fromLLToPixel(LatLng ll, int zoom);
        LatLng fromPixelToLL(LatLng px, int zoom);

    private:
        double m_Bc[SIZE];
        double m_Cc[SIZE];
        LatLng m_zc[SIZE];
        double m_Ac[SIZE];
        double minMax(double a, double b, double c);
        double degreesToRadians(double);
        double radiansToDegrees(double);
};

#endif
