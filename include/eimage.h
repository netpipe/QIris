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

#ifndef EIMAGE_H
#define EIMAGE_H

#include <QImage>

class ImageFilter;

class EImage : public QImage {
public:
    EImage();
    EImage(const QImage &image);
    EImage(const EImage &image);
    ~EImage();
    void applyFilter(ImageFilter* filter, QRect region = QRect());
    void additiveBlend(EImage* anotherImage);
    void maximumBlend(EImage* anotherImage);
    void minimumBlend(EImage* anotherImage);
    void threshold(int min, int max);
    void threshold(int t);
    void invert();
    void buildHistogram();
    void buildSmoothHistogram();
    void getIrisThreshold(int& firstMaxIndex, int& secondMaxIndex, int& minIndex);
    int getPupilThreshold();
    void medianFilter(int kernel);
    int* getHistogram();
    int* getSmoothHistogram();
    
    int getOtsuThreshold();
    int getOtsuThresholdWithoutBlack();
    int getOtsuThresholdWithoutWhite();
private:
    int median(int* histogram);
    void setup();
    int histogram[256];
    int smoothHistogram[256];
};

#endif // EIMAGE_H
