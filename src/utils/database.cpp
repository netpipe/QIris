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

#include "database.h"
#include "iriscode.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <cmath>

/// The database file name
QString Database::dbName = "irisDB.txt";

bool Database::add(QString fileName, bool bitCode[2048], bool bitCodeMask[2048], double& hamDist, QString& foundIris) {
	// First search the database so that we're not adding a duplicate
	for(int i=0; i < 2048; ++i) {
		if(i == 2047) {
			qDebug() << "========= Number of useful bits was 0, so not adding ===========";
			return false;
		}
		if(bitCodeMask[i] == true)
			break;
	}

	if(search(bitCode, bitCodeMask, hamDist, foundIris))
		return false;

	QFile database(dbName);
	database.open(QIODevice::Append);
	if(database.error()) {
		qDebug() << "Massive fail: couldn't open DB";
		return false;
	}
	QTextStream file(&database);

	IrisCode irisCode(bitCode, bitCodeMask, 0);
	// Take in the name, and the two codes with each number
	// separated by a space
	file << fileName;
	file << ' ';
	for(int count = 0; count < 64; ++count) {
		file << irisCode.getCode(count);
		file << ' ';
	}
	for(int count = 0; count < 64; ++count) {
		file << irisCode.getMask(count);
		file << ' ';
	}
		file << '\n';

	database.close();
	// If all was well, then it was added to the DB, so return true
	return true;
}

bool Database::search(bool bitCode[2048], bool bitCodeMask[2048], double& hamDist, QString& foundIris) {
	double threshold = 0.3; // Our Hamming distance threshold -- what we consider a "match"
	// We take in each bit code from the file and compare them;
	// these are the variables that we store them in
	Bitcode32 tempBitCode[64];
	Bitcode32 tempMaskCode[64];
	IrisCode* tempIrisCode;
	QFile database(dbName);
	double min = 1.0; // Will be used for holding the minimum hamming distance
	int rotation; // The rotation that we're on
	QString tempWord;
	int minRotation = 0;

	for(rotation = -56; rotation <= 56; rotation += 8) {
		IrisCode irisCode(bitCode, bitCodeMask, rotation);
		//irisCode.print();

		database.open(QIODevice::ReadOnly);
		// If the file doesn't exist, we would get an error here
		if(database.error()) {
			// ...so create the file first
			qDebug() << "No database file (called" << dbName << ") was found. Creating...";
			database.open(QIODevice::WriteOnly);
			database.close();
			database.open(QIODevice::ReadOnly);
		}

		QTextStream file(&database);
		if(database.error())
			qDebug() << "Massive fail: couldn't open DB";

		// Search through the whole file for each iris code
		while(!file.atEnd()) {

			file >> tempWord;  // the iris' name that we've just taken in
			if(tempWord == "") // It's the empty space at the end, boys.
				break;

			for(int count = 0; count < 64; ++count)
				file >> tempBitCode[count];
			for(int count = 0; count < 64; ++count)
				file >> tempMaskCode[count];

			tempIrisCode = new IrisCode(tempBitCode, tempMaskCode);
			hamDist = irisCode.compare_with(tempIrisCode);

			// Perform all rotations, and just find the min
			if(hamDist < min) {
				min = hamDist;
				minRotation = rotation;
				foundIris = tempWord;
			}

			//qDebug() << "With file:" << tempWord << "at rotation"<< rotation / 8 << "hamming distance is:" << hamDist;
			delete tempIrisCode;
		}

		database.close();
	}

	if(min < threshold) {
		hamDist = min;
		qDebug() << "Iris found! With Hamming distance of" << min << "name is:" << foundIris << "and with" << minRotation / 8 << "rotations";
		database.close();
		return true;
	}
	return false;
}

QString Database::certainty(double hamDist) {
	// There's nothing obvious for calculating the integral of binomial distributions
	// though maybe we could use the normal distribution approx. in the future
	if(hamDist <= 0.26)
		return "10 trillion (2 x 10^13)";
	else if(hamDist <= 0.27)
		return "1 trillion (1 x 10^12)";
	else if(hamDist <= 0.28)
		return "100 billion (1 x 10^11)";
	else if(hamDist <= 0.29)
		return "13 billion";
	else if(hamDist <= 0.30)
		return "1.5 billion";
	else if(hamDist <= 0.31)
		return "185 million";
	else if(hamDist <= 0.32)
		return "26 million";
	else if(hamDist <= 0.33)
		return "4 million";
	else if(hamDist <= 0.34)
		return "690 thousand";
	else if(hamDist <= 0.35)
		return "133 thousand";
	else
		return "Not known";
}
