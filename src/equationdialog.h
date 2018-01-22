#ifndef EQUATIONDIALOG_H
#define EQUATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QAbstractButton>
#include "ttk_spline.h"

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

    QString getName() const;
    void setName(const QString &value);
    QString getTitle() const;
    void setTitle(const QString &value);

public slots:
    void setSelex (selectivity *slx);
    void changeSelex ();
    void setDataModel (ss_model *data);
    void setXvals(const QStringList &vals);
    void setMidBin (float val);
//    void setAges ();

    void setEquationNumber (int num);
    void setParameters (tablemodel *params);

    void getParameterValues();
    void setParameterValues();

    void refresh ();  // update from fleet values
    void update ();   // update from dialog values
    void updateSel ();
    void resizeEvent(QResizeEvent *event);

    void setSlider1 (double min, double max, double value);
    void slider1Changed(int value);
    void value1Changed (double value);
    void min1Changed (double value);
    void max1Changed (double value);

    void setSlider2 (float min, float max, float value);
    void slider2Changed(int value);
    void value2Changed (double value);
    void min2Changed (double value);
    void max2Changed (double value);

    void setSlider3 (float min, float max, float value);
    void slider3Changed(int value);
    void value3Changed (double value);
    void min3Changed (double value);
    void max3Changed (double value);

    void setSlider4 (float min, float max, float value);
    void slider4Changed(int value);
    void value4Changed (double value);
    void min4Changed (double value);
    void max4Changed (double value);

    void setSlider5 (float min, float max, float value);
    void slider5Changed(int value);
    void value5Changed (double value);
    void min5Changed (double value);
    void max5Changed (double value);

    void setSlider6 (float min, float max, float value);
    void slider6Changed(int value);
    void value6Changed (double value);
    void min6Changed (double value);
    void max6Changed (double value);

    void setSlider7 (float min, float max, float value);
    void slider7Changed(int value);
    void value7Changed (double value);
    void min7Changed (double value);
    void max7Changed (double value);

    void setSlider8 (float min, float max, float value);
    void slider8Changed(int value);
    void value8Changed (double value);
    void min8Changed (double value);
    void max8Changed (double value);

    void setSlider9 (float min, float max, float value);
    void slider9Changed(int value);
    void value9Changed (double value);
    void min9Changed (double value);
    void max9Changed (double value);

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
    void numbersUpdated();
    void linearUpdated(float);
    void closed ();
    void applyingValues();
    void applyingComplete();

private:
    Ui::equationDialog *ui;

    QString name;
    QString title;

    Fleet *fleet;
    selectivity * selex;
    ss_model * dataModel;

    int equationNum;
    int numSliders;
    tablemodel *parameters;
    int special;
    bool building;

    QChart *cht;
    QChartView *chartview;
    QLineSeries *ascendSeries;
    QLineSeries *dscendSeries;
    QLineSeries *join1Series;
    QLineSeries *join2Series;
    QLineSeries *join3Series;
    QLineSeries *selSeries;
    QList<QPointF> firstPoints;

    float yMax;

    QValueAxis *axisXsel;
    QValueAxis *axisY;
    QValueAxis *axisYalt;

    QList<float> xValList;

    double min1, max1, val1;
    double min2, max2, val2;
    double min3, max3, val3;
    double min4, max4, val4;
    double min5, max5, val5;
    double min6, max6, val6;
    double min7, max7, val7;
    double min8, max8, val8;
    double min9, max9, val9;

    int binMin;
    int binMax;
    int binWidth;
    double binMidWidth;

    int join1;
    int join2;
    int join3;

private slots:
    void showSliders (int num);
    void showBins (bool flag);
    void showJoins (int num);
    void updateTicks(QRectF rect);
    void updateTicks(int xT = 7, int yT = 7);
    void resetChart(bool create = false);

    void blank (int num, int rep = 0, QString msg = QString(""));
    void constant (float val = 1.0);
    void updateConstant();
    void updateConstant (float val, float first, float last);
    void constantRange ();
    void updateConstantRange ();
    void logistic ();
    void updateLogistic ();
    void mirror (int sliders = 0);
    void updateMirror (int sliders = 0);
    void linear (float scale);
    void linearScaled();
    void updateLinear (float scale);
    void updateLinearScaled();
    void updateLinearExp (float scale);
    void dblLogistic ();
    void updateDblLogistic ();
    void dblLogPeak ();
    void updateDblLogPeak ();
    void updateDblLogIf ();
    void updateDblLogSmooth();
    void dblNormCasal ();
    void updateDblNormCasal ();
    void dblNormPlateau ();
    void updateDblNormPlateau ();
    void dblNormal ();
    void updateDblNormal ();
    void dblNormEndpts ();
    void updateDblNormEndpts ();
    void expLogistic ();
    void updateExpLogistic ();
    void eachAge ();
    void updateEachAge ();
    void randomWalk (float scale);
    void updateRandomWalk (float scale);
    void coleGauss ();
    void updateColeGauss();
    void cubicSpline (float scale);
    void updateCubicSpline (float scale);

    float evaluateLine(QPointF pt1, QPointF pt2, float x = 0);
    float logist (double value);
    float maxXvalue (const QList<QPointF> &pointlist);
    float minYvalue (const QList<QPointF> &pointlist);
    float maxYvalue (const QList<QPointF> &pointlist);
    float aveXvalue (const QList<float> &xvals);
    float aveYvalue (const QList<QPointF> &pointlist);
};

#endif // EQUATIONDIALOG_H
