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

/*	Implementation File for Filter Class
	Author: William Snell
	Date: 18/01/10  
*/
#include <iostream>
#include <iomanip>
#include "filter.h"

using namespace std;

Filter::Filter(int _dimension){
	dimension = _dimension;
	
	filterarray = new double *[dimension];
	
	// Construct Array
	for(int i = 0; i < dimension; i++)
		filterarray[i]=new double [dimension];
}

double Filter::getFilter(int row, int col) const{
    return filterarray[row][col];
}

void Filter::print(){
    for(int i = 0; i < dimension; i++) {
        for(int j = 0; j < dimension; j++) {
            cout << setw(15) << filterarray[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

Filter::~Filter(){
    for(int i=0; i < dimension; i++)
        delete [] filterarray[i];
    delete [] filterarray;
}

int Filter::getDimension() const{
    return dimension;
}

void Filter::multiplyBy(const Filter &otherFilter){
    if(dimension != otherFilter.getDimension())
        cout << "ERROR! You cannot multiply two filters which are not the same size!\nThe multiplication was not performed." << endl;
    for(int i = 0; i < dimension; i++) {
        for(int j = 0; j < dimension; j++) {
            filterarray[i][j] *= otherFilter.getFilter(i, j);
        }
    }
}
    
        
        
        
        
        
        
        
        
        
        
        
        

