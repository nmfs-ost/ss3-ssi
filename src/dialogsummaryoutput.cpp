#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>
#include <QtCharts/QLegend>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QXYLegendMarker>
#include <QtCharts/QValueAxis>
#include <QtCore/QtMath>

QT_CHARTS_USE_NAMESPACE

#include "dialogsummaryoutput.h"
#include "ui_chartdialog.h"

dialogSummaryOutput::dialogSummaryOutput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chartDialog)
{
    ui->setupUi(this);

    delete ui->verticalLayout_right;

    setWindowTitle(tr("ss_summary.sso  Plots"));

    connect (ui->pushButton_refresh, SIGNAL(released()), SLOT(refreshData()));
    connect (ui->pushButton_done, SIGNAL(released()), SLOT(close()));

    createChart();
    axisYalt->setRange(0, 1);
    if (parent == nullptr)
        position = pos();
    else
        position = QPoint(1000, 300);
    window = QSize(850, 450);
    reset();
}

void dialogSummaryOutput::createChart() {

    axisX = new QValueAxis();
    axisX->setTitleText("Year");
    axisY = new QValueAxis();
    axisY->setTitleText("Biomass");
    axisYalt = new QValueAxis();
    axisYalt->setTitleText("F_std");

    summaryChart = new QChart();
    summaryChart->setTitle("Summary Chart");
    summaryChart->addAxis(axisX, Qt::AlignBottom);
    summaryChart->addAxis(axisY, Qt::AlignLeft);
    summaryChart->addAxis(axisYalt, Qt::AlignRight);
    summaryChart->legend()->setAlignment(Qt::AlignRight);
    connectMarkers(summaryChart);
    //summaryChart->legend()->show();
    summaryView = new QChartView(summaryChart);
    ui->verticalLayout_left->addWidget(summaryView);

    Spawning = new QLineSeries();
    Spawning->setName("SpawnBio");
    summaryChart->addSeries(Spawning);
    Spawning->attachAxis(axisX);
    Spawning->attachAxis(axisY);
    Recruits = new QLineSeries();
    Recruits->setName("Recruits");
    summaryChart->addSeries(Recruits);
    Recruits->attachAxis(axisY);
    Recruits->attachAxis(axisY);
    Fishing = new QLineSeries();
    Fishing->setName("F_std");
    summaryChart->addSeries(Fishing);
    Fishing->attachAxis(axisX);
    Fishing->attachAxis(axisYalt);
    TotalCatch = new QLineSeries();
    TotalCatch->setName("TotCatch");
    summaryChart->addSeries(TotalCatch);
    TotalCatch->attachAxis(axisX);
    TotalCatch->attachAxis(axisY);
}

void dialogSummaryOutput::reset()
{
//    removeCharts();
//    seriesNames.clear();
//    seasons.clear();
    Spawning->clear();
    Recruits->clear();
    Fishing->clear();
    TotalCatch->clear();

    numBmassSeries = 0;
    numOtherSeries = 0;

    firstYear = 0;
    lastYear = 0;
    firstForecastYear = 10000;

    maxBmass = 0.0;
    maxOther = 1.0;
    setGeometry(position.rx(), position.ry(), window.rwidth(), window.rheight());
}

dialogSummaryOutput::~dialogSummaryOutput()
{
    deleteChart();
//    removeCharts();
//    delete axisX;
//    delete axisY;
//    delete axisYalt;
    delete ui;
}

void dialogSummaryOutput::deleteChart() {
    delete Spawning;
    delete Recruits;
    delete Fishing;
    delete TotalCatch;

    disconnectMarkers(summaryChart);
    delete summaryChart;
    delete summaryView;

    delete axisX;
    delete axisY;
    delete axisYalt;
}

void dialogSummaryOutput::refreshData()
{
//    QRect qr (geometry());
//    QRect tempqr (pos(), QSize(225, 140));
    reset();
//    setGeometry(QRect(pos(), QSize(100, 50)));
    readData();
    refreshSeries();
//    setGeometry(qr);
}

