#ifndef GROWTH_PATTERN_H
#define GROWTH_PATTERN_H

#include <QObject>
#include <QStringList>

#include "growth_morph.h"
#include "parametermodelTV.h"
#include "setupmodel.h"

class growthPattern : public QObject
{
    Q_OBJECT

public:
    growthPattern(ss_model *parent);
    growthPattern(const growthPattern &rhs);
    ~growthPattern();

    growthPattern &operator =(const growthPattern &rhs);

public slots:
    int getNum_morphs() const;
    void setNum_morphs(int value);

    growth_morph * getMorph(int index) const;
    void setMorph(int index, growth_morph *&value);

    void setNumNatMParams (int num);// {natMrtParams->setNumParams(num);}
    int getNumNatMParams () {return natMrtParams->getNumParams();}
    void addNatMParam (QStringList data) {setNatMParam(natMrtParams->getNumParams(), data);}
    void setNatMParam (QStringList data) {setNatMParam(0, data);}
    void setNatMParam (int index, QStringList data);
    QStringList getNatMParam(int index) {return natMrtParams->getParameter(index);}
    void setNatMParamHeader (int index, QString hdr);
    QString getNatMParamHeader (int index) {return natMrtParams->getParamHeader(index);}
    tablemodel *getNatMParams() {return natMrtParams->getParamTable();}
    longParameterModel *getNatMParamModel () {return natMrtParams;}
//    void setNumNatMTVParams (int num) {natMParams->getParamVarsModel()->setRowCount(num);}
    int getNumNatMTVParams () {return natMrtVarParams->getNumVarParams();}
//    void addNatMTVParam (QStringList data) {setNatMParam(natMrtVarParams->getParamCount(), data);}
    void setNatMTVParam (QStringList data) {setNatMTVParam(0, data);}
    void setNatMTVParam (int index, QStringList data);
    QStringList getNatMTVParam(int index) {return natMrtVarParams->getVarParameter(index);}
    QString getNatMTVParamHeader (int index) {return natMrtVarParams->getVarParamHeader(index);}
    tablemodel *getNatMTVParams() {return natMrtVarParams->getVarParamTable();}

    void setNatMMaleAgeList (QStringList data) {natMAges->setRowData(1, data);}
    QStringList getNatMMaleAgeList () {return natMAges->getRowData(1);}
    void setNatMFemAgeList (QStringList data) {natMAges->setRowData (0, data);}
    QStringList getNatMFemAgeList () {return natMAges->getRowData(0);}
    tablemodel *getNatMAges () {return natMAges;}

    void setFractionFemaleParam (QStringList data) {fracFmParams->setParameter(0, data);}
    QStringList getFractionFemaleParam () {return fracFmParams->getParameter(0);}
    tablemodel *getFractionFemaleParams () {return fracFmParams->getParamTable();}

    void setNumGrowthParams (int num);// {growthParams->setNumParams(num);}
    int getNumGrowthParams () {return growthParams->getNumParams();}
    void addGrowthParam (QStringList data) {setGrowthParam(growthParams->getNumParams(), data);}
    void setGrowthParam (int index, QStringList data);
    QStringList getGrowthParam(int index) {return growthParams->getParameter(index);}
    void setGrowthParamHeader (int index, QString hdr);
    QString getGrowthParamHeader (int index) {return growthParams->getParamHeader(index);}
    tablemodel *getGrowthParams() {return growthParams->getParamTable();}
    longParameterModel *getGrowthParamModel () {return growthParams;}
//    void setNumGrowthTVParams (int num) {growthVarParams->getVarParamModel()->setNumParams(num);}
    int getNumGrowthTVParams () {return growthVarParams->getNumVarParams();}
//    void addGrowthTVParam (QStringList data) {setGrowthTVParam(growthVarParams->getNumVarParams(), data);}
    void setGrowthTVParam (int index, QStringList data);
    QStringList getGrowthTVParam(int index) {return growthVarParams->getVarParameter(index);}
    QString getGrowthTVParamHeader(int index) {return growthVarParams->getVarParamHeader(index);}
    tablemodel *getGrowthTVParams() {return growthVarParams->getVarParamTable();}

    void setNumCVParams (int num) {cvParams->setNumParams(num);}
    int getNumCVParams () {return cvParams->getNumParams();}
    void addCVParam (QStringList data) {setCVParam(cvParams->getNumParams(), data);}
    void setCVParam (int index, QStringList data);
    QStringList getCVParam(int index) {return cvParams->getParameter(index);}
    tablemodel *getCVParams() {return cvParams->getParamTable();}


private:
    ss_model *parnt;

    int num_morphs;
    QList<growth_morph *> morphs;

    void clear();
    growthPattern& copy (const growthPattern &rhs);

    tablemodel *natMAges;
    longParameterModel *natMrtParams;
    timeVaryParameterModel *natMrtVarParams;

    longParameterModel *growthParams;
    timeVaryParameterModel *growthVarParams;
    longParameterModel *cvParams;

    longParameterModel *fracFmParams;


    longParameterModel *matureParams;
    timeVaryParameterModel *matureVarParams;

};

#endif // GROWTH_PATTERN_H
