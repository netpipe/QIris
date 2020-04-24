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
 * Gaussian header
 * Michael Boyd
 * 18/01/10
 */

#include "gaussian.h"

// peak has a default value specified in gaussian.h
Gaussian::Gaussian(int _dimension, double _peak): Filter(_dimension)
{
	peak = _peak;

	// Scale the constants so that gaussian is always in the same range
	// Uses alpha = dimension * (4sqrt(-ln(1/3)))**-1
	// The gaussian will have the value peak/3 at each of its edges
	// and peak/9 at its corners
	alpha = (_dimension-1) * 0.4770322291;
	beta = alpha;
}

double Gaussian::waveletValue(int rho, int phi)
{

    return peak * exp( - pow(static_cast<double>(rho), 2) / pow(alpha, 2) )
        * exp( - pow(static_cast<double>(phi), 2) / pow(beta, 2) );

}

/* Begin Function */    
void Gaussian::generateFilter(){
	
	int rho;
	int phi;

    for(int i = 0; i < dimension; i++) {
        rho = i - (dimension / 2);
        for(int j = 0; j < dimension; j++) {
            phi = j - (dimension / 2);
            filterarray[i][j] = waveletValue(rho, phi);
        }
    }
}
/* End Function */
