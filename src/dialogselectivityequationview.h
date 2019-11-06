#ifndef DIALOGSELECTIVITYEQUATIONVIEW_H
#define DIALOGSELECTIVITYEQUATIONVIEW_H

#include "dialogequationview.h"
#include "fleet.h"

enum SelexType {
    Age,
    Size
};

class DialogSelexEquationView : public DialogEquationView
{
public:
    DialogSelexEquationView(QWidget *parent = nullptr, SelexType typ = Size);
    ~DialogSelexEquationView();

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
    void changedSelex (QStringList ql);
    void changeSelex ();

    void setXvals(const QList<float> &vals);
    void setXvalStrings(const QStringList &vals);

    void resetValues();
    void restoreAll();

    void setParameterHeaders ();

    void setup(); // start new
    void refresh ();  // update from population values
    void update ();   // update from dialog values

signals:
    void linearUpdated(float);

private:
    Fleet *fleet;
    int fleetNum;
    selectivity * selex;
    SelexType type;
    int genders;
    int special, male;

private slots:
    void constant (float val = 1.0);
    void updateConstant(float val = 1.0);
    void updateConstant (float val, float first, float last);
    void constantRange ();
    void updateConstantRange (float val = 1.0);
    void logistic ();
    void updateLogistic ();
    void mirror (int sliders = 0);
    void updateMirror (int sliders = 0);
    void linear (float scale);
    void linearPlain();
    void linearScaled();
    void updateLinear (float scale = 0.);
    void updateLinearScaled();
    void updateLinearPlain();
    void updateLinearExp (float scale = 0.);
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

};

#endif // DIALOGSELECTIVITYEQUATIONVIEW_H
