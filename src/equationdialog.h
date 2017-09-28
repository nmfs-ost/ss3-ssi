#ifndef EQUATIONDIALOG_H
#define EQUATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QAbstractButton>

#include <qchart.h>
#include <qlineseries.h>
#include <QChartView>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE

#include "selectivity.h"
#include "fleet.h"
#include "model.h"

namespace Ui {
class equationDialog;
}

class equationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit equationDialog(QWidget *parent = 0);
    ~equationDialog();

    int getSpecial() const;
    void setSpecial(int value);

    Fleet *getFleet() const;
    void setFleet(Fleet *value);

public slots:
    void setSelex (selectivity *slx);
    void setDataModel (ss_model *data);
    void setXvals(QStringList vals);
    void setMidBin (float val);
//    void setAges ();

    void setEquationNumber (int num);
    void setParameters (tablemodel *params);

    void getParameterValues();
    void setParameterValues();

    void refresh ();  // update from fleet values
    void update ();   // update from dialog values

    void setSlider1 (float min, float max, float init);
    void slider1Changed(int value);
    void value1Changed (double value);
    void min1Changed (double value);
    void max1Changed (double value);

    void setSlider2 (float min, float max, float init);
    void slider2Changed(int value);
    void value2Changed (double value);
    void min2Changed (double value);
    void max2Changed (double value);

    void setSlider3 (float min, float max, float init);
    void slider3Changed(int value);
    void value3Changed (double value);
    void min3Changed (double value);
    void max3Changed (double value);

    void setSlider4 (float min, float max, float init);
    void slider4Changed(int value);
    void value4Changed (double value);
    void min4Changed (double value);
    void max4Changed (double value);

    void setSlider5 (float min, float max, float init);
    void slider5Changed(int value);
    void value5Changed (double value);
    void min5Changed (double value);
    void max5Changed (double value);

    void setSlider6 (float min, float max, float init);
    void slider6Changed(int value);
    void value6Changed (double value);
    void min6Changed (double value);
    void max6Changed (double value);

    void setSlider7 (float min, float max, float init);
    void slider7Changed(int value);
    void value7Changed (double value);
    void min7Changed (double value);
    void max7Changed (double value);

    void setSlider8 (float min, float max, float init);
    void slider8Changed(int value);
    void value8Changed (double value);
    void min8Changed (double value);
    void max8Changed (double value);

    void binMinChanged (int value);
    void binMaxChanged (int value);
    void binWidthChanged (int value);
    void binMidChanged (double value);

    void join1Changed (int value);
    void join2Changed (int value);
    void join3Changed (int value);

    void apply();
    void resetValues();
    void restoreAll();
    void close();

    void buttonClicked (QAbstractButton *btn);
    void closeEvent (QCloseEvent *event);

    void parametersChanged();
    void setupChanged();

signals:
    void closed ();

private:
    Ui::equationDialog *ui;

    QLabel *title;

    Fleet *fleet;
    selectivity * selex;
    ss_model * dataModel;

    int equationNum;
    tablemodel *parameters;
    int special;

    QChart *cht;
    QChartView *chartview;
    QLineSeries *firstSeries;
    QLineSeries *lastSeries;
    QLineSeries *selSeries;

    QValueAxis *axisXsel;
    QValueAxis *axisY;
    QValueAxis *axisYalt;

    QList<float> xValList;

    double min1, max1;
    double min2, max2;
    double min3, max3;
    double min4, max4;
    double min5, max5;
    double min6, max6;
    double min7, max7;
    double min8, max8;

    int binMin, binMax, binWidth;
    double binMidWidth;

    int join1, join2, join3;

    void showSliders (int num);
    void showBins (bool flag);
    void showJoins (int num);
    void blank ();
    void constant (float val);
    void linear ();
    void logistic ();

};

#endif // EQUATIONDIALOG_H
