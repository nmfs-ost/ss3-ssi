/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>


#include "fleet.h"
#include "population.h"
#include "ss_observation.h"
//#include "area.h"
#include "ss_forecast.h"
#include "block_pattern.h"
#include "sd_reporting.h"
#include "method_setup.h"

#include "tablemodel.h"
#include "tableview.h"
#include "spinboxdelegate.h"
#include "sscomposition.h"


/** a class to keep data about seasons */
class Season : public QObject
{
    Q_OBJECT
public:
    explicit Season();

public slots:
    void reset();
    float getNumMonths() const;
    void setNumMonths(float value);

    float getDuration() const;
    void setDuration(float value);
    int getNumSubSeasons () const;
    void setNumSubSeasons(int value);

    bool getSpawning() const;
    void setSpawning(bool value);

private:
    float numMonths; /**< The number of months in this season */
    float duration; /**< The fraction of the year for this season */
    int numSubSeasons; /**< The number of sub seasons, always even */
    bool spawning;  /**< Whether spawning occurs in this season (only one per year) */
};


/** This is the main data class and holds pointers to all the other data classes */
class ss_model : public QObject
{
    Q_OBJECT
public:
    explicit ss_model(QWidget *parent = nullptr);
    ~ss_model ();

    void clear();

    tablemodel *sdYearsModel;

    tablemodel *mbweightModel;
    QStringList mbweightHeader;

    QList<int> iSdValues;

    QList <Fleet *> fleets;
    population *  pPopulation;
    int  iNumAreas;
    ss_forecast *forecast;

    bool getUse2DAR1() const;
    void setUse2DAR1(bool value);
    void changeUse2DAR1(bool value);

    int getTimeVaryBoundsCheck() const;
    void setTimeVaryBoundsCheck(int value);

public slots:
    void reset();

    void incrementYear ();

    int getSelexAdjustMethod() const;
    void setSelexAdjustMethod(int value);

    int getReadWtAtAge() const;
    void setReadWtAtAge(int value);

    Fleet *getFleet(int index);
    Fleet *newFleet (QString name = QString(""));
    Fleet *duplicateFleet (Fleet *oldfl);
    void addFleet (Fleet *flt);
    void deleteFleet (int index);
    population *getPopulation() {return pPopulation;}
    ss_forecast *getForecast() {return forecast;}

    void setReadMonths(bool flag) {readMonths = flag;}
    bool getReadMonths() {return readMonths;}

    void set_use_softbounds (int flag) {bUseSoftBounds = flag;}
    void set_use_softbounds (bool flag) {bUseSoftBounds = flag;}
    bool get_use_softbounds () {return bUseSoftBounds;}
    void set_prior_likelihood (int flag) {bPriorLikelihood = (flag == 1);}
    void set_prior_likelihood (bool flag) {bPriorLikelihood = flag;}
    bool get_prior_likelihood () {return bPriorLikelihood;}
    void set_last_estim_phase (int phase) {iLastEstimPhase = phase;}
    int get_last_estim_phase () {return iLastEstimPhase;}
    void set_mc_burn (int interval) {iMcBurn = interval;}
    int mc_burn () {return iMcBurn;}
    void set_mc_thin (int interval) {iMcThin = interval;}
    int mc_thin () {return iMcThin;}
    void set_jitter_param (double value) {dJitter = value;}
    double jitter_param () {return dJitter;}
    void set_bio_sd_min_year (int year) {iBioSdMinYr = year;}
    int bio_sd_min_year () {return iBioSdMinYr;}
    void set_bio_sd_max_year (int year) {iBioSdMaxYr = year;}
    int bio_sd_max_year () {return iBioSdMaxYr;}

    void set_num_std_years (int num_yrs);
    int get_num_std_years () {return sdYearsModel->columnCount();}
    void set_std_years_text (QString txt);
    void set_std_year (int index, QString year);
    QString get_std_year (int index) {return sdYearsModel->getRowData(0).at(index);}
    QStringList get_std_years ();
    QString get_std_years_text ();

    void set_convergence_criteria (double value) {dConvergence = value;}
    double get_convergence_criteria () {return dConvergence;}

