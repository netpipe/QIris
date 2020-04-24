/****************************************************************************************
 * Copyright (c) 2010   www.projectiris.co.uk                                           *
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

#include <QApplication>
#include "mainwindow.h"
#include <QDirIterator>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include "eimage.h"

// Filters
#include "imagefilter.h"
#include "median.h"
#include "gaussianfilter.h"
#include "sobelfilterx.h"
#include "sobelfiltery.h"
#include "erosionfilter.h"
#include "dilationfilter.h"


int main(int argc, char *argv[])
{
	QDir outputDir;
	// Create output directory if it doesn't exist yet
	outputDir.mkdir("output"); 

    QApplication app(argc, argv);
    MainWindow mainWin;
	// If file given, load it automatically
	if (argc == 2)
		mainWin.loadFile(argv[1]);
	// Allow us to do some batch filtering on images
	else if(argc == 3 && QString(argv[1]) == "-b") {
		QString inputFolder = QDir::currentPath() + "/" + argv[2];
		QString outputFolder = QDir::currentPath() + "/" + argv[3];
		QDir outputDir;

		// Create output directory if it doesn't exist yet
		outputDir.mkdir(outputFolder); 
		QStringList filterList;
		// Filter to only one image per person
		filterList << "*/2/*" << "*1_2.bmp";

		// Iterate over all subdirectories, getting the right files
		QDirIterator it(inputFolder, filterList, QDir::Files, QDirIterator::Subdirectories);
		while (it.hasNext()) {
			QString currentFile = it.next();
            qDebug() << "Now converting" << QFileInfo(currentFile).baseName();
			mainWin.loadFile(currentFile);
		}
	}
	mainWin.show();
	return app.exec();
}
