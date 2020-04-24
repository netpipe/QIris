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

/* Implementation File for IrisCode Class
    Author: William Snell
    Date: 18/01/10
    */

#include <iostream>
#include <cmath>
#include "../../include/iriscode.h"

using namespace std;
/* Original constructor (Now removed since it is identical to constructing with zero shift 
IrisCode::IrisCode(bool boolCode[], bool boolMask[], int shift){
	for(int i=0; i < 64; i++){
		code[i] = 0;
		mask[i] = 0;
	}
	
	for(int i=0; i < 2048; i++){
			if(boolCode[i])
				code[i/32] += pow(2,(31 - i % 32));
			if(boolMask[i])
				mask[i/32] += pow(2,(31 - i % 32));
	}
}
*/

IrisCode::IrisCode(bool boolCode[], bool boolMask[], int shift){
	while(shift < 0 || 2047 < shift) {
		if(shift < 0)
			shift += 2048;
		if(shift > 2047)
			shift -= 2048;
	}

	for(int i=0; i < 64; i++){
		code[i] = 0;
		mask[i] = 0;
	}
	
	for(int i=0; i < 2048; i++){
			if(boolCode[i])
				code[((i+shift)/32)%64] += static_cast<Bitcode32> (pow(2.0,(31 - (i+shift) % 32)));
			if(boolMask[i])
				mask[((i+shift)/32)%64] += static_cast<Bitcode32>(pow(2.0,(31 - (i+shift) % 32)));
	}
}



IrisCode::IrisCode(Bitcode32 _code[64], Bitcode32 _mask[64]) {
	for(int i=0; i<64; ++i) {
			code[i] = _code[i];
			mask[i] = _mask[i];
	}
}

double IrisCode::compare_with(IrisCode* other){

	Bitcode32 temp[64];
	int top=0;
	int bottom=0;

	for(int i=0; i<64; i++)
		temp[i] = code[i] ^ other->getCode(i);

	for(int i=0; i<64; i++)
		temp[i] = temp[i] & mask[i];

	for(int i=0; i<64; i++)
		temp[i] = temp[i] & other->getMask(i);

	for(int i=0; i<64; i++){
	    for(int j=0; j<32; j++){
    		if(temp[i]&0x01)
    			++top;
    	      temp[i]>>=1;
    	}
	}

	for(int i=0; i<64; i++)
		temp[i] = mask[i] & other->getMask(i);
	
	for(int i=0; i<64; i++){
        for(int j=0; j<32; j++){
    		if(temp[i]&0x01)
	    		++bottom;
	          temp[i]>>=1;
	    }
	}
	
	/* In the case (included in test2.cpp) where number of useful bits is very low (192 in this case), top can equal zero. Due to the way the HD is calculated this gives a false positive. The following deals with cases of top = 0 or bottom =0 
	
We note that we may wish to form a better formula, talk to Duncan about this */
	
	/* debugging */
	//cout << "\nTop = " << top << endl;
	//cout << "Bottom = " << bottom << endl << endl;

   if(bottom == 0){
        cout << "ERROR! NUMBER USEFUL BITS WAS ZERO!" << endl;
        return 1;
        }
        
	 if(top == 0){
        cout << "NUMBER OF DIFFERENCES WERE ZERO!" << endl;
        return (static_cast<double>(2048) - bottom)/2048;
        }	 
        
	return static_cast<double>(top)/bottom;
}
/* End Function */


Bitcode32 IrisCode::getCode(int i){
    return code[i];
}
/* End Function */


Bitcode32 IrisCode::getMask(int i){
    return mask[i];
}	
/* End Function */

void IrisCode::print(){
	cout << "Code: ";
	for(int i=0; i < 64; i++)
		cout << code[i] << " ";
	cout << endl;
	cout << "Mask: ";
	for(int i=0; i < 64; i++)
		cout << mask[i] << " ";
	cout << endl;
}
		

