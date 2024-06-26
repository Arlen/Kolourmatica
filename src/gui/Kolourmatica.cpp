/***********************************************************************
|*  Copyright (C) 2010, 2024 Arlen Avakian
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


#include <Console.hpp>

#include <QtWidgets/QApplication>

#include <iostream>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto *console = new Console;
    console->show();

    std::cout << "Kolourmatica 2024.6" << std::endl;
    std::cout << "Copyright (C) 2010-2024 Arlen Avakian" << std::endl;

    return QApplication::exec();
}
