#ifndef GROWTH_PATTERN_H
#define GROWTH_PATTERN_H

#include "growth_morph.h"
#include "parametermodelTV.h"
#include "setupmodel.h"

class growthPattern
{
public:
    growthPattern(ss_model *parent);
    growthPattern(const growthPattern &rhs);
    ~growthPattern();

    int getNum_morphs() const;
    void setNum_morphs(int value);

    growth_morph * getMorph(int index) const;
    void setMorph(int index, growth_morph *&value);

    growthPattern &operator =(const growthPattern &rhs);

    void setNumNatMParams (int num) {natMrtParams->setNumParams(num);}
    int getNumNatMParams () {return natMrtParams->getNumParams();}
    void addNatMParam (QStringList data) {setNatMParam(natMrtParams->getNumParams(), data);}
    void setNatMParam (QStringList data) {setNatMParam(0, data);}
    void setNatMParam (int index, QStringList data);
    QStringList getNatMParam(int index) {return natMrtParams->getParameter(index);}
    tablemodel *getNatMParams() {return natMrtParams->getParamTable();}
//    void setNumNatMTVParams (int num) {natMParams->getParamVarsModel()->setRowCount(num);}
    int getNumNatMTVParams () {return natMrtParams->getNumParams();}
//    void addNatMTVParam (QStringList data) {setNatMParam(natMTVParams->getParamCount(), data);}
    void setNatMTVParam (QStringList data) {setNatMTVParam(0, data);}
    void setNatMTVParam (int index, QStringList data);
    QStringList getNatMTVParam(int index) {return natMrtParams->getParameter(index);}
    tablemodel *getNatMTVParams() {return natMrtParams->getParamTable();}
    longParameterModel *getNatMParamModel () {return natMrtParams;}

    void setNatMMaleAgeList (QStringList data) {natMAges->setRowData(1, data);}
    QStringList getNatMMaleAgeList () {return natMAges->getRowData(1);}
    void setNatMFemAgeList (QStringList data) {natMAges->setRowData (0, data);}
    QStringList getNatMFemAgeList () {return natMAges->getRowData(0);}
    tablemodel *getNatMAges () {return natMAges;}

    void setFractionFemaleParam (QStringList data) {fracFmParams->setParameter(0, data);}
    QStringList getFractionFemaleParam () {return fracFmParams->getParameter(0);}
    tablemodel *getFractionFemaleParams () {return fracFmParams->getParamTable();}//->getParameters();}

    void setNumGrowthParams (int num) {growthParams->setNumParams(num);}
    int getNumGrowthParams () {return growthParams->getNumParams();}
    void addGrowthParam (QStringList data) {setGrowthParam(growthParams->getNumParams(), data);}
    void setGrowthParam (int index, QStringList data);
    QStringList getGrowthParam(int index) {return growthParams->getParameter(index);}
    tablemodel *getGrowthParams() {return growthParams->getParamTable();}
    void setNumGrowthTVParams (int num) {growthParams->getParamModel()->setNumParams(num);}
    int getNumGrowthTVParams () {return growthParams->getNumParams();}
    void addGrowthTVParam (QStringList data) {setGrowthTVParam(growthParams->getNumParams(), data);}
    void setGrowthTVParam (int index, QStringList data);
    QStringList getGrowthTVParam(int index) {return growthParams->getParameter(index);}
    tablemodel *getGrowthTVParams() {return growthParams->getParamTable();}
    longParameterModel *getGrowthParamModel () {return growthParams;}

/*    void setNumTimeVaryParams (int num) {timeVaryParams->setParamCount(num);}
    int getNumTimeVaryParams () {return timeVaryParams->getParamCount();}
    void addTimeVaryParam (QStringList data) {setGrowthParam(timeVaryParams->getParamCount(), data);}
    void setTimeVaryParam (int index, QStringList data);
    void setTimeVaryParamHeader (int index, QString header);
    QStringList getTimeVaryParam(int index) {return timeVaryParams->getParameter(index);}
    QString getTimeVaryParamHeader (int index) {return timeVaryParams->getParamHeader(index);}
    tablemodel *getTimeVaryParams() {return timeVaryParams->getParameters();}*/

    void setNumCVParams (int num) {cvParams->setNumParams(num);}
    int getNumCVParams () {return cvParams->getNumParams();}
    void addCVParam (QStringList data) {setCVParam(cvParams->getNumParams(), data);}
    void setCVParam (int index, QStringList data);
    QStringList getCVParam(int index) {return cvParams->getParameter(index);}
    tablemodel *getCVParams() {return cvParams->getParamTable();}
//    void setNumCVTVParams (int num) {cvParams->getParamVarsModel()->setRowCount(num);}
//    int getNumCVTVParams () {return cvParams->getNumParamVars();}
//    void addCVTVParam (QStringList data) {setCVTVParam(getNumCVTVParams(), data);}
//    void setCVTVParam (int index, QStringList data);
//    QStringList getCVTVParam(int index) {return cvParams->getParamVarData(index);}
//    tablemodel *getCVTVParams() {return cvParams->getParamVarsModel();}

    void setNumDevParams (int num) {devParams->setNumParams(num);}
    int getNumDevParams () {return devParams->getNumParams();}
    void addDevParam (QStringList data) {setDevParam(devParams->getNumParams(), data);}
    void setDevParam (int index, QStringList data);
    QStringList getDevParam(int index) {return devParams->getParameter(index);}
    tablemodel *getDevParams() {return devParams->getParamTable();}
    longParameterModel *getDevParamModel () {return devParams;}

private:
    ss_model *parnt;

    int num_morphs;
    QList<growth_morph *> morphs;

    void clear();
    growthPattern& copy (const growthPattern &rhs);

    longParameterModel *fracFmParams;
    //timeVaryParameterModel *fracFmTVparams;

    longParameterModel *devParams;
    //timeVaryParameterModel *devTVparams;

    longParameterModel *growthParams;
    //timeVaryParameterModel *growthTVparams;
    longParameterModel *cvParams;
    longParameterModel *cohtDvParams;
    //timeVaryParameterModel *cohtDvTVparams;
    longParameterModel *matureParams;
    //timeVaryParameterModel *matureTVparams;

    longParameterModel *natMrtParams;
    //timeVaryParameterModel *natMrtTVparams;
    tablemodel *natMAges;
};

#endif // GROWTH_PATTERN_H
