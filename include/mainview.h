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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>

class IrisView;
class UiBarChart;
class ProcessIris;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QGroupBox;
class QFrame;
class QProgressBar;
class QTabWidget;


class MainView : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    MainView(QWidget *parent);
    bool loadImage(QString fileName);


public slots:
    void enableButtons();
    //void disableButtons();
    void imageUnwrapped(QImage*, QImage*);
    void resizeIrisBox(QSize newSize);
    void cleanedUp();
    void pupilFound();
    void irisFound();
    void irisComplete();
    void autodetectFailed();
    void showProgressBar();
    void hideProgressBar(int value);
    void on_addButton_clicked();
    void on_searchButton_clicked();
    void plotHistogram(int* hist);
    void markHistogram(int x);


private slots:
    void connectProcessIris(ProcessIris* p);

signals:
    void imageLoaded();

protected:
    void changeNotes(const QString &string);
private:
    QString curFile;
    QWidget *leftWidget;
    QFrame *irisBox;
    QLabel *altimgBox;
    QLabel *heatimgBox;
    QWidget *sideBox;
    IrisView *view;
    QPixmap *altImg;
    QPixmap *heatImg;
    QPushButton *searchButton;
    QPushButton *addButton;
    QLabel *notes;
    QProgressBar *progressBar;
    QVBoxLayout *sideLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *mainLayout;
    QTabWidget *bottomBox;
    UiBarChart *barChart;

};

#endif // MAINVIEW_H
