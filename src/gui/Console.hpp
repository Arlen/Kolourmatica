/***********************************************************************
|*  Copyright (C) 2011, 2024 Arlen Avakian
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


#pragma once

#include <ChromaticAdaptation.hpp>
#include <converter.hpp>
#include <types.hpp>

#include <QtWidgets/QWidget>

#include <tuple>
#include <vector>


class QComboBox;
class QGridLayout;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class View;


class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget* parent = nullptr);

private:
    using Input  = std::tuple<QComboBox *, QLineEdit *, QLineEdit *, QLineEdit *, QPushButton *>;
    using Output = std::tuple<QComboBox *, QLineEdit *, QLineEdit *, QLineEdit *>;

    void initWidgets();

    void setupView(QVBoxLayout *layout);

    void setupControls(QGridLayout *layout);

    void doCompute();

    void clearInput();

    void clearOutput();

    void setDoubleValidator(QLineEdit *const lineEdit);

    void setSrcRefWhiteDisabled(bool disable);

    void setDstRefWhiteDisabled(bool disable);

    QComboBox *_srwCombo{};
    QComboBox *_srwFovCombo{};
    QComboBox *_drwCombo{};
    QComboBox *_drwFovCombo{};
    View* _view;

    km::Illuminant _srw;
    km::Illuminant _drw;
    int _camIndex;
    Input _input;
    Output _output;

    std::vector<std::unique_ptr<km::ChromaticAdaptation>> _cam;
    km::Converter conv;

private slots:
    void setFrom(int index);

    void setTo(int index);

    void setSrcRefWhite(int index);

    void setDstRefWhite(int index);

    void setSrcObserver(int index);

    void setDstObserver(int index);

    void setAdaptationMethod(int index);

    void compute();
};
