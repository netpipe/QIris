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
#include "uiparabola.h"

UiParabola::UiParabola(QWidget *parent) : UiElement(parent) {
    resize(parent->size());
	visible = false;
}

/// Creates the parabola based on the three points
void UiParabola::fitParabola(QPointF p[3]) {
	qreal denom = (p[0].x() - p[1].x()) * (p[0].x() - p[2].x()) * (p[1].x() - p[2].x());

	coefficients[0] = (p[2].x() * (p[1].y() - p[0].y()) + p[1].x() * (p[0].y() - p[2].y()) + p[0].x() * (p[2].y() - p[1].y())) / denom;
	coefficients[1] = (pow(p[2].x(),2) * (p[0].y() - p[1].y()) + pow(p[1].x(),2) * (p[2].y() - p[0].y()) + pow(p[0].x(),2) * (p[1].y() - p[2].y())) / denom;
	coefficients[2] = (p[1].x() * p[2].x() * (p[1].x() - p[2].x()) * p[0].y() + p[2].x() * p[0].x() * (p[2].x() - p[0].x()) * p[1].y() + p[0].x() * p[1].x() * (p[0].x() - p[1].x()) * p[2].y()) / denom;

}

void UiParabola::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(Qt::green);
	fitParabola(points);
	/// No Qt tool for drawing parabolas, so just go through the
	/// width of the image and draw
	for (int i = 0; i < width; ++i)
		painter.drawPoint(i, int((coefficients[0] * i*i) + (coefficients[1] * i) + coefficients[2])); // ax^2 + bx + c
}

void UiParabola::change(UiCircle* p1, UiCircle* p2, UiCircle* p3,  qreal imgWidth) {
	visible = true;
	width = imgWidth;
	points[0] = p1->centre();	
	points[1] = p2->centre();	
	points[2] = p3->centre();	
	fitParabola(points);
    update();
}

bool UiParabola::inParabola(QPointF p) {
	bool checkBelow = false;
	if(coefficients[0] > 0)
		checkBelow = true;
	/// If it's within the parabola, then return true
	if((coefficients[0] * p.x()*p.x()) + (coefficients[1] * p.x()) + coefficients[2] < p.y()) {
		if(checkBelow)
			return true;
		else
			return false;
	}
	else {
		if(checkBelow)
			return false;
		else
			return true;
	}
}

bool UiParabola::exists() {
	return visible;
}

void UiParabola::reset() {
	visible = false;
}
