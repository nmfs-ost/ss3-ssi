#include "dialogequationview.h"
#include "ui_dialogequationview.h"

#include <QAbstractButton>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QMessageBox>
#include <QMap>
#include <QDesktopWidget>
#include <QScreen>
#include <QMoveEvent>
#include <QResizeEvent>

QT_CHARTS_USE_NAMESPACE

using namespace tk;

#ifndef PRECISION
#define PRECISION .0005
#endif
#ifndef SLIDER_SCALE
#define SLIDER_SCALE 1000
#endif

#include <cmath>



DialogEquationView::DialogEquationView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEquationView)
{
    ui->setupUi(this);
    title = QString("Equation");
    name = QString("Equation View");
    setWindowTitle(title);

    if (parent != nullptr)
        position = parent->pos();
    else
        position = pos();
    window = size();

    equationNum = 1;
    numParams = 0;
    parameters = nullptr;

    building = false;
    waiting = false;
    updating = false;

    yMax = -10;
    cht = nullptr;
    chartview = nullptr;
    join1Series = nullptr;
    join2Series = nullptr;
    join3Series = nullptr;
    ascendSeries = nullptr;
    dscendSeries = nullptr;
    valSeries = nullptr;
    ptSeries = nullptr;
    resetChart(true);

    xValList.append(0);
    setXvals(0, 10, 1);
    valSeries->append(0, 0);
    valSeries->append(1, 0);

    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    showBins(false);
    showJoins(0);
    showInt1(false);

    setJoinOne(20);
    setJoinTwo(20);
    setJoinThree(20);

    connect (ui->spinBox_int1, SIGNAL(valueChanged(int)), this, SLOT(intVar1Changed(int)));
    intvar1 = ui->spinBox_int1->value();

    parameterView = new DialogParameterView(this);
    parameterView->setName("Parameters");
    parameterView->moveDelta(position);
    connect (parameterView, SIGNAL(hidden()), this, SLOT(setParametersVisible()));
    connect (ui->pushButton_showParameters, SIGNAL(toggled(bool)), this, SLOT(setParametersVisible(bool)));
    setParametersVisible();

    connectAll();
}

DialogEquationView::~DialogEquationView()
{
    delete parameterView;

    resetChart();
    delete join1Series;
    delete join2Series;
    delete join3Series;
    delete ascendSeries;
    delete dscendSeries;
    delete ptSeries;
    delete valSeries;

    delete axisX;
    delete axisY;
    delete axisYalt;

    delete cht;
    delete chartview;

    delete ui;
}

