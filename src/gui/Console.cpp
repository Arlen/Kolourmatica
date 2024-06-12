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


#include <Console.hpp>
#include <Illuminant.hpp>
#include <RGB.hpp>
#include <View.hpp>
#include <enums.hpp>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>


namespace
{
    QString viewButtonText(QString text, bool flag)
    {
        if (flag) {
            QString pattern = QString("%1  %2  %3");
            return pattern
                .arg(QString().fill(':', 8))
                .arg(text.toUpper())
                .arg(QString().fill(':', 8));
        }

        return text;
    }
}

Console::Console(QWidget* parent) : QWidget(parent)
{
    _cam.push_back(std::make_unique<km::XYZScalingCA>());
    _cam.push_back(std::make_unique<km::VonKriesCA>());
    _cam.push_back(std::make_unique<km::BradfordCA>());
    _cam.push_back(std::make_unique<km::BypassCA>());

    _camIndex = 2;

    initWidgets();
    setAdaptationMethod(_camIndex);

    /// NOTE:
    /// QCombobox::currentIndexChanged will not trigger if setCurrentIndex() is
    /// given the same value as the current index; the current index value by
    /// this point is zero.
    /// setting current index to 3, which is D50, so that _srw and _drw get initialized.
    _srwCombo->setCurrentIndex(3);
    _drwCombo->setCurrentIndex(3);
}

void Console::initWidgets()
{
    auto *layoutA = new QVBoxLayout(this);
    layoutA->setContentsMargins(0, 0, 0, 0);
    layoutA->setSpacing(0);

    /// adding views.
    setupViews(layoutA);

    /// adding view buttons.
    auto *layoutB1 = new QHBoxLayout;
    setupViewButtons(layoutB1);
    layoutA->addLayout(layoutB1);

    connect(_front,  &QPushButton::toggled, [this]{ _frontView->setVisible(_front->isChecked());   });
    connect(_left,   &QPushButton::toggled, [this]{ _leftView->setVisible(_left->isChecked());     });
    connect(_right,  &QPushButton::toggled, [this]{ _rightView->setVisible(_right->isChecked());   });
    connect(_top,    &QPushButton::toggled, [this]{ _topView->setVisible(_top->isChecked());       });
    connect(_bottom, &QPushButton::toggled, [this]{ _bottomView->setVisible(_bottom->isChecked()); });

    /// adding controls.
    auto *widget = new QWidget;
    auto *layoutB2 = new QGridLayout(widget);
    setupControls(layoutB2);
    layoutA->addWidget(widget);
    layoutA->setAlignment(widget, Qt::AlignHCenter);

    clearInput();
    clearOutput();
}

void Console::setupViews(QVBoxLayout *layout)
{
    auto* scene = new QGraphicsScene(this);
    scene->setSceneRect(-1000, -1000, 2000, 2000);

    auto* view = new QGraphicsView(scene);
    view->setFrameShape(QFrame::NoFrame);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setBackgroundBrush(QBrush(QColor(32, 32, 32, 255)));

    _frontView  = new View(View::Side::Front);
    _leftView   = new View(View::Side::Left);
    _rightView  = new View(View::Side::Right);
    _topView    = new View(View::Side::Top);
    _bottomView = new View(View::Side::Bottom);

    _frontView->setVisible(false);
    _leftView->setVisible(false);
    _rightView->setVisible(false);
    _topView->setVisible(false);
    _bottomView->setVisible(false);

    scene->addItem(_frontView);
    scene->addItem(_leftView);
    scene->addItem(_rightView);
    scene->addItem(_topView);
    scene->addItem(_bottomView);

    layout->addWidget(view);
}

