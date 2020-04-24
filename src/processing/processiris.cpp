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

#include <QtGui>
#include <cmath>
#include "processiris.h"
#include "imagefilter.h"
#include "median.h"
#include "gaussianfilter.h"
#include "sobelfilterx.h"
#include "sobelfiltery.h"
#include "erosionfilter.h"
#include "dilationfilter.h"
#include "eimage.h"
#include "gabortoimage.h"
#include "iriscode.h"
#include "eutils.h"
#include <iostream>

ProcessIris::ProcessIris(QPointF pupilCentre, int innerRadius, int outerRadius, QImage *image, QObject *parent, QString fullImageName)
        : QObject(parent), polarImg(256, outerRadius - innerRadius, QImage::Format_Indexed8),
        heatImg(polarImg.size(),QImage::Format_RGB32)
{
        _imageName = fullImageName;
        _pupilCentre = QPoint(pupilCentre.x(), pupilCentre.y());
        _innerRadius = innerRadius;
        _outerRadius = outerRadius;
        img = image;
        pupilThreshold = 0;

        heatImg.fill(0);

        // Copy the colour table
        polarImg.setColorTable(img->colorTable());
        irisRectangle = NULL;
        irisMask = NULL;
		eyelids = NULL;

        EImage* copy = new EImage(*img);
		//copy->medianFilter(9);
        pupilThreshold = copy->getPupilThreshold();
		delete copy;
        //copy->threshold(pupilThreshold);

        //connections
        connect(this, SIGNAL(imageUnwrapped(QImage*, QImage*)), parent, SIGNAL(imageUnwrapped(QImage*, QImage*)));
        connect(this, SIGNAL(initialiseProgressBar(int,int)), parent, SIGNAL(initialiseProgressBar(int,int)));
        connect(this, SIGNAL(updateProgressBar(int)), parent, SIGNAL(updateProgressBar(int)));
}

int ProcessIris::getPixelValue(int theta, int r) {
        if(img->valid(getPixelLoc(theta, r)))
                return img->pixelIndex(QPoint(getPixelLoc(theta, r)));
        // If out of bounds, output an obvious checker bkgd.
        return (theta % 2 || r % 2) ? 0 : 200;
}

QPoint ProcessIris::getPixelLoc(int rtheta, int r) {
        int x, y;
        // 128 instead of 180 as we are using 256 angles, not 360 now
        qreal theta = rtheta * (EUtils::PI / 128);
        // Try r without scaling:
        r = static_cast<int>(_innerRadius + r);

        x = static_cast<int>(r * cos(theta));
        y = static_cast<int>(r * sin(theta));

        x += static_cast<int>(_pupilCentre.x());
        y += static_cast<int>(_pupilCentre.y());

        QPoint point(x,y);
        return point;
}

void ProcessIris::fillIrisStrip(UiParabola* parabola1, UiParabola* parabola2) {
        int angles = 256;
        int width = _outerRadius - _innerRadius;  // the iris width
        /// the current point that we're dealing with, temp value
        QPoint currentPoint;
        // Create 2D array of the iris strip and the mask of bad bits
        irisRectangle = new int *[width];
        irisMask = new bool *[width];

        for (int rows = 0; rows < width; ++rows) {
                irisRectangle[rows] = new int [angles];
                irisMask[rows] = new bool [angles];

                for ( int cols = 0; cols < angles; ++cols ) {
                        currentPoint = getPixelLoc(cols, rows);
						int pixelValue = getPixelValue(cols, rows);

                        // Fill in the pixel value to the strip and extra bitmap
                        irisRectangle[rows][cols] = pixelValue;


                        if(!parabola2->isVisible())
                                irisMask[rows][cols] = true;
                        polarImg.setPixel(QPoint(cols,rows), pixelValue);

                        // If the user added parabolas, then add in the
                        // respective info to the mask
						// First check that there are no specular highlights or 
						// bad black bits of the pupil
						if(pixelValue < pupilThreshold) {
							irisMask[rows][cols] = true;
							if(cols % 2 || rows % 2)
								polarImg.setPixel(QPoint(cols,rows), 255); 
						}
						else if(pixelValue > 230) {
							irisMask[rows][cols] = true;
							if(cols % 2 || rows % 2)
								polarImg.setPixel(QPoint(cols,rows), 0);
						}
						else {
							if(parabola1->isVisible() || parabola1->exists()) {
								if(parabola1->inParabola(currentPoint))
									irisMask[rows][cols] = true;
								else {
									if(cols % 2 || rows % 2)
										polarImg.setPixel(QPoint(cols,rows), 0); // maybe we could make this look nicer
									irisMask[rows][cols] = false;
								}
							}

							if(parabola2->isVisible() || parabola2->exists()) {
								if(!parabola2->inParabola(currentPoint)) {
									if(cols % 2 || rows % 2)
										polarImg.setPixel(QPoint(cols,rows), 0);
									irisMask[rows][cols] = false;
								}
							}
						}

                }
        }

        //polarImg.save("output/" + _imageName + "-unwrapped.bmp", "BMP", -1);
        // Only generate the heatmap when there are no parabolas
        heatmap(&polarImg);
        gaborImage(&polarImg, irisMask, bitCode, bitCodeMask);
    QImage polarImgScaled = polarImg.scaled(img->width(), 70);
    QImage heatImgScaled = heatImg.scaled(img->width(), 70);
        emit imageUnwrapped(&polarImgScaled, &heatImgScaled);
}

