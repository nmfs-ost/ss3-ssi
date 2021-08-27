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

    setWindowTitle(tr("ss_summary.sso  Plots"));

    connect (ui->pushButton_refresh, SIGNAL(released()), SLOT(refreshData()));
    connect (ui->pushButton_done, SIGNAL(released()), SLOT(close()));

    axisX = new QValueAxis();
    axisY = new QValueAxis();
    axisYalt = new QValueAxis();
    axisYalt->setRange(0, 4);
    if (parent == nullptr)
        position = pos();
    else
        position = parent->pos();
    window = QSize(225, 140);
    reset();
}

void chartDialog::reset()
{
    removeCharts();
    seriesNames.clear();
    seasons.clear();

    numBmassSeries = 0;
    numOtherSeries = 0;

    firstYear = 0;
    lastYear = 0;
    firstForecastYear = 10000;

    maxBmass = 0.0;
    maxOther = 4.0;
    setGeometry(position.rx(), position.ry(), window.rwidth(), window.rheight());
}

chartDialog::~chartDialog()
{
    delete axisX;
    delete axisY;
    delete axisYalt;
    delete ui;
}

void chartDialog::refreshData()
{
//    QRect qr (geometry());
//    QRect tempqr (pos(), QSize(225, 140));
    reset();
//    setGeometry(QRect(pos(), QSize(100, 50)));
    readData();
    resetCharts();
//    setGeometry(qr);
}

void chartDialog::readData()
{
    QString line, tempstring;
    QStringList values;
    QStringList title;
    float xvalue = 0.0;
    float yvalue = 0.0;
    bool okay = false;
//    int areaNum = 0;
//    float year;
//    int seas = 0;
//    int index = 0;

    QFile report(QString("ss_summary.sso"));
    if (report.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&report);

        seriesNames << "SpwnStck" << "SpwnRcrt" << "Fishing" << "TotCatch";
        createCharts(1, seriesNames);

        while (!stream.atEnd())
        {
            line = stream.readLine();

            if    (line.startsWith("#V"))
            {
                title = line.split(';', QString::SkipEmptyParts);
                tempstring = title.at(3);
                ui->label_app->setText(tempstring.replace('_', ' ').simplified());
                tempstring = QString(QString("Version %1_%2: Build date %3").arg(title.at(0)).arg(title.at(2)).arg(title.at(1)));
                ui->label_ver->setText(tempstring);
            }
            else if(line.contains("SSB_"))
            {
                values = line.split(' ');
                title = values.at(0).split('_');
                xvalue = title.last().toFloat();
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
                    bmassSeries.at(0)->append(xvalue, yvalue);
                }
            }
            else if(line.contains("Recr_"))
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
            else if(line.contains("F_"))
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
            else if(line.contains("TotCatch"))
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

        report.close();
    }
}

void chartDialog::resetCharts()
{
    axisX->setRange(firstYear, lastYear);
    axisX->applyNiceNumbers();
    axisY->setRange(0, maxBmass);
    axisY->applyNiceNumbers();
    axisYalt->setRange(0, maxOther);

    for (int i = 0; i < bmassCharts.count(); i++)
    {
        bmassCharts[i]->addAxis(axisX, Qt::AlignBottom);
        bmassCharts[i]->addAxis(axisY, Qt::AlignLeft);
        bmassCharts[i]->addAxis(axisYalt, Qt::AlignRight);
//        bmassCharts[i]->axisX()->setRange(firstYear, lastYear);
//        bmassCharts[i]->axisY()->setRange(0, maxBmass);
//        static_cast<QValueAxis *>(bmassCharts[i]->axisX())->applyNiceNumbers();
//        static_cast<QValueAxis *>(bmassCharts[i]->axisY())->applyNiceNumbers();
    }
    for (int i = 0; i < otherCharts.count(); i++)
    {
        otherCharts[i]->addAxis(axisX, Qt::AlignBottom);
        otherCharts[i]->addAxis(axisYalt, Qt::AlignLeft);
//        otherCharts[i]->axisX()->setRange(firstYear, lastYear);
//        otherCharts[i]->axisY()->setRange(0, maxOther);
//        static_cast<QValueAxis *>(otherCharts[i]->axisX())->applyNiceNumbers();
//        static_cast<QValueAxis *>(otherCharts[i]->axisY())->applyNiceNumbers();
    }

    for (int i = 0; i < bmassSeries.count(); i++)
        bmassSeries.at(i)->attachAxis(axisX);
    bmassSeries.at(0)->attachAxis(axisY);
    bmassSeries.at(1)->attachAxis(axisY);
    bmassSeries.at(2)->attachAxis(axisYalt);
    bmassSeries.at(3)->attachAxis(axisY);
}

void chartDialog::createCharts(int areaNum, QStringList serNames)
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
            ui->verticalLayout_left->removeWidget(static_cast<QWidget*>(bmassChartViews.at(i)));
            delete bmassChartViews.at(i);
        }
        bmassChartViews.clear();
        for (int i = 0; i < otherChartViews.count(); i++)
        {
            ui->verticalLayout_right->removeWidget(static_cast<QWidget*>(otherChartViews.at(i)));
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

//    float x1 = static_cast<float>(year);
//    float x2 = seasons.last() / 2.0;
//    float yr = x1 - x2;

    qls->append(QPointF(year, 0.0));
    qls->append(QPointF(year, val));
    qls->setName(name);
    qls->setColor(Qt::darkGray);

    return qls;
}

void chartDialog::resizeEvent(QResizeEvent *evt) {
    window = size();
    position = pos();
    QDialog::resizeEvent(evt);
}

void chartDialog::moveEvent(QMoveEvent *evt) {
    position = pos();
    QDialog::moveEvent(evt);
}


