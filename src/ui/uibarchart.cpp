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
#include "uibarchart.h"

UiBarChart::UiBarChart(QWidget *parent, int width, int height) : QWidget(parent) {
    resize(width,height);
    plotData = NULL;
    unitWidth = 0;
    axisMargin = 4;
    chart = new QPixmap(width - axisMargin, height);
    chart->fill(Qt::transparent);
    setupAxes();
}

void UiBarChart::plot(int* histogram, int length) {
    if (plotData)
        delete [] plotData;

    chart->fill(Qt::transparent);
    setupAxes();

    plotData = new double[length];

    unitWidth = ((size().width()) / length);
    double max = -1;

    // Get stats from hist
    for (int i = 0; i < length; i++) {
        if (histogram[i] > max)
            max = static_cast<double>(histogram[i]);
    }

    for (int i = 0; i < length; i++) {
        plotData[i] = (histogram[i] / max) * (chart->height() - 2 * axisMargin);
        drawBar((1 + (i * unitWidth) + axisMargin), chart->height() - axisMargin, unitWidth, plotData[i]);
    }
    update();
}

void UiBarChart::mark(int x) {
    // Marks a point on the histogram.
    int _x = axisMargin + 1 + x;
    int _y_bottom = axisMargin + chart->height();
    int _y_top = axisMargin;
    QPoint start(_x, _y_top);
    QPoint end(_x,_y_bottom);

    QPainter p(chart);
    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    pen.setBrush(Qt::gray);
    p.setPen(pen);
    p.drawLine(start, end);

    update();
}

void UiBarChart::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.drawPixmap(0,0, *chart);
}

void UiBarChart::setupAxes() {
    QPainter p(chart);
    p.setPen(Qt::gray);
    //p.drawLine(axisMargin, axisMargin, axisMargin, chart->height() - axisMargin);
    p.drawLine(axisMargin, chart->height() - axisMargin, chart->width() + axisMargin, chart->height() - axisMargin);
}

void UiBarChart::drawBar(double x, double y, double w, double value) {
    QPointF barOrigin(x, y-value);
    QPointF barTip(x+w, y);
    QPainter p(chart);
    p.fillRect(QRectF(barOrigin, barTip), QColor(187,235, 41, 255));
}

UiBarChart::~UiBarChart() {
    if (plotData)
        delete [] plotData;
    if (chart)
        delete chart;  //Crashes on exit? Already deleted?
}
