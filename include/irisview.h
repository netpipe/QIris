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

#ifndef IRISVIEW_H
#define IRISVIEW_H

#include <QWidget>

class UiCircle;
class ProcessIris;
class UiParabola;
class IrisCode;

class IrisView : public QWidget {
    Q_OBJECT
public:
    IrisView(QWidget *parent);
    bool loadImage(const QString filename);
    void cleanUp();
    ProcessIris* getIris();
	void drawOuterCircle();
	void drawInnerCircle();
	void unwrapIris();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
signals:
    void hasBeenResized(QSize size);
    void pupilFound();
    void irisFound();
    void irisComplete();
    void autodetectFailed();
    void imageUnwrapped(QImage*, QImage*);
    void cleanedUp();
    void initialiseProgressBar(int min, int max);
    void updateProgressBar(int progress);
    void creatingProcessIris(ProcessIris* p);
private:
	QString fileName;
    QPointF calcCircCentre(QPointF a, QPointF b, QPointF c);
    qreal calcCircRadius(QPointF a, QPointF c);
	/// Tells you the point nearest to you, within maxDistance
	int nearestPoint(QPointF current, int maxDistance);
    QImage img;
    UiCircle* points[10];
    UiCircle* circle;
    UiParabola* parabola1;
    UiParabola* parabola2;
    UiCircle* outerCircle;
    ProcessIris* iris;
    // Iris new'd on the fly.
    int lastSet;
	// if we're modifying a point, this is which one it is
    int lastChanged;
	int hoverPoint;
    bool ignoreInput;
    qreal circleRadius;
    qreal outerCircleRadius;

};

#endif // IRISVIEW_H
