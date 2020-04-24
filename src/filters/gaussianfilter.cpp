/****************************************************************************************
 * Copyright (c) 2010                                                                   *
 * Michael Boyd <mb109@doc.ic.ac.uk>, Dragos Carmaciu <dc2309@doc.ic.ac.uk>,            *
 * Francis Giannaros <kg109@doc.ic.ac.uk>, Thomas Payne <tp1809@doc.ic.ac.uk> and       *
 * William Snell <ws1309@doc.ic.ac.uk>.                                                 *
 *   Students at Imperial College London <http://imperial.ac.uk/computing>              *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#include "gaussianfilter.h"

GaussianFilter::GaussianFilter() : ImageFilter(5) {}

void GaussianFilter::generateFilter() {
    // Discrete approximation to gaussian with sigma 1.4
    filterarray[0][0] = 2; filterarray[0][1] = 4; filterarray[0][2] = 5; filterarray[0][3] = 4; filterarray[0][4] = 2;
    
    filterarray[1][0] = 4; filterarray[1][1] = 9; filterarray[1][2] = 12; filterarray[1][3] = 9; filterarray[1][4] = 4;
    
    filterarray[2][0] = 5; filterarray[2][1] = 12; filterarray[2][2] = 15; filterarray[2][3] = 12; filterarray[2][4] = 5;
    
    filterarray[3][0] = 4; filterarray[3][1] = 9; filterarray[3][2] = 12; filterarray[3][3] = 9; filterarray[3][4] = 4;
    
    filterarray[4][0] = 2; filterarray[4][1] = 4; filterarray[4][2] = 5; filterarray[4][3] = 4; filterarray[4][4] = 2;
}

int GaussianFilter::outputPixel() {
    int sum(0), pixelValue(0);
    for (int i = 0; i < dimension*dimension; i++)
        sum += values[i];
    
    pixelValue = sum / 115;
    
    if (pixelValue > 255)
        pixelValue = 255;
    else if (pixelValue < 0)
        pixelValue = 0;
    
    return pixelValue;
}
