#ifndef DIALOGSELECTIVITYEQUATIONVIEW_H
#define DIALOGSELECTIVITYEQUATIONVIEW_H

#include "dialogequationview.h"
#include "model.h"
#include "selectivity.h"

#include <QObject>
#include <QStringList>
#include <QWidget>


class DialogSelexEquationView : public DialogEquationView
{
    Q_OBJECT

public:
    explicit DialogSelexEquationView(QWidget *parent, SelexType typ = Size);
    ~DialogSelexEquationView() override;

    Fleet *getFleet() const;
    void setFleet(Fleet *flt);

    int getSpecial() const;
    void setSpecial(int value);

    int getMale() const;
    void setMale(int value);

    void setOption(int value);

public slots:
    void setSelex (selectivity *slx);
    void changingSelex ();
    void changedSelex (QStringList &ql);
    void changeSelex ();

    void setXvals(const QList<double> &vals) override;
    void setXvalStrings(const QStringList &vals) override;
    void setBinVals(float start, float end, int step);
    void setBinVals(const QList<float> &vals);
    void setBinValStrings(const QStringList &vals);

    void resetValues() override;
    void restoreAll() override;

    void setParameterHeaders (int scale = 0);

    void setup () override;     // start new
    void update () override;   // update from dialog values

signals:
    void linearUpdated(float);

private:
    Fleet *fleet;
    int fleetNum;
    selectivity * selex;
    SelexType slxType;
    int genders;
    int special;
    int male;
    int flag;
    QList<double> bins;

private slots:
    void constant (double val = 1.0);
    void updateConstant(double val = 1.0);
    void updateConstant (double val, double first, double last);
    void constantRange ();
    void updateConstantRange (double val = 1.0);
    void logistic ();
    void updateLogistic ();
    void mirror (int num = 0);
    void updateMirror (int flt = 0);
    void linearSeg (int scale);
    void linearPlain();
    void linearScaled();
    void updateLinear (int scale = 0);
    void updateLinearScaled(int scale);
    void updateLinearPlain();
    void updateLinearExp (int scale = 0);
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
    void randomWalk (float scale = 0.);
    void updateRandomWalk (float scale = 0.);
    void coleGauss ();
    void updateColeGauss();
    void cubicSpline (float scale = 0.);
    void updateCubicSpline (float scale = 0.);
    void twoSexRandomWalk ();
    void updateTwoSexRandom ();
    void twoSexEachAge ();
    void updateTwoSexEachAge ();

    bool checkScaleSliders(int first, int secnd, QList<double> bins, double &binLo, double &binHi);
    double aveYvalue(const QList<QPointF> &pointlist, int lowBin = 1, int highBin = 100);
};

#endif // DIALOGSELECTIVITYEQUATIONVIEW_H
