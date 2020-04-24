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
#include <QDebug>
#include <cmath>
#include "irisview.h"
#include "uicircle.h"
#include "processiris.h"
#include "sinwavelet.h"
#include "median.h"
#include "gaussian.h"
#include "uiparabola.h"

IrisView::IrisView(QWidget *parent) : QWidget(parent) {
	// So you can use mouse events without having to click (i.e. hover)
	setMouseTracking(true);
    resize(parent->size());
    ignoreInput = true;
    lastSet = 0;
    lastChanged = -1;
	hoverPoint = 0;
    // Initialise UI Elements
    circle = new UiCircle(this, QPointF(0.0,0.0), 1.0);
    circle->hide();

	// NULL until created later
	iris = NULL;

	// Create the parabolas and circles
	parabola1 = new UiParabola(this);
	parabola1->hide();

	parabola2 = new UiParabola(this);
	parabola2->hide();

    outerCircle = new UiCircle(this, QPointF(0.0,0.0), 1.0);
    outerCircle->hide();

    connect(this, SIGNAL(hasBeenResized(QSize)), circle, SLOT(callToResize(QSize)));
    connect(this, SIGNAL(hasBeenResized(QSize)), outerCircle, SLOT(callToResize(QSize)));
    for (int i = 0; i < 10; i++) {
        points[i] = new UiCircle(this, QPointF(0.0,0.0), 1.0);
        points[i]->hide();
        connect(this, SIGNAL(hasBeenResized(QSize)), points[i], SLOT(callToResize(QSize)));

    } 
}

void IrisView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), img);
}

void IrisView::mousePressEvent(QMouseEvent *event) {

	if(event->button() == Qt::RightButton) {
		// Reset it all on rightclick!
		cleanUp();
		return;
	}
	// If we're not ignoring input, then set the point
	if(event->button() == Qt::LeftButton ) {
		if(lastSet >= 0 && lastSet <= 11) {
			// Find which point is within range -- if it returns -1
			// then no point is nearby
			int nearestP = nearestPoint(event->posF(), 5);
			if(nearestP < 0 && !ignoreInput) {
				points[lastSet]->change(event->posF(), 3.0);
				points[lastSet]->show();
				lastChanged = -1;
			}
			else if(nearestP >= 0) {
				// If a point is found, modify that one instead of creating
				// a new one
				points[nearestP]->change(event->posF(), 3.0);
				points[nearestP]->show();
				lastChanged = nearestP;
			}
		}
	}
	mouseMoveEvent(event);
}

void IrisView::mouseMoveEvent(QMouseEvent *event) {
	//qDebug() << img.pixelIndex(event->pos());

	// Turn the point yellow that we're hovering over or moving
	if(nearestPoint(event->posF(), 5) >= 0) {
		hoverPoint = nearestPoint(event->posF(), 5);
		points[hoverPoint]->changeColour(true);
	}
	else
		points[hoverPoint]->changeColour(false);

	if (event->buttons() == Qt::LeftButton && (!ignoreInput || lastChanged >= 0) ) {
		// If lastChanged is -1, that means we're setting a new point
		// otherwise change the point we're selecting
		if(lastChanged == -1)
			points[lastSet]->change(event->posF(), 3.0);
		else
			points[lastChanged]->change(event->posF(), 3.0);

		// General procedure: first check the lastSet value -- otherwise do whatever
		// you need for the point in the given range
		if(lastSet < 2 || (lastChanged >= 0 && lastChanged < 3 && lastSet < 3)) {
		}
		else if(lastSet == 2 || (lastChanged >= 0 && lastChanged < 3 && lastSet >= 2)) {
			drawInnerCircle();
			// Not ideal having here, but we really do need both things happen in this case
			if(lastSet >= 4)
				drawOuterCircle();
		}
		else if(lastSet == 3 || lastChanged == 3) {
			drawOuterCircle();
		}
		else if(lastChanged >= 4 && lastSet < 7) {
			// Do nothing, just modifying a random point, and no parabolas exist
		}
		else if(lastSet == 6 || (lastChanged >= 4 && lastChanged <= 6 && lastSet >= 7)) {
			parabola1->change(points[4], points[5], points[6], img.width());
			parabola1->show();
		}
		else if(lastSet == 9 || (lastChanged >= 7 && lastChanged <= 10 && lastSet >= 9)) {
			parabola2->change(points[7], points[8], points[9], img.width());
			parabola2->show();
		}
		// Generate iris strip on move -- potentially very slow! We could do this in a better way
                /*  if(lastSet >= 3) {
			if(iris)
				delete iris;
                        if (circle->radius() < outerCircle->radius()) {
                            iris = new ProcessIris(circle->centre(), circle->radius(), outerCircle->radius(), &img, this);
                            iris->fillIrisStrip(parabola1, parabola2);
                        }
                }*/
	}
}

