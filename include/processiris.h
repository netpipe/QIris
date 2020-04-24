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

#ifndef PROCESSIRIS_H
#define PROCESSIRIS_H

#include "filter.h"
#include "uiparabola.h"
#include "median.h"
#include "sinwavelet.h"
#include "coswavelet.h"
#include "gabortopixel.h"
#include "eimage.h"

class QObject;
class IrisCode;

//!  Processing for the iris image
/*!
 Tasks include generating the 2d array of the iris, 
edge detecting, and more */

class ProcessIris : public QObject {
    Q_OBJECT
    
public:    
    ProcessIris(QPointF pupilCentre, int innerRadius, int outerRadius, QImage *image, QObject *parent, QString imageName);

	/// Returns the pixel value at radius r and at an angle theta
    int getPixelValue(int theta, int r);
	
	/// Basically returns the cartesian co-ordinates given
	/// the polar ones
    QPoint getPixelLoc(int theta, int r);

	/// Will fill the iris strip taking into consideration the two parabolas
	/// If they don't exist, it acts accordingly
	void fillIrisStrip(UiParabola* parabola1, UiParabola* parabola2);
    bool findPupil(QPoint *points);
    bool findIris(QPoint* pupilPoints, QPoint* irisPoints);
	bool findEyelids(QPoint *points);

    /// Generate a gabor wavelet 'heatmap' of the iris strip
	void heatmap(QImage * unwrappedIris);
    void findCircle(QImage *image, QPoint* points, QRect centreRegion = QRect(100,100,130,80), int minRadius = 20, int maxRadius = 38);
	IrisCode* getIrisCode();

	bool* getBitCode();
	bool* getBitCodeMask();
	
	void printBitCode(bool bitCode[2048]);
	
	/* Test for otsu */
	void otsuTest();


	/// Destructor, deletes the irisRectangle and mask
	~ProcessIris();

signals:
	void imageUnwrapped(QImage*, QImage*);
	void buildingHistogram(int* histogram);
	void initialiseProgressBar(int min, int max);
	void updateProgressBar(int progress);
	void markHistogram(int x);

private:
	QPoint _pupilCentre;
	QString _imageName;
	int pupilThreshold;

	/// Radius of the inner circle -- that is, the pupil
	int _innerRadius;

	/// Radius of the inner circle -- that is, the iris
	int _outerRadius;
	QImage* img;

	/// For generating the unwrapped.bmp
	QImage polarImg;
	QImage heatImg;
	int** irisRectangle;
	bool** irisMask;
	bool bitCode[2048];
	bool bitCodeMask[2048];
	EImage* eyelids;
};

int sq(int);

void printBitCode(bool bitCode[2048]) ;

#endif // PROCESSIRIS_H