void Console::setupViewButtons(QHBoxLayout *layout)
{
    _front  = new QPushButton("Front");
    _left   = new QPushButton("Left");
    _right  = new QPushButton("Right");
    _top    = new QPushButton("Top");
    _bottom = new QPushButton("Bottom");

    _front->setDisabled(true);
    _left->setDisabled(true);
    _right->setDisabled(true);
    _top->setDisabled(true);
    _bottom->setDisabled(true);

    _front->setCheckable(true);
    _left->setCheckable(true);
    _right->setCheckable(true);
    _top->setCheckable(true);
    _bottom->setCheckable(true);

    layout->addWidget(_front);
    layout->addWidget(_left);
    layout->addWidget(_right);
    layout->addWidget(_top);
    layout->addWidget(_bottom);

    connect(_front,  &QPushButton::toggled, [this]{ _front->setText(viewButtonText("Front", _front->isChecked()));    });
    connect(_left,   &QPushButton::toggled, [this]{ _left->setText(viewButtonText("Left", _left->isChecked()));       });
    connect(_right,  &QPushButton::toggled, [this]{ _right->setText(viewButtonText("Right", _right->isChecked()));    });
    connect(_top,    &QPushButton::toggled, [this]{ _top->setText(viewButtonText("Top", _top->isChecked()));          });
    connect(_bottom, &QPushButton::toggled, [this]{ _bottom->setText(viewButtonText("Bottom", _bottom->isChecked())); });
}

void Console::setupControls(QGridLayout *layout)
{
    layout->setContentsMargins(0, 2, 0, 2);
    layout->setSpacing(1);

    QLabel *label1 = new QLabel(" From ");
    QLabel *label2 = new QLabel(" To ");

    /// the input line.
    get<0>(_input) = new QComboBox;
    get<0>(_input)->setFixedSize(144, 28);
    get<1>(_input) = new QLineEdit;
    get<1>(_input)->setFixedSize(128, 28);
    setDoubleValidator(get<1>(_input));
    get<2>(_input) = new QLineEdit;
    get<2>(_input)->setFixedSize(128, 28);
    setDoubleValidator(get<2>(_input));
    get<3>(_input) = new QLineEdit;
    get<3>(_input)->setFixedSize(128, 28);
    setDoubleValidator(get<3>(_input));
    get<4>(_input) = new QPushButton("==");
    get<4>(_input)->setFixedSize(32, 28);
    connect(get<4>(_input), SIGNAL(clicked()), this, SLOT(compute()));
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 0, 0);
    layout->addWidget(label1, 0, 1);
    layout->addWidget(get<0>(_input), 0, 2);
    layout->addWidget(get<1>(_input), 0, 3);
    layout->addWidget(get<2>(_input), 0, 4);
    layout->addWidget(get<3>(_input), 0, 5);
    layout->addWidget(get<4>(_input), 0, 6);
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 0, 7);

    /// the output line.
    get<0>(_output) = new QComboBox;
    get<0>(_output)->setFixedSize(144, 28);
    get<1>(_output) = new QLineEdit;
    get<1>(_output)->setFixedSize(128, 28);
    setDoubleValidator(get<1>(_output));
    get<2>(_output) = new QLineEdit;
    get<2>(_output)->setFixedSize(128, 28);
    setDoubleValidator(get<2>(_output));
    get<3>(_output) = new QLineEdit;
    get<3>(_output)->setFixedSize(128, 28);
    setDoubleValidator(get<3>(_output));
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 1, 0);
    layout->addWidget(label2, 1, 1);
    layout->addWidget(get<0>(_output), 1, 2);
    layout->addWidget(get<1>(_output), 1, 3);
    layout->addWidget(get<2>(_output), 1, 4);
    layout->addWidget(get<3>(_output), 1, 5);
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 1, 6);

    /// adding colour space names to the QComboBoxes in input and output lines.
    for (const auto &cs: enums::ColourSpaceArray) {
        get<0>(_input)->addItem(enums::to_string(cs));
        get<0>(_output)->addItem(enums::to_string(cs));
    }

    get<0>(_input)->setCurrentIndex(0);
    connect(get<0>(_input), &QComboBox::currentIndexChanged, this, &Console::setFrom);
    for (View *view: {_frontView, _leftView, _rightView, _topView, _bottomView}) {
        connect(get<0>(_input), &QComboBox::currentIndexChanged, view, &View::setFrom);
    }

    get<0>(_output)->setCurrentIndex(1);
    connect(get<0>(_output), &QComboBox::currentIndexChanged, this, &Console::setTo);
    for (View *view: {_frontView, _leftView, _rightView, _topView, _bottomView}) {
        connect(get<0>(_output), &QComboBox::currentIndexChanged, view, &View::setTo);
    }

    /// the reference white and chromatic adaptation lines.
    auto *label3 = new QLabel(" Source Ref. White ");
    auto *label4 = new QLabel(" Destination Ref. White ");
    auto *label5 = new QLabel(" Chromatic Adaptation ");
    _srwCombo = new QComboBox;
    _srwCombo->setFixedSize(144, 28);
    _srwFovCombo = new QComboBox;
    _srwFovCombo->setFixedSize(92, 28);
    _drwCombo = new QComboBox;
    _drwCombo->setFixedSize(144, 28);
    _drwFovCombo = new QComboBox;
    _drwFovCombo->setFixedSize(92, 28);
    auto *comboBox5 = new QComboBox;
    comboBox5->setFixedSize(144, 28);

    /// source reference white line.
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 2, 0);
    layout->addWidget(label3, 2, 1);
    layout->addWidget(_srwCombo, 2, 2);
    layout->addWidget(_srwFovCombo, 2, 3);
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 2, 6);

    /// destination reference white line.
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 3, 0);
    layout->addWidget(label4, 3, 1);
    layout->addWidget(_drwCombo, 3, 2);
    layout->addWidget(_drwFovCombo, 3, 3);
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 3, 6);

    /// chromatic adaptation line.
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 4, 0);
    layout->addWidget(label5, 4, 1);
    layout->addWidget(comboBox5, 4, 2);
    layout->addItem(new QSpacerItem(128, 1,
                                    QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Ignored), 4, 6);

    for (const auto &rw: enums::RefWhiteArray) {
        _srwCombo->addItem(enums::to_string(rw));
        _drwCombo->addItem(enums::to_string(rw));
    }

    connect(_srwCombo, &QComboBox::currentIndexChanged, this, &Console::setSrcRefWhite);
    for (View *view: {_frontView, _leftView, _rightView, _topView, _bottomView}) {
        connect(_srwCombo, &QComboBox::currentIndexChanged, view, &View::setSrcRefWhite);
    }

    connect(_drwCombo, &QComboBox::currentIndexChanged, this, &Console::setDstRefWhite);
    for (View *view: {_frontView, _leftView, _rightView, _topView, _bottomView}) {
        connect(_drwCombo, &QComboBox::currentIndexChanged, view, &View::setDstRefWhite);
    }

    _srwFovCombo->addItem(" 1931 2" + QString(QChar(730)));
    _srwFovCombo->addItem(" 1964 10" + QString(QChar(730)));
    connect(_srwFovCombo, &QComboBox::currentIndexChanged, this, &Console::setSrcObserver);
    for (View *view: {_frontView, _leftView, _rightView, _topView, _bottomView}) {
        connect(_srwFovCombo, &QComboBox::currentIndexChanged, view, &View::setSrcObserver);
    }

    _drwFovCombo->addItem(" 1931 2" + QString(QChar(730)));
    _drwFovCombo->addItem(" 1964 10" + QString(QChar(730)));
    connect(_drwFovCombo, &QComboBox::currentIndexChanged, this, &Console::setDstObserver);
    for (View *view: {_frontView, _leftView, _rightView, _topView, _bottomView}) {
        connect(_drwFovCombo, &QComboBox::currentIndexChanged, view, &View::setDstObserver);
    }

    for (const char *s: {"XYZ Scaling", "Von Kries", "Bradford", "None"}) {
        comboBox5->addItem(s);
    }
    comboBox5->setCurrentIndex(_camIndex);
    connect(comboBox5, &QComboBox::currentIndexChanged, this, &Console::setAdaptationMethod);

    for (View *view: {_frontView, _leftView, _rightView, _topView, _bottomView}) {
        connect(comboBox5, &QComboBox::currentIndexChanged, view, &View::setAdaptationMethod);
    }
}

