/***********************************************************************
|*  Copyright (C) 2010 Arlen Avakian
|*
|*  This file is part of Kolourmatica.
|*
|*  Kolourmatica is free software: you can redistribute it and/or modify
|*  it under the terms of the GNU General Public License as published by
|*  the Free Software Foundation, either version 3 of the License, or
|*  (at your option) any later version.
|*
|*  Kolourmatica is distributed in the hope that it will be useful,
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*  GNU General Public License for more details.
|*
|*  You should have received a copy of the GNU General Public License
|*  along with Kolourmatica.  If not, see <http://www.gnu.org/licenses/>.
|*
|************************************************************************/


#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QScrollArea>

#include "MainWindow.hpp"
#include "ConversionTab.hpp"


MainWindow::MainWindow() : QWidget(){

  InitWidgets();
}

void MainWindow::InitWidgets(){

  pConversion_ = new ConversionTab;

  pScrollArea_ = new QScrollArea;
  pScrollArea_->setFrameShape(QFrame::Panel);
  pScrollArea_->setWidget(pConversion_);
  pScrollArea_->setWidgetResizable(true);

  pSplitter_ = new QSplitter(Qt::Vertical);
  pSplitter_->setHandleWidth(12);
  pSplitter_->addWidget(new QWidget);
  pSplitter_->addWidget(pScrollArea_);

  pMainLayout_ = new QVBoxLayout(this);
  pMainLayout_->setContentsMargins(0, 0, 0, 0);
  pMainLayout_->setSpacing(0);
  pMainLayout_->addWidget(pSplitter_);
}
