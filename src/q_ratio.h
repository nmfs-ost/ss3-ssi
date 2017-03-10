#ifndef Q_RATIO_H
#define Q_RATIO_H

#include <QString>
#include <QList>

#include "method_setup.h"
//#include "short_parameter.h"
#include "setupmodel.h"
//#include "model.h"

class ss_model;

class q_ratio : public QObject
{
    Q_OBJECT
public:
    q_ratio(ss_model *model);
    ~q_ratio();

    void setParentModel (ss_model model);
    void reset();

    void setFleetName(QString name);
    void setValues(QStringList values);
    void setValues(QString str);
    QString getSetup();
    tablemodel *getSetupTable () {return setup->getTable();}


    bool getDoPower();
    void setDoPower(bool value);

    bool getDoExtraSD();
    void setDoExtraSD(bool value);

//    float getOffset();
//    void setOffset(float value);

    QString getPower();
    void setPower(QStringList values);

//    QString getVariable();
//    void setEnvLink(QStringList values);

    QString getExtra();
    void setExtra(QStringList values);

    QString getBase();
    void setBase(QStringList values);

    void setNumParams (int num) {params->setNumParams(num);}
    int getNumParams() {return params->getNumParams();}
    void setParameter (int index, QStringList values) {params->setParameter(index, values);}
    void setParameterData (int index, QStringList values) {params->setParamData(index, values);}
    QStringList getParameter(int index) {return params->getParameter(index);}
    tablemodel *getParamTable() {return params->getParamTable();}
    void setParamHdrs(int errType = 1);

    int getNumTVParams () {return varParams->getNumVarParams();}
    void setTVParam (int index, QStringList data) {varParams->setVarParameter(index, data);}
    QStringList getTVParam (int index) {return varParams->getVarParameter(index);}
    tablemodel *getTVParams () {return varParams->getVarParamTable();}

    int getAutoGenerate () {return varParams->getAutoGenerate();}
    void setAutoGenerate (int flag) {varParams->setAutoGenerate(flag);}
    int getLinkType ();
    int getDoBiasAdj ();
    int getFloatQ ();

public slots:
    void changeSetupData(QList<int> values);
    void changeSetup (int errType);
    void changeSetup ();

    void changeParamData();
    void checkParamValues(int index, QStringList paramValues);

signals:
    void setupChanged();
    void parametersChanged();
    void varyParamsChanged();

private:
    setupModel *setup;
    longParameterModel *params;
    timeVaryParameterModel *varParams;

    QList<int> paramsUsed;

    QString fltName;

};

#endif // Q_RATIO_H
