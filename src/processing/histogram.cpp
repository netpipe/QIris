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

#include "histogram.h"


Histogram::Histogram(int* _histogram){
	for(int i=0; i < 256; i++){
		histogram[i] = _histogram[i];
		//cout << i << " " << _histogram[i] << endl;
	}
}
	
double Histogram::omega(int k){
	double omega=0;
	for(int i=0; i < k; i++)
		omega+=probability_distribution[i];	
	return omega;
}

double Histogram::mew(int k){
	double mew=0;
	for(int i=0; i < k; i++)
		mew+=(i+1)*probability_distribution[i];	
	return mew;
}

double Histogram::goodness(int k){
	if(omega(k) == 1 || omega(k) == 0)
		return 0;
	//cout << "mew k " << mew(k) << endl;
	//cout << "omega k " << omega(k) << endl;
	return (mew(256)*omega(k) - mew(k))*(mew(256)*omega(k) - mew(k)) / (omega(k) * (1 - omega(k)));
}

void Histogram::createGoodnessArray(){
	for(int i=0; i < 256; i++){
		goodnessArray[i]=goodness(i);
		//cout << goodnessArray[i] << endl;
	}
}

int Histogram::getThreshold(){ 
	int max=0;
		
	pixel_count=0;
	for(int i=0; i < 256; i++)
		pixel_count+=histogram[i];	
	//cout << pixel_count << endl;
		
	for(int i=0; i < 256; i++){
		probability_distribution[i]=static_cast<double>(histogram[i])/pixel_count;
		//cout << probability_distribution[i] << endl;
	}
			
	createGoodnessArray();
	
	for(int i=0; i < 256; i++)
		if(goodnessArray[i] > goodnessArray[max])
			max = i;
	//cout << "Otsu thresholded on:" << threshold << endl;
	return max;
}

int Histogram::getThresholdWithoutBlack(){ 
	int max=0;
	
	for(int i=0; i < 80; i++)
		histogram[i]=0; // disregard blacks for pupil
	
	pixel_count=0;
	for(int i=0; i < 256; i++)
		pixel_count+=histogram[i];	
	//cout << pixel_count << endl;
		
	for(int i=0; i < 256; i++){
		probability_distribution[i]=static_cast<double>(histogram[i])/pixel_count;
		//cout << probability_distribution[i] << endl;
	}
			
	createGoodnessArray();
	
	for(int i=0; i < 256; i++)
		if(goodnessArray[i] > goodnessArray[max])
			max = i;
	//cout << "Otsu thresholded on:" << threshold << endl;
	return max;
}

int Histogram::getThresholdWithoutWhite(){ 
	int max=0;
	
	for(int i=0; i < 80; i++)
		histogram[i]=0; // disregard blacks for pupil
		
	histogram[255]=0; // disregard white
	
	pixel_count=0;
	for(int i=0; i < 256; i++)
		pixel_count+=histogram[i];	
	//cout << pixel_count << endl;
		
	for(int i=0; i < 256; i++){
		probability_distribution[i]=static_cast<double>(histogram[i])/pixel_count;
		//cout << probability_distribution[i] << endl;
	}
			
	createGoodnessArray();
	
	for(int i=0; i < 256; i++)
		if(goodnessArray[i] > goodnessArray[max])
			max = i;
	//cout << "Otsu thresholded on:" << threshold << endl;
	return max;
}




