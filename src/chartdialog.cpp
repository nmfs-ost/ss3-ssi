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
#include <QtCore/QtMath>

QT_CHARTS_USE_NAMESPACE

#include "chartdialog.h"
#include "ui_chartdialog.h"

chartDialog::chartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chartDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Report.sso  Time_Series charts"));

    connect (ui->pushButton_refresh, SIGNAL(released()), SLOT(refreshData()));
    connect (ui->pushButton_done, SIGNAL(released()), SLOT(close()));

    reset();
}

void chartDialog::reset()
{
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
    reset();
    readData();
}

void chartDialog::readData()
{
    QStringList values;
    float xvalue = 0.0;
    float yvalue = 0.0;
    bool okay = false;
    int areaNum = 0;
    float year;
    int seas = 0;
    int index = 0;

    QFile report(QString("Report.sso"));
    if (!report.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&report);
    removeSeries();
    seasons.clear();
    seriesNames.clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
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
//                setupCheckBoxes (seriesNames);
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
//                    if (seas > 1)
//                        xvalue += seasons.at(seas - 2);

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
                            bmassSeries.at(index + i)->append(QPointF(xvalue, yvalue));
                    }
                    index = numOtherSeries * (areaNum - 1);
                    for (int i = 0; i < numOtherSeries; i++)
                    {
                        yvalue = values.at(i + 8).toFloat(&okay);
                        if (okay)
                            otherSeries.at(index + i)->append(QPointF(xvalue, yvalue));
                    }
                }
                line = stream.readLine();
            }
            if (!okay)
                break;

        }

    }
    report.close();
}

void chartDialog::createCharts(int areaNum, QStringList serNames)
{
    QChart *newcht = 0;
    QChartView *newview = 0;
    QLineSeries *newseries = 0;
    if (areaNum > bmassCharts.count())
    {
        // create layout
//        QHBoxLayout *layout = new QHBoxLayout(this);
//        cDialog->addLayout(layout);
        newcht = new QChart();
        newcht->setTitle(QString("Area %1").arg(QString::number(areaNum)));
        newcht->legend()->setAlignment(Qt::AlignRight);
        newcht->setGeometry(0, 0, 600, 600);
        newcht->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        newview = new QChartView (newcht);
        newview->setGeometry(0, 0, 600, 600);
        newview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        bmassCharts.append(newcht);
        for (int i = 0; i < 4; i++)
        {
            newseries = new QLineSeries(newcht);
            newseries->setName(serNames.at(i));
            bmassSeries.append(newseries);
            newcht->addSeries(newseries);
        }
        connectMarkers(newcht);
        newcht->createDefaultAxes();
//        layout->addWidget(newview);//(newview, areaNum, 1, 1, 1);
        ui->gridLayout->addWidget(newview, areaNum, 1);
//        addChart(newview, areaNum, 1);

        newcht = new QChart ();
        newcht->setTitle(QString("Area %1").arg(QString::number(areaNum)));
        newcht->legend()->setAlignment(Qt::AlignRight);
        newcht->setGeometry(0, 0, 600, 600);
        newcht->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        newview = new QChartView (newcht);
        newview->setGeometry(0, 0, 600, 600);
        newview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        otherCharts.append(newcht);
        for (int i = 4; i < serNames.count(); i++)
        {
            newseries = new QLineSeries(newcht);
            newseries->setName(serNames.at(i));
            otherSeries.append(newseries);
            newcht->addSeries(newseries);
        }
        connectMarkers(newcht);
        newcht->createDefaultAxes();
//        layout->addWidget(newview);
//        m_mainLayout->addWidget(newview, areaNum, 2, 1, 1);
        ui->gridLayout->addWidget(newview, areaNum, 2);
//        cDialog->addChart(newview, areaNum, 2);
  //      cDialog->setSize(800, 600*areaNum);

        if (areaNum == 1)
        {
            numBmassSeries = bmassSeries.count();
            numOtherSeries = otherSeries.count();
        }
    }
}

void chartDialog::removeSeries()
{
    QLineSeries *series;
    QChart *cht;

    while (bmassCharts.count() > 0)
    {
        cht = bmassCharts.takeLast();
        cht->removeAllSeries();
        delete cht;
    }
    while (bmassSeries.count() > 0)
    {
        series = bmassSeries.takeLast();
        delete series;
    }

    while (otherCharts.count() > 0)
    {
        cht = otherCharts.takeLast();
        cht->removeAllSeries();
        delete cht;
    }
    while (otherSeries.count() > 0)
    {
        series = otherSeries.takeLast();
        delete series;
    }

/*     // Remove all series from charts
     while (spwn_series.count() > 0) {
         series = spwn_series.last();
         spwn_chart->removeSeries(series);
         spwn_series.removeLast();
         delete series;
     }
     while (recr_series.count() > 0) {
         series = recr_series.last();
         recr_chart->removeSeries(series);
         recr_series.removeLast();
         delete series;
     }
     while (biom_series.count() > 0) {
         series = biom_series.last();
         biom_chart->removeSeries(series);
         biom_series.removeLast();
         delete series;
     }
     while (smry_series.count() > 0) {
         series = smry_series.last();
         smry_chart->removeSeries(series);
         smry_series.removeLast();
         delete series;
     }
     while (totc_series.count() > 0) {
         series = totc_series.last();
         totc_chart->removeSeries(series);
         totc_series.removeLast();
         delete series;
     }
     while (retc_series.count() > 0) {
         series = retc_series.last();
         retc_chart->removeSeries(series);
         retc_series.removeLast();
         delete series;
     }*/

}

void chartDialog::removeSeries(QChart *cht, QList<QLineSeries *>seriesList)
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