void DialogEquationView::connectAll()
{
    connect (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
}

void DialogEquationView::disconnectAll()
{
    disconnect (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
}

void DialogEquationView::setXvalsConst(const QList<double> &vals)
{
    double min, max, step;

    if (vals.isEmpty() || vals.count() < 2)
    {
        min = getBinMin();
        max = getBinMax();
        step = getBinStep();
    }
    else {
        min = vals.first();
        max = vals.last();
        step = vals.at(1) - vals.at(0);
     }
    setXvals(min, max, step);
}

void DialogEquationView::setXvalsConst(const QStringList &vals)
{
    double min, max, step;
    int count;
    if (vals.isEmpty())
    {
        min = getBinMin();
        max = getBinMax();
        step = getBinStep();
    }
    else {
        count = vals.count();
        if (count > 1) {
            min = vals.first().toDouble();
            max = vals.last().toDouble();
            step = vals.at(1).toDouble() - vals.at(0).toDouble();
        }
        else {
            min = 0.0; max = 1.0; step = 0.02;
        }
    }
    setXvals(min, max, step);
}

void DialogEquationView::setXvals(const QList<double> &vals)
{
    if (!vals.isEmpty()) {
        xValList.clear();
        for (int i = 0; i < vals.count(); i++)
            xValList.append(vals.at(i));
    }
    axisX->setRange(vals.first(), vals.last());
}

void DialogEquationView::setXvals(double min, double max, double step)
{
    double val = min;
    while ((max - min) / step < 100)
        step /= 2.0;

    xValList.clear();
    for (int i = 0; val < (max + step/2.0); i++) {
        xValList.append(val);
        val += step;
    }
    axisX->setRange(min, max);
}


void DialogEquationView::setXvalStrings(const QStringList &vals)
{
    if (!vals.isEmpty()) {
        QList<double> f_vals;

        for (int i = 0; i < vals.count(); i++)
            f_vals.append(QString(vals.at(i)).toDouble());
        setXvals(f_vals);
     }
}

// This assumes that parameters are already set
//
void DialogEquationView::setEquationNumber(int num)
{
    if (equationNum != num) {
        disconnectAll();
        parameterView->paramsChanged();
        connectAll();
        equationNum = num;
        setup();
    }
    else {
        restoreAll();
    }
}

void DialogEquationView::changeEquationNumber(int num)
{
    if (equationNum != num) {
        equationNum = num;
        setup();
    }
    else {
        update();
    }
}

void DialogEquationView::setParameters(tablemodel *params)
{
    parameters = params;
    if (params != nullptr)
    {
        disconnectAll();//(parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
        parameterView->setParameterTable(params);
        parameterView->reset();
        connectAll();//(parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    }
}


void DialogEquationView::setParameterHeader(int pnum, QString hdr)
{
    parameterView->setName(pnum, hdr);
}

void DialogEquationView::setParametersVisible(bool vis) {
    ui->pushButton_showParameters->setChecked(vis);
    if (vis) {
        parameterView->show();
        ui->pushButton_showParameters->setText("Hide Parameters");
    }
    else {
        parameterView->hide();
        ui->pushButton_showParameters->setText("Show Parameters");
    }
}

void DialogEquationView::setupLimits()
{
    limit1->setName(QString("Limit1"));
    limit1->setPen(QPen(QBrush(Qt::darkGreen), 2));
    cht->addSeries(limit1);
    limit1->attachAxis(axisX);
    limit1->attachAxis(axisYalt);
    limit2->setName(QString("Limit2"));
    limit2->setPen(QPen(QBrush(Qt::darkBlue), 2));
    cht->addSeries(limit2);
    limit2->attachAxis(axisX);
    limit2->attachAxis(axisYalt);
}
void DialogEquationView::setLimits(double xval1, double xval2)
{
    double altYmin = axisYalt->min();
    double altYmax = axisYalt->max();
    limit1->clear();
    limit2->clear();
    limit1->append(QPointF(xval1-.01, altYmin));
    limit1->append(QPointF(xval1, altYmax));
    limit2->append(QPointF(xval2, altYmin));
    limit2->append(QPointF(xval2+.01, altYmax));
}

void DialogEquationView::refresh()
{
    restoreAll();
}

void DialogEquationView::setup()
{
    if (parameters == nullptr)
        return;

    while (updating)
        continue;

    {
        updating = true;

        resetChart();

        switch (equationNum)
        {
        case 1: // case 1: Not yet implemented
            blank (NotYet, 1);
            break;

        case 2: // case 2: linear example equation
            linear();
            break;

        default:
            blank (NotUsed, 0);
            break;

        }
        updating = false;
    }
    updateGrid(cht->rect());
    QDialog::update();
}

void DialogEquationView::update()
{
    if (parameters == nullptr)
        return;

    while (updating)
        continue;

    {
        updating = true;
        setMessageVisible(false);

        switch (equationNum)
        {
        case 1: // case 1: Not yet implemented
            blank(NotYet, 1);
            break;

        case 2: // case 2: linear example equation
            updateLinear();
            break;

        }
        updating = false;
    }
    updateGrid(cht->rect());

    QDialog::update();
}

void DialogEquationView::setIntVar1Range(int min, int max) {
    ui->spinBox_int1->setRange(min, max);
}

int DialogEquationView::getIntVar1() {
    return intvar1;
}

void DialogEquationView::setIntVar1(int value)
{
    intvar1 = value;
    if (ui->spinBox_int1->value() != value)
        ui->spinBox_int1->setValue(value);
}

void DialogEquationView::setIntVar1Label(QString &txt) {
    ui->label_int1->setText(txt);
}

void DialogEquationView::setIntVar1InfoLabel(QString &txt) {
    ui->label_int1_info->setText(txt);
}

void DialogEquationView::intVar1Changed(int value)
{
    if (intvar1 != value) {
        setIntVar1 (value);
        update();
    }
}

void DialogEquationView::apply()
{
    parameterView->apply();
}

void DialogEquationView::reset()
{
    // reset parameters
    parameterView->reset();
}

void DialogEquationView::resetValues()
{
    equationNum = 1;
    numParams = parameters->rowCount();

    building = false;
    waiting = false;
    updating = false;
    yMax = 1.0;
}

void DialogEquationView::restoreAll()
{
    disconnectAll();// (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    resetValues();
    // reset parameters
    parameterView->setParameterTable(parameters);
    parameterView->reset();
    connectAll();// (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    setup();
}

void DialogEquationView::setVisible(bool visible) {
    if (visible) {
        if (!isVisible()) {
            restoreAll();
    //        connectAll();
            resize(window);
            if (position != QPoint(0, 0))
            {
                move(position);
                if (parameterView->pos().rx() == 0)
                    parameterView->pos().setX(position.rx());
            }
            updateGrid(rect());
            QDialog::setVisible(visible);
            setParametersVisible(visible);
        }
    }
    else {
        if (isVisible()) {
    //        disconnectAll();
            position = pos();
            window = size();
            QDialog::setVisible(visible);
            setParametersVisible(visible);
            emit hidden();
        }
    }
}
void DialogEquationView::show() {
    if (!isVisible())
        setVisible(true);
}

void DialogEquationView::hide() {
    if (isVisible())
        setVisible(false);
}

void DialogEquationView::close()
{
    parameterView->hide();
    hide();
}

void DialogEquationView::buttonClicked(QAbstractButton *btn)
{
    if      (btn->text().contains("Apply")) {
        apply();
    }
    else if (btn->text().contains("Reset")) {
        restoreAll();
    }
    else if (btn->text().contains("Close")) {
        close();
    }
    else if (btn->text().contains("Ok")) {
        apply();
        close();
    }
    else if (btn->text().contains("Cancel")) {
        reset();
        close();
    }
}

void DialogEquationView::closeEvent(QCloseEvent *evt)
{
    Q_UNUSED(evt);
    close();
}

void DialogEquationView::resizeEvent(QResizeEvent *event)
{
    window = size();
    updateGrid(cht->rect());
    QDialog::resizeEvent(event);
}
void DialogEquationView::moveEvent(QMoveEvent *event)
{
    position = pos();
    QDialog::moveEvent(event);

    int scr = QApplication::desktop()->screenNumber(position);
    parameterView->setScreen(scr);
}

void DialogEquationView::parametersChanged()
{
    update();
}

void DialogEquationView::parameterViewClosed()
{
    setParametersVisible (false);
}

void DialogEquationView::setupChanged()
{
    restoreAll();
}

QString DialogEquationView::getTitle() const
{
    return title;
}

void DialogEquationView::setTitle(const QString &value)
{
    title = QString(value);
    if (name.isEmpty())
        setWindowTitle(QString("%1").arg(title));
    else
        setWindowTitle(QString("%1 %2").arg(name, title));
}

void DialogEquationView::setLabel(const QString &value)
{
    ui->label_title->setText(value);
    setLabelVisible(true);
}

void DialogEquationView::setLabelVisible(bool flag) {
    ui->label_title->setVisible(flag);
}

QString DialogEquationView::getMessage() {
    return ui->label_msg->text();
}

void DialogEquationView::setMessage(const QString &value)
{
    ui->label_msg->setText(value);
    setMessageVisible(true);
}

void DialogEquationView::setMessageVisible(bool flag) {
    ui->label_msg->setVisible(flag);
}

QString DialogEquationView::getName() const
{
    return name;
}

void DialogEquationView::setName(const QString &value)
{
    name = QString(value);
    if (title.isEmpty())
        setWindowTitle(QString("%1").arg(name));
    else
        setWindowTitle(QString("%1 %2").arg(name, title));

    parameterView->setName(value);
}

void DialogEquationView::updateAxis(QValueAxis *axs, double min, double max) {
    axs->setRange(min, max);
}

void DialogEquationView::updateGrid(QRectF rect)
{
    int xTicks = static_cast<int>(rect.width() / 100);
    int yTicks = static_cast<int>(rect.height() / 60);

/*    valSeries->detachAxis(axisY);
    int yMax = static_cast<int>((maxYvalue(valSeries->points()) + 100) / 100) * 100;
//    axisXsel->setRange(0, 1);
    axisY->setRange(0, yMax);
//    selSeries->attachAxis(axisXsel);
    valSeries->attachAxis(axisY);*/

    xTicks = xTicks < 5? 3: xTicks;
    updateTicks(xTicks, yTicks);
}

void DialogEquationView::updateTicks(int xT, int yT)
{
    int xticks = xT;// % 2? xT: (xT + 1);
    int yticks = yT % 2? yT: (yT + 1);
    axisX->setTickCount(xticks);

    axisY->setTickCount(yticks);
    axisYalt->setTickCount(yticks);
}

void DialogEquationView::resetChart(bool create)
{
    setLabelVisible(false);
    setMessageVisible(false);
    showJoins(0);
    showInt1(false);
    ui->label_title->hide();
    if (chartview != nullptr)
    {
        create = false;
        chartview->setVisible(false);
        ui->verticalLayout_graph->removeWidget(chartview);
    }
    else
    {
        create = true;
    }

    if (!create)
    {
        delete axisX;
        delete axisY;
        delete axisYalt;
        delete join1Series;
        delete join2Series;
        delete join3Series;
        delete ascendSeries;
        delete dscendSeries;
        delete ptSeries;
        delete cubicSeries;
        delete valSeries;
        delete cht;
        delete chartview;
    }
    chartview = new QChartView(this);
    chartview->setChart(new QChart());
    cht = chartview->chart();

    axisX = new QValueAxis();
    axisX->setTitleText(QString("X"));
    axisX->setRange(0, 1);
    cht->addAxis(axisX, Qt::AlignBottom);
    axisY = new QValueAxis();
    axisY->setTitleText(QString("Y"));
    cht->addAxis(axisY, Qt::AlignLeft);
    axisYalt = new QValueAxis();

    join1Series = new QLineSeries(cht);
    join2Series = new QLineSeries(cht);
    join3Series = new QLineSeries(cht);
    ascendSeries = new QLineSeries(cht);
    dscendSeries = new QLineSeries(cht);
    ptSeries = new QScatterSeries(cht);
    cubicSeries = new QSplineSeries(cht);

    valSeries = new QLineSeries(cht);
    valSeries->setName(QString("Value"));
    cht->addSeries(valSeries);
    valSeries->attachAxis(axisX);
    valSeries->attachAxis(axisY);

    firstPoints.clear();

    yMax = 1;
    intvar1 = 1;

    ui->verticalLayout_graph->addWidget(chartview);
    chartview->show();
    cht->show();
    cht->legend()->setVisible(false);
    int sizew = static_cast<int>(cht->size().width());
    int sizeh = static_cast<int>(cht->size().height());
    resizeEvent(new QResizeEvent(QSize(sizew, sizeh), QSize(sizew, sizeh)));
    repaint();
}

void DialogEquationView::showBins(bool flag) {
    ui->label_bins->setVisible(flag);
    ui->label_binsMin->setVisible(flag);
    ui->spinBox_binsMin->setVisible(flag);
    ui->label_binsMax->setVisible(flag);
    ui->spinBox_binsMax->setVisible(flag);
    ui->label_binsWidth->setVisible(flag);
    ui->spinBox_binsWidth->setVisible(flag);
    ui->label_binsMidBin->setVisible(flag);
    ui->doubleSpinBox_binsMidBin->setVisible(flag);
}

int DialogEquationView::getBinMin () {
    return ui->spinBox_binsMin->value();
}

void DialogEquationView::setBinMin (int min) {
    ui->spinBox_binsMin->setValue(min);
}

int DialogEquationView::getBinMax () {
    return ui->spinBox_binsMax->value();
}

void DialogEquationView::setBinMax (int max) {
    ui->spinBox_binsMax->setValue(max);
}

int DialogEquationView::getBinStep () {
    return ui->spinBox_binsWidth->value();
}

void DialogEquationView::setBinStep (int step) {
    ui->spinBox_binsWidth->setValue(step);
}

double DialogEquationView::getMidBin() {
    return ui->doubleSpinBox_binsMidBin->value();
}

void DialogEquationView::setMidBin(double mid) {
    ui->doubleSpinBox_binsMidBin->setValue(mid);
}

void DialogEquationView::binsChanged()
{
    // Do something?
}

void DialogEquationView::showJoins(int num) {
    ui->label_steepness->hide();
    ui->label_steepJoin1->hide();
    ui->spinBox_steepJoin1->hide();
    ui->label_steepJoin2->hide();
    ui->spinBox_steepJoin2->hide();
    ui->label_steepJoin3->hide();
    ui->spinBox_steepJoin3->hide();

    switch (num) {
    case 3:
        ui->label_steepJoin3->show();
        ui->spinBox_steepJoin3->show();
        [[clang::fallthrough]];
    case 2:
        ui->label_steepJoin2->show();
        ui->spinBox_steepJoin2->show();
        [[clang::fallthrough]];
    case 1:
        ui->label_steepness->show();
        ui->label_steepJoin1->show();
        ui->spinBox_steepJoin1->show();
    }
}

int DialogEquationView::getJoinOne() {
    return ui->spinBox_steepJoin1->value();
}

void DialogEquationView::setJoinOne(int val) {
    ui->spinBox_steepJoin1->setValue(val);
}

int DialogEquationView::getJoinTwo() {
    return ui->spinBox_steepJoin2->value();
}

void DialogEquationView::setJoinTwo(int val) {
    ui->spinBox_steepJoin2->setValue(val);
}

int DialogEquationView::getJoinThree() {
    return ui->spinBox_steepJoin3->value();
}

void DialogEquationView::setJoinThree(int val) {
    ui->spinBox_steepJoin3->setValue(val);
}

QList<QPointF> DialogEquationView::getValuePoints() {
    return valSeries->points();
}

// An additional spinBox for a value
void DialogEquationView::showInt1(bool flag, QString label, QString labelinfo)
{
    ui->label_int1->setText(label);
    ui->label_int1_info->setText(labelinfo);
    ui->spinBox_int1->setVisible(flag);
    ui->label_int1->setVisible(flag);
    ui->label_int1_info->setVisible(flag);
}

// No equation for various reasons
void DialogEquationView::blank (BlankType btype, int eqn, int rep, QString msg)
{
    QString info("\n\n   THIS PAGE INTENTIONALLY LEFT BLANK");
    QString numstr(QString::number(eqn > 0? eqn: equationNum));
    QString repstr(QString::number(rep));
    QString label(QString("Option %1").arg(numstr));

    numParams = 0;
    parameterView->setNumParamsShown(numParams);
    showJoins(0);
    showBins(false);

    switch (btype) {
    case Discontinued:
        label.append(QString(" is discontinued"));
        info.prepend(QString("%1\n   OPTION DISCONTINUED ").arg(msg));
        break;

    case Replaced:
        label.append(QString(" has been replaced with option %1").arg(repstr));
        info.prepend(QString("%1\n OPTION REPLACED WITH OPTION %2 ").arg(msg, repstr));
        break;

    case NoEquation:
        label.append(tr(" has no equation."));
        info.prepend(msg);
        break;

    case NotUsed:
        label.append(tr(" is not being used. Please consult the manual."));
        info.prepend(QString("%1\n    OPTION NOT USED ").arg(msg));
        break;

    case NotYet:
        label.append(tr(" is not yet implemented. Please be patient."));
        info.prepend(QString ("%1\n OPTION NOT YET IMPLEMENTED ").arg(msg));
        numParams = rep;
        parameterView->setNumParamsShown(numParams);
        break;
    }

    // Chart label
    setLabel(label);

    // INTENTIONALLY LEFT BLANK
    setMessage(info);

    chartview->setVisible(false);
}

// sample equation logic
void DialogEquationView::linear() {
    // set name of this equation/option/method
    setLabel(QString("Linear equation as example"));

    // hide the chart temporarily
    chartview->setVisible(false);

    // set the number of parameters and their names and types (see DialogParameterView)
    numParams = 2;
    parameterView->setNumParamsShown(numParams);
    parameterView->setName(0, "Slope");
    parameterView->setType(0, "Value");
    parameterView->setName(1, "Intercept");
    parameterView->setType(1, "Value");

    // add any other elements: axisYalt, join1series, ascendSeries, etc.

    // update the equation with current values
    updateLinear();

    // show the chart
    chartview->setVisible(true);
}

// sample update logic
void DialogEquationView::updateLinear() {
    // get current values and set other values as necessary
    // including additional series and alternate y-axis
    double m = parameterView->getInput(0); // slope
    double b = parameterView->getInput(1); // intercept
    double xVal = 0;
    double yVal = 0;
    // set any message if needed
    setMessage(QString("This is an example equation - reimplement if needed"));

    // clear the previous values
    valSeries->clear();
    // compute a y for each x and add to the series
    for (int i = 0; i < xValList.count(); i++) {
        xVal = xValList.at(i);
        yVal = m * xVal + b;
        valSeries->append(xVal, yVal);
    }
    // adjust scale of axis to upper 10s
    yMax = static_cast<int>((maxYvalue(valSeries->points()) + 10) / 10) * 10;
    axisY->setRange(0, yMax);
}


// JoinFunction from tpl code:
//FUNCTION dvariable Join_Fxn(const prevariable& MinPoss, const prevariable& MaxPoss, const prevariable& Inflec, const prevariable& Xvar, const prevariable& Y1, const prevariable& Y2)
//  {
//    RETURN_ARRAYS_INCREMENT();
//  dvariable Yresult;
//  dvariable join;
//  join=1.000/(1.000+mfexp(1000.0*(Xvar-Inflec)/(MaxPoss-MinPoss)));  //  steep joiner at the inflection
//  Yresult=Y1*(join)+Y2*(1.000-join);
//    RETURN_ARRAYS_DECREMENT();
//  return Yresult;
//  }
double DialogEquationView::joinFunction(double minPoss, double maxPoss, double inflec, double xVar, double y1, double y2)
{
    double result = 0.0;
    double join;
    join = 1.0 / (1.0 + exp(1000.0 * (xVar - inflec) / (maxPoss - minPoss))); // steep joiner at the inflection
    result = y1 * (join) + y2 * (1.0 - join);
    return result;
}

// Evaluate a set of points for a specific x value
double DialogEquationView::evaluatePoints(QList<QPointF> &points, double xval) {
    double yval = 0;
    int i = 1;
    while (points.at(i).x() < xval) {
        i++;
    }
    yval = evaluateLine(points.at(i-1), points.at(i), xval);
    return yval;
}
// Evaluate a line between two points at a specific x value
double DialogEquationView::evaluateLine(QPointF pt1, QPointF pt2, double x)
{
    double xrange = pt2.x() - pt1.x();
    double yrange = pt2.y() - pt1.y();
    double x1 = x - pt1.x();
    double y1 = yrange * x1/xrange;
    return pt1.y() + y1;
}


/** Returns the minimum x-value of a point list */
double DialogEquationView::minXvalue(const QList<QPointF> &pointlist)
{
    double value = 1000.0;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).x() < value)
            value = pointlist.at(i).x();
    }
    return value;
}

