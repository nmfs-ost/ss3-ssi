/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

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
    growthPattern(QObject *parent);
    growthPattern(const growthPattern &rhs);
    ~growthPattern();

    growthPattern &operator =(const growthPattern &rhs);

public slots:
    int getNum_morphs() const;
    void setNum_morphs(int value);

    growth_morph * getMorph(int index) const;
    void setMorph(int index, growth_morph *value);

    void setNatMMaleAgeList (QStringList data) {natMAges->setRowData(1, data);}
    QStringList getNatMMaleAgeList () {return natMAges->getRowData(1);}
    void setNatMFemAgeList (QStringList data) {natMAges->setRowData (0, data);}
    QStringList getNatMFemAgeList () {return natMAges->getRowData(0);}
    tablemodel *getNatMAges () {return natMAges;}

    void setNumGenders(int num);

    int getTimeVaryReadParams () {return timeVaryReadParams;}
    void setTimeVaryReadParams (int value);

    // Female
    void setNumNatMParams (int num);// {natMrtParams->setNumParams(num);}
    int getNumNatMParams () {return femNatMrtParams->getNumParams();}
    void addFemNatMParam (QStringList data) {setFemNatMParam(femNatMrtParams->getNumParams(), data);}
    void setFemNatMParam (QStringList data) {setFemNatMParam(0, data);}
    void setFemNatMParam (int index, QStringList data);
    QStringList getFemNatMParam(int index) {return femNatMrtParams->getParameter(index);}
    void setFemNatMParamHeader (int index, QString hdr);
    QString getFemNatMParamHeader (int index) {return femNatMrtParams->getParamHeader(index);}
    tablemodel *getFemNatMParams() {return femNatMrtParams->getParamTable();}
    longParameterModel *getFemNatMParamModel () {return femNatMrtParams;}
    int getFemNumNatMTVParams () {return femNatMrtVarParams->getNumVarParams();}
    void setFemNatMTVParam (QStringList data) {setFemNatMTVParam(0, data);}
    void setFemNatMTVParam (int index, QStringList data);
    QStringList getFemNatMTVParam(int index) {return femNatMrtVarParams->getVarParameter(index);}
    QString getFemNatMTVParamHeader (int index) {return femNatMrtVarParams->getVarParamHeader(index);}
    tablemodel *getFemNatMTVParams() {return femNatMrtVarParams->getVarParamTable();}

    void setFractionFemaleParam (QStringList data);
    void setFractionFemParamHeader (QString hdr);
    QStringList getFractionFemaleParam () {return fracFmParams->getParameter(0);}
    tablemodel *getFractionFemaleParams () {return fracFmParams->getParamTable();}
    void setFracFmTVParam (int index, QStringList data);
    int getNumFracFmTVParams () {return fracFmVarParams->getNumVarParams();}
    QStringList getFracFmTVParam(int index) {return fracFmVarParams->getVarParameter(index);}
    tablemodel *getFracFmTVParams() {return fracFmVarParams->getVarParamTable();}

    void setNumGrowthParams (int num);
    int getNumGrowthParams () {return femGrowthParams->getNumParams();}
    void addFemGrowthParam (QStringList data) {setFemGrowthParam(femGrowthParams->getNumParams(), data);}
    void setFemGrowthParam (int index, QStringList data);
    QStringList getFemGrowthParam(int index) {return femGrowthParams->getParameter(index);}
    void setFemGrowthParamHeader (int index, QString hdr);
    QString getFemGrowthParamHeader (int index) {return femGrowthParams->getParamHeader(index);}
    tablemodel *getFemGrowthParams() {return femGrowthParams->getParamTable();}
    longParameterModel *getFemGrowthParamModel () {return femGrowthParams;}
    int getFemNumGrowthTVParams () {return femGrowthVarParams->getNumVarParams();}
    void setFemGrowthTVParam (int index, QStringList data);
    QStringList getFemGrowthTVParam(int index) {return femGrowthVarParams->getVarParameter(index);}
    QString getFemGrowthTVParamHeader(int index) {return femGrowthVarParams->getVarParamHeader(index);}
    tablemodel *getFemGrowthTVParams() {return femGrowthVarParams->getVarParamTable();}

    void setNumCVParams (int num);
    int getFemNumCVParams () {return femCvParams->getNumParams();}
    void addFemCVParam (QStringList data) {setFemCVParam(femCvParams->getNumParams(), data);}
    void setFemCVParam (int index, QStringList data);
    void setFemCVParamHeader (int index, QString hdr);
    QStringList getFemCVParam(int index) {return femCvParams->getParameter(index);}
    tablemodel *getFemCVParams() {return femCvParams->getParamTable();}
    void setFemCVTVParam (int index, QStringList data);
    QStringList getFemCVTVParam(int index) {return femCvVarParams->getVarParameter(index);}
    tablemodel *getFemCVTVParams() {return femCvVarParams->getVarParamTable();}

    void setNumFemMatrParams (int num);
    int getNumFemMatrParams () {return femMatureParams->getNumParams();}
    void addFemMatureParam (QStringList data) {setFemCVParam(femMatureParams->getNumParams(), data);}
    void setFemMatureParam (int index, QStringList data);
    void setFemMatureParamHeader (int index, QString hdr);
    QStringList getFemMatureParam(int index) {return femMatureParams->getParameter(index);}
    tablemodel *getFemMatureParams() {return femMatureParams->getParamTable();}
    void setFemMatureTVParam (int index, QStringList data);
    QStringList getFemMatureTVParam(int index) {return femMatureVarParams->getVarParameter(index);}
    tablemodel *getFemMatureTVParams() {return femMatureVarParams->getVarParamTable();}

    // Male
    void addMaleNatMParam (QStringList data) {setMaleNatMParam(maleNatMrtParams->getNumParams(), data);}
    void setMaleNatMParam (QStringList data) {setMaleNatMParam(0, data);}
    void setMaleNatMParam (int index, QStringList data);
    QStringList getMaleNatMParam(int index) {return maleNatMrtParams->getParameter(index);}
    void setMaleNatMParamHeader (int index, QString hdr);
    QString getMaleNatMParamHeader (int index) {return maleNatMrtParams->getParamHeader(index);}
    tablemodel *getMaleNatMParams() {return maleNatMrtParams->getParamTable();}
    longParameterModel *getMaleNatMParamModel () {return maleNatMrtParams;}
    int getMaleNumNatMTVParams () {return maleNatMrtVarParams->getNumVarParams();}
    void setMaleNatMTVParam (QStringList data) {setMaleNatMTVParam(0, data);}
    void setMaleNatMTVParam (int index, QStringList data);
    QStringList getMaleNatMTVParam(int index) {return maleNatMrtVarParams->getVarParameter(index);}
    QString getMaleNatMTVParamHeader (int index) {return maleNatMrtVarParams->getVarParamHeader(index);}
    tablemodel *getMaleNatMTVParams() {return maleNatMrtVarParams->getVarParamTable();}

    int getMaleNumGrowthParams () {return maleGrowthParams->getNumParams();}
    void addMaleGrowthParam (QStringList data) {setMaleGrowthParam(maleGrowthParams->getNumParams(), data);}
    void setMaleGrowthParam (int index, QStringList data);
    QStringList getMaleGrowthParam(int index) {return maleGrowthParams->getParameter(index);}
    void setMaleGrowthParamHeader (int index, QString hdr);
    QString getMaleGrowthParamHeader (int index) {return maleGrowthParams->getParamHeader(index);}
    tablemodel *getMaleGrowthParams() {return maleGrowthParams->getParamTable();}
    longParameterModel *getMaleGrowthParamModel () {return maleGrowthParams;}
    int getMaleNumGrowthTVParams () {return maleGrowthVarParams->getNumVarParams();}
    void setMaleGrowthTVParam (int index, QStringList data);
    QStringList getMaleGrowthTVParam(int index) {return maleGrowthVarParams->getVarParameter(index);}
    QString getMaleGrowthTVParamHeader(int index) {return maleGrowthVarParams->getVarParamHeader(index);}
    tablemodel *getMaleGrowthTVParams() {return maleGrowthVarParams->getVarParamTable();}

    int getMaleNumCVParams () {return maleCvParams->getNumParams();}
    void addMaleCVParam (QStringList data) {setMaleCVParam(maleCvParams->getNumParams(), data);}
    void setMaleCVParam (int index, QStringList data);
    void setMaleCVParamHeader (int index, QString hdr);
    QStringList getMaleCVParam(int index) {return maleCvParams->getParameter(index);}
    tablemodel *getMaleCVParams() {return maleCvParams->getParamTable();}
    void setMaleCVTVParam (int index, QStringList data);
    QStringList getMaleCVTVParam(int index) {return maleCvVarParams->getVarParameter(index);}
    tablemodel *getMaleCVTVParams() {return maleCvVarParams->getVarParamTable();}

    void setNumWtLenParams (int num);
    int getNumWtLenParams () {return femWtLenParams->getNumParams();}
    void addFemWtLenParam (QStringList data) {setFemWtLenParam(getNumWtLenParams(), data);}
    void setFemWtLenParam (int index, QStringList data);
    QStringList getFemWtLenParam (int index) {return femWtLenParams->getParameter(index);}
    void setFemWtLenParamHeader (int index, QString hdr);
    QString getFemWtLenParamHeader (int index) {return femWtLenParams->getParamHeader(index);}
    tablemodel *getFemWtLenParams () {return femWtLenParams->getParamTable();}
    longParameterModel *getFemWtLenParamModel () {return femWtLenParams;}
    int getNumWtLenTVParams () {return femWtLenVarParams->getNumVarParams();}
    void setFemWtLenTVParam (int index, QStringList data);
    QStringList getFemWtLenTVParam (int index) {return femWtLenVarParams->getVarParameter(index);}
    QString getFemWtLenTVParamHeader (int index) {return femWtLenVarParams->getVarParamHeader(index);}
    tablemodel *getFemWtLenTVParams () {return femWtLenVarParams->getVarParamTable();}

    void addMaleWtLenParam (QStringList data) {setMaleWtLenParam(getNumWtLenParams(), data);}
    void setMaleWtLenParam (int index, QStringList data);
    QStringList getMaleWtLenParam (int index) {return maleWtLenParams->getParameter(index);}
    void setMaleWtLenParamHeader (int index, QString hdr);
    QString getMaleWtLenParamHeader (int index) {return maleWtLenParams->getParamHeader(index);}
    tablemodel *getMaleWtLenParams () {return maleWtLenParams->getParamTable();}
    longParameterModel *getMaleWtLenParamModel () {return maleWtLenParams;}
    int getNumMaleWtLenTVParams () {return maleWtLenVarParams->getNumVarParams();}
    void setMaleWtLenTVParam (int index, QStringList data);
    QStringList getMaleWtLenTVParam (int index) {return maleWtLenVarParams->getVarParameter(index);}
    QString getMaleWtLenTVParamHeader (int index) {return maleWtLenVarParams->getVarParamHeader(index);}
    tablemodel *getMaleWtLenTVParams () {return maleWtLenVarParams->getVarParamTable();}

    // Hermaphroditism
    void setNumHermaphParams (int num);
    int getNumHermaphParams () {return hermaphParams->getNumParams();}
    void addHermaphParam (QStringList data) {setFemCVParam(hermaphParams->getNumParams(), data);}
    void setHermaphParam (int index, QStringList data);
    void setHermaphParamHeader (int index, QString hdr);
    QStringList getHermaphParam(int index) {return hermaphParams->getParameter(index);}
    QString getHermaphParamHeader (int index) {return hermaphParams->getParamHeader(index);}
    tablemodel *getHermaphParams() {return hermaphParams->getParamTable();}
    int getNumHermaphTVParams() {return hermaphVarParams->getNumVarParams();}
    void setHermaphTVParam (int index, QStringList data);
    QStringList getHermaphTVParam(int index) {return hermaphVarParams->getVarParameter(index);}
    QString getHermaphTVParamHeader (int index) {return hermaphVarParams->getVarParamHeader(index);}
    tablemodel *getHermaphTVParams() {return hermaphVarParams->getVarParamTable();}


