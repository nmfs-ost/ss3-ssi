#ifndef GROWTH_H
#define GROWTH_H

#include <QList>

#include "growth_pattern.h"
#include "growth_season_effects.h"
#include "parametermodelTV.h"
#include "setupmodel.h"

class ss_growth
{
public:
    ss_growth(ss_model *parent = 0);
    ~ss_growth();

    void reset();

    int getNum_patterns() const;
    void setNum_patterns(int value);
    growthPattern *getPattern(int index) {return patterns[index];}
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
    int getNaturalMortLorenzenRef() const {return natMort_lorenzen_ref_age;}
    void setNaturnalMortLorenzenRef(int value) {natMort_lorenzen_ref_age = value;}
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
    int getModel() const;
    void setModel(int value);
    float getAge_for_l1() const;
    void setAge_for_l1(float value);
    float getAge_for_l2() const;
    void setAge_for_l2(float value);
    float getAgeMin_for_K () const {return age_min_for_K;}
    void setAgeMin_for_K (float mink) {age_min_for_K = mink;}
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

    void setNumWtLenParams (int num) {wtLenParams->setNumParams(num);}
    int getNumWtLenParams () {return wtLenParams->getNumParams();}
    void addWtLenParam (QStringList data) {setWtLenParam(getNumWtLenParams(), data);}
    void setWtLenParam (int index, QStringList data);
    QStringList getWtLenParam (int index) {return wtLenParams->getParameter(index);}
    tablemodel *getWtLenParams () {return wtLenParams->getParamTable();}
    longParameterModel *getWtLenParamModel () {return wtLenParams;}
    void setNumWtLenTVParams (int num) {wtLenVarParams->setNumParams(num);}
    int getNumWtLenTVParams () {return wtLenVarParams->getNumVarParams();}
    void addWtLenTVParam (QStringList data) {setWtLenTVParam(getNumWtLenParams(), data);}
    void setWtLenTVParam (int index, QStringList data);
    QStringList getWtLenTVParam (int index) {return wtLenVarParams->getVarParameter(index);}
    tablemodel *getWtLenTVParams () {return wtLenVarParams->getVarParamTable();}
    float getFirst_mature_age() const;
    void setFirst_mature_age(float value);

    void setCohortParam (QStringList data) {cohortParam->setParameter (0, data);}
    QStringList getCohortParam () {return cohortParam->getParameter(0);}
    tablemodel *getCohortParams () {return cohortParam->getParamTable();}

    int getNumDevParams();

    int getNumEnvLinkParams() {return environmentParams->getNumParams();}
    void addEnvironParam (QStringList data) {setEnvironParam(getNumEnvLinkParams(), data);}
    void setEnvironParam (int index, QStringList data) {environmentParams->setParameter(index, data);}
    QStringList getEnvironParam(int index) {return environmentParams->getParameter(index);}
    tablemodel *getEnvironParams() {return environmentParams->getParamTable();}

    int getNumBlockParams() {return blockParams->getNumParams();}
    void addBlockParam (QStringList data) {setBlockParam(getNumBlockParams(), data);}
    void setBlockParam (int index, QStringList data) {blockParams->setParameter(index, data);}
    QStringList getBlockParam(int index) {return blockParams->getParameter(index);}
    tablemodel *getBlockParams() {return blockParams->getParamTable();}

/*    int getHermaphroditism() const;
    void setHermaphroditism(int value);
    int getHermaphSeason() const {return hermaphSeason;}
    void setHermaphSeason(int value) {hermaphSeason = value;}
    int getHermaphMales() const {return hermaphMales;}
    void setHermaphMales(int value) {hermaphMales = value;}
    tablemodel *getHermaphParams () {return hermaphParams;}
    QStringList getHermaphParam (int index) {return hermaphParams->getRowData(index);}
    void setHermaphParam (int index, QStringList data) {hermaphParams->setRowData(index, data);}*/
/*    growth_pattern * getPattern(int i) const;
    void setPattern(int i, growth_pattern *&value);*/

//    int getNum_params() const {return paramtable->rowCount();}
//    void setNum_params(int value);

//    void setParameter(int index, QStringList data);
//    void addParameter(QStringList data);
//    QStringList getParameter(int index);
//    tablemodel *getParamsModel() {return paramtable;}

    longParameter * getParam(int i) const;
    void setParam(int i, longParameter *&value);

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
    ss_model *parnt;
    QList<growthPattern *> patterns;
    int num_patterns;

    int num_morphs;
    float morph_within_ratio;
//    float morph_dist[5];
    int num_ages;

    float fraction_female;
    int natural_mortality_type;
    int natMortNumBreakPoints;
    int natMort_lorenzen_ref_age;
    tablemodel *natMortBreakPoints;
    QStringList natMortHeader;
    tablemodel *natMortAges;


    int model;
    float age_for_l1;
    float age_for_l2;
    float age_min_for_K;
    float age_max_for_K;
    float age_exp_decay;
    float sd_add;
    float feature;
    int cv_growth_pattern;
    int maturity_option;
    tablemodel *matAgeValues;
    float first_mature_age;
    longParameterModel *wtLenParams;
    timeVaryParameterModel *wtLenVarParams;
//    tablemodel *wtLenTVParams;

    int param_offset_method;
    int adjustment_method;
    int param_dev_phase;

    int hermaphroditism;
    int hermaphSeason;
    int hermaphMales;

    tablemodel *morphdisttable;

    longParameterModel *cohortParam;
    timeVaryParameterModel *cohortVarParam;
//    parameterModelTV *cohortParam;
    longParameterModel *fracFemaleParams;
    timeVaryParameterModel *fracFemaleVarParams;
//    parameterModelTV *fracfemaleParams;

//    QList<longParameter *> params;
    int num_params;

    int devPhase;

    int customEnvLink;
    longParameterModel *environmentParams;
    timeVaryParameterModel *environmentVarParams;
//    parameterModelTV *environmentParams;
    int customBlock;
    longParameterModel *blockParams;
    timeVaryParameterModel *blockVarParams;
//    parameterModelTV *blockParams;

    int timeVaryMethod;
    int timeVaryReadParams;
};

#endif // GROWTH_H