void Console::doCompute()
{
    // Matrix3 CAM = chromaticAdaptationMatrix(SRC_RW, DST_RW, METHOD);
    // XYZ.from(SRC, SRC_RW);
    // XYZ = CAM * XYZ
    // SRC.from(XYZ, DST_RW);
    // SRC

    km::XYZ xyz;
    Point3d tri;

    tri(0) = get<1>(_input)->text().toFloat();
    tri(1) = get<2>(_input)->text().toFloat();
    tri(2) = get<3>(_input)->text().toFloat();

    const auto toIndex   = get<0>(_input)->currentIndex();
    const auto fromIndex = get<0>(_output)->currentIndex();

    conv.set_colour_value(static_cast<enums::ColourSpace>(fromIndex), tri, _srw);
    xyz.tri = conv.to_XYZ(static_cast<enums::ColourSpace>(fromIndex)).tri;
    Eigen::Matrix3d cam = _cam[_camIndex]->apply(_srw, _drw);
    xyz.tri = cam * xyz.tri;
    tri = conv.from_XYZ(static_cast<enums::ColourSpace>(toIndex), xyz, _drw);

    get<1>(_output)->setText(QString::number(tri(0), 'f', 10));
    get<2>(_output)->setText(QString::number(tri(1), 'f', 10));
    get<3>(_output)->setText(QString::number(tri(2), 'f', 10));
}

