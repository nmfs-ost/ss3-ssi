#ifndef POPULATION_WIDGET_H
#define POPULATION_WIDGET_H

#include <QWidget>

#include "model.h"
#include "ss_mortality.h"
#include "ss_recruitment.h"
#include "growth.h"
#include "ss_fecundity.h"


namespace Ui {
class population_widget;
}

class population_widget : public QWidget
{
    Q_OBJECT

public:
    explicit population_widget(ss_model * m_data, QWidget *parent = 0);
    ~population_widget();

public slots:
    void set_model (ss_model *model);
    void reset();
    void refresh();

    void changeGrowthPattern(int num);

    int getFecundityOption ();
    void setFecundityOption (int opt);
    int getFecundityOffsetOption ();
    void setFecundityOffsetOption(int opt);
    int getFecundityAdjustment ();
    void setFecundityAdjustment(int opt);

    void setMaturityOpt(int opt);
    int getMaturityOpt();

    void setMortOption (int opt);
    void changeMortOption (int opt);
    void changeMortNumBkpts (int num);
    void changeNumGrowthPat(int num);
    void changeNumSubMorph(int num);
    void setGrowthModel(int opt);
    void changeGrowthModel(int opt);
    int getGrowthModel();
    void changeAmin (double val);
    void changeAmax (double val);
    void changeKageMin (double val);
    void changeKageMax (double val);
    void changeExpDecay (double val);
    void changeSDadd (double val);
    void changeCVmethod (int num);
    void changeTimeVaryMethod (int num);

    void setSpawnRecrSpec (int spec);
    void setRecrArea (int value);
    void changeRecrArea ();
    int getRecrArea ();
    void setRecrDistParam (int method);
    void changeRecrDistParam (int method);
    int getRecrDistParam ();

    void changeFMortMethod (int opt);
    void changeFMortBpark ();
    void changeFMortBparkYr (int yr);
    void changeFMortMaxF ();
    void changeFMortStartF ();
    void changeFMortPhase (int phs);
    void changeFMortNumInput (int num);
    void changeFMortNumIters (int num);

signals:
    void readWtAtAgeSS (bool flag);

private slots:
    void changeFractionFemale();
    void setHermaphOptions(int herm);
    void changeHermaph(bool flag);
    void changeHermaphSeas(int seas);
    void changeHermaphMales(int opt);
    void changeMaturityOpt(int opt);
    void changeFecundityOption(int opt);
    void changeFecundityOffsetOption(int opt);
    void changeFecundityAdjustment(int opt);
    void changeFirstMatureAge();
    void changeGrowthMorphRatio(double ratio);
    void changeGrowthTimeVaryRead(int flag);

    void changeNumRecrDevs (int num);
    void changeSpwnRecReadTimeVary (int flag);

    void changeSeasParams();
    void changeRecNumAssigns(int num);
    void changeRecAssignInteract (bool flag);
    void changeSpawnRecrSpec(int num);

    void changeMoveNumDefs(int value);
    void defsChanged(QModelIndex tl, QModelIndex br);
    void setMoveParamTitles();
    void setMoveParamTitle(int def);
    void changeMoveFirstAge();

private:
    Ui::population_widget *ui;

    tableview *growthMorphDistView;
    tableview *growthParamsView;
    tableview *growthTVParamsView;
    tableview *cvParamsView;
//    tableview *cvTVParamsView;

    tableview *recruitDistView;
    tableview *recruitParamsView;
    tableview *recruitTVParamsView;
    tableview *recruitAssignParamsView;

    tableview *wtlenParamsView;
    tableview *wtlenTVParamsView;

    tableview *maturityParamsView;
    tableview *maturityTVParamsView;

    tableview *fecundParamsView;
    tableview *fecundTVParamsView;
    tableview *fractionFemView;
    tableview *hermaphParamsView;

    tableview *moveDefsView;
    tableview *moveParamsView;

    tableview *mortBreakPtsView;
    tableview *mortAgesView;
    tableview *mortParamsView;
    tableview *mortTVParamsView;

    tableview *mortInputsView;
    tableview *mortInitialParamsView;

    tableview *seasonParamsView;
    tableview *seasonTVParamsView;

    ss_model *model_data;
    population *pop;
    spawn_recruit *spwn_rcr;
    ss_movement *movemnt;
    ss_fecundity *fecund;
    ss_growth   *grwth;
    ss_mortality *mort;

//    tableview *timeVaryParamsView;
};

#endif // POPULATION_WIDGET_H
