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

#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include "filter.h"

class ImageFilter : public Filter {
public:
    ImageFilter(int _dimension);
    virtual ~ImageFilter();
    virtual void generateFilter() = 0;
    // outputPixel() varies depending on the filter type, for
    // example, a median filter will sort and return the median.
    // A gaussian will average and return that etc.
    virtual int outputPixel() = 0;    
    // addValue() takes an x, y location on the filter matrix and
    // a pixel value from the ORIGINAL image and adds the multiple
    // of the pixelValue and its own x,y value to the appropriate
    // index in the 1d values array.
    void addValue(int x, int y, int pixelValue);
protected:
    // int* values dimensional version of the array which stores
    // the values in a form which can be easily sorted, summed and
    // averaged as necessary.
    int* values;
};

#endif // IMAGEFILTER_H