bool* ProcessIris::getBitCode() {
        return bitCode;
}

bool* ProcessIris::getBitCodeMask() {
        return bitCodeMask;
}

void ProcessIris::printBitCode(bool bitCode[2048]) {
        for(int i = 0; i < 2048; ++i)
                cout << bitCode[i];
        cout << endl;
}

bool ProcessIris::findEyelids(QPoint *points) {
	int width = _pupilCentre.x();
	int height = eyelids->height() / 2 - 1;
	int pointDist = 50; // distance between which to place points
	int padding = 21;   // the padding that we want to use for the top eyelid
	// Initiale all points to (0,0)
	for(int j = 0; j < 6; ++j)
		points[j] = QPoint(0,0);

	for(int i=0; i < height; ++i) {
		// First eyelid -- top-down
		
		for(int j = 0; j < 3; ++j) {
			int distAlong = (width) - pointDist + (j*pointDist);
			if(points[j] == QPoint(0,0) && eyelids->pixelIndex(distAlong+50, i) != 255)
				points[j] = QPoint(distAlong, i+padding);
		}
		
		/*
		if(points[0] == QPoint(0,0) && eyelids->pixelIndex(width+50, i) != 255) {
			points[0] = QPoint(width, i+padding);
			points[1] = QPoint(width-pointDist, i+padding+extra);
			points[2] = QPoint(width+pointDist, i+padding+extra);
		}
		*/
		// Second eyelid -- bottom-up
		for(int j = 3; j < 6; ++j) {
			int distAlong = (width) - pointDist + ((j-3)*pointDist);
			if(points[j] == QPoint(0,0) && eyelids->pixelIndex(distAlong, (height*2) - i) != 255)
				points[j] = QPoint(distAlong, (2*height) - i);
		}
	}
		// ================Testing purposes, ugly code ==================
	EImage outputImage = *img;
	//for(int count = 0; count < 256; ++count) {
		//outputImage.setPixel(getPixelLoc(count,0), 255);
		//outputImage.setPixel(getPixelLoc(count,_outerRadius - _innerRadius), 255);
		//// Make a thicker line
		//outputImage.setPixel(getPixelLoc(count,_outerRadius - _innerRadius - 1), 255);
	//}

	qreal coefficients[3];
	qreal denom = (points[0].x() - points[1].x()) * (points[0].x() - points[2].x()) * (points[1].x() - points[2].x());

	coefficients[0] = (points[2].x() * (points[1].y() - points[0].y()) + points[1].x() * (points[0].y() - points[2].y()) + points[0].x() * (points[2].y() - points[1].y())) / denom;
	if(coefficients[0] <= 0) {
		if(points[0].y() < points[1].y())
			points[1] = QPoint(points[1].x(), 2*points[0].y()  - points[1].y());
		if(points[2].y() < points[1].y())
			points[2] = QPoint(points[2].x(), 2*points[0].y()  - points[2].y());
		if(points[0].y() == points[1].y() && points[1].y() == points[2].y()) {
			points[0] = QPoint(points[0].x(), points[0].y() + 5);
			points[2] = QPoint(points[2].x(), points[2].y() + 5);
		}
		coefficients[0] = (points[2].x() * (points[1].y() - points[0].y()) + points[1].x() * (points[0].y() - points[2].y()) + points[0].x() * (points[2].y() - points[1].y())) / denom;
	}

        coefficients[1] = (pow(points[2].x(),2.0) * (points[0].y() - points[1].y()) + pow(points[1].x(),2.0) * (points[2].y() - points[0].y()) + pow(points[0].x(),2.0) * (points[1].y() - points[2].y())) / denom;
	coefficients[2] = (points[1].x() * points[2].x() * (points[1].x() - points[2].x()) * points[0].y() + points[2].x() * points[0].x() * (points[2].x() - points[0].x()) * points[1].y() + points[0].x() * points[1].x() * (points[0].x() - points[1].x()) * points[2].y()) / denom;

	for (int i = 0; i < outputImage.width()-1; ++i) {
		int y = int((coefficients[0] * i*i) + (coefficients[1] * i) + coefficients[2]);
		if(y < img->height()) {
			outputImage.setPixel(i, y, 255); 
			outputImage.setPixel(i+1, y, 255);
		}
	}

denom = (points[3].x() - points[4].x()) * (points[3].x() - points[5].x()) * (points[4].x() - points[5].x());

coefficients[0] = (points[5].x() * (points[4].y() - points[3].y()) + points[4].x() * (points[3].y() - points[5].y()) + points[3].x() * (points[5].y() - points[4].y())) / denom;

	if(coefficients[0] >= 0) {
		if(points[3].y() > points[4].y())
			points[3] = QPoint(points[3].x(), 2*points[4].y() - points[3].y());
		if(points[5].y() > points[4].y())
			points[5] = QPoint(points[5].x(), 2*points[4].y() - points[5].y());
		coefficients[0] = (points[5].x() * (points[4].y() - points[3].y()) + points[4].x() * (points[3].y() - points[5].y()) + points[3].x() * (points[5].y() - points[4].y())) / denom;
	}

coefficients[1] = (pow(points[5].x(),2.0) * (points[3].y() - points[4].y()) + pow(points[4].x(),2.0) * (points[5].y() - points[3].y()) + pow(points[3].x(),2.0) * (points[4].y() - points[5].y())) / denom;
coefficients[2] = (points[4].x() * points[5].x() * (points[4].x() - points[5].x()) * points[3].y() + points[5].x() * points[3].x() * (points[5].x() - points[3].x()) * points[4].y() + points[3].x() * points[4].x() * (points[3].x() - points[4].x()) * points[5].y()) / denom;

	for (int i = 0; i < outputImage.width()-1; ++i) {
		int y = int((coefficients[0] * i*i) + (coefficients[1] * i) + coefficients[2]);
		if(y < img->height()) {
			outputImage.setPixel(i, y, 255); 
			outputImage.setPixel(i+1, y, 255);
		}
	}

	// =========================
	//outputImage.minimumBlend(copy);
	QPixmap* t = new QPixmap(outputImage.size());
	t->fill(Qt::white);
	QPainter p(t);
	p.drawImage(0,0,outputImage);
	p.end();
	//t->save("output/" + _imageName + "-circles.bmp", "BMP", -1);
	delete t;

	for(int j = 0; j < 6; ++j) {
		//qDebug() << "Point is: " << points[j];
		if(points[j] == QPoint(0,0))
			return false;
	}
	return true;

}




