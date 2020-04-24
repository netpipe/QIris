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
 * Database Class -- for interfacing with the database
 */
#ifndef DATABASE_H
#  define DATABASE_H

class IrisCode;
class QString;
#include <QtGlobal>

class Database {
	public:
		// First searches the database, and if it doesn't find a matching iris
		// then it adds it to the database
		// Searches the database for the given irisCode, and puts the foundIris name
		// in the foundIris QString
		static bool add(QString fileName, bool bitCode[2048], bool bitCodeMask[2048], double& hamDist, QString& foundIris);
		
		static bool search(bool bitCode[2048], bool bitCodeMask[2048], double& hamDist, QString& foundIris);
		// Returns the certainty X (in the form 1 in X) of a false match given
		// the hamming distance, hamDist
		// Based on the original Daugman paper
		static QString certainty(double hamDist);
	private:
		// The name of the text file for the database
		static QString dbName;
};

#endif /* ifndef DATABASE_H */