    void set_retrospect_year (int year) {iRetrospectYr = year;}
    int get_retrospect_year () {return iRetrospectYr;}
    void set_biomass_min_age (int age) {iBiomassMinAge = age;}
    int get_biomass_min_age () {return iBiomassMinAge;}
    void set_depletion_basis (int basis) {iDepletionBasis = basis;}
    int get_depletion_basis () {return iDepletionBasis;}
    void set_depletion_denom (double denom) {dDeplDenom = denom;}
    double get_depletion_denom () {return dDeplDenom;}
    void set_spr_basis (int basis) {iSprBasis = basis;}
    int get_spr_basis () {return iSprBasis;}
    void set_f_units (int units) {iFUnits = units;}
    int get_f_units () {return iFUnits;}
    void set_f_min_age (int age) {iFMinAge = age;}
    int get_f_min_age () {return iFMinAge;}
    void set_f_max_age (int age) {iFMaxAge = age;}
    int get_f_max_age () {return iFMaxAge;}
    void set_f_basis (int basis) {iFBasis = basis;}
    int get_f_basis () {return iFBasis;}
    void setALKTol (float val);
    float getALKTol () {return fALKTol;}
    void setRandSeed (int seed);
    int getRandSeed ();

    void set_start_year (int year);
    int get_start_year () {return iStartYr;}
    void set_end_year (int year);
    int get_end_year () {return iEndYr;}
    int getTotalYears() {return iTotalYears;}
    void set_num_seasons (int seasns) ;
    Season *getSeason(int index);
    int getSeasonByMonth(float month);
    float getMonthBySeasonFleet(int seas, int fleet);
    int get_num_seasons () {return seasons.count();}
    void set_months_per_season (int seasn, float months);
    float get_months_per_season (int seasn) {return seasons.at(seasn)->getNumMonths();}
    void rescale_months_per_season ();
    int getTotalMonths();
    void set_spawn_season (int seasn) ;
    int get_spawn_season () ;
    void set_spawn_month (float month);
    float get_spawn_month ();
    int find_season (float month);
    float find_month (int fleet, int season);
    int getTotalSeasons();
    void set_num_subseasons(int value);
    int get_num_subseasons () const;
    void set_num_fisheries (int n_fisheries) {iNumFisheries = n_fisheries;}
    int get_num_fisheries ();
    void set_num_surveys (int n_surveys) {iNumSurveys = n_surveys;}
    int get_num_surveys ();
    void set_num_fleets (int n_fleets = 0);
    int get_num_fleets () {return fleets.count();}
    void assignFleetNumbers ();
    int getNumActiveFleets ();
    Fleet *getActiveFleet (int num);
    void set_num_areas (int n_areas);
    int get_num_areas () {return iNumAreas;}
    int getNumLinesCatch ();
    void add_fleet_catch_per_season (int fishery, int yr, int season, double num, double se = 0);
//    double fleet_catch_per_season (int fishery, int yr, int season);
    void set_fleet_units_err_type (int fleet, int units, int err_type);
    int fleet_discard_count();
    int fleet_discard_obs_count();
    void setUseMeanBwt (int flag) {useMeanBwtData = flag == 1? true: false;}
    bool getUseMeanBwt () {return useMeanBwtData;}
    void set_mean_body_wt_obs_count(int count) {mbweightModel->setRowCount(count);}
    int mean_body_wt_count () {return mbweightModel->rowCount();}
    tablemodel *getMeanBwtModel() {return mbweightModel;}
    void set_mean_body_wt_df (int df) {i_mean_bwt_df = df;}
    int mean_body_wt_df () {return i_mean_bwt_df;}
    void setMeanBwtObs (int index, QStringList data)
        {mbweightModel->setRowData(index, data);}
    QStringList getMeanBwtObs (int index) {return mbweightModel->getRowData(index);}

    void set_num_ages (int ages);
    int get_num_ages () {return iNumAges;}
    void set_num_genders (int genders);
    int get_num_genders () {return iNumGenders;}

