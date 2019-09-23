#include "dialogequationview.h"
#include "ui_dialogequationview.h"

#include <QAbstractButton>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QMessageBox>
#include <QMap>

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
    name = QString("");
    setWindowTitle(title);

    equationNum = 1;
    parameters = nullptr;
    numParams = 0;

    yMax = -10;
    chartview = nullptr;

    setXvals(1.0);
    setXvalStrings(QStringList());
    resetChart(true);
    selSeries->append(0, 0);
    selSeries->append(1, 0);
//    cht->addSeries(selSeries);
    chartview->setChart(cht);

    parameterView = new DialogParameterView(this);
    parameterView->setTitle(name);
    parameterView->hide();
    connect (parameterView, SIGNAL(closed()), this, SLOT(parameterViewClosed()));
    connect (ui->pushButton_showParameters, SIGNAL(toggled(bool)), parameterView, SLOT(setVisible(bool)));
    ui->pushButton_showParameters->setChecked(false);


    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    building = false;
    waiting = false;
    updating = false;
    showBins(false);
    showJoins(false);
    showSPR(false);
    connect (ui->spinBox_SPR, SIGNAL(valueChanged(int)), this, SLOT(intVar1Changed(int)));
    intvar1 = ui->spinBox_SPR->value();
}

DialogEquationView::~DialogEquationView()
{
    parameterView->reset();
    resetChart();
    delete ui;
}

