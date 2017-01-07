#ifndef Q_RATIO_H
#define Q_RATIO_H

#include <QString>
#include <QList>

#include "method_setup.h"
//#include "short_parameter.h"
#include "parametermodelTV.h"
#include "model.h"

class q_ratio
{
public:
    q_ratio(ss_model *model);
    ~q_ratio();

    void reset();
    q_ratio *copy (q_ratio *rhs);

    void setup(QStringList values);
    void setup(QString str);
    QString getSetup();

    parameterModel *getSetupModel() {return qsetup;}

    int getDoPower() const;
    void setDoPower(int value);

    int getDoEnvLink() const;
    void setDoEnvLink(int value);

    int getDoExtraSD() const;
    void setDoExtraSD(int value);

    int getType() const;
    void setType(int value);

    float getOffset() const;
    void setOffset(float value);

    QString getPower() const;
    void setPower(QStringList values);

    QString getVariable() const;
    void setEnvLink(QStringList values);

    QString getExtra() const;
    void setExtra(QStringList values);

    QString getBase() const;
    void setBase(QStringList values);

    void setParamHdrs (QString name);
    void setNumParams (int num);
    int getNumParams();
    void setParameter (int index, QStringList values);
    void setParameter (int index, QString text);
    QStringList getParameter(int index);
    parameterModelTV *getParamModel() {return params;}

    int getNumTVParams () {return params->getTimeVarParameters()->rowCount();}
    void setTVBlkParam (int index, int row, QStringList data);
    void setTVDevParam (int index, int row, QStringList data);
    void setTVEnvParam (int index, QStringList data);
    QStringList getTVParam (int index) {return params->getTimeVarParam(index);}
    shortParameterModel *getTVParams () {return params->getTimeVarParameters();}

    int getTypeIndex() const;
    void setTypeIndex(int value);

    int getLinkType ();
    int getDoBiasAdj ();
    int getFloatQ ();

public slots:
    void setupChanged ();
    void setupChanged (int error_type);

private:
    parameterModel *qsetup;
    parameterModelTV *params;
//    parameterModelTV *tvParams;
    int BaseIndex;
    int doBlkTnd;
    int BlkTndIndex;
    int doRndWlk;
    int RndWlkIndex;
    int doPower;
    int powerIndex;
    int doEnvVar;
    int EnvIndex;
    int doExtraSD;
    int ExtraIndex;
    int type;
    int typeIndex;
    float offset;
    bool doOffset;
};

#endif // Q_RATIO_H
