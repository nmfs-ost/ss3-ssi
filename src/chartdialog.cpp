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

#include "chartdialog.h"
#include "ui_chartdialog.h"

chartDialog::chartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chartDialog)
{
    ui->setupUi(this);

    delete ui->verticalLayout_right;

    setWindowTitle(tr("SS_summary.sso  Plots"));

    connect (ui->pushButton_refresh, SIGNAL(released()), SLOT(refreshData()));
    connect (ui->pushButton_done, SIGNAL(released()), SLOT(close()));

    reset();
}

void chartDialog::reset()
{
    removeCharts();
    seriesNames.clear();
    seasons.clear();

    numBmassSeries = 0;
    numOtherSeries = 0;

    firstYear = 10000;
    lastYear = 0;
    firstForecastYear = 10000;

    maxBmass = 0.0;
    maxOther = 0.0;
}

chartDialog::~chartDialog()
{
    delete ui;
}

void chartDialog::refreshData()
{
    QRect qr (geometry());
    QRect tempqr (pos(), QSize(225, 140));
    reset();
    setGeometry(QRect(pos(), QSize(100, 50)));
    readData();
    resetCharts();
    setGeometry(qr);
}

void chartDialog::readData()
{
    QString line;
    QStringList values;
    QStringList title;
    float xvalue = 0.0;
    float yvalue = 0.0;
    bool okay = false;
    int areaNum = 0;
    float year;
    int seas = 0;
    int index = 0;

    QFile report(QString("ss_summary.sso"));
    if (!report.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&report);

    seriesNames << "SpwnBio" << "RecrBio" << "F_Bio" << "CtchBio";
    createCharts(1, seriesNames);

    while (!stream.atEnd())
    {
        line = stream.readLine();
        /* Reading from Report.sso
        if (line.contains("Season_Durations", Qt::CaseInsensitive))
        {
            values = line.split(" ", QString::SkipEmptyParts);
            for (int i = 1; i < values.count(); i++)
                seasons.append(values.at(i).toFloat());
        }
        if (line.contains("Time_Series", Qt::CaseInsensitive))
        {
            if (seasons.count() < 1)
                continue;
            line = stream.readLine();
            values = line.split(" ", QString::SkipEmptyParts);
            for (int i = 4; i < values.count(); i++)
            {
                seriesNames.append(values.at(i));
            }

            line = stream.readLine();
            while ((okay = !(line.contains("SPR_series", Qt::CaseInsensitive))))
            {
                values = line.split(" ", QString::SkipEmptyParts);
                if (!values.isEmpty())
                {
                    areaNum = values.at(0).toInt(&okay);
                    if (!okay)
                        break;
                    if (bmassCharts.count() < areaNum)
                    {
                        // create QCharts, QChartViews, and QLineSeries
                        createCharts(areaNum, seriesNames);
                    }
                    // find xvalue for this line
                    year = values.at(1).toFloat(&okay);
                    seas = values.at(3).toInt(&okay);
                    xvalue = year;
                    for (int s = 2; s <= seas; s++)
                        xvalue += seasons.at(s - 2);
                    if (year < firstYear)
                        firstYear = year;
                    if (year > lastYear)
                        lastYear = year;

                    if (values.at(2).contains("FORE"))
                    {
                        if (firstForecastYear > year)
                            firstForecastYear = (int)year;
                    }

                    index = numBmassSeries * (areaNum - 1);
                    for (int i = 0; i < numBmassSeries; i++)
                    {
                        yvalue = values.at(i + 4).toFloat(&okay);
                        if (okay)
                        {
                            if (yvalue > maxBmass)
                                maxBmass = yvalue;
                            bmassSeries.at(index + i)->append(QPointF(xvalue, yvalue));
                        }
                    }
                    index = numOtherSeries * (areaNum - 1);
                    for (int i = 0; i < numOtherSeries; i++)
                    {
                        yvalue = values.at(i + 8).toFloat(&okay);
                        if (okay)
                        {
                            if (yvalue > maxOther)
                                maxOther = yvalue;
                            otherSeries.at(index + i)->append(QPointF(xvalue, yvalue));
                        }
                    }
                }
                line = stream.readLine();
            }
            if (!okay)
                break;

        }*/
        if    (line.contains("SSB_"))
        {
            values = line.split(' ');
            title = values.at(0).split('_');
            xvalue = title.last().toFloat();
            yvalue = values.at(1).toFloat(&okay);
            if (okay && xvalue > 1900)
            {
                if (xvalue < firstYear)
                    firstYear = xvalue;
                if (xvalue > lastYear)
                    lastYear = xvalue;
                if (yvalue > maxBmass)
                    maxBmass = yvalue;
                bmassSeries.at(0)->append(xvalue, yvalue);
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
                bmassSeries.at(1)->append(xvalue, yvalue);
            }
        }
        else if (line.contains("F_"))
        {
            values = line.split(' ');
            title = values.at(0).split('_');
            xvalue = title.last().toFloat();
            yvalue = values.at(1).toFloat(&okay);
            if (okay && xvalue > 1900)
            {
                if (yvalue > maxBmass)
                    maxBmass = yvalue;
                bmassSeries.at(2)->append(xvalue, yvalue);
            }

        }
        else if (line.contains("TotCatch"))
        {
            values = line.split(' ');
            title = values.at(0).split('_');
            xvalue = title.last().toFloat();
            yvalue = values.at(1).toFloat(&okay);
            if (okay && xvalue > 1900)
            {
                if (yvalue > maxBmass)
                    maxBmass = yvalue;
                bmassSeries.at(3)->append(xvalue, yvalue);
            }

        }
    }
    bmassSeries[0]->setName(QString("SpwnStck"));
    bmassSeries[1]->setName(QString("SpwnRcrt"));
    bmassSeries[2]->setName(QString("Fishing"));
    bmassSeries[3]->setName(QString("TotCatch"));
    report.close();
}

