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
#include "uicircle.h"

UiCircle::UiCircle(QWidget *parent, QPointF centre, qreal radius) : UiElement(parent), _centre(centre), _radius(radius) {
    resize(parent->size());
	activeColour = false;
	setMouseTracking(true);
}

void UiCircle::paintEvent(QPaintEvent *) {
    QPainter painter(this);
	if(activeColour)
		painter.setPen(Qt::yellow);
	else 
		painter.setPen(Qt::green);
    painter.drawEllipse(_centre, _radius, _radius);
}

void UiCircle::change(QPointF centre, qreal radius) {
    _centre = centre;
    _radius = radius;
    update();
}

QPointF UiCircle::centre() {
    return _centre;
}

qreal UiCircle::radius() {
    return _radius;
}

void UiCircle::changeColour(bool i) {
	if(!i)
		activeColour = false;
	else
		activeColour = true;
	update();
}
