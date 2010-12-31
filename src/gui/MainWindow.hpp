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


#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui/QWidget>

class ConversionConsole;
class View;

class QVBoxLayout;
class QSplitter;
class QTabWidget;


class MainWindow : public QWidget{

public:
  MainWindow();

private:
  void initWidgets();

  QVBoxLayout* pMainLayout_;
  QSplitter* pSplitter_;
  ConversionConsole* pConversion_;
  View* pView_;
};

#endif
