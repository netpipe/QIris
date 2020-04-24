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

/* TEST FILE FOR SINE AND COSINE FILTERS 

PLEASE DO NOT MODIFY */

#include "gaussian.h"
#include "filter.h"
#include "sinusoidal.h"
#include "coswavelet.h"
#include "sinwavelet.h"
#include "imagefilters.h"
#include "sobelfilter.h"

int main(){
/*
    SinWavelet first(3);
    first.generateFilter();
    first.print();
    SinWavelet second(7);
    second.generateFilter();
    second.print();
 
    CosWavelet first1(3);
    first1.generateFilter();
    first1.print();
    CosWavelet second1(7);
    second1.generateFilter();
    second1.print();
 */
    Gaussian first2(5);
    first2.generateFilter();
    first2.print();
    Gaussian second2(7);
    second2.generateFilter();
    second2.print();
   
    
    return 0;
}
