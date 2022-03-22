#ifndef DIALOGSPWNRCREQUATIONVIEW_H
#define DIALOGSPWNRCREQUATIONVIEW_H

#include <QObject>
#include <QWidget>
#include <QStringList>

#include "dialogequationview.h"
#include "population.h"

class DialogSpwnRcrEquationView : public DialogEquationView
{
    Q_OBJECT

public:
    explicit DialogSpwnRcrEquationView(QWidget *parent = nullptr);
    ~DialogSpwnRcrEquationView();

    int getOption() const;
    void setOption(int value);

    population *getPopulation() const;
    void setPopulation(population *popultn);

public slots:
    void changeEquationNumber(int num);
    void setParameters (tablemodel *params);

    void resetValues();
    void restoreAll();

    void setup();     // start new
    void update ();   // update from dialog values

signals:

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
    void bevertonHoltSigmoidal ();
    void updateBevertonHoltSigmoidal ();
    void survivorship ();
    void updateSurvivorship ();
    void shepherd ();
    void updateShepherd ();
    void shepherdReParm ();
    void updateShepherdReParm ();
    void rickerReParm ();
    void updateRickerReParm ();
    void sailaLorda ();
    void updateSailaLorda ();

};

#endif // DIALOGSPWNRCREQUATIONVIEW_H
