/****************************************************************************************
 * Copyright (c) 2010  www.projectiris.co.uk                                            *
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
#include "../include/mainwindow.h"
#include "../include/irisview.h"
#include "../include/mainview.h"

MainWindow::MainWindow() {
    //view = new IrisView(this);
    mainView = new MainView(this);
    setCentralWidget(mainView);
    createActions();
    createMenus();
    createStatusBar();


    setWindowIcon(QIcon(":/images/ego.png"));
    setCurrentFile("");
        //setStyleSheet("QWidget { background: rgb(233,228,244) }");
}

void MainWindow::createActions() {
    openAction = new QAction("&Open Iris", this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip("Open an iris image...");
    connect(openAction, SIGNAL(triggered()),
            this, SLOT(open()));

    //preferencesAction = new QAction("&Preferences", this);
    //preferencesAction->setStatusTip("Edit preferences and settings.");

	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip("Show the Iris Recognition About Box");
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();

    menuBar()->addSeparator();

    //editMenu = menuBar()->addMenu("&Edit");
    //editMenu->addAction(preferencesAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar() {
    statusMessage = new QLabel("");
    statusMessage->setAlignment(Qt::AlignHCenter);
    statusMessage->setMinimumSize(statusMessage->sizeHint());

    statusBar()->addWidget(statusMessage);
}

void MainWindow::updateStatusBar() {
    // Change status
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Iris Image",
                                                    ".",
                                                    "Iris images (*.bmp)\nAll files (*.*)");
    if (!fileName.isEmpty())
        loadFile(fileName);

}

bool MainWindow::loadFile(const QString &fileName) {
    if(!mainView->loadImage(fileName)) {
            statusBar()->showMessage("Load failed!", 2000);
            return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage("File loaded", 2000);
    //resize(mainView->view->size());
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curFile = fileName;
    setWindowModified(false);
    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                   .arg("Iris View"));
}

QString MainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::showAbout() {
	QMessageBox::about(this, tr("About Iris Recognition"),
			tr("<h2>Iris Recognition 0.1</h2>"
				"<h4><a href='www.projectiris.co.uk'>www.projectiris.co.uk</a></h4>"
				"<p>An application enabling biometric authentication "
				"using pattern-recognition on the irides "
				"of an individual's eyes.</p>"
				"Copyright &copy; 2010 <br />"
			"Michael Boyd, Dragos Carmaciu, Francis Giannaros, <br />Thomas Payne and William Snell."
			"<br />Students at <a href='http://doc.imperial.ac.uk'>Imperial College London</a>"
			"<br />Under the supervision of Professor Duncan Gillies."
			));
}
