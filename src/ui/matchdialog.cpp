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

#include "matchdialog.h"
#include <QtGui>

MatchDialog::MatchDialog(QString _text) {
    text = new QLabel(_text);
    okButton = new QPushButton("OK");
    okButton->setDefault(true);
    okButton->setEnabled(true);

    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(text);
    mainLayout->addWidget(okButton);
    setLayout(mainLayout);

    setWindowTitle("Database Search");
    setFixedSize(sizeHint());
}
