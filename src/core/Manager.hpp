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


#ifndef MANAGER_HPP
#define MANAGER_HPP


class Manager{

public:
  typedef enum{ ColorSpaceXYZ, ColorSpacexyY, ColorSpaceLuv, ColorSpaceLab,
		ColorSpacesRGB } ColorSpaceType;
  static Manager& rInstance();
  ~Manager();

private:
  Manager(){ }
  Manager(const Manager& ){ }
  Manager& operator=(const Manager& ){ return *pManager_; }

  static Manager* pManager_;
  static bool deleted_;

  ColorSpaceType workingColorSpace_;
  ColorSpaceType systemColorSpace_;
  ColorSpaceType sourceColorSpace_;
};

#endif
