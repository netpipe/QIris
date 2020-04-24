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

#ifndef IRISCODE_H
#define IRISCODE_H

typedef	unsigned long int Bitcode32;

class IrisCode {
	private:
	Bitcode32 code[64];
	Bitcode32 mask[64];

	public:
	/*!
        * Constructor which constructs an iris code for a given shift. 
        * 
        * @param boolCode Iris bitcode as a 2048 bool array.
        * @param boolMask Iris bitmask as a 2048 bool array.
        * @param shift Rotational shift.
        */
		IrisCode(bool _code[], bool _mask[], int shift);
	/*!
		* Constuctor which constructs iris codes from the database.
		*
		* @param _code Iris bitcode as an array of 64 32-bit integers.
		* @param _mask Iris bitmask as an array of 64 32-bit integers.
		*/
		IrisCode(Bitcode32 _code[64], Bitcode32 _mask[64]);
	
	/*!
		* Function which compares two iris codes utilizing the full potential of a 32-bit architecture. 
		*
		* @param other Iris code to compare with.
		* @return Hamming distance between the two.
		*/
		double compare_with(IrisCode* other);
		
		
	/*!
		* Get a 32 bit segment of the iris bitcode
		*
		* @param i Number of segment.
		* @return A 32 bit bitcode.
		*/
		Bitcode32 getCode(int i);
		
	/*!
		* Get a 32 bit segment of the iris bitmask
		*
		* @param i Number of segment.
		* @return A 32 bit bitmask.
		*/
		Bitcode32 getMask(int i);
        void print();
		
};

#endif


