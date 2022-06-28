/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef GROWTH_H
#define GROWTH_H

#include <QObject>
#include <QList>
#include <QStringList>

#include "growth_pattern.h"
#include "growth_season_effects.h"
#include "parametermodelTV.h"
#include "setupmodel.h"

class ss_growth : public QObject
{
    Q_OBJECT

public:
    ss_growth(QObject *parent = nullptr);
    ~ss_growth();

    void reset();


public slots:
    int getGrowthBinMethod() const;
    void setGrowthBinMethod(int value);
    float getGrowthBinMin() const;
    void setGrowthBinMin(float value);
    float getGrowthBinMax() const;
    void setGrowthBinMax(float value);
    float getGrowthBinWidth() const;
    void setGrowthBinStep(float value);
    void generateGrowthBins ();
    int getNumGrowthBins () {return growthBins->columnCount();}
    void setNumGrowthBins (int value) {growthBins->setColumnCount(value);}
    tablemodel *getGrowthBinTable();
    QStringList getGrowthBins ();
    void setGrowthBins(QStringList data);

    int getNum_patterns() const;
    void setNum_patterns(int value);
    growthPattern *getPattern(int index);
    int getParam_dev_phase() const;
    void setParam_dev_phase(int value);
    int getAdjustment_method() const;
    void setAdjustment_method(int value);
    int getParam_offset_method() const;
    void setParam_offset_method(int value);
    float getFraction_female() const;
    void setFraction_female(float value);
    int getNatural_mortality_type() const;
    void setNatural_mortality_type(int value);
    int getNaturalMortLorenzenRefMin() const {return natMort_lorenzen_ref_age1;}
    void setNaturnalMortLorenzenRefMin(int value);
    int getNaturalMortLorenzenRefMax() const {return natMort_lorenzen_ref_age2;}
    void setNaturnalMortLorenzenRefMax(int value) {natMort_lorenzen_ref_age2 = value;}
    int getNaturalMortLorenzenRef () {return getNaturalMortLorenzenRefMin();}
    void setNaturalMortLorenzenRef (int ref) {setNaturnalMortLorenzenRefMin(ref);}
    int getNatMortMaunderOption () {return getNaturalMortLorenzenRefMin();}
    void setNatMortMaunderOption (int opt) {setNaturnalMortLorenzenRefMin(opt);}
    int getNatMortNumBreakPts () const;
    void setNatMortNumBreakPts (int num);
    tablemodel *getNatMortValues() {return natMortBreakPoints;}
    QStringList getNatMortBreakPts ();
    void setNatMortBreakPts (QStringList data);
    void setNatMortAgesHeader(int ages);
    tablemodel *getNatMortAgeValues() {return natMortAges;}
    QStringList getNatMortAges();
    void setNatMortAges (QStringList data);

    void setNumAges (int ages) {num_ages = ages;}
    void setNumGenders (int num);
    int getModel() const;
    void setModel(int value);
    float getAge_for_l1() const;
    void setAge_for_l1(float value);
    float getAge_for_l2() const;
    void setAge_for_l2(float value);
    int getNumKmults () const {return num_K_mults;}
    void setNumKmults (int nmults) {num_K_mults = nmults;}
    float getAgeMin_for_K () const {return age_min_for_K;}
    void setAgeMin_for_K (float mink) {age_min_for_K = mink;}
    float getAgeMid_for_K () const {return age_mid_for_K;}
    void setAgeMid_for_K (float midk) {age_mid_for_K = midk;}
    float getAgeMax_for_K () const {return age_max_for_K;}
    void setAgeMax_for_K (float maxk) {age_max_for_K = maxk;}
    float getExpDecay ();
    void setExpDecay (float value);
    float getFeature () {return feature;}
    void setFeature (float value) {feature = value;}

    float getSd_add() const;
    void setSd_add(float value);

    int getCv_growth_pattern() const;
    void setCv_growth_pattern(int value);
    int getMaturity_option() const;
    void setMaturity_option(int value);
    void setNumMatAgeValues(int value) {matAgeValues->setColumnCount(value);}
    int getNumMatAgeValues () {return matAgeValues->columnCount();}
    tablemodel *getMatAgeValues() {return matAgeValues;}
    QStringList getMatAgeVals () {return matAgeValues->getRowData(0);}
    void setMatAgeVals (QStringList data) {matAgeValues->setRowData(0, data);}

    float getFirst_mature_age() const;
    void setFirst_mature_age(float value);

    void setCohortParam (QStringList data);// {cohortParam->setParameter (0, data);}
    QStringList getCohortParam () {return cohortParam->getParameter(0);}
    tablemodel *getCohortParams () {return cohortParam->getParamTable();}
    void setCohortTVParam (int row, QStringList data) {cohortVarParam->setVarParameter(row, data);}
    int getNumCohortTVParams () {return cohortVarParam->getNumVarParams();}
    QStringList getCohortTVParam (int row) {return cohortVarParam->getVarParameter(row);}
    tablemodel *getCohortTVParams () {return cohortVarParam->getVarParamTable();}

    int getNum_morphs() const;
    void setNum_morphs(int value);
    float getMorph_within_ratio() const;
    void setMorph_within_ratio(float value);
    void setMorph_dist(QStringList values);
    QStringList getMorphDist_str();
    float getMorph_dist (int index);
    void setMorph_dist (int index, float value);
    void setMorphDist ();
    tablemodel *getMorphDistModel() {return morphdisttable;}

    int getDevPhase() const;
    void setDevPhase(int value);

    int getCustomBlock() const;
    void setCustomBlock(int value);

    int getCustomEnvLink() const;
    void setCustomEnvLink(int value);

    int getTimeVaryMethod() const;
    void setTimeVaryMethod(int value);

    int getTimeVaryReadParams() const;
    void setTimeVaryReadParams(int value);
    void autogenerateTimeVaryParams ();

private:
    ss_model *ssModel;

    float growthBinMin;
    float growthBinMax;
    float growthBinStep;
    int growthBinMethod;
    tablemodel *growthBins;

    QList<growthPattern *> patterns;
    int num_patterns;

    int num_morphs;
    float morph_within_ratio;
//    float morph_dist[5];
    int num_ages;
    int num_genders;

    float fraction_female;
    int natural_mortality_type;
    int natMortNumBreakPoints;
    int natMort_lorenzen_ref_age1;
    int natMort_lorenzen_ref_age2;
    tablemodel *natMortBreakPoints;
    QStringList natMortHeader;
    tablemodel *natMortAges;


    int model;
    float age_for_l1;
    float age_for_l2;
    int num_K_mults;
    float age_min_for_K;
    float age_mid_for_K;
    float age_max_for_K;
    float age_exp_decay;
    float sd_add;
    float feature;
    int cv_growth_pattern;
    int maturity_option;
    tablemodel *matAgeValues;
    float first_mature_age;

    int param_offset_method;
    int adjustment_method;
    int param_dev_phase;

    int hermaphroditism;
    int hermaphSeason;
    int hermaphMales;

    tablemodel *morphdisttable;

    longParameterModel *cohortParam;
    timeVaryParameterModel *cohortVarParam;


    int num_params;

    int devPhase;

    int customEnvLink;
    int customBlock;

    int timeVaryMethod;
    int timeVaryReadParams;
};

#endif // GROWTH_H
