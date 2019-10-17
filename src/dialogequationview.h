#ifndef DIALOGEQUATIONVIEW_H
#define DIALOGEQUATIONVIEW_H

#include <QDialog>
#include <QLabel>
#include <QAbstractButton>
#include "ttk_spline.h"
#include "parameterview.h"

#include <qchart.h>
#include <qlineseries.h>
#include <QChartView>
#include <QValueAxis>
#include <qscatterseries.h>

QT_CHARTS_USE_NAMESPACE

#include "selectivity.h"
#include "fleet.h"
#include "model.h"
#include "dialogparameterview.h"


namespace Ui {
class DialogEquationView;
}

class DialogEquationView : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEquationView(QWidget *parent = nullptr);
    ~DialogEquationView();

    QString getName() const;
    void setName(const QString &value);
    QString getTitle() const;
    void setTitle(const QString &value);
    void setLabel(const QString &value);

public slots:
    virtual void setXvalsConst(const QList<float> &vals);
    virtual void setXvalsConst(const QStringList &vals);
    virtual void setXvals(const QList<float> &vals);
    virtual void setXvals(double min, double max, double step);
    virtual void setXvalStrings(const QStringList &vals);


    void setEquationNumber (int num);
    void changeEquationNumber (int num);
    void setParameters (tablemodel *params);

    void setParameterHeader(int pnum, QString hdr);
    void setParametersVisible (bool vis = false);

    void setIntVar1Range (int min, int max);
    int getIntVar1 ();
    void setIntVar1 (int value);
    virtual void intVar1Changed (int value);

    void apply();
    void reset();
    virtual void resetValues();
    virtual void restoreAll();
    void show ();
    void hide ();
    void close();

    virtual void parametersChanged();
    void parameterViewClosed();
    virtual void setupChanged();

    void refresh ();  // update from population values
    virtual void setup(); // start new
    virtual void update ();   // update from dialog values
    void resizeEvent(QResizeEvent *event);

    void buttonClicked (QAbstractButton *btn);
    void closeEvent (QCloseEvent *event);

    int getBinMin ();
    void setBinMin (int min);
    int getBinMax ();
    void setBinMax (int max);
    int getBinStep ();
    void setBinStep (int step);
    double getMidBin ();
    void setMidBin (double mid);
    virtual void binsChanged ();
    void setJoinOne (int val);
    void setJoinTwo (int val);
    void setJoinThree (int val);

signals:
    void numbersUpdated();
    void hidden ();
    void applyingValues();
    void applyingComplete();

protected:
    QString name;
    QString type;
    QString title;
    QSize window;
    QPoint position;

    int equationNum;
    int numParams;
    tablemodel *parameters;
    DialogParameterView *parameterView;

    bool building;
    bool waiting;
    bool updating;

    QChart *cht;
    QChartView *chartview;
    QLineSeries *ascendSeries;
    QLineSeries *dscendSeries;
    QLineSeries *valSeries;
    QScatterSeries *ptSeries;
    QList<QPointF> firstPoints;

    float yMax;

    QValueAxis *axisX;
    QValueAxis *axisY;
    QValueAxis *axisYalt;

    QList<double> xValList;

    int intvar1;

protected slots:
    void connectAll();
    void disconnectAll();

    void updateGrid(QRectF rect);
    void updateTicks(int xT = 11, int yT = 9);
    void resetChart(bool create = false);

    void showSPR (bool flag);
    void showJoins (int num);
    void showBins (bool flag);

    void notYet (int eqn = 0, int num = 0);
    void blank (int eqn, int rep = 0, QString msg = QString(""));

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
    Ui::DialogEquationView *ui;
};

#endif // DIALOGEQUATIONVIEW_H
