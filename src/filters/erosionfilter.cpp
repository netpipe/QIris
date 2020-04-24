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

#include "erosionfilter.h"

ErosionFilter::ErosionFilter(int _dimension) : ImageFilter(_dimension) {}

void ErosionFilter::generateFilter() {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            filterarray[i][j] = 1;
        }
    }
}

int ErosionFilter::outputPixel() {
    bool match = false;
    for (int i = 0; i < dimension*dimension; i++) {
        if (values[i] != 0) {
            // if any of the matrix values are not
            // black, i.e. white, erode matrix
            match = true;
        }
    }
    return match ? 255 : 0;
}
