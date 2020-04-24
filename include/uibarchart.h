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

#ifndef UIBARCHART_H
#define UIBARCHART_H

#include <QWidget>

class QPixmap;

class UiBarChart : public QWidget {
public:
    UiBarChart(QWidget *parent = 0, int width = 320, int height = 70);
    ~UiBarChart();
    void plot(int* histogram, int length = 256);
    void mark(int x);
protected:
    void paintEvent(QPaintEvent*);
private:
    void setupAxes();
    void drawBar(double x, double y, double w, double value);
    double* plotData;
    int unitWidth;
    int axisMargin;
    QPixmap *chart;
};

#endif // UIBARCHART_H
