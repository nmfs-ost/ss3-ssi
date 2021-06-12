/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SELECTIVITY_H
#define SELECTIVITY_H

#include <QObject>
#include <QString>
#include <QList>

#include "method_setup.h"
//#include "long_parameter.h"
#include "selex_equation.h"
#include "parametermodelTV.h"
#include "setupmodel.h"

class Fleet;

enum SelexType {
    Age,
    Size
};

class selectivity : public QObject
{
    Q_OBJECT
public:
    explicit selectivity (QObject *model, int method = 0);
    selectivity(QObject *model, SelexType sType, int method = 0);
    ~selectivity();

    int getNumGenders() const;
    void setNumGenders(int value);

    QList<float> getBinVals() const;
    void setBinVals(const QStringList &value);
    float getBinValue (int index);
    void setBins (const QStringList &value);
    void setAltBins (const QStringList &value);
    int getNumBins () {return bins.count();}
    int getNumAltBins () {return altbins.count();}
    QStringList &getBins() {return bins;}
    QStringList &getAltBins() {return altbins;}

    void reset();
    void connectSigs ();
    void disconnectSigs ();

    SelexType getType() const;
    void setType(const SelexType &value);


signals:
    void setupChanged(QStringList values);
    void dataChanged();
    void parametersChanged();
    void startingSetupChanges();

public slots:
    void setFleet (Fleet *flt) {fleet = flt;}
    void setFleetName (QString name) {fisheryName = name;}
    void setFleetNum (int num) {fisheryNum = num;}
    void setNumAges (int ages) {numAges = ages;}
    void setNumLens (int lens) {setNumBinVals(lens);}
    void setNumBinVals(int num);
    int getNumXvals() {return numXvals;}
    void setPattern (int value);
    int getPattern () {return setup->getValue(0);}
    void setDiscard (int value);
    int getDiscard () {return setup->getValue(1);}
    void setMale (int value);
    int getMale () {return setup->getValue(2);}
    void setSpecial (int value);
    int getSpecial () {return setup->getValue(3);}
    void setSetup(QString text);
    QString getSetupText ();
    void setSetup(QStringList strList);
    const QStringList getSetup () {return setup->getData();}
    void setSetup(QList<int> values);
    void changeSetup(QList<int> values);
    void setTVautogenerate (int val);
    void changeTVautogenerate (int val);

    tablemodel *getSetupModel () {return setup->getTable();}

    void setParameter (int index, QString text);
    void setParameter (int index, QStringList strList);
    QString getParameterText (int index);
    QStringList getParameter (int index);
    void setParameterLabel (int index, QString label);
    QString getParameterLabel (int index);
    int getNumParameters();
    void setNumParameters (int num);
    tablemodel *getParameterModel() {return parameters->getParamTable();}

    int getNumTimeVaryParameters ();
    void setTimeVaryParameter (int index, QStringList strList);
    void setTimeVaryParameterLabel (int index, QString label);
    QString getTimeVaryParameterText (int index);
    QStringList getTimeVaryParameter (int index) {return varParameters->getVarParameter(index);}
    QString getTimeVaryParameterLabel (int index) {return varParameters->getVarParamHeader(index);}
    tablemodel *getTimeVaryParameterModel() {return varParameters->getVarParamTable();}

    int getNumRetainParameters() {return retainParameters->getNumParams();}
    void setRetainParameter(int index, QStringList strList);
    void setRetainParameterLabel (int index, QString label);
    QString getRetainParameterText (int index);
    QStringList getRetainParameter (int index) {return retainParameters->getParameter(index);}
    QString getRetainParameterLabel (int index) {return retainParameters->getParamHeader(index);}
    longParameterModel *getRetainParameterModel () {return retainParameters;}
    tablemodel *getRetainParameterTable () {return retainParameters->getParamTable();}

