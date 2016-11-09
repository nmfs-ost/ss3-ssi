#ifndef Q_H
#define Q_H

#include <QString>
#include <QList>

#include "method_setup.h"
#include "short_parameter.h"
#include "parametermodel.h"

class q_ratio
{
public:
    q_ratio();
    ~q_ratio();

    void reset();
    q_ratio *copy (q_ratio *rhs);

    void setup(QStringList values);
    void setup(QString str);
    QString getSetup();

    parametermodel *getSetupModel() {return qsetup;}

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
    parametermodel *getParamModel() {return params;}

    void setNumTVParams (int num) {tvParams->setRowCount(num);}
    int getNumTVParams () {return tvParams->rowCount();}
    void addTVParam (QStringList data) {setTVParam(getNumTVParams(), data);}
    void setTVParam (int index, QStringList data);
    QStringList getTVParam (int index) {return tvParams->getRowData(index);}
    parametermodel *getTVParams () {return tvParams;}

    int getTypeIndex() const;
    void setTypeIndex(int value);

    int getLinkType ();
    int getDoBiasAdj ();
    int getFloatQ ();

public slots:
    void setupChanged ();
    void setupChanged (int error_type);

private:
    parametermodel *qsetup;
    parametermodel *params;
    parametermodel *tvParams;
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

#endif // Q_H
