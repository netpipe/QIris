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

#include "eimage.h"
#include "imagefilter.h"
#include "median.h"
#include "histogram.h"
#include <QDebug>
#include <QColor>

EImage::EImage() : QImage() {
    for (int i = 0; i < 256; i++)
        histogram[i] = smoothHistogram[i] = 0;
}

EImage::EImage(const QImage& image) : QImage(image) {
    setup();
}

EImage::EImage(const EImage& image) : QImage(image) {
    setup();
}

void EImage::setup() { // Creates the 2D arrays
    for (int i = 0; i < 256; i++)
        histogram[i] = 0;
}

void EImage::applyFilter(ImageFilter *filter, QRect region) {
    // Filter origin cannot be 0,0 thus we need a piece of code to
    // save the offset, basically (n-1)/2
    int offset = (filter->getDimension() - 1) / 2;
    
    
    // An image is kept as reference to prevent feedback and this
    // is where the original pixel data is read from during the
    // for loops.
    QImage refImage = copy(this->rect());
    
    // If no rectangle is passed, or the rectangle is empty then
    // apply to whole image.
    if (region.isEmpty())
        region = this->rect();
    
    for(int imgX = region.topLeft().x(); imgX < region.bottomRight().x(); imgX++) {
        for(int imgY = region.topLeft().y(); imgY < region.bottomRight().y(); imgY++) {

            for (int filterX = -offset; filterX <= offset; filterX++) {
                for (int filterY = -offset; filterY <= offset; filterY++) {
                    // currentPoint stores the x,y coordinate addition of the position
                    // in the current image loop (the first two for loops) and the position
                    // in the current filter loop.  we can use QImage::valid() to make sure
                    // we are not out of the bounds of the 
                    QPoint currentPoint(imgX + filterX, imgY + filterY);
                    int pixelValue = 0;
                    if (refImage.valid(currentPoint))
                        pixelValue = refImage.pixelIndex(currentPoint);

                    filter->addValue(filterX+offset, filterY+offset, pixelValue);
                }
            }
			int newVal = filter->outputPixel();
			setPixel(QPoint(imgX,imgY), newVal);
        }
    }
}

void EImage::additiveBlend(EImage* otherImage) {
    // An image is kept as reference to prevent feedback and this
    // is where the original pixel data is read from during the
    // for loops.
    QImage refImage = copy(this->rect());
    for(int imgX = 0; imgX < refImage.width(); imgX++) {
        for(int imgY = 0; imgY < refImage.height(); imgY++) { 
            int sum = refImage.pixelIndex(imgX,imgY) + otherImage->pixelIndex(imgX,imgY);
            if (sum > 255)
                sum = 255;
            else if (sum < 0)
                sum = 0;
            setPixel(imgX, imgY, 255 - sum);
        }
    }
}

void EImage::threshold(int min, int max) {
    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            if (pixelIndex(x,y) < min)
                setPixel(x,y, 0);
            else if (pixelIndex(x,y) >= max)
                setPixel(x,y, 255);
        }
    }
}

void EImage::threshold(int t) {
    threshold(t, t+1);
}


void EImage::invert() {
    for(int imgX = 0; imgX < width(); imgX++) {
        for(int imgY = 0; imgY < height(); imgY++) {
            setPixel(QPoint(imgX,imgY), abs(pixelIndex(imgX, imgY) - 255));
        }
    }
}

void EImage::buildHistogram() {
    for(int imgY = 0; imgY < height(); imgY++) {
        uchar* row = scanLine(imgY);
        for(int imgX = 0; imgX < width(); imgX++) {
            histogram[*(row+imgX)]++;
        }
    }
}

void EImage::buildSmoothHistogram() {
    buildHistogram();
    // The aim of this is to smooth the histogram from the image
    // to enable us to more accurately find the max and min.
    smoothHistogram[0] = (histogram[0] + histogram[1]) / 2;
    for (int i = 1; i < 255; ++i)
        smoothHistogram[i] = (histogram[i-1] + histogram[i] + histogram[i+1]) / 3;
    smoothHistogram[255] = (histogram[254] + histogram[255]) / 2;
}

int EImage::getPupilThreshold() {
    buildHistogram();
    int pupilMax = -1;
    int pupilMaxIndex = -1;
    for (int i = 0; i < 90; ++i) {
        if(histogram[i] > pupilMax) {
            pupilMax = histogram[i];
            pupilMaxIndex = i;
        }
    }
    return pupilMaxIndex + 8;
}

void EImage::getIrisThreshold(int& firstMaxIndex, int& secondMaxIndex, int& minIndex) {
    // The histogram generally shows 4 peaks as a property of the image.
    // The darkest peak represents the mass of dark pixels in the pupil,
    // the next lightest peak is usually the overall mass of pixels in
    // the iris.  It is usually a good idea to apply the median filter
    // first to the image to extract the best peaks out of the image
    // to avoid variation from noise.
    medianFilter(9);
    buildSmoothHistogram();
    // Plotted on a graph, we should have some distinct peaks, two for
    // the greyish centres.  The first, and highest peak is what we
    // want to threshold around so we drop the 8 outermost groups
    // and find the maximum value.
    int firstMax = -1;
    firstMaxIndex = -1;
    for (int i = 90; i < 240; ++i)  {
           //qDebug() << histogram[i];
        if (smoothHistogram[i] > firstMax) {
            firstMax = smoothHistogram[i];
            firstMaxIndex = i;
        }
    }

    // Now find the second maximum so that we can threshold around
    // this central point. Possibly more accurate.
    int secondMax = -1;
    secondMaxIndex = -1;
    //first search to the right
        for (int i = firstMaxIndex + 20; i < 240; ++i)  {
           //qDebug() << histogram[i];
            if (smoothHistogram[i] > secondMax) {
                secondMax = smoothHistogram[i];
                secondMaxIndex = i;
            }
        }
    //now search to the left
        for (int i = firstMaxIndex - 20; i > 90; --i)  {
           //qDebug() << histogram[i];
            if (smoothHistogram[i] > secondMax) {
                secondMax = smoothHistogram[i];
                secondMaxIndex = i;
            }
        }

    // Now find the minimum between these two.
    int min = firstMax;
    minIndex = -1;
    if (firstMaxIndex > secondMaxIndex) {
        //swap them round
        int temp = firstMaxIndex;
        firstMaxIndex = secondMaxIndex;
        secondMaxIndex = temp;
    }
    for (int i = firstMaxIndex; i < secondMaxIndex; ++i) {
        if (smoothHistogram[i] < min) {
            min = smoothHistogram[i];
            minIndex = i;
        }
    }
//
//    qDebug() << "Iris peak: " << firstMaxIndex;
//    qDebug() << "Second iris peak: " << secondMaxIndex;
//    qDebug() << "Iris trough: " << minIndex;
    // Returns lowest bound of pixelIndex demarking iris.
    //return ((secondMaxIndex - localMaxIndex) / 2) + localMaxIndex;
}

