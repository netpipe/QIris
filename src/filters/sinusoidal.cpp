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

#include "sinusoidal.h"
#include "gaussian.h"

Sinusoidal::Sinusoidal(int dimension) : Filter(dimension){
}


void Sinusoidal::generateFilter(){
	
	int phi;
    double sum=0;
    
        for(int j = 0; j < dimension; j++) {
            phi = j - (dimension / 2);
            filterarray[0][j] = waveletValue(phi, dimension);
            sum+=filterarray[0][j];
        }
        for(int j = 0; j < dimension; j++){
            filterarray[0][j]-=(sum / dimension);    
        }
        for(int i = 1; i < dimension; i++){
            for(int j = 0; j < dimension; j++){
                filterarray[i][j] = filterarray[0][j];
            }
		}
		
    Gaussian temp(dimension);
    temp.generateFilter();
    multiplyBy(temp);


	// make every row have equal +ve and -ve
	
	for(int i = 0; i < dimension; ++i) {

		double rowSum = 0.0;

		for(int j = 0; j < dimension; ++j) {
			rowSum += filterarray[i][j];
		}

		for(int j = 0; j < dimension; ++j) {
			filterarray[i][j] -= rowSum / static_cast<double>(dimension);
		}
	}
}
