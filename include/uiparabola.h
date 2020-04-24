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

#ifndef UIPARABOLA_H
#define UIPARABOLA_H

#include "uielement.h"
#include "uicircle.h"

//!  Class for constructing the parabolas (for the eyelids)
/*!
 Class takes in all the points set in irisview, but only
  uses the last three ones to construct a parabola */

class UiParabola : public UiElement {
    Q_OBJECT
public:
    UiParabola(QWidget *parent);

	/// Changes the parabola; i.e. from mouseMoveEvent
    void change(UiCircle* p1, UiCircle* p2, UiCircle* p3,  qreal imgWidth);

	/// Checks if point p is below/above the parabola based on bool
	bool inParabola(QPointF p);
	bool exists();
	void reset();
protected:
    void paintEvent(QPaintEvent *);
private:
	/*! Takes in an array of points, and uses the last three to construct a parabola
	 * which it puts into the parabola's array of coefficients */
	void fitParabola(QPointF ppoints[3]);
	/// The three points of the parabola
    QPointF points[3];
	/// The three coefficients of the parabola
    qreal coefficients[3];
	qreal width;  /// how long along the x-axis to draw parabola
	bool visible;
};

#endif // UIPARABOLA_H