bool ProcessIris::findPupil(QPoint *points) {
        SobelFilter* x = new SobelFilterX();
        x->generateFilter();
        SobelFilter* y = new SobelFilterY();
        y->generateFilter();
        // Make an EImage by copying a QImage
        EImage* copy = new EImage(*img);
        //copy->save("pupil_step_1_start.bmp");
		copy->medianFilter(9);
        //copy->save("pupil_step_2_median.bmp");
        copy->threshold(pupilThreshold);
        //copy->save("pupil_step_3_thresh.bmp");

    int sumx, sumy, amount;
    sumx = sumy = amount = 0;
    for (int y = 0; y < copy->height(); y++) {
        uchar* row = copy->scanLine(y);
        for (int x = 0; x < copy->width(); x++) {
            if(!*(row+x)) {
                sumx += x;
                sumy += y;
                amount++;
            }
        }
    }

        // If sumx and sumy are 0, that means that the filter
        // destroyed the pupil, so autodetection failed
        if(sumx == 0 || sumy == 0)  {
                delete copy;
                delete x;
                delete y;
                return false;
        }
    sumx /= amount;
    sumy /= amount;
    QPoint centre(sumx, sumy);
    uchar* centreRow = copy->scanLine(sumy);
    int radius = -1;
    for(radius = sumx; !*(centreRow+radius); radius++) {}
    radius -= sumx - 2; // 2 as pad

    //copy->setPixel(sumx, sumy, 255);

        //copy->save("filtered.bmp");
        //copy->save("output/" + _imageName + "-filtered.bmp", "BMP", -1);
        // Make a new copy to blend additively
        EImage* temp = new EImage(*copy);

        // Sobel filter:
        copy->applyFilter(x);
        temp->applyFilter(y);

        copy->additiveBlend(temp);
       //copy->save("pupil_step_4_sobel.bmp");
        delete temp;

    // Default parameters for findCircle are specified for pupil
    findCircle(copy, points, QRect(sumx - 1, sumy - 1, 2,2), radius, radius+4);
        // Done with old images!
        delete copy;
        delete x;
        delete y;

        if (points[0] == points[1] || points[1] == points[2] || points[2] == points[0])
                return false;
        return true;
}

