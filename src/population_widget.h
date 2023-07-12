/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef POPULATION_WIDGET_H
#define POPULATION_WIDGET_H

#include <QWidget>

#include "model.h"
#include "ss_mortality.h"
#include "ss_recruitment.h"
#include "growth.h"
#include "ss_fecundity.h"
#include "dialogspwnrcrequationview.h"


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

    void setMortOption ();
    void setMaunderOption (int opt);
    void setMortOption (int opt);
    void setInitialMort (tablemodel *fmort);
    void changeMortOption (int opt);
    void changeMortNumBkpts (int num);
    void changeLorenzRef (int num);
    void changeMaunderOpt (int num);
    void changeLorenzMinAge (int num);
    void changeLorenzMaxAge (int num);
    void changeNumGrowthPat(int num);
    void changeNumSubMorph(int num);
    void normalizeGrowthMorphDist();
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

    void setSRequationDialogVisible(bool checked = false);
    void setSpawnRecrSpec (int spec);
    void setRecrArea (int value);
    void changeRecrArea ();
    int getRecrArea ();
    void setRecrDistParam (int method);
    void changeRecrDistParam (int method);
    void changeRecrDistAssignments (int method);
    int getRecrDistParam ();

    void setFMortMethod (int opt);
    void changeFMortMethod (int opt);
    void changeFMortBpark ();
    void changeFMortBparkYr (int yr);
    void changeFMortMaxF ();
    void changeFMortStartF ();
    void changeFMortPhase (int phs);
    void changeFMortNumInput (int num);
    void changeFMortNumIters (int num);
    void addMortInitParam();
    void subMortInitParam();
    void addFleetSpecificF();
    void subFleetSpecificF();

signals:
    void readWtAtAgeSS (int flag);

private slots:
    void changeFracFemParams();
    void changeFracFemTVParams();
    void setHermaphOptions(int herm);
    void changeHermaph(bool flag);
    void changeHermaphSeas(int seas);
    void changeHermaphMales(int opt);
    void changeHermaphParams();
    void changeHermaphTVParams();

    void changeMaturityOpt(int opt);
    void changeFecundityOption(int opt);
    void changeFecundityOffsetOption(int opt);
    void changeFecundityAdjustment(int opt);
    void changeFirstMatureAge();
    bool changeGrowthMorphRatio(double ratio);
    void changeGrowthTimeVaryRead(int flag);

    void changeGrowthBinMethod(int value);
    void changeGrowthBinMin(double min);
    void changeGrowthBinMax(double max);
    void changeGrowthBinWidth(double wid);
    void changeGrowthBinLength(int num);

    void changeSpwnRecReadTimeVary (int flag);

    void changeGrowthParams();
    void changeGrowthTVParams();
    void changeCVParams();
    void changeCVTVParams();
    void changeNatMAges();
    void changeNatMParams();
    void changeNatMTVParams();
    void changeWtLenParams();
    void changeWtLenTVParams();
    void changeCohortParams();
    void changeCohortTVParams();
    void changeFecundityParams();
    void changeFecundityTVParams();

    void changeRecrFullParams ();
    void changeRecrTVParams ();
    void changeRecrAssignParams ();
    void changeRecrDistParams ();
    void changeRecrDistTVParams ();
    void changeRecrCycleParams ();
    void changeRecrDevParams ();
    void changeRecNumAssigns(int num);
    void changeSpawnRecrSpec(int num);
    void changeRecAssignInteract (int value);
    void changeRecDoDev (int num);
    void changeRecrDevStartYr (int yr);
    void changeRecrDevEndYr (int yr);
    void changeRecrDevPhase (int phase);
    void changeRecrDoAdvancedOpts (bool flag);
    void changeRecrDevEarlyStart (int yr);
    void changeRecrDevEarlyPhase (int phase);
    void changeRecrFcastPhase (int phase);
    void changeRecrFcastLambda (QString s_lam);
    void changeRecrNoBiasLastYr (double yr);
    void changeRecrFullBiasFirstYr (double yr);
    void changeRecrFullBiasLastYr (double yr);
    void changeRecrNoBiasFirstYr (double yr);
    void changeRecrBiasMax (QString s_max);
    void changeRecrCycles (int cycles);
    void changeRecrDevMin (QString s_min);
    void changeRecrDevMax (QString s_max);
    void changeRecrNumDevs (int num);

    void changeMoveNumDefs(int value);
    void moveDefsChanged(QModelIndex tl, QModelIndex br);
    void setMoveParamTitles();
    void setMoveParamTitle(int def);
    void changeMoveFirstAge();
    void changeMoveParams();
    void changeMoveTVParams();

    void changeSeasParams();

private:
    Ui::population_widget *ui;

    tableview *mortBreakPtsView;
    tableview *mortAgesView;
    tableview *mortFemParamsView;
    tableview *mortFemTVParamsView;
    tableview *mortMaleParamsView;
    tableview *mortMaleTVParamsView;

    tableview *mortInputsView;
    tableview *mortInitialParamsView;
    tableview *mortFleetFView;

    tableview *growthBinsView;

    growthPattern *currPattern;

    tableview *growthMorphDistView;
    tableview *growthMorphStDev;

    tableview *growthFemParamsView;
    tableview *growthFemTVParamsView;
    tableview *cvFemParamsView;
    tableview *cvFemTVParamsView;
    tableview *wtlenFemParamsView;
    tableview *wtlenFemTVParamsView;

    tableview *growthMaleParamsView;
    tableview *growthMaleTVParamsView;
    tableview *cvMaleParamsView;
    tableview *cvMaleTVParamsView;
    tableview *wtlenMaleParamsView;
    tableview *wtlenMaleTVParamsView;

    tableview *maturityParamsView;
    tableview *maturityTVParamsView;

    tableview *fecundParamsView;
    tableview *fecundTVParamsView;
    tableview *fracFemParamsView;
    tableview *fracFemTVParamsView;
    tableview *hermaphParamsView;
    tableview *hermaphTVParamsView;

    DialogSpwnRcrEquationView *srEquationView;
    tableview *recruitParamsView;
    tableview *recruitTVParamsView;
    tableview *recruitDistParamsView;
    tableview *recruitDistTVParamsView;
    tableview *recruitAssignParamsView;
    tableview *recruitCyclesParamsView;
    tableview *recruitDevsView;

    tableview *moveDefsView;
    tableview *moveParamsView;
    tableview *moveTVParamsView;

    tableview *seasonParamsView;
    tableview *seasonTVParamsView;

    ss_model *model_data;
    population *pop;


};

#endif // POPULATION_WIDGET_H