    void set_run_number (int r_num) {iNumRuns = r_num;}
    int get_run_number () {return iNumRuns;}

    void setUseLengthComp (int flag) {useLenCompData = (flag == 1)? true: false;}
    bool getUseLengthComp () {return useLenCompData;}
    void set_length_composition (compositionLength *l_data) {lengthData = l_data;}
    compositionLength *get_length_composition () {return lengthData;}
    void set_age_composition (compositionAge *a_data) {ageData = a_data;}
    compositionAge *get_age_composition () {return ageData;}
    void set_morph_composition (compositionMorph *m_data) {morphData = m_data;}
    compositionMorph *get_morph_composition () {return morphData;}

    void setUseMeanSAA (int flag) {useMeanSAAData = (flag == 1)? true: false;}
    bool getUseMeanSAA () {return useMeanSAAData;}
    void setNumEnvironVars (int num) {obsEnvironVars->setNumVars(num);}
    int getNumEnvironVars () {return obsEnvironVars->getNumVars();}
    void setNumEnvironVarObs (int num) {obsEnvironVars->setNumObs(num);}
    int getNumEnvironVarObs () {return obsEnvironVars->getNumObs();}
    void addEnvironVarObs (QStringList txtlst);
    void setEnvironVarObs (int index, QStringList txtlst);
    QStringList get_environ_var_obs (int index) {return obsEnvironVars->getObservation(index);}
    tablemodel *getEnvVariables() {return obsEnvironVars->getObservations();}

    void setNumGeneralCompMethods(int num);
    int getNumGeneralCompMethods () {return cListGeneralMethods.count();}
    void setDMError(int num) {iDMError = num;}
    int getDMError() {return iDMError;}
    void addGeneralCompMethod (compositionGeneral *method) {cListGeneralMethods.append (method);}
    compositionGeneral *getGeneralCompMethod (int index) {return cListGeneralMethods.at(index);}
    void copyGeneralCompMethod (compositionGeneral *method);
    void deleteGeneralCompMethod (compositionGeneral *method);
    void deleteGeneralCompMethod (int index);


    void setDoTags (int flag);
    int getDoTags () {return doTags;}
    void setNumTagGroups (int num);
    int getNumTagGroups () {return tagData->getNumTagGroups();}
    void setTagLatency (int period);
    int getTagLatency () {return tagData->getLatency();}
    void setTagMaxPeriods (int periods) {tagData->setMaxPeriods(periods);}
    int getTagMaxPeriods () {return tagData->getMaxPeriods();}
    void setTagObservation (int index, QStringList data) {tagData->setObservation(index, data);}
    QStringList getTagObservation (int index) {return tagData->getObservation(index);}
    tablemodel *getTagObservations () {return tagData->getObservations();}
    void setNumTagRecaps (int num) {recapData->setNumRecapEvnts(num);}
    int getNumTagRecaps () {return recapData->getNumRecapEvnts();}
    void addRecapObservation (QStringList data) {recapData->addRecapEvent(data);}
    int getNumRecapObservations () {return recapData->getNumObs();}
    QStringList getRecapObservation (int index) {return recapData->getObservation(index);}
    tablemodel *getRecapObservations () {return recapData->getObservations();}
    void setMinRecap (int min) {minRecap = min;}
    int getMinRecap () {return minRecap;}

    void setDoMorphComp (bool flag) {doMorphComp = flag;}
    bool getDoMorphComp () {return doMorphComp;}

    void setNumBlockPatterns (int num);// {iNumBlockPatterns = num;}
    int getNumBlockPatterns() {return iNumBlockPatterns;}
    void setBlockPattern (int index, BlockPattern *bp);
    void addBlockPattern (BlockPattern * bp) {blockPatterns.append (bp);}
    BlockPattern * getBlockPattern (int index) {return blockPatterns[index];}

