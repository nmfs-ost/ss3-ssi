/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DIALOGSUMMARYOUTPUT_H
#define DIALOGSUMMARYOUTPUT_H

#include <QDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "dialogtable.h"


QT_CHARTS_USE_NAMESPACE

namespace Ui {
class chartDialog;
}

class dialogSummaryOutput : public QDialog
{
    Q_OBJECT

public:
    explicit dialogSummaryOutput(QWidget *parent = nullptr);
    ~dialogSummaryOutput();

    void createChart();
    void reset();
    void deleteChart();

    QString getLabel() const;
    void setLabel(const QString &value);

public slots:
    void setDirectory(QString &dir);
    void refreshData();
    void readData();
    void refreshSeries();
    void showTable(bool flag);
    void closeTable();
//    void createCharts(int areaNum, QStringList serNames);
//    void removeCharts();
//    void removeCharts(QChart *cht, QList<QLineSeries *>seriesList);

    void connectMarkers(QChart *cht);
    void disconnectMarkers(QChart *cht);
    void handleMarkerClicked();
    QLineSeries *division(int year, float val, QString name);

private:
    Ui::chartDialog *ui;

    QString label;
    tablemodel *table;
    DialogTable *tabledialog;

    QString currentDir;
    QFile reportFile;

    QSize window;
    QPoint position;

    QValueAxis *axisX;
    QValueAxis *axisY;
    QValueAxis *axisYalt;

    QList<float> seasons;

    int numBmassSeries;
    int numOtherSeries;

    int firstYear;
    int lastYear;
    int firstForecastYear;
    float maxBmass;
    float maxOther;

    QStringList seriesNames;

    QChart *summaryChart;
    QChartView *summaryView;
    QLineSeries *Spawning;
    QLineSeries *Recruits;
    QLineSeries *Fishing;
    QLineSeries *TotalCatch;
    // by area 4 series
//    QList<QChart *> bmassCharts;
//    QList<QChartView *> bmassChartViews;
//    QList<QLineSeries *> bmassSeries;


    // by area and number of columns
//    QList<QChart *> otherCharts;
//    QList<QChartView *> otherChartViews;
//    QList<QLineSeries *> otherSeries;
    void updateGrid(QRectF rect);
    void updateTicks(int xT, int yT);

private slots:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGSUMMARYOUTPUT_H
