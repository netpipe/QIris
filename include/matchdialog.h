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

//! MatchDialog class.
/*!
  This subclasses the QDialog class and provides a dialog when the database is searched, providing the relevant results.
  */

#ifndef MATCHDIALOG_H
#define MATCHDIALOG_H

#include <QDialog>

//Forward Declarations
class QLabel;
class QPushButton;

class MatchDialog : public QDialog {
    Q_OBJECT

public:
    MatchDialog(QWidget *parent = 0);
    // Don't seem to need *parent...
    //MatchDialog(QString _text, QWidget *parent = 0);
    MatchDialog(QString _text);
private:
    QLabel *text;
    QPushButton *okButton;

};

#endif // MATCHDIALOG_H