void DialogEquationView::connectAll()
{
    connect (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
//    connect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));
//    connect (this, SIGNAL(linearUpdated(float)), this, SLOT(updateLinearExp(float)));
}

void DialogEquationView::disconnectAll()
{
    disconnect (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
//    disconnect (this, SIGNAL(numbersUpdated()), this, SLOT(update()));
//    disconnect (this, SIGNAL(linearUpdated(float)), this, SLOT(updateLinearExp(float)));
}

void DialogEquationView::setXvals(double max)
{
    double val;
    int limit = static_cast<int>(max * 100.0);
    xValList.clear();
    for (int i = 0; i <= limit; i+=2)
    {
        val = static_cast<double>(i / 100.0);
        xValList.append(val);
    }
}

void DialogEquationView::setXvalStrings(const QStringList &vals)
{
    if (!vals.isEmpty()) {
        QList<float> f_vals;

        for (int i = 0; i < vals.count(); i++)
            f_vals.append(QString(vals.at(i)).toFloat());

        setXvals(f_vals.last());
    }
    setXvals(1.0);
}

// This assumes that parameters are already set
//
void DialogEquationView::setEquationNumber(int num)
{
    if (equationNum != num) {
        equationNum = num;
        setup();
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
    disconnect(parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    parameters = params;
    parameterView->setNumParamsShown(numParams);
    parameterView->setParameterTable(params);
    connect(parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
}


void DialogEquationView::setParameterHeader(int pnum, QString hdr)
{
    parameterView->setName(pnum, hdr);
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
        case 1: // case 1: null
            notYet(1);
            break;

        default:
            blank(0);
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

        switch (equationNum)
        {
        case 1: // case 1: null
            notYet(1);
            break;

        default:
            blank(0);
            break;

        }
        updating = false;
    }
    updateGrid(cht->rect());

    QDialog::update();
}

void DialogEquationView::setIntVar1Range(int min, int max) {
    ui->spinBox_SPR->setRange(min, max);
}

int DialogEquationView::getIntVar1() {
    return intvar1;
}

void DialogEquationView::setIntVar1(int value)
{
    intvar1 = value;
    if (ui->spinBox_SPR->value() != value)
        ui->spinBox_SPR->setValue(value);
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
    parameterView->reset();
    resetValues();
    update();
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
    disconnect (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    // get values from parameters
    parameterView->reset();
    connect (parameterView, SIGNAL(inputChanged()), this, SLOT(update()));
    setup();
}

void DialogEquationView::setVisible(bool visible) {
    if (visible)
    {
        restoreAll();
        connectAll();
    }
    else {
        disconnectAll();
    }
    QDialog::setVisible(visible);
}
void DialogEquationView::show() {
    restoreAll();
    connectAll();
    QDialog::show();
}

void DialogEquationView::close()
{
    disconnectAll();
    parameterView->cancel();
    hide();
    emit closed();
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

void DialogEquationView::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    parameterView->close();
    hide();
    emit closed();
}

void DialogEquationView::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    updateGrid(cht->rect());
}

void DialogEquationView::parametersChanged()
{
    update();
}

void DialogEquationView::parameterViewClosed()
{
    ui->pushButton_showParameters->setChecked(false);
}

void DialogEquationView::setupChanged()
{
    refresh();
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
        setWindowTitle(QString("%1 -  %2").arg(name, title));
}

void DialogEquationView::setLabel(const QString &value)
{
    ui->label_title->setText(value);
    ui->label_title->show();
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
        setWindowTitle(QString("%1 -  %2").arg(name, title));
}



void DialogEquationView::updateGrid(QRectF rect)
{
    int xTicks = static_cast<int>(rect.width() / 100);
    int yTicks = static_cast<int>(rect.height() / 60);

    selSeries->detachAxis(axisY);
    int yMax = static_cast<int>((maxYvalue(selSeries->points()) + 100) / 100) * 100;
//    axisXsel->setRange(0, 1);
    axisY->setRange(0, yMax);
//    selSeries->attachAxis(axisXsel);
    selSeries->attachAxis(axisY);

    xTicks = xTicks < 5? 3: xTicks;
    updateTicks(xTicks, yTicks);
}

void DialogEquationView::updateTicks(int xT, int yT)
{
    int xticks = xT;// % 2? xT: (xT + 1);
    int yticks = yT % 2? yT: (yT + 1);
    axisXsel->setTickCount(xticks);

    axisY->setTickCount(yticks);
    axisYalt->setTickCount(yticks);
}

void DialogEquationView::resetChart(bool create)
{
    showJoins(false);
    showSPR(false);
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
        chartview->hide();
        cht->hide();
        delete axisXsel;
        delete axisY;
        delete axisYalt;
        if (ascendSeries != nullptr)
            delete ascendSeries;
        if (dscendSeries != nullptr)
            delete dscendSeries;
        if (ptSeries != nullptr)
            delete ptSeries;
        delete selSeries;
        delete cht;
        delete chartview;
    }
    chartview = new QChartView(this);
    chartview->setChart(new QChart());
    cht = chartview->chart();
    ascendSeries = nullptr;
    dscendSeries = nullptr;
    ptSeries = nullptr;
    selSeries = new QLineSeries(cht);
    selSeries->setName(QString("SB"));
    cht->addSeries(selSeries);
    axisXsel = new QValueAxis();
    axisXsel->setTitleText(QString("R"));
    axisXsel->setRange(0, 1);
    cht->addAxis(axisXsel, Qt::AlignBottom);
    axisY = new QValueAxis();
    axisY->setTitleText(QString("SB"));
//    axisY->setRange(0, 1);
    cht->addAxis(axisY, Qt::AlignLeft);
    axisYalt = new QValueAxis();
    selSeries->attachAxis(axisXsel);
    selSeries->attachAxis(axisY);

    firstPoints.clear();

    yMax = 1;
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

// An additional spinBox for a value
void DialogEquationView::showSPR(bool flag)
{
    ui->spinBox_SPR->setVisible(flag);
    ui->label_SPR->setVisible(flag);
    ui->label_SPR_info->setVisible(flag);
}

// Not yet implemented
void DialogEquationView::notYet(int eqn, int num)
{
    if (eqn == 0)
        eqn = equationNum;
    if (num == 0)
        num = parameters->rowCount();
    numParams = num;
    parameterView->setNumParamsShown(numParams);

    QString msg(QString("Option %1 is not yet implemented").arg(
                       QString::number(eqn)));
    QString info("\n   NOT YET IMPLEMENTED \n   THIS PAGE INTENTIONALLY LEFT BANK");

    setTitle(msg);

    // INTENTIONALLY LEFT BLANK
    setLabel(info);

    chartview->setVisible(false);
}

// case 1: Null or error
void DialogEquationView::blank (int eqn, int rep, QString msg)
{
    QString info("\n   THIS PAGE INTENTIONALLY LEFT BLANK");
    QString numstr(QString::number(eqn > 0? eqn: equationNum));
    QString repstr(QString::number(rep));

    numParams = 0;
    parameterView->setNumParamsShown(numParams);

    // If replaced by another option
    if (rep > 0)
    {
        msg.append(QString(", use option %1 instead").arg(repstr));
        info.prepend(QString("   OPTION REPLACED WITH OPTION %1 ").arg(repstr));
    }
    // If discontinued
    else if (msg.isEmpty())
    {
        msg.append(QString("Option %1 is discontinued").arg(numstr));
        info.prepend(QString("   OPTION DISCONTINUED "));
    }
    else
    {
        msg.prepend(QString("Option %1: ").arg(numstr));
    }
    // Window title
    setTitle(msg);

    // INTENTIONALLY LEFT BLANK
    setLabel(info);

    chartview->setVisible(false);
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

// Evaluate a line between two points at a specific x value
double DialogEquationView::evaluateLine(QPointF pt1, QPointF pt2, double x)
{
    double slope = (pt2.y() - pt1.y()) / (pt2.x() - pt1.x());
    double y = slope * (x - pt2.x()) + pt2.y();
    return y;
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
double DialogEquationView::aveXvalue(const QList<float> &xvals)
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
    double value = 0.;
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
    return (value / count);
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