private:
    ss_model *ssModel;

    int num_morphs;
    QList<growth_morph *> morphs;

    void clear();
    growthPattern& copy (const growthPattern &rhs);

    tablemodel *natMAges;

    int timeVaryReadParams;

    longParameterModel *femNatMrtParams;
    timeVaryParameterModel *femNatMrtVarParams;
    longParameterModel *maleNatMrtParams;
    timeVaryParameterModel *maleNatMrtVarParams;

    longParameterModel *femGrowthParams;
    timeVaryParameterModel *femGrowthVarParams;
    longParameterModel *maleGrowthParams;
    timeVaryParameterModel *maleGrowthVarParams;

    longParameterModel *femCvParams;
    timeVaryParameterModel *femCvVarParams;
    longParameterModel *maleCvParams;
    timeVaryParameterModel *maleCvVarParams;

    longParameterModel *femWtLenParams;
    timeVaryParameterModel *femWtLenVarParams;
    longParameterModel *maleWtLenParams;
    timeVaryParameterModel *maleWtLenVarParams;

    longParameterModel *fracFmParams;
    timeVaryParameterModel *fracFmVarParams;

    longParameterModel *femMatureParams;
    timeVaryParameterModel *femMatureVarParams;

    longParameterModel *hermaphParams;
    timeVaryParameterModel *hermaphVarParams;

};

#endif // GROWTH_PATTERN_H