void IrisView::mouseReleaseEvent(QMouseEvent *event) {
    // First check that an image has been loaded
    if (!img.isNull()) {
                // After dropping a point, change the colour back to green
                points[hoverPoint]->changeColour(false);
        if(event->button() == Qt::LeftButton) {
		// Let's make our life easy -- just dec lastSet if 
		// lastChanged was used, then return its value to -1
		if(lastChanged != -1) {
			lastChanged = -1;
			lastSet--;
		}
		if((lastSet == 0 || lastSet == 1))
			++lastSet;
		else if(lastSet == 2 ) {
			++lastSet;
			emit pupilFound();
		}
		else if(lastSet == 3) {
			++lastSet;
			emit irisFound();
		}
		else if((lastSet >= 4 && lastSet <= 8) || (lastChanged >= 0 && lastChanged <= 8))
			++lastSet;
		else if(lastSet == 9 ) {
			ignoreInput = true;
			emit irisComplete();
			//lastSet++;
		}
		//else if(lastSet == 10)
			//lastSet++;
		// If more than 3 points have been set, then we should be generating
		// the iris strip
		if(lastSet == 4 || lastSet == 7 || lastSet == 9) {
			if (circle->radius() < outerCircle->radius()) {
				unwrapIris();
                            }
                    }
		}
	}
}

bool IrisView::loadImage(const QString s) {
	QTime t;
	t.start();
	fileName = QFileInfo(s).baseName();
    if (img.load(s)) {
        if (img.format() != QImage::Format_Indexed8) {
            return false;
//            QImage* tempImg = new QImage(img.convertToFormat(QImage::Format_Indexed8, Qt::MonoOnly));
//            img = *tempImg;
//            img.save("converted.bmp");
//            delete tempImg;
         }
        resize(img.rect().width(), img.rect().height());
        cleanUp();
        update();
        // On load edge detect
        if (iris)
            delete iris;
        iris = new ProcessIris(QPointF(0,0),0.0,0.0, &img, this, fileName); // Making empty iris.
        emit creatingProcessIris(iris);

        QPoint pupilPoints[3];
        QPoint irisPoints[3];
		QPoint eyelidPoints[6];

        
        if (iris->findIris(pupilPoints, irisPoints)) {
            for (int i = 0; i < 3; i++) {
                points[i]->change(pupilPoints[i], 3);
                points[i]->show();
            }
            // Only uses one of the 3 from pupilPoints, Hough just likes to output all 3...
            points[3]->change(irisPoints[0], 3);
            points[3]->show();
            lastSet = 4;
            QPointF pupilCentre = calcCircCentre(pupilPoints[0],pupilPoints[1],pupilPoints[2]);
            qreal pupilRadius = calcCircRadius(pupilPoints[1], pupilCentre);
            circle->change(pupilCentre, pupilRadius);
            circle->show();
			drawOuterCircle();
            //unwrapIris();
            emit pupilFound();
			emit irisFound();
                        if(iris->findEyelids(eyelidPoints)) {
				for (int i = 0; i < 6; i++) {
					points[i+4]->change(eyelidPoints[i], 3);
					points[i+4]->show();
				}
				parabola1->change(points[4], points[5], points[6], img.width());
				parabola1->show();
				parabola2->change(points[7], points[8], points[9], img.width());
				parabola2->show();
				lastSet = 9;
                unwrapIris();
				emit irisComplete();
			}
			else {
				qDebug() << "Eyelid finding failed!";
			}
		} else {
			// Auto-detect failed
			qDebug() << "======================== Auto detection FAILED  ======================== ";
            emit autodetectFailed();
        }
		qDebug("Image loading, detection and saving took: %d ms", t.elapsed());
        return true;
    }
    return false;
}

