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

class selectivity : public QObject
{
    Q_OBJECT
public:
    explicit selectivity (ss_model *model, int method = 0);
    ~selectivity();

public slots:
    void setPattern (int value);
    int getPattern () {return setup->getValue(0);}// pattern;}
    void setDiscard (int value);
    int getDiscard () {return setup->getValue(1);}//discard;}
    void setMale (int value);
    int getMale () {return setup->getValue(2);}// male;}
    void setSpecial (int value);
    int getSpecial () {return setup->getValue(3);}// special;}
    void setNumAges(int ages) {numAges = ages;}
    void setSetup(QString text);
    QString getSetupText ();
    void setSetup(QStringList strList);
    QStringList getSetup () {return setup->getData();}
    void setSetup(QList<int> values);

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
    QStringList getTimeVaryParameter (int index) {return varParameters->getVarParameter(index);}//timeVaryParameters->getParameter(index);}
    QString getTimeVaryParameterLabel (int index) {return varParameters->getVarParamHeader(index);}//timeVaryParameters->getParamHeader(index);}
    tablemodel *getTimeVaryParameterModel() {return varParameters->getVarParamTable();}//timeVaryParameters;}

    int getNumRetainParameters() {return retainParameters->getNumParams();}
    void setRetainParameter(int index, QStringList strList);
    void setRetainParameterLabel (int index, QString label);
    QString getRetainParameterText (int index);
    QStringList getRetainParameter (int index) {return retainParameters->getParameter(index);}
    QString getRetainParameterLabel (int index) {return retainParameters->getParamHeader(index);}
    shortParameterModel *getRetainParameterModel () {return retainParameters;}

    int getNumDiscardParameters();
    void setDiscardParameter(int index, QStringList strList);
    void setDiscardParameterLabel (int index, QString label);
    QString getDiscardParameterText (int index);
    QStringList getDiscardParameter (int index) {return discardParameters->getParameter(index);}
    QString getDiscardParameterLabel (int index) {return discardParameters->getParamHeader(index);}
    shortParameterModel *getDiscardParameterModel () {return discardParameters;}

    int getNumSpecialParameters();
    void setSpecialParameter(int index, QStringList strList);
    void setSpecialParameterLabel (int index, QString label);
    QString getSpecialParameterText (int index);
    QStringList getSpecialParameter (int index) {return specialParameters->getParameter(index);}
    QString getSpecialParameterLabel (int index) {return specialParameters->getParamHeader(index);}
    shortParameterModel *getSpecialParameterModel () {return specialParameters;}

    int getNumMaleParameters();
    void setMaleParameter(int index, QStringList strList);
    void setMaleParameterLabel (int index, QString label);
    QString getMaleParameterText (int index);
    QStringList getMaleParameter (int index) {return maleParameters->getParameter(index);}
    QString getMaleParameterLabel (int index) {return maleParameters->getParamHeader(index);}
    shortParameterModel *getMaleParameterModel () {return maleParameters;}

//    double operator()() {return evaluate();}
//    double evaluate();

    void setMethod (int method);

protected:
//    void setEquation (int method);

    int pattern;
    int discard;
    int male;
    int special;

    int numAges;

    setupModel *setup;
    longParameterModel *parameters;
    timeVaryParameterModel *varParameters;

//    shortParameterModel *timeVaryParameters;

    shortParameterModel *retainParameters;
    shortParameterModel *discardParameters;
    shortParameterModel *maleParameters;
    shortParameterModel *specialParameters;

//    double evaluate(int f, float m);

//    selex_equation *equation;

};

#endif // SELECTIVITY_H