void dialogSummaryOutput::readData()
{
    QString line;
    QStringList values;
    QStringList title;
    int xvalue = 0.0;
    float yvalue = 0.0;
    bool okay = false;
//    int areaNum = 0;
//    float year;
//    int seas = 0;
//    int index = 0;

    QFile report(QString("ss_summary.sso"));
    if (report.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&report);

//        seriesNames << "SpwnStck" << "SpwnRcrt" << "Fishing" << "TotCatch";
//        createCharts(1, seriesNames);

        while (!stream.atEnd())
        {
            line = stream.readLine();

            if    (line.contains("SSB_"))
            {
                values = line.split(' ');
                title = values.at(0).split('_');
                xvalue = title.last().toInt();
                yvalue = values.at(1).toFloat(&okay);
                if (okay && xvalue > 1900)
                {
                    if (firstYear == 0)
                        firstYear = xvalue;
                    else if (xvalue < firstYear)
                        firstYear = xvalue;
                    if (xvalue > lastYear)
                        lastYear = xvalue;
                    if (yvalue > maxBmass)
                        maxBmass = yvalue;
                    Spawning->append(xvalue, yvalue);
                }
            }
            else if (line.contains("Recr_"))
            {
                if (line.contains("ForeRecr"))
                    continue;

                values = line.split(' ');
                title = values.at(0).split('_');
                xvalue = title.last().toFloat();
                yvalue = values.at(1).toFloat(&okay);
                if (okay && xvalue > 1900)
                {
                    if (yvalue > maxBmass)
                        maxBmass = yvalue;
                    Recruits->append(xvalue, yvalue);
                }
            }
            else if (line.contains("F_"))
            {
                values = line.split(' ');
                title = values.at(0).split('_');
                xvalue = title.last().toInt();
                yvalue = values.at(1).toFloat(&okay);
                if (okay && xvalue > 1900)
                {
                    if (yvalue > maxBmass)
                        maxBmass = yvalue;
                    Fishing->append(xvalue, yvalue);
                }

            }
            else if (line.contains("TotCatch"))
            {
                values = line.split(' ');
                title = values.at(0).split('_');
                xvalue = title.last().toInt();
                yvalue = values.at(1).toFloat(&okay);
                if (okay && xvalue > 1900)
                {
                    if (yvalue > maxBmass)
                        maxBmass = yvalue;
                    TotalCatch->append(xvalue, yvalue);
                }

            }
        }

        report.close();
    }
}

void dialogSummaryOutput::refreshSeries()
{
    axisX->setRange(firstYear, lastYear);
    axisX->applyNiceNumbers();
    axisY->setRange(0, maxBmass);
    axisY->applyNiceNumbers();
    axisYalt->setRange(0, maxOther);
    axisYalt->setTickCount(axisY->tickCount());

//    for (int i = 0; i < bmassCharts.count(); i++)
//    {
//        bmassCharts[i]->addAxis(axisX, Qt::AlignBottom);
//        bmassCharts[i]->addAxis(axisY, Qt::AlignLeft);
//        bmassCharts[i]->addAxis(axisYalt, Qt::AlignRight);
//        bmassCharts[i]->axisX()->setRange(firstYear, lastYear);
//        bmassCharts[i]->axisY()->setRange(0, maxBmass);
//        static_cast<QValueAxis *>(bmassCharts[i]->axisX())->applyNiceNumbers();
//        static_cast<QValueAxis *>(bmassCharts[i]->axisY())->applyNiceNumbers();
//    }
//    for (int i = 0; i < otherCharts.count(); i++)
//    {
//        otherCharts[i]->addAxis(axisX, Qt::AlignBottom);
//        otherCharts[i]->addAxis(axisYalt, Qt::AlignLeft);
//        otherCharts[i]->axisX()->setRange(firstYear, lastYear);
//        otherCharts[i]->axisY()->setRange(0, maxOther);
//        static_cast<QValueAxis *>(otherCharts[i]->axisX())->applyNiceNumbers();
//        static_cast<QValueAxis *>(otherCharts[i]->axisY())->applyNiceNumbers();
//    }

//    for (int i = 0; i < bmassSeries.count(); i++)
//        bmassSeries.at(i)->attachAxis(axisX);
    Spawning->attachAxis(axisX);
    Spawning->attachAxis(axisY);
    Recruits->attachAxis(axisX);
    Recruits->attachAxis(axisY);
    Fishing->attachAxis(axisX);
    Fishing->attachAxis(axisYalt);
    TotalCatch->attachAxis(axisX);
    TotalCatch->attachAxis(axisY);
}