void Console::clearInput()
{
    get<1>(_input)->setText(QString::number(0.0, 'f', 10));
    get<2>(_input)->setText(QString::number(0.0, 'f', 10));
    get<3>(_input)->setText(QString::number(0.0, 'f', 10));
}

void Console::clearOutput()
{
    get<1>(_output)->setText(QString::number(0.0, 'f', 10));
    get<2>(_output)->setText(QString::number(0.0, 'f', 10));
    get<3>(_output)->setText(QString::number(0.0, 'f', 10));
}

void Console::setDoubleValidator(QLineEdit *const lineEdit)
{
    auto *doubleValidator = new QDoubleValidator(lineEdit);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    doubleValidator->setDecimals(10);
    lineEdit->setValidator(doubleValidator);
}

void Console::setSrcRefWhiteDisabled(bool disable)
{
    /// if reference white is disabled, so is the observer year.

    /*
      TODO:
      Right before disabling the comboboxes, set the comboboxes to the current
      source ref white.
     */
    _srwCombo->setDisabled(disable);
    _srwFovCombo->setDisabled(disable);
}

void Console::setDstRefWhiteDisabled(bool disable)
{
    /// if reference white is disabled, so is the observer year.

    /*
      TODO:
      ditto
    */
    _drwCombo->setDisabled(disable);
    _drwFovCombo->setDisabled(disable);
}

void Console::setFrom(int fromIndex)
{
    clearInput();

    const bool disabled = fromIndex >= 6 && fromIndex <= 21;
    setSrcRefWhiteDisabled(disabled);

    if (disabled) {
        /// if disabled, it means an RGB colour space has been selected,
        /// which has it's own defined illuminant
        _srw = conv.get_rgb_illuminant(static_cast<enums::ColourSpace>(fromIndex));
    }
}

void Console::setTo(int toIndex)
{
    clearOutput();

    const bool disabled = toIndex >= 6 && toIndex <= 21;
    setDstRefWhiteDisabled(disabled);

    if (disabled) {
        /// if disabled, it means an RGB colour space has been selected,
        /// which has it's own defined illuminant
        _drw = conv.get_rgb_illuminant(static_cast<enums::ColourSpace>(toIndex));
    }

    if (!disabled) {
        _front->setChecked(false);
        _left->setChecked(false);
        _right->setChecked(false);
        _top->setChecked(false);
        _bottom->setChecked(false);
    }

    _front->setDisabled(!disabled);
    _left->setDisabled(!disabled);
    _right->setDisabled(!disabled);
    _top->setDisabled(!disabled);
    _bottom->setDisabled(!disabled);
}

void Console::setSrcRefWhite(int index)
{
    clearOutput();
    const auto srwFovIndex = _srwFovCombo->currentIndex();
    _srw = km::get_illuminant(static_cast<enums::RefWhite>(index), static_cast<enums::RefWhiteFov>(srwFovIndex));
}

void Console::setDstRefWhite(int index)
{
    clearOutput();
    const auto drwFovIndex = _drwFovCombo->currentIndex();
    _drw = km::get_illuminant(static_cast<enums::RefWhite>(index), static_cast<enums::RefWhiteFov>(drwFovIndex));
}

void Console::setSrcObserver(int index)
{
    clearOutput();
    const auto srwIndex = _srwCombo->currentIndex();
    _srw = km::get_illuminant(static_cast<enums::RefWhite>(srwIndex), static_cast<enums::RefWhiteFov>(index));
}

void Console::setDstObserver(int index)
{
    clearOutput();
    const auto drwIndex = _drwCombo->currentIndex();
    _drw = km::get_illuminant(static_cast<enums::RefWhite>(drwIndex), static_cast<enums::RefWhiteFov>(index));
}

void Console::setAdaptationMethod(int index)
{
    clearOutput();
    _camIndex = index;
}

void Console::compute() { doCompute(); }
