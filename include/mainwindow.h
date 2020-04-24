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

//! MainWindow class.
/*!
  This subclasses the Qt QMainWindow class and provides the cross-platform window application that is presented to the end-user.  It is mostly concerned with setting up Qt window functionality (menus and bars etc.) and utilises the MainView class as the central widget.
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward class declarations.
class IrisView;
class MainView;
class QAction;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow {
    //! Q_OBJECT is a macro used by Qt to implement its alternative to callback functions (signals and slots).
    Q_OBJECT
public:
    MainWindow();
    bool loadFile(const QString &fileName);

private slots:
    void open();
    void updateStatusBar();
    void showAbout();

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    IrisView *view;
    MainView *mainView;
    QString curFile;
    QAction *separatorAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *openAction;
    QAction *preferencesAction;
    QAction *aboutAction;

    QLabel *statusMessage;

};

#endif // MAINWINDOW_H