    int getNumRetainTimeVaryParameters ();
    void setRetainTimeVaryParameter (int index, QStringList strList);
    void setRetainTimeVaryParameterLabel (int index, QString label);
    QString getRetainTimeVaryParameterText (int index);
    QStringList getRetainTimeVaryParameter (int index) {return retainVarParameters->getVarParameter(index);}
    QString getRetainTimeVaryParameterLabel (int index) {return retainVarParameters->getVarParamHeader(index);}
    tablemodel *getRetainTimeVaryParameterModel() {return retainVarParameters->getVarParamTable();}

    int getNumDiscardParameters();
    void setDiscardParameter(int index, QStringList strList);
    void setDiscardParameterLabel (int index, QString label);
    QString getDiscardParameterText (int index);
    QStringList getDiscardParameter (int index) {return discardParameters->getParameter(index);}
    QString getDiscardParameterLabel (int index) {return discardParameters->getParamHeader(index);}
    longParameterModel *getDiscardParameterModel () {return discardParameters;}
    tablemodel *getDiscardParameterTable () {return discardParameters->getParamTable();}

    int getNumDiscardTimeVaryParameters ();
    void setDiscardTimeVaryParameter (int index, QStringList strList);
    void setDiscardTimeVaryParameterLabel (int index, QString label);
    QString getDiscardTimeVaryParameterText (int index);
    QStringList getDiscardTimeVaryParameter (int index) {return discardVarParameters->getVarParameter(index);}
    QString getDiscardTimeVaryParameterLabel (int index) {return discardVarParameters->getVarParamHeader(index);}
    tablemodel *getDiscardTimeVaryParameterModel() {return discardVarParameters->getVarParamTable();}

/*    int getNumSpecialParameters();
    void setSpecialParameter(int index, QStringList strList);
    void setSpecialParameterLabel (int index, QString label);
    QString getSpecialParameterText (int index);
    QStringList getSpecialParameter (int index) {return specialParameters->getParameter(index);}
    QString getSpecialParameterLabel (int index) {return specialParameters->getParamHeader(index);}
    shortParameterModel *getSpecialParameterModel () {return specialParameters;}
    tablemodel * getSpecialParameterTable () {return specialParameters->getParamTable();}*/

    int getNumMaleParameters();
    void setMaleParameter(int index, QStringList strList);
    void setMaleParameterLabel (int index, QString label);
    QString getMaleParameterText (int index);
    QStringList getMaleParameter (int index) {return maleParameters->getParameter(index);}
    QString getMaleParameterLabel (int index) {return maleParameters->getParamHeader(index);}
    longParameterModel *getMaleParameterModel () {return maleParameters;}
    tablemodel * getMaleParameterTable () {return maleParameters->getParamTable();}

    int getNumMaleTimeVaryParameters ();
    void setMaleTimeVaryParameter (int index, QStringList strList);
    void setMaleTimeVaryParameterLabel (int index, QString label);
    QString getMaleTimeVaryParameterText (int index);
    QStringList getMaleTimeVaryParameter (int index) {return maleVarParameters->getVarParameter(index);}
    QString getMaleTimeVaryParameterLabel (int index) {return maleVarParameters->getVarParamHeader(index);}
    tablemodel *getMaleTimeVaryParameterModel() {return maleVarParameters->getVarParamTable();}

    void setMethod (int method);
    void setDefaultParams (int method = -1, int special = -1);
    void autogenParameters(int flag = 0);
    int getMinSel() const;
    void setMinSel(int value);

protected:
    ss_model *data_model;
    Fleet *fleet;

//    void setEquation (int method);
    QString fisheryName;
    int fisheryNum;

    SelexType type;

    QStringList bins;
    QStringList altbins;

    QList<float> xVals;
    QList<float> binVals;
    int numXvals;
    int numAges;
    int numGenders;
    int minSel;

    setupModel *setup;
    longParameterModel *parameters;
    timeVaryParameterModel *varParameters;

    longParameterModel *retainParameters;
    timeVaryParameterModel *retainVarParameters;
    longParameterModel *discardParameters;
    timeVaryParameterModel *discardVarParameters;
    longParameterModel *maleParameters;
    timeVaryParameterModel *maleVarParameters;

    void autogenCubicSpline1(int scale = 0);
    void autogenCubicSpline2(int scale = 0);


};

#endif // SELECTIVITY_H