    void setTagTimeVaryReadParams(int flag) {tagTvParamsRead = flag;}
    int getTagTimeVaryReadParams () {return tagTvParamsRead;}
    void setTagLoss(int flag);
    int getTagLoss() {return tag_loss;}
    void setTagLossParameter(longParameter *lp);
    void setTagLossParameter(QString text);
    longParameter *getTagLossParameter() {return tag_loss_param;}
    longParameterModel *getTagLossInit() {return tagLossInit;}
    longParameterModel *getTagLossChronic() {return tagLossChronic;}
    longParameterModel *getTagOverdispersion() {return tagOverdispersion;}
    longParameterModel *getTagReportFleet() {return tagReportFleet;}
    longParameterModel *getTagReportDecay() {return tagReportDecay;}
    timeVaryParameterModel *getTagLossInitTV() {return tagLossInitTV;}
    timeVaryParameterModel *getTagLossChronicTV() {return tagLossChronicTV;}
    timeVaryParameterModel *getTagOverdispersionTV() {return tagOverdispersionTV;}
    timeVaryParameterModel *getTagReportFleetTV() {return tagReportFleetTV;}
    timeVaryParameterModel *getTagReportDecayTV() {return tagReportDecayTV;}

    void setInputValueVariance (int flag) {i_input_variance = flag;}
    int getInputValueVariance () {return i_input_variance;}

    void setLambdaMaxPhase (int phs) {lambdaSetup.setA(phs);}
    int getLambdaMaxPhase () {return lambdaSetup.getA();}
    void setLambdaSdOffset (int sd) {lambdaSetup.setB(sd);}
    int getLambdaSdOffset () {return lambdaSetup.getB();}
    void setLambdaNumChanges(int num) {lambdaSetup.setC(num);}
    int getLambdaNumChanges();
    void setNumLambdaAdjustments(int num);
    int getNumLambdaAdjustments() {return lambdas->rowCount();}
    void addLambdaAdjustment(QStringList ladj);
    QStringList getLambdaAdjustment (int row);
    tablemodel *getLambdaAdjustmentModel() {return lambdas;}

    int getAddVariance() const;
    void setAddVariance(int value);
    void setAddVarSetupFromText (QString txt);
    QString getAddVarSetupToText ();
    void setAddVarSetupFleet(int val) {add_var_setup[0] = val;}
    int getAddVarSetupFleet () {return add_var_setup[0];}
    void setAddVarSetupLenAge(int val) {add_var_setup[1] = val;}
    int getAddVarSetupLenAge () {return add_var_setup[1];}
    void setAddVarSetupYear(int val) {add_var_setup[2] = val;}
    int getAddVarSetupYear () {return add_var_setup[2];}
    void setAddVarSetupNSlxBins(int val);
    int getAddVarSetupNSlxBins () {return add_var_setup[3];}
    void setAddVarSetupGPatt(int val) {add_var_setup[4] = val;}
    int getAddVarSetupGPatt () {return add_var_setup[4];}
    void setAddVarSetupNGAges(int val);
    int getAddVarSetupNGAges () {return add_var_setup[5];}
    void setAddVarSetupArNaa(int val) {add_var_setup[6] = val;}
    int getAddVarSetupArNaa () {return add_var_setup[6];}
    void setAddVarSetupYrNaa(int val) {add_var_setup[7] = val;}
    int getAddVarSetupYrNaa () {return add_var_setup[7];}
    void setAddVarSetupNaaBins(int val);
    int getAddVarSetupNaaBins () {return add_var_setup[8];}

    sd_reporting *getAddSdReporting () {return additionalSdReporting;}
    int getAddSdReprtActive() {return additionalSdReporting->getActive();}
    QStringList getAddSdReprtSelex () {return additionalSdReporting->getSelex();}
    QStringList getAddSdReprtGrowth () {return additionalSdReporting->getGrowth();}
    QStringList getAddSdReprtNumAtAge () {return additionalSdReporting->getNumAtAge();}
    QStringList getAddSdReprtNatMort () {return additionalSdReporting->getNatMort();}
    QStringList getAddSdReprtSelexBins () {return additionalSdReporting->getSelexBins();}
    QStringList getAddSdReprtGrwthBins () {return additionalSdReporting->getGrowthBins();}
    QStringList getAddSdReprtAtAgeBins () {return additionalSdReporting->getNumAtAgeBins();}
    QStringList getAddSdReprtNatMortBins () {return additionalSdReporting->getNatMortBins();}
    void setAddVarSelexBins (int index, float val);
    float getAddVarSelexBins (int index) {return add_var_slx_bins[index];}
    void setAddVarGrwthBins (int index, float val);
    float getAddVarGrwthBins (int index) {return add_var_age_bins[index];}
    void setAddVarNumAaBins (int index, float val);
    float getAddVarNumAaBins (int index) {return add_var_Naa_bins[index];}

