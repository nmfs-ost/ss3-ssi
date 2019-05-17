#ifndef SREQUATIONDIALOG_H
#define SREQUATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <QAbstractButton>
#include "ttk_spline.h"


QT_CHARTS_USE_NAMESPACE

#include "population.h"

namespace Ui {
class srEquationDialog;
}

class srEquationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit srEquationDialog(QWidget *parent = nullptr);
    ~srEquationDialog();

    int getOption() const;
    void setOption(int value);

    population *getPopulation() const;
    void setPopulation(population *value);

    QString getName() const;
    void setName(const QString &value);
    QString getTitle() const;
    void setTitle(const QString &value);
    void setLabel(const QString &value);

public slots:
    void setXvals();
    void setXvalStrings(const QStringList &vals);

    void setEquationNumber (int num);
    void changeEquationNumber (int num);
//    void updateGraph ();
    void setParameters (tablemodel *params);

    void getParameterValues();
    void setParameterValues();
    void setParameterHeaders();

    void setSlider1 (double min, double max, double value);
    void slider1Changed(int value);
    void value1Changed (double value);
    void min1Changed (double value);
    void max1Changed (double value);

    void setSlider2 (double min, double max, double value);
    void slider2Changed(int value);
    void value2Changed (double value);
    void min2Changed (double value);
    void max2Changed (double value);

    void setSlider3 (double min, double max, double value);
    void slider3Changed(int value);
    void value3Changed (double value);
    void min3Changed (double value);
    void max3Changed (double value);

    void setIntVar1 (int value);
    void intVar1Changed (int value);

    void apply();
    void reset();
    void resetValues();
    void restoreAll();
    void close();

    void parametersChanged();
    void setupChanged();

    void refresh ();  // update from population values
    void setup(); // start new
    void update ();   // update from dialog values
    void resizeEvent(QResizeEvent *event);

    void buttonClicked (QAbstractButton *btn);
    void closeEvent (QCloseEvent *event);

signals:
    void numbersUpdated();
    void linearUpdated(float);
    void closed ();
    void applyingValues();
    void applyingComplete();

private:

    QString name;
    QString type;
    QString title;

    population *pop;
    int populationNum;
    int genders;

    int equationNum;
    int numSliders;
    int numParams;
    tablemodel *parameters;
    int special, male;
    bool building;
    bool waiting;
    bool updating;

    QChart *cht;
    QChartView *chartview;
    QLineSeries *ascendSeries;
    QLineSeries *dscendSeries;
    QLineSeries *selSeries;
    QScatterSeries *ptSeries;
    QList<QPointF> firstPoints;

    float yMax;

    QValueAxis *axisXsel;
    QValueAxis *axisY;
    QValueAxis *axisYalt;

    QList<double> xValList;

    double min1, max1, val1;
    double min2, max2, val2;
    double min3, max3, val3;

    int intvar1;


private slots:
    void showSliders (int num);
    void updateGrid(QRectF rect);
    void updateTicks(int xT = 11, int yT = 9);
    void resetChart(bool create = false);

    void notYet (int option = 0, int num = 0);
    void blank (int num, int rep = 0, QString msg = QString(""));

    void ricker ();
    void updateRicker ();
    void bevertonHoltStandard ();
    void updateBevertonHoltStandard ();
    void constant ();
    void updateConstant ();
    void hockeyStick ();
    void updateHockeyStick ();
    void bevertonHoltBzeroFlat ();
    void updateBevertonHoltBzeroFlat ();
    void survivorship ();
    void updateSurvivorship ();
    void shepherd ();
    void updateShepherd ();
    void shepherdReParm ();
    void updateShepherdReParm ();
    void rickerReParm ();
    void updateRickerReParm ();

    double joinFunction (double minPoss, double maxPoss, double inflec, double xVar, double y1, double y2);

    double evaluateLine (QPointF pt1, QPointF pt2, double x = 0);
    double minXvalue (const QList<QPointF> &pointlist);
    double maxXvalue (const QList<QPointF> &pointlist);
    double minYvalue (const QList<QPointF> &pointlist);
    double maxYvalue (const QList<QPointF> &pointlist);
    double aveXvalue (const QList<float> &xvals);
    double aveYvalue(const QList<QPointF> &pointlist, int start = 0, int stop = 1000);
    void fillValues (const QList<QPointF> fewpoints, QList<double> xvals, QList<QPointF> &fullpoints);
    double posfun(const double &x, const double eps, double& pen);

private:
    Ui::srEquationDialog *ui;
};

#endif // SREQUATIONDIALOG_H
