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
#include "irisview.h"
#include "processiris.h"
#include "mainview.h"
#include "iriscode.h"
#include "database.h"
#include "matchdialog.h"
#include "uibarchart.h"

MainView::MainView(QWidget *parent) : QWidget(parent) {

    //set up boxes for layout
    leftWidget = new QWidget(this);
    irisBox = new QFrame(leftWidget);
    irisBox->setFrameShape(QFrame::Panel);
    irisBox->setFrameShadow(QFrame::Raised);
    irisBox->setFixedSize(330,290);

    // Tabs on the bottom:
    bottomBox = new QTabWidget(leftWidget);
    bottomBox->setFixedSize(328, 108);
    bottomBox->setVisible(false);


    altimgBox = new QLabel();
    altimgBox->setMargin(2);

    heatimgBox = new QLabel();
    heatimgBox->setMargin(2);
    sideBox = new QWidget(this);

    barChart = new UiBarChart();

    view = new IrisView(irisBox);
    view->setGeometry(4,4,320,280);
    connect(view, SIGNAL(hasBeenResized(QSize)), this, SLOT(resizeIrisBox(QSize)));
    searchButton = new QPushButton("&Search Database", sideBox);
    searchButton->setDefault(true);
    searchButton->setEnabled(false);
    addButton = new QPushButton("&Add to Database", sideBox);
    addButton->setEnabled(false);
    notes = new QLabel("Use the File menu to load an iris image.", sideBox);
    notes->setWordWrap(true);
    notes->setFixedWidth(120);
    notes->setFrameShape(QFrame::Panel);
    notes->setFrameShadow(QFrame::Raised);

    //create a progress bar for the processing and then hide it until relevant
    progressBar = new QProgressBar(sideBox);
    progressBar->setVisible(false);

    sideLayout = new QVBoxLayout(sideBox);
    sideLayout->addWidget(notes);
    sideLayout->addStretch();
    sideLayout->addWidget(progressBar);
    sideLayout->addWidget(searchButton);
    sideLayout->addWidget(addButton);
    leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->addWidget(irisBox);
    leftLayout->addStretch();
    leftLayout->addWidget(bottomBox);
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(leftWidget);
    mainLayout->addStretch();
    mainLayout->addWidget(sideBox);

    // Add tabs:
    bottomBox->addTab(altimgBox, "&Unwrapped");
    bottomBox->addTab(barChart, "H&istogram");
    bottomBox->addTab(heatimgBox, "H&eatmap");

    connect(view, SIGNAL(imageUnwrapped(QImage*, QImage*)), this, SLOT(imageUnwrapped(QImage*, QImage*)));
    connect(view, SIGNAL(irisFound()), this, SLOT(enableButtons()));
    connect(view, SIGNAL(cleanedUp()), this, SLOT(cleanedUp()));
    connect(view, SIGNAL(initialiseProgressBar(int,int)), this, SLOT(showProgressBar()));
    connect(view, SIGNAL(initialiseProgressBar(int,int)), progressBar, SLOT(setRange(int,int)));
    connect(view, SIGNAL(updateProgressBar(int)), progressBar, SLOT(setValue(int)));
    connect(progressBar, SIGNAL(valueChanged(int)), this, SLOT(hideProgressBar(int)));
    //connections to keep notes section updated
    connect(view, SIGNAL(pupilFound()), this, SLOT(pupilFound()));
    connect(view, SIGNAL(irisFound()), this, SLOT(irisFound()));
    connect(view, SIGNAL(irisComplete()), this, SLOT(irisComplete()));
    connect(view, SIGNAL(autodetectFailed()), this, SLOT(autodetectFailed()));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(on_searchButton_clicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
    //connect for histogram
    connect(view, SIGNAL(creatingProcessIris(ProcessIris*)), this, SLOT(connectProcessIris(ProcessIris*)));

    setLayout(mainLayout);
        resize(sizeHint().width(), sizeHint().height() + 150);
	parent->resize(size());
        //setSizePolicy(0);
}

void MainView::changeNotes(const QString &string) {
    notes->setText(string);
}


void MainView::imageUnwrapped(QImage* image, QImage* heat) {
    altimgBox->setPixmap(altImg->fromImage(*image));
    heatimgBox->setPixmap(heatImg->fromImage(*heat));
	// Adjust box's size accordingly
    altimgBox->setFixedSize(image->width() + 9,image->height() + 9);
    heatimgBox->setFixedSize(heat->width() + 9,heat->height() + 9);
	resize(sizeHint());
	//parent->resize(size());
}

// on irisview's cleanUp(), clear the unwrapped image
// and disable the buttons again.
void MainView::cleanedUp() {
    altimgBox->setPixmap(QPixmap());
    heatimgBox->setPixmap(QPixmap());
    changeNotes("Click on three points on the outer edge of the pupil.");
    searchButton->setEnabled(false);
    addButton->setEnabled(false);
}

void MainView::resizeIrisBox(QSize newSize) {
    irisBox->setFixedSize(newSize.width() + 9, newSize.height() + 9);
}

void MainView::enableButtons() {
    searchButton->setEnabled(true);
    addButton->setEnabled(true);
}

void MainView::pupilFound() {
    changeNotes("Click on one point along the outer edge of the iris.");
}

void MainView::irisFound() {
    changeNotes("Click on three points along each of the eyelids so that the exclusion zone can be determined.");
}

void MainView::irisComplete() {
    changeNotes("Now that the iris has been identified, select from the buttons below.");
}

void MainView::autodetectFailed() {
    changeNotes("The auto-detection on this image has failed. Please right-click on the image to reset.");
}

void MainView::showProgressBar() {
    progressBar->setVisible(true);
}

void MainView::hideProgressBar(int value) {
    if (value == progressBar->maximum())
    progressBar->setVisible(false);
}

void MainView::on_addButton_clicked() {
	double hamDist;
	QString matchingIris;
	QString resultText;
	if(Database::add(curFile, view->getIris()->getBitCode(), view->getIris()->getBitCodeMask(), hamDist, matchingIris))
		resultText = "Added to database.";
	else
		resultText = "Failed to add to database because this iris is already in there!\n The matching file is: " + matchingIris;
	MatchDialog *dialog = new MatchDialog(resultText);
	dialog->show();
}

void MainView::on_searchButton_clicked() {
	double hamDist;
	QString matchingIris;
	QString resultText;
	if(Database::search(view->getIris()->getBitCode(), view->getIris()->getBitCodeMask(), hamDist, matchingIris)) {
		resultText = "Iris found in database!\n";
		resultText += "The matching file is: ";
		resultText += matchingIris + '\n';
		//resultText += "Chance of a false match is: 1 in ";
		//resultText += Database::certainty(hamDist);
	} else
		resultText = "Iris not found in database!";
	MatchDialog *dialog = new MatchDialog(resultText);
	dialog->show();
}

bool MainView::loadImage(QString fullFileName) {
	bool result = false;
	curFile = QFileInfo(fullFileName).fileName();
	result = view->loadImage(fullFileName);
	
	/*
	 * Uncomment this to make it automatically add to the database
	double hamDist;
	QString matchingIris;
	if(result) {
		if(Database::add(curFile, view->getIris()->getBitCode(), view->getIris()->getBitCodeMask(), hamDist, matchingIris))
			qDebug() << "Image added to database";
		else
			qDebug() << "Failed to add to database because this iris is already in there!\n The matching file is: " << matchingIris;
		return true;
	}
	else
		return false;
		*/
	
	return result;
}

void MainView::plotHistogram(int* hist) {
    barChart->plot(hist, 256);
}

void MainView::markHistogram(int x) {
    barChart->mark(x);
}

void MainView::connectProcessIris(ProcessIris* p) {
    connect(p, SIGNAL(buildingHistogram(int*)), this, SLOT(plotHistogram(int*)));
    connect(p, SIGNAL(markHistogram(int)), this, SLOT(markHistogram(int)));
    bottomBox->setVisible(true);
}