    int checkyearvalue(QString str);
    int checkyearvalue(int value);
    int refyearvalue(int value);

    int getCustomEnviroLink() const;
    void setCustomEnviroLink(int value);

    int getCustomBlockSetup() const;
    void setCustomBlockSetup(int value);

    int getCustomSelParmDevPhase() const;
    void setCustomSelParmDevPhase(int value);

    int getCustomSelParmDevAdjust() const;
    void setCustomSelParmDevAdjust(int value);

    bool getReadSelectivityPriors();
    void setReadSelectivityPriors(bool flag);

    QString getDataFileName ();
    QString getControlFileName ();

signals:
    void data_file_changed (QString fname);
    void control_file_changed (QString fname);
    void darChanged(bool value);

private:
    bool readMonths;
    bool readSelxPriors;
    bool readWtAtAge;
    bool bUseSoftBounds;
    bool bPriorLikelihood;
    int iLastEstimPhase;
    int iMcBurn;
    int iMcThin;
    double dJitter;
    int iBioSdMinYr;
    int iBioSdMaxYr;

    double dConvergence;
    int iRetrospectYr;
    int iBiomassMinAge;
    int iDepletionBasis;
    double dDeplDenom;
    int iSprBasis;

    int iFUnits;
    int iFMinAge;
    int iFMaxAge;
    int iFBasis;
    float fALKTol;

    int iStartYr;
    int iEndYr;
    int iTotalYears;
    float fSpawnMonth;
    int iNumFisheries;
    int iNumSurveys;
    int i_num_predators;
    int iNumAges;
    int iNumGenders;

    int iNumRuns;

    int rSeed;

    bool useMeanBwtData;
    int i_mean_bwt_df;

    bool useLenCompData;
    compositionLength *lengthData;

    int iDMError;
    compositionAge *ageData;
    QList<compositionGeneral *> cListGeneralMethods;
    bool doMorphComp;
    compositionMorph *morphData;

    bool useMeanSAAData;

    environmentalVars *obsEnvironVars;
    int doTags;
    tagObservation *tagData;
    recapObservation *recapData;
    int minRecap;

    QList<BlockPattern *> blockPatterns;
    int iNumBlockPatterns;

    sd_reporting *additionalSdReporting;

    QList <Season *> seasons;

    int timeVaryBoundsCheck;
    int tagTvParamsRead;
    int tag_loss;
    longParameter *tag_loss_param;
    longParameterModel *tagLossInit;
    longParameterModel *tagLossChronic;
    longParameterModel *tagOverdispersion;
    longParameterModel *tagReportFleet;
    longParameterModel *tagReportDecay;
    timeVaryParameterModel *tagLossInitTV;
    timeVaryParameterModel *tagLossChronicTV;
    timeVaryParameterModel *tagOverdispersionTV;
    timeVaryParameterModel *tagReportFleetTV;
    timeVaryParameterModel *tagReportDecayTV;

    int i_input_variance;

    method_setup lambdaSetup;
    tablemodel *lambdas;

    int i_add_variance;
    QList<int> add_var_setup;
    QList<float> add_var_slx_bins;
    QList<float> add_var_age_bins;
    QList<float> add_var_Naa_bins;

    int customEnviroLink;
    int customBlockSetup;
    int customSelParmDevPhase;
    int customSelParmDevAdjust;

    int selexAdjustMethod;

    bool use2DAR1;
};

double checkdoublevalue(QString value);
float checkfloatvalue(QString value);
int checkintvalue(QString value);

bool floatEquals (float a, float b);

#endif // MODEL_H
