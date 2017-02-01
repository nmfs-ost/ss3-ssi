#ifndef Q_RATIO_H
#define Q_RATIO_H

#include <QString>
#include <QList>

#include "method_setup.h"
//#include "short_parameter.h"
#include "setupmodel.h"
#include "model.h"

class q_ratio : public setupParamVarModel
{
public:
    q_ratio(ss_model *model);
    ~q_ratio();

    void setParentModel (ss_model model);
    void reset();
 //   q_ratio *copy (q_ratio *rhs);

    void setup(QStringList values);
    void setup(QString str);
    QString getSetup();

 //   longParameterModel *getSetupModel() {return qsetup;}

    int getDoPower();
    void setDoPower(int value);

    int getDoEnvLink();
    void setDoEnvLink(int value);

    int getDoExtraSD();
    void setDoExtraSD(int value);

    int getType();
    void setType(int value);

    float getOffset();
    void setOffset(float value);

    QString getPower();
    void setPower(QStringList values);

    QString getVariable();
    void setEnvLink(QStringList values);

    QString getExtra();
    void setExtra(QStringList values);

    QString getBase();
    void setBase(QStringList values);

  //  void setParamHdrs (QString name);
 //   void setNumParams (int num);
 //   int getNumParams();
 //   void setParameter (int index, QStringList values);
 //   void setParameter (int index, QString text);
 //   QStringList getParameter(int index);
 //   parameterModelTV *getParamModel() {return getP;}

  //  int getNumTVParams () {return params->getTimeVaryParams()->rowCount();}
 //   void setTVBlkParam (int index, int row, QStringList data);
 //   void setTVDevParam (int index, int row, QStringList data);
 //   void setTVEnvParam (int index, QStringList data);
 //   QStringList getTVParam (int index) {return params->getTimeVaryParam(index);}
 //   tablemodel *getTVParams () {return getParamVarsModel();}

    int getTypeIndex();
    void setTypeIndex(int value);

    int getLinkType ();
    int getDoBiasAdj ();
    int getFloatQ ();

public slots:
    void changeSetup (int errType);
    void changeSetup ();
//    void setupChanged ();
  //  void setupChanged (int error_type);

private:

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
