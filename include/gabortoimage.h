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

/*
 * $Id$
 */
#ifndef GABORTOIMAGE_H
#  define GABORTOIMAGE_H

#include "gabortopixel.h"
#include "filter.h"
#include "sinusoidal.h"
#include "sinwavelet.h"
#include "coswavelet.h"
#include "gaussian.h"
#include <QtGui>

void gaborImage(QImage * rawImage, bool ** mask, bool bitCode[2048], bool bitCodeMask[2048]);
bool checkFilter(int radius, int theta, int filterHeight, bool ** mask);

#endif /* ifndef GABORTOIMAGE_H */

