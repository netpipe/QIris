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

/*
 * applies a sinusoidal filter to one pixel
 */
#include "gabortopixel.h"
#include <cstdlib>

bool gaborPixel(int rho, int phi, Sinusoidal & sinusoidalFilter, QImage * rawImage, bool ** mask) {

	// size of the filter to be applied
	int filterSize = sinusoidalFilter.getDimension();
	
	// running total used for integration
	double runningTotal = 0.0;

	// translated co-ords within image
	int imageX;
	int imageY;
	int angles = rawImage->width();

	for(int i = 0; i < filterSize; ++i) {
		for(int j = 0; j < filterSize; ++j) {

			// Actual angular position within the image
			imageY = j + phi - (filterSize/2);

			// Allow filters to loop around the image in the angular direction
			imageY %= angles;
			if(imageY < 0 )
				imageY += angles;

			// Actual radial position within the image
			imageX = i + rho - (filterSize/2);

			// If the bit is good then apply the filter and add this to the sum
			if(mask[imageX][imageY]) {
				runningTotal += (sinusoidalFilter.getFilter(i,j)) *
					static_cast<double>
					(rawImage->pixelIndex(imageY,imageX));
			}
		}
	}

	// Return true if +ve and false if -ve
	return (runningTotal >= 0.0) ? true: false;
}
