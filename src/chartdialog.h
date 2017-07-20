#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class chartDialog;
}

class chartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chartDialog(QWidget *parent = 0);
    ~chartDialog();

    void reset();

public slots:
    void refreshData();
    void readData();
    void createCharts(int areaNum, QStringList serNames);
    void removeSeries();
    void removeSeries(QChart *cht, QList<QLineSeries *>seriesList);

    void connectMarkers(QChart *cht);
    void disconnectMarkers(QChart *cht);
    void handleMarkerClicked();
    QLineSeries *division(int year, float val, QString name);

private:
    Ui::chartDialog *ui;

    QList<float> seasons;

    int numBmassSeries;
    int numOtherSeries;

    int firstYear;
    int lastYear;
    int firstForecastYear;
    float maxBmass;
    float maxOther;

    QStringList seriesNames;

    // by area 4 series
    QList<QChart *> bmassCharts;
    QList<QChartView *> bmassChartViews;
    QList<QLineSeries *> bmassSeries;


    // by area and number of columns
    QList<QChart *> otherCharts;
    QList<QChartView *> otherChartViews;
    QList<QLineSeries *> otherSeries;
};

#endif // CHARTDIALOG_H