bool ProcessIris::findIris(QPoint* pupilPoints, QPoint* irisPoints) {
	otsuTest(); /* WILL ADDED THIS TODO: MOVE THIS EVENTUALLY */
    // To find the iris we must threshold the peak grey value once median
    // filtered to give us an indication of the important pixels.
    // From that we can edge detect...
    EImage* copy = new EImage(*img);
    copy->medianFilter(3);
    int max1, max2, min;
    copy->getIrisThreshold(max1, max2, min);
    emit buildingHistogram(copy->getSmoothHistogram());
    if (max1 > 170 || max1 <= 130) { // assume foul play
//        qDebug() << "First hist peak out of range, so using default.";
        max1 = 160;
    }
    copy->threshold(max1+13);
    emit markHistogram(max1);
    emit markHistogram(max2);
    emit markHistogram(max1+13);
  
  
    copy->threshold((max1 + max2) / 2);

    SobelFilter *x = new SobelFilterX();
    SobelFilter *y = new SobelFilterY();
    x->generateFilter();
    y->generateFilter();
    EImage* copy2 = new EImage(*copy);
    copy->applyFilter(x);
    copy2->applyFilter(y);
    copy->additiveBlend(copy2);
    //copy->save("output/" + _imageName + "-the one.bmp", "BMP", -1);
    delete copy2;
    delete x;
    delete y;

    // Now should have okay estimation of whatever curvature of the iris
    // is available for Hough.
    //copy->save("iris.bmp");
        //copy->save("output/" + _imageName + "-iris.bmp", "BMP", -1);

    // Find pupil -- if it fails, then all autodetection failed
        if(!findPupil(pupilPoints)) {
                delete copy;
                return false;
        }

    // Calculate parameter constraints for iris
    _pupilCentre = EUtils::calcCircCentre(pupilPoints[0],pupilPoints[1],pupilPoints[2]);
    QPoint centreOffset(5,5);
    QPoint topLeft = _pupilCentre - centreOffset;
    QPoint botRight = _pupilCentre + centreOffset;
        _innerRadius = EUtils::calcCircRadius(pupilPoints[0], _pupilCentre);

    // Find iris
    findCircle(copy, irisPoints, QRect((_pupilCentre - centreOffset),(_pupilCentre + centreOffset)), (copy->width() / 4), (copy->height() / 2));
        _outerRadius = EUtils::calcCircRadius(irisPoints[0], _pupilCentre);

        // Drawing the edge-detected pupil and iris circles
        // This process takes roughly 200ms, so disable for optimal performance

        // That's right, having to draw the ellipse ourselves as
        // QPainter refuses to drawEllipse()
        EImage outputImage = *img;
        for(int count = 0; count < 256; ++count) {
                outputImage.setPixel(getPixelLoc(count,0), 255);
                outputImage.setPixel(getPixelLoc(count,_outerRadius - _innerRadius), 255);
                // Make a thicker line
                outputImage.setPixel(getPixelLoc(count,_outerRadius - _innerRadius - 1), 255);
        }
        outputImage.minimumBlend(copy);
        QPixmap* t = new QPixmap(outputImage.size());
        t->fill(Qt::white);
        QPainter p(t);
        p.drawImage(0,0,outputImage);
        //p.drawText(10,25,"Dragos loves rubbing his hands.");
        p.end();
        //t->save("output/" + _imageName + "-circles.bmp", "BMP", -1);
        delete t;
    delete copy;
    // Assume it went well

    heatmap(&polarImg);

    return true;
}

bool withinRange(int x, int average);