QPointF IrisView::calcCircCentre(QPointF a, QPointF b, QPointF c) {
    qreal h, k;
    // Check proximity otherwise discrete maths fail.
    if (a == b || b == c || c == a) {
        qDebug() << "Proximity Fail!";
        return QPointF(-1,-1);
    }
    h = ((pow(a.x(),2) + pow(a.y(),2)) * (b.y()-c.y()) + (pow(b.x(),2) + pow(b.y(),2) ) * (c.y()-a.y()) + (pow(c.x(),2) +pow(c.y(),2) ) * (a.y()-b.y())) / (2 * (a.x() * b.y() - b.x() * a.y() - a.x() * c.y() + c.x() * a.y() + b.x() * c.y() - c.x() * b.y()));
    k = ((pow(a.x(),2) +pow(a.y(),2) ) * (c.x()-b.x()) + (pow(b.x(),2) +pow(b.y(),2) ) * (a.x()-c.x()) + (pow(c.x(),2) +pow(c.y(),2) ) * (b.x()-a.x()) ) / (2 * (a.x() * b.y() - b.x() * a.y() - a.x() * c.y() + c.x() * a.y() + b.x() * c.y() - c.x() * b.y()) );
    return QPointF(h,k);
}

qreal IrisView::calcCircRadius(QPointF a, QPointF c) {
    qreal r, x, y;
    x = a.x() - c.x();
    y = a.y() - c.y();
    r = sqrt(pow(x, 2) + pow(y, 2));
    return r;
}

void IrisView::cleanUp() {
	// Hide points and put them back at the origin
	// (so they don't ruin any movements in the future)
    for (int i = 0; i < 10; ++i) {
       points[i]->hide();
	   points[i]->change(QPointF(0,0), 3.0);
	}

    circle->hide();
    outerCircle->hide();
	parabola1->hide();
	parabola1->reset();
	parabola2->hide();
	parabola2->reset();
    lastSet = 0;
	lastChanged = -1;
    if(!img.isNull())
        ignoreInput = false;
    if(iris) {
		delete iris;
		iris = NULL;
	}
    emit cleanedUp(); // clear unwrapped image
}

void IrisView::resizeEvent(QResizeEvent *e) {
    emit hasBeenResized(e->size());
}

void IrisView::drawInnerCircle() {
	circleRadius = calcCircRadius(points[0]->centre(), calcCircCentre(points[0]->centre(), points[1]->centre(), points[2]->centre()));
	circle->change(calcCircCentre(points[0]->centre(), points[1]->centre(), points[2]->centre()), circleRadius );
	circle->show();
}

void IrisView::drawOuterCircle() {
	outerCircleRadius = calcCircRadius(points[3]->centre(), circle->centre());
	outerCircle->change(circle->centre(), outerCircleRadius);
	outerCircle->show();
}

void IrisView::unwrapIris() {
	if(iris)
		delete iris;
	iris = new ProcessIris(circle->centre(), circle->radius(), outerCircle->radius(), &img, this, fileName);
	iris->fillIrisStrip(parabola1, parabola2);
}

int IrisView::nearestPoint(QPointF current, int maxDistance) {
	int distance = 0;
	for(int count = 0; count <= lastSet; ++count) {
		// Distance between the mouse and each point
		distance = int(sqrt(pow(abs(current.x() - points[count]->centre().x()), 2) + pow(abs(current.y() - points[count]->centre().y()), 2)));
		// Return the first point found within the specified radius
		if(distance < maxDistance) {
			return count;
		}
	}
	// If no point found nearby, say so by returning -1
	return -1;
}

ProcessIris* IrisView::getIris() {
    return iris;
}