/** Returns the maximum x-value of a point list */
double DialogEquationView::maxXvalue(const QList<QPointF> &pointlist)
{
    double value = -1000.0;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).x() > value)
            value = pointlist.at(i).x();
    }
    return value;
}

/** Returns the minimum y-value of a point list */
double DialogEquationView::minYvalue(const QList<QPointF> &pointlist)
{
    double value = 1000.0;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).y() < value)
            value = pointlist.at(i).y();
    }
    return value;
}

/** Returns the maximum y-value of a point list */
double DialogEquationView::maxYvalue(const QList<QPointF> &pointlist)
{
    double value = -1000;
    for (int i = 0; i < pointlist.count(); i++)
    {
        if (pointlist.at(i).y() > value)
            value = pointlist.at(i).y();
    }
    return value;
}

/** Returns the average of the values of a float list */
double DialogEquationView::aveXvalue(const QList<double> &xvals)
{
    double value = 0.;
    int i = 0;
    for (i = 0; i < xvals.count(); i++)
        value += static_cast<double>(xvals.at(i));
    value /= i;
    return value;
}

/** Returns the average of the y-values of a point list */
double DialogEquationView::aveYvalue(const QList<QPointF> &pointlist, int start, int stop)
{
    double value = 0., ave = 0.;
    double count = 0.0;
    int i = 0, end = 2;
    int listend = pointlist.count() - 1;
    if (start > listend)
        start = listend;
    if (start < 0)
        start = 0;
    if (stop > listend)
        stop = listend;
    if (stop < 0)
        stop = 0;

    i = start;
    end = stop;

    for (; i <= end; i++)
    {
        value += pointlist.at(i).y();
        count += 1.0;
    }
    ave = count > 0? value / count: value;
    return ave;
}

void DialogEquationView::fillValues(const QList<QPointF> fewpoints, QList<double> xvals, QList<QPointF> &fullpoints)
{
    int i = 0, j =0;
    double yVal = 0;
    int fplimit = fewpoints.count() - 1;

    while (xvals.at(j) < fewpoints.at(0).x())
    {
        yVal = 0.0;
        fullpoints.append(QPointF(xvals.at(j++), yVal));
    }
    for (i = 0; i < fplimit; i++)
    {
        while (j < xvals.count() && xvals.at(j) < fewpoints.at(i+1).x())
        {
            yVal = evaluateLine(fewpoints.at(i), fewpoints.at(i+1), xvals.at(j));
            fullpoints.append(QPointF(xvals.at(j++), yVal));
        }
    }
    yVal = fewpoints.last().x();
    for (; j < xvals.count(); j++)
    {
        fullpoints.append(QPointF(xvals.at(j), yVal));
    }
}

// posfun from ADMB source (more or less)
double DialogEquationView::posfun(const double &x, const double eps, double& pen)
{
  if (x >= eps)
  {
    return x;
  }
  else
  {
    pen += .01 * (x-eps * x-eps);
    return eps / (2-x/eps);
  }
}