void ProcessIris::findCircle(QImage *image, QPoint* points, QRect centreRegion, int minRadius, int maxRadius) {

    // Initialise Parameters:
    if(centreRegion.isNull()) {
        qDebug() << "Warning: Null rectangle, using whole image.";
        centreRegion = QRect(QPoint(0,0), image->size());
    }

    // qDebug() << "Centre region: " << centreRegion;
    // qDebug() << "Min rad: " << minRadius << ", Max rad:" << maxRadius;

        // Make 3D accumulator:
        int a_min = centreRegion.left();
        int a_max = centreRegion.right();
        int b_min = centreRegion.top();
        int b_max = centreRegion.bottom();
        int r_min = minRadius;
        int r_max = maxRadius;

        int a = a_max - a_min;
        int b = b_max - b_min;
        int r = r_max - r_min;

    //emit initialiseProgressBar(30, img->width() - 31);

        // The eventual value
        int top_r = 0;
        int top_a = 0;
        int top_b = 0;

        // The sum of the values which are >= maxVotes - 1
        int total_r = 0;
        int total_a = 0;
        int total_b = 0;

        // Amount of values that are >= maxVotes - 1
        int amount = 0;

        // The max amount of votes that any has
        int maxVotes = 0;

        int ***acc = new int**[a];
        for (int i = 0; i < a; i++) {
                acc[i] = new int*[b];
                for (int j = 0; j < b; j++) {
                        acc[i][j] = new int[r];
                }
        }

        // Initialise accumulator to 0
        for (int p = 0; p < a; p++) {
                for (int q = 0; q < b; q++) {
                        for (int s = 0; s < r; s++) {
                                acc[p][q][s] = 0;
                        }
                }
        }

        // For each black point, find the circles which satisfy the equation where the parameters
        // are limited by a,b and r.
        for (int y = 0; y < image->height(); y++) {
        //if (x%5)
            uchar* row = image->scanLine(y);
            //emit updateProgressBar(x);  //emit every 5 loops
                for (int x = 0; x < image->width(); x++) {
                        // For each pixel
                        if(!*(row+x)) {
                                for (int _a = 0; _a < a; _a++) {
                                        for (int _b = 0; _b < b; _b++) {
                                                for (int _r = 0; _r < r; _r++) {
                                                        if ( sq(x - (_a+a_min)) + sq(y - (_b+b_min)) == sq(_r+r_min) ) {
                                                                acc[_a][_b][_r]++;
                                                                if(acc[_a][_b][_r] >= maxVotes) {
                                                                        maxVotes = acc[_a][_b][_r];
                                                                }
                                                                if(acc[_a][_b][_r] >= maxVotes - 1) {
                                                                        //qDebug() << "a = " << _a + a_min << "; b = " << _b + b_min << "; r = " << _r + r_min << "has " << acc[_a][_b][_r] << "votes..";
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }

        for (int _a = 0; _a < a; _a++) {
                for (int _b = 0; _b < b; _b++) {
                        for (int _r = 0; _r < r; _r++) {
                                if(acc[_a][_b][_r] >= maxVotes -1) {
                                        total_a += _a + a_min;
                                        total_b += _b + b_min;
                                        total_r += _r + r_min;
                                        amount++;
                                        //qDebug() << "a = " << _a + a_min << "; b = " << _b + b_min << "; r = " << _r + r_min << "has " << acc[_a][_b][_r] << "votes..";
                                }
                        }
                }
        }

        // Get the initial average values
        top_a = total_a / amount;
        top_b = total_b / amount;
        top_r = total_r / amount;

        // Setting two points -- one for the centre, and one for the length of the radius
        //img->setPixel(QPoint(top_a, top_b), 255);
        //img->setPixel(QPoint(top_a+top_r, top_b), 255);
        points[0] = QPoint(top_a+top_r, top_b);
        points[1] = QPoint(top_a-top_r, top_b);
        points[2] = QPoint(top_a, top_b+top_r);

        // For drawing only:
//        QImage temp(image->size(), QImage::Format_ARGB32);
//        QPoint c = EUtils::calcCircCentre(points[0],points[1],points[2]);
//        qreal rad = EUtils::calcCircRadius(c,points[0]);
//        QPainter p(&temp);
//        p.drawImage(0,0,*image);
//        p.setPen(Qt::red);
//        p.drawEllipse(static_cast<QPointF>(c), rad, rad);
//        p.end();
//        temp.save("pupil_step_5_hough.bmp");
//        exit(2);

    //qDebug() << "maxVotes was:" << maxVotes;

        // Delete acc
        for (int i = 0; i < a; i++) {
                for(int j = 0; j < b; j++) {
                        delete [] acc[i][j];
                }
                delete [] acc[i];
        }
        delete [] acc;
}

bool withinRange(int x, int average) {
        // 0.05 being the proximity
        if(abs(average - x) < 0.05 * average)
                return true;
        return false;
}

inline int sq(int a) {
        return a*a;
}

void ProcessIris::heatmap(QImage * unwrappedIris) {
        int width = unwrappedIris->width();
        int rDimension = unwrappedIris->height();

        //heatImg = new QImage(unwrappedIris->size(),QImage::Format_RGB32);

        bool ** mask = new bool * [rDimension];
        for(int i = 0; i < rDimension; ++i)
                mask[i] = new bool[width];

        for(int i=0; i < rDimension; ++i)
                for(int j=0; j < width; ++j)
                        mask[i][j] = true;

        int filterSize = 9;

        SinWavelet* pSin = new SinWavelet(filterSize);
        CosWavelet* pCos = new CosWavelet(filterSize);

        pSin->generateFilter();
        pCos->generateFilter();

        bool real;
        bool imag;
        QRgb colour;

        for(int i = 0; i < rDimension; i++) {
                for(int j = 0; j < width; j++) {
                        if( (i <= (filterSize/2)) || i >= (rDimension-(filterSize/2)) )
                                colour = qRgb(0,0,0);
                        else {
                                real = gaborPixel(i,j,*pCos,unwrappedIris,mask);
                                imag = gaborPixel(i,j,*pSin,unwrappedIris,mask);
                                if(imag) {
                                        if(real)
                                                colour = qRgb(91,102,166);
                                        else
                                                colour = qRgb(91,140,77);
                                }
                                else {
                                        if(real)
                                                colour = qRgb(120,120,120);
                                        else
                                                colour = qRgb(217,179,145);
                                }
                        }
                        heatImg.setPixel(QPoint(j, i), colour);
                }
        }

    //heatImg.save("heat.bmp");
        delete pSin;
        delete pCos;

        for (int i = 0; i < rDimension; ++i) {
                delete [] mask[i];
        }
        delete [] mask;
}


/* Test function for Otsu methods */
/* This function currently takes the image, blurs it and then 
	applies an otsu threshold ignoring black, this should result
	in an image showing the eyelids.
	
	This eyelid image is then used to mark out the eyelids for iris
	detection. We threshold the iris ignoring white and black and hence
	we are just comparing the greys of the iris and the white of the eye.
	
	We also mark the eyelids black for a combined feature image just to show
	off otsu's power.	
	
	Note confusing names: withoutwhite is without white(255) or black (<80).
	*/

void ProcessIris::otsuTest(){
	int height = img->height();
	int width = img->width();
	
	eyelids = new EImage(*img);
	EImage* iris = new EImage(*img);
	EImage* combined = new EImage(*img);
	
	eyelids->medianFilter(9);  // blur and threshold to get eyelids
	eyelids->threshold(eyelids->getOtsuThresholdWithoutBlack()); // thresholding with black (<80 ie. pupil) removed gives better results on eyelid detection.
	eyelids->medianFilter(9);
	//eyelids->save("./output/otsu-eyelids-" + _imageName + ".bmp", "BMP", -1);
	
	for(int i = 0; i < height; i++)
		for(int j=0; j < width; j++)
			if(eyelids->pixelIndex(j,i)!=0){
				iris->setPixel(j,i,255); // set the eyelids to white for second run of otsu (will be ignored)
				combined->setPixel(j,i,0); // set the eyelids to black for second run of otsu
			}
	iris->threshold(iris->getOtsuThresholdWithoutWhite()); // run otsu ignoring white eyelids - to get iris
	combined->threshold(combined->getOtsuThresholdWithoutBlack()); //run otsu ignoring black eyelids - to get combined feature 
	
	//combined->save("./output/otsu-combined-" + _imageName + ".bmp", "BMP", -1);
	//iris->save("./output/otsu-iris-" + _imageName + ".bmp", "BMP", -1);
	
	delete iris;
}
	
	

/// Clean up the two dynamic 2d arrays
ProcessIris::~ProcessIris() {
	if(eyelids)
		delete eyelids;
	if(irisRectangle && irisMask) {
		for (int i = 0; i < polarImg.height(); ++i) {
			delete [] irisRectangle[i];
			delete [] irisMask[i];
		}
		delete [] irisRectangle;
		delete [] irisMask;
	}
}


