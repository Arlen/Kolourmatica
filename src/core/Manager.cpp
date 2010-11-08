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


#include "Manager.hpp"

#include <iostream>


Manager* Manager::pManager_ = NULL;
bool Manager::deleted_ = false;


const Manager& Manager::Instance(){

  if(pManager_ == NULL){
    if(deleted_){
      std::cerr << "Manager: Dead Reference Detected!" << std::endl;
    }else{
	static Manager manager;
	pManager_ = &manager;
    }
  }
  return *pManager_;
}

void Manager::SetSystemColorSpace(CSType cs){

  system_ = cs;
}

void Manager::SetWorkingColorSpace(CSType cs){

  working_ = cs;
}


Manager::~Manager(){

  pManager_ = NULL;
  deleted_ = true;
}

Manager::Manager(){

  working_ = CSxyY;
  system_ = CSsRGB;
}