EImage::~EImage() {}

void EImage::medianFilter(int kernel){
	
	QImage* reference = new QImage(*this);

    int height = this->height();
    int width = this->width();
	
//	std::cout << height << " " << width << std::endl;

	int kernelHistogram[256];
	int** columnHistograms;
	
	columnHistograms = new int*[width];
	for(int i=0; i < width; i++){
		columnHistograms[i] = new int[256];
		for(int k=0; k < 256; k++)
			columnHistograms[i][k]=0;
	}

	// We initalize column histograms with their centres off the
	// top of the image, which ensures they are in the correct 
	// place when brought down a step by the image
	for(int j=0; j < width; j++)
		for(int i=0; i < kernel-1; i++)	
			columnHistograms[j][reference->pixelIndex(j,i)]++;
			
	for(int row=0; row < height; row++){
	
		// First we shift down the first kernel column histograms of the row
		for(int col=0; col < kernel; col++){
			if((row - kernel - 1) > 0 && (col + kernel) < width)
				columnHistograms[col][reference->pixelIndex(col, row - kernel - 1)]--;
			if((row + kernel) < height && (col + kernel) < width) 
				columnHistograms[col][reference->pixelIndex(col, row + kernel)]++;
		}
		// 	We then set kernel histogram for new row: first set to all
		// zeros, then add the first kernel column histograms (shifted down above) to it 
		for(int k=0; k < 256; k++)
			kernelHistogram[k]=0;
		for(int col=0; col <= kernel; col++)
			for(int k=0; k < 256; k++)
				kernelHistogram[k]+=columnHistograms[col][k];
		
		// We then, for each column, make the pixel and then shift the column histogram down, and add it 
		for(int col=0; col < width; col++){
            //setPixel(QPoint(col, row), median(kernelHistogram));
            *(scanLine(row) + col) = median(kernelHistogram);
			
			// Remove old pixel and add new pixel to shit column histogram down
			if((row - kernel - 1) > 0 && (col + kernel) < width)
				columnHistograms[col+kernel][reference->pixelIndex(col + kernel, row - kernel - 1)]--;
			if((row + kernel) < height && (col + kernel) < width) 
				columnHistograms[col+kernel][reference->pixelIndex(col + kernel, row + kernel)]++;
	
			// Remove old and add new column histogram
			for(int k=0; k < 256; k++){
				if(col-kernel-1 > 0)
					kernelHistogram[k]-=columnHistograms[col-kernel-1][k];
				if(col+kernel < width)
					kernelHistogram[k]+=columnHistograms[col+kernel][k];
			}
		}
	}
	

	for(int i=0; i < width; i++)
		delete [] columnHistograms[i];
	delete [] columnHistograms;
	
	delete reference;
}

// Subfunction used to calculate the median of a histogram.
int EImage::median(int* kernelHistogram){
	int total=0;
	int median=-1;
	int count=0;

	for(int i=0; i<256; i++)
		total+=kernelHistogram[i];

	while(count < (total-1)/2+1){
		median++;
		count+=kernelHistogram[median];
	}
	return median;
}

int* EImage::getHistogram() {
    return histogram;
}

int* EImage::getSmoothHistogram() {
    return smoothHistogram;
}

void EImage::maximumBlend(EImage *otherImage) {
    QImage refImage = copy(this->rect());
    for(int imgX = 0; imgX < refImage.width(); imgX++) {
        for(int imgY = 0; imgY < refImage.height(); imgY++) {
            setPixel(imgX, imgY, max(refImage.pixelIndex(imgX, imgY), otherImage->pixelIndex(imgX, imgY)));
        }
    }
}

void EImage::minimumBlend(EImage *otherImage) {
    QImage refImage = copy(this->rect());
    for(int imgX = 0; imgX < refImage.width(); imgX++) {
        for(int imgY = 0; imgY < refImage.height(); imgY++) {
            setPixel(imgX, imgY, min(refImage.pixelIndex(imgX, imgY), otherImage->pixelIndex(imgX, imgY)));
        }
    }
}


/* Most the code for this function is in the histogram class */
int EImage::getOtsuThreshold(){
    buildHistogram();
	Histogram histo(histogram);
	return histo.getThreshold();
}

int EImage::getOtsuThresholdWithoutBlack(){
    buildHistogram();
	Histogram histo(histogram);
	return histo.getThresholdWithoutBlack();
}

int EImage::getOtsuThresholdWithoutWhite(){
    buildHistogram();
	Histogram histo(histogram);
	return histo.getThresholdWithoutWhite();
}
/* End of function */


