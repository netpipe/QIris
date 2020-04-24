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

#include "eutils.h"

const qreal EUtils::PI = atan(1) * 4;

inline int EUtils::sq(int a) {
    return a*a;
}

QPoint EUtils::calcCircCentre(QPointF a, QPointF b, QPointF c) {
    qreal h, k;
    // Check proximity otherwise discrete maths fail.
    if (a == b || b == c || c == a) {
        qDebug() << "Proximity Fail!";
        return QPoint(-1,-1);
    }
    h = ((pow(a.x(),2) + pow(a.y(),2)) * (b.y()-c.y()) + (pow(b.x(),2) + pow(b.y(),2) ) * (c.y()-a.y()) + (pow(c.x(),2) +pow(c.y(),2) ) * (a.y()-b.y())) / (2 * (a.x() * b.y() - b.x() * a.y() - a.x() * c.y() + c.x() * a.y() + b.x() * c.y() - c.x() * b.y()));
    k = ((pow(a.x(),2) +pow(a.y(),2) ) * (c.x()-b.x()) + (pow(b.x(),2) +pow(b.y(),2) ) * (a.x()-c.x()) + (pow(c.x(),2) +pow(c.y(),2) ) * (b.x()-a.x()) ) / (2 * (a.x() * b.y() - b.x() * a.y() - a.x() * c.y() + c.x() * a.y() + b.x() * c.y() - c.x() * b.y()) );
    return QPoint(int(h),int(k));
}

qreal EUtils::calcCircRadius(QPointF a, QPointF c) {
    qreal r, x, y;
    x = a.x() - c.x();
    y = a.y() - c.y();
    r = sqrt(pow(x, 2) + pow(y, 2));
    return r;
}