void chartDialog::resetCharts()
{
    for (int i = 0; i < bmassCharts.count(); i++)
    {
        bmassCharts[i]->axisX()->setRange(firstYear, lastYear);
        bmassCharts[i]->axisY()->setRange(0, maxBmass);
        static_cast<QValueAxis *>(bmassCharts[i]->axisX())->applyNiceNumbers();
        static_cast<QValueAxis *>(bmassCharts[i]->axisY())->applyNiceNumbers();
    }
    for (int i = 0; i < otherCharts.count(); i++)
    {
        otherCharts[i]->axisX()->setRange(firstYear, lastYear);
        otherCharts[i]->axisY()->setRange(0, maxOther);
        static_cast<QValueAxis *>(otherCharts[i]->axisX())->applyNiceNumbers();
        static_cast<QValueAxis *>(otherCharts[i]->axisY())->applyNiceNumbers();
    }
}

void chartDialog::createCharts(int areaNum, QStringList serNames)
{
    QChart *newcht = 0;
    QChartView *newview = 0;
    QLineSeries *newseries = 0;
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

        if (areaNum == 1)
        {
            numBmassSeries = bmassSeries.count();
            numOtherSeries = otherSeries.count();
        }
    }
}

void chartDialog::removeCharts()
{
    QLineSeries *series;
    QChart *cht;

    while (bmassCharts.count() > 0)
    {
        for (int i = 0; i < numBmassSeries; i++)
        {
            series = bmassSeries.last();
            bmassCharts.last()->removeSeries(series);
            bmassSeries.takeLast();
            delete series;
        }
        cht = bmassCharts.takeLast();
        disconnectMarkers(cht);
        delete cht;
    }

    while (otherCharts.count() > 0)
    {
        for (int i = 0; i < numOtherSeries; i++)
        {
            series = otherSeries.last();
            otherCharts.last()->removeSeries(series);
            otherSeries.takeLast();
            delete series;
        }
        cht = otherCharts.takeLast();
        disconnectMarkers(cht);
        delete cht;
    }

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

void chartDialog::removeCharts(QChart *cht, QList<QLineSeries *>seriesList)
{
    // Remove last series from chart
    if (seriesList.count() > 0) {
        QLineSeries *series = seriesList.last();
        cht->removeSeries(series);
        seriesList.removeLast();
        delete series;
    }
}

void chartDialog::connectMarkers(QChart *cht)
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

void chartDialog::disconnectMarkers(QChart *cht)
{
//![2]
    foreach (QLegendMarker* marker, cht->legend()->markers()) {
        QObject::disconnect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
    }
//![2]
}

void chartDialog::handleMarkerClicked()
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

QLineSeries *chartDialog::division(int year, float val, QString name)
{
    QLineSeries *qls = new QLineSeries(this);
    float x1 = (float)(year);
    float x2 = seasons.last() / 2.0;
    float yr = x1 - x2;

    qls->append(QPointF(yr , 0.0));
    qls->append(QPointF(yr , val));
    qls->setName(name);
    qls->setColor(Qt::darkGray);

    return qls;
}


