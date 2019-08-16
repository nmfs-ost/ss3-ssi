#ifndef DIALOGSPWNRCREQUATIONVIEW_H
#define DIALOGSPWNRCREQUATIONVIEW_H

#include <QObject>

#include "dialogequationview.h"
#include "population.h"

class DialogSpwnRcrEquationView : public DialogEquationView
{
public:
    DialogSpwnRcrEquationView(QWidget *parent = nullptr);
    ~DialogSpwnRcrEquationView();

    int getOption() const;
    void setOption(int value);

    population *getPopulation() const;
    void setPopulation(population *value);

public slots:
    void setXvals();
    void setXvalStrings(const QStringList &vals);

//    void setEquationNumber (int num);
//    void updateGraph ();
//    void setParameters (tablemodel *params);

//    void apply();
//    void reset();
    void resetValues();
    void restoreAll();

    void parametersChanged();
    void setupChanged();

    void refresh ();  // update from population values
    void setup(); // start new
    void update ();   // update from dialog values
//    void updateLinearExp(float);

signals:
//    void linearUpdated(float);

private:
    population *pop;
    int populationNum;
    int genders;


private slots:
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

};

#endif // DIALOGSPWNRCREQUATIONVIEW_H