/*void dialogSummaryOutput::createCharts(int areaNum, QStringList serNames)
{
    QChart *newcht = nullptr;
    QChartView *newview = nullptr;
    QLineSeries *newseries = nullptr;
    if (areaNum > bmassCharts.count())
    {
        // create layout
        newcht = new QChart();
        newcht->setTitle(QString("Summary Charts"));//("Area %1").arg(QString::number(areaNum)));
        newcht->legend()->setAlignment(Qt::AlignRight);
        newcht->setGeometry(0, 0, 100, 100);
        newview = new QChartView (newcht);
        bmassCharts.append(newcht);
        bmassChartViews.append(newview);
        for (int i = 0; i < 4; i++)
        {
            newseries = new QLineSeries(newcht);
            newseries->setName(serNames.at(i));
            bmassSeries.append(newseries);
            newcht->addSeries(newseries);
        }
        connectMarkers(newcht);
        newcht->createDefaultAxes();

        ui->verticalLayout_left->addWidget(newview, 1);
*/
/*        if (serNames.count() > 4)
        {
        newcht = new QChart ();
        newcht->setTitle(QString("Area %1").arg(QString::number(areaNum)));
        newcht->legend()->setAlignment(Qt::AlignRight);
        newcht->setGeometry(0, 0, 100, 100);
        newview = new QChartView (newcht);
        otherCharts.append(newcht);
        otherChartViews.append(newview);
        for (int i = 4; i < serNames.count(); i++)
        {
            newseries = new QLineSeries(newcht);
            newseries->setName(serNames.at(i));
            otherSeries.append(newseries);
            newcht->addSeries(newseries);
        }
        connectMarkers(newcht);
        newcht->createDefaultAxes();

        ui->verticalLayout_right->addWidget(newview, 1);
        }*/
/*
        if (areaNum == 1)
        {
            numBmassSeries = bmassSeries.count();
            numOtherSeries = otherSeries.count();
        }
    }
}

void dialogSummaryOutput::removeCharts()
{
    QLineSeries *series;
    QList<QLineSeries *> seriesList;
    QChart *cht;

    while (bmassCharts.count() > 0)
    {
        cht = bmassCharts.takeLast();
        disconnectMarkers(cht);
        while (cht->series().count() > 0)
        {
            series = static_cast<QLineSeries *>(cht->series().takeLast());
            cht->removeSeries(series);
            delete series;
        }
        delete cht;
    }
    bmassCharts.clear();
    bmassSeries.clear();

    while (otherCharts.count() > 0)
    {
        for (int i = 0; i < numOtherSeries; i++)
        {
            series = otherSeries.takeLast();
            otherCharts.last()->removeSeries(series);
            delete series;
        }
        cht = otherCharts.takeLast();
        disconnectMarkers(cht);
        delete cht;
    }
    otherCharts.clear();
    otherSeries.clear();

    if (!ui->verticalLayout_left->isEmpty())
    {
        for (int i = 0; i < bmassChartViews.count(); i++)
        {
            ui->verticalLayout_left->removeWidget((QWidget*)bmassChartViews.at(i));
            delete bmassChartViews.at(i);
        }
        bmassChartViews.clear();
        for (int i = 0; i < otherChartViews.count(); i++)
        {
            ui->verticalLayout_right->removeWidget((QWidget*)otherChartViews.at(i));
            delete otherChartViews.at(i);
        }
        otherChartViews.clear();
    }

}

void dialogSummaryOutput::removeCharts(QChart *cht, QList<QLineSeries *>seriesList)
{
    // Remove last series from chart
    if (seriesList.count() > 0) {
        QLineSeries *series = seriesList.last();
        cht->removeSeries(series);
        seriesList.removeLast();
        delete series;
    }
}*/

void dialogSummaryOutput::connectMarkers(QChart *cht)
{
//![1]
    // Connect all markers to handler
    foreach (QLegendMarker* marker, cht->legend()->markers()) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
        QObject::connect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
    }
//![1]
}

void dialogSummaryOutput::disconnectMarkers(QChart *cht)
{
//![2]
    foreach (QLegendMarker* marker, cht->legend()->markers()) {
        QObject::disconnect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
    }
//![2]
}

void dialogSummaryOutput::handleMarkerClicked()
{
//![3]
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible()) {
            alpha = 0.5;
        }

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

QLineSeries *dialogSummaryOutput::division(int year, float val, QString name)
{
    QLineSeries *qls = new QLineSeries(this);

//    float x1 = static_cast<float>(year);
//    float x2 = seasons.last() / 2.0;
//    float yr = x1 - x2;

    qls->append(QPointF(year, 0.0));
    qls->append(QPointF(year, val));
    qls->setName(name);
    qls->setColor(Qt::darkGray);

    return qls;
}

void dialogSummaryOutput::resizeEvent(QResizeEvent *evt) {
    window = size();
    position = pos();
    updateGrid(summaryChart->rect());
    QDialog::resizeEvent(evt);
}

void dialogSummaryOutput::moveEvent(QMoveEvent *evt) {
    position = pos();
    QDialog::moveEvent(evt);
}

void dialogSummaryOutput::updateGrid(QRectF rect)
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

void dialogSummaryOutput::updateTicks(int xT, int yT)
{
    int xticks = xT;// % 2? xT: (xT + 1);
    int yticks = yT % 2? yT: (yT + 1);
    axisX->setTickCount(xticks);

    axisY->setTickCount(yticks);
    axisYalt->setTickCount(yticks);
}


