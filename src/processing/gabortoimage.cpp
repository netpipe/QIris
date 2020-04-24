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
 *  
 */

#include "gabortoimage.h"

void gaborImage(QImage * rawImage, bool ** mask, bool bitCode[2048], bool bitCodeMask[2048]) {

	int width = rawImage->width();		// theta direction
	int height = rawImage->height();	// r direction

	// Number of slices image is cut up into. Ideally angular slices should divide
	// 360, and size of bitCode without a remainder. More importantly, their product should
	// be divisible by 32
	int angularSlices = 256;
	int radialSlices = 1024 / angularSlices;

	// maximum filter size - set to 1/3 of image height to avoid large, uninformative filters
	int maxFilter = height / 3;
	
	int filterHeight;
	
	// tracks the position which needs to be modified in the bitcode and bitcodemask
	int bitCodePosition = 0;

	SinWavelet * pSine = NULL;
	CosWavelet * pCosine = NULL;

	for(int aSlice = 0; aSlice < angularSlices; ++aSlice) {

		int theta = aSlice;
		
		for(int rSlice = 0; rSlice < radialSlices; ++rSlice) { 
		
			// Works out which pixel in the image to apply the filter to
			// Uniformly positions the centres of the filters between radius=3 and radius=height/2
			// Does not consider putting a filter centre at less than radius=3, to avoid tiny filters
			int radius = ((rSlice * (height - 6)) / (2*radialSlices)) + 3 ;

			// Set filter dimension to the largest filter that fits in the image
			filterHeight = (radius < (height - radius)) ? 2*radius-1: 2*(height - radius)-1;

			// If the filter size exceeds the width of the image then correct this
			if(filterHeight > width - 1) 
				filterHeight = width - 1;

			// If the filter size exceeds the maximum size specified earlier then correct this
			if(filterHeight > maxFilter)
				filterHeight = maxFilter;

			pSine = new SinWavelet(filterHeight);
			pCosine = new CosWavelet(filterHeight);

			pSine->generateFilter();
			pCosine->generateFilter();

			// Apply the filters to the calculated pixel in the image and set bitCode accordingly
			bitCode[bitCodePosition] = gaborPixel(radius,theta,*pCosine,rawImage,mask);
			bitCode[bitCodePosition + 1] = gaborPixel(radius,theta,*pSine,rawImage,mask);

			if(pSine) { 
				delete pSine;
				pSine = NULL;
			}
			if(pCosine) {
				delete pCosine;
				pCosine = NULL;
			}

			// Check whether the pixel itself is bad
			if(mask[radius][theta])
				bitCodeMask[bitCodePosition] = true;
			else 
				bitCodeMask[bitCodePosition] = false;

			// Check whether a filter is good or bad
			if(!checkFilter(radius,theta,filterHeight,mask))
				bitCodeMask[bitCodePosition] = false;

			// We're assuming that pairs of bits in the bitCodeMask are equal
			bitCodeMask[bitCodePosition + 1] = bitCodeMask[bitCodePosition];

			bitCodePosition+=2;
		}
	}

	// Make sure all the pointers are being good
	if(pSine) { 
		delete pSine;
		pSine = NULL;
	}

	if(pCosine) {
		delete pCosine;
		pCosine = NULL;
	}

}

// Checks if a filter is "good" or not
bool checkFilter(int radius, int theta, int filterHeight, bool ** mask) {

	int sum = 0;
	double goodRatio = 0.5; //ratio of good bits in a good filter
	double ratio;

	// Check the mask of all pixels within the range of the filter
	for(int rPos = radius - (filterHeight/2); rPos <= radius + (filterHeight/2); ++rPos)
		for(int tPos = theta - (filterHeight/2); tPos <= theta + (filterHeight/2); ++tPos)  
			sum += mask[rPos][tPos];

	ratio = static_cast<double>(sum) / static_cast<double>(filterHeight*filterHeight);

	// If the ratio of good pixels to total pixels in the filter is good, return true
	return (ratio >= goodRatio) ? true : false ;

}
