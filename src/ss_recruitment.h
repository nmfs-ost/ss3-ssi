/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SS_RECRUITMENT_H
#define SS_RECRUITMENT_H

#include "short_parameter.h"
#include "long_parameter.h"
#include "input_file.h"
#include "parametermodelTV.h"
#include "fileIOgeneral.h"
#include "setupmodel.h"

#include "ss_observation.h"

#include <QObject>
#include <QList>
#include <QString>
//#include <map>

class spawn_recruit : public QObject
{
    Q_OBJECT

public:
    explicit spawn_recruit(ss_model *parent = 0);
    ~spawn_recruit();

    QString toText();
//    QString toXML();
  //  QString toJSON();
    void fromFile(ss_file *file);
//    void fromXML (xml_file *file);
  //  void fromJSON (json_file *file);

    void reset ();
    void setParameterHeaders();
    void setDefaultParameters();

public slots:
    void setOption (int option);
    int getOption () {return getMethod();}
    void setMethod (int value);
    int getMethod () {return method;}

    void setSPR (float spr) {tempSPR = spr;}
    float getSPR () {return tempSPR;}

    void setUseSteepness (int value) {use_steepness = value;}
    int getUseSteepness () {return use_steepness;}

    void setFeature (int value) {futureFeature = value;}
    int getFeature () {return futureFeature;}

    QStringList getParameter (int num);
    void setParameter (int num, QStringList data);
    QString getParameterHeader (int num);
    void setParameterHeader (int num, QString hdr);
    longParameterModel *getFullParameterModel () {return full_parameters;}
    tablemodel *getFullParameters() {return full_parameters->getParamTable();}
    int getNumFullParameters() {return getFullParameters()->rowCount();}
    void setFullParameter(int index, QStringList values);
    void setFullParameterHeader(int index, QString hdr);
    QStringList getFullParameter(int index);
    void setParamNums();

    int getDistribMethod() const {return distrib_method;}
    void setDistribMethod (int value);
    int getDistribArea () const {return distrib_area;}
    void setDistribArea (int value) {distrib_area = value;}
    bool getDoRecruitInteract() const;
    void setDoRecruitInteract(bool value);
    void setDoRecruitInteract (int value);
    int getNumAssignments();
    void setNumAssignments(int rows);
    tablemodel *getAssignments() const;
    void setAssignment(int row, QStringList data);
    int getNumAssignTimings () const;
    QStringList getAssignTimings () const;
    void setAssignTimings ();
    QStringList getAssignment(int row);
    tablemodel *getCycleParams () {return cycleParams->getParamTable();}
    void setNumCycleParams (int num) {cycleParams->setNumParams(num);}
    int getNumCycleParams () {return cycleParams->getNumParams();}
    void addCycleParam (QStringList data) {setInteractParam(getNumInteractParams(), data);}
    void setCycleParam (int index, QStringList data);
    QStringList getCycleParam(int index) {return cycleParams->getParameter(index);}


    tablemodel *getRecruitDevTable () {return recruitDevs;}
    void setNumRecDev(int value) {recruitDevs->setRowCount(value);}
    int getNumRecDev() {return recruitDevs->rowCount();}
    void setRecruitDev (int index, QStringList data) {recruitDevs->setRowData(index, data);}
    QStringList getRecruitDev (int index) {return recruitDevs->getRowData(index);}

    tablemodel *getInteractParams () {return interactParams->getParamTable();}
    void setNumInteractParams (int num) {interactParams->setNumParams(num);}
    int getNumInteractParams () {return interactParams->getNumParams();}
    void addInteractParam (QStringList data) {setInteractParam(getNumInteractParams(), data);}
    void setInteractParam (int index, QStringList data);
    QStringList getInteractParam(int index) {return interactParams->getParameter(index);}


    tablemodel *getDistParams() {return distParams->getParamTable();}
    void setNumDistParams (int num);
    int getNumDistParams () {return distParams->getNumParams();}
    void addDistParam (QStringList data) {setDistParam(getNumDistParams(), data);}
    void setDistParam (int index, QStringList data);
    QStringList getDistParam(int index) {return distParams->getParameter(index);}
    void setDistTVParam (int index, QStringList data);
    tablemodel *getDistTVParams() {return distVarParams->getVarParamTable();}
    int getNumDistTVParams () {return distVarParams->getNumVarParams();}
    QStringList getDistTVParam(int index) {return distVarParams->getVarParameter(index);}

    void setTimeVaryReadParams(int flag);
    int getTimeVaryReadParams() {return readtvparams;}
    tablemodel *getTimeVaryParams () {return varParameters->getVarParamTable();}
    timeVaryParameterModel *getTVParameterModel() {return varParameters;}
    void setNumTVParameters(int num) {varParameters->setNumVarParams(num);}
    int getNumTVParameters() {return varParameters->getNumVarParams();}
    void addTVParameter(QStringList values) {setTVParameter(varParameters->getNumVarParams(), values);}
    void setTVParameter(int index, QStringList values);
    void setTVParameterHeader(int index, QString hdr) {varParameters->setVarParamHeader(index, hdr);}
    QStringList getTVParameter(int index) {return varParameters->getVarParameter(index);}

    int getRecDevStartYr() const;
    void setRecDevStartYr(int value);

    int getRecDevEndYr() const;
    void setRecDevEndYr(int value);

    int getRecDevPhase() const;
    void setRecDevPhase(int value);

    bool getAdvancedOpts() const;
    void setAdvancedOpts(bool value);

    int getRecDevEarlyStart() const;
    void setRecDevEarlyStart(int value);

    int getRecDevEarlyPhase() const;
    void setRecDevEarlyPhase(int value);

    int getFcastRecPhase() const;
    void setFcastRecPhase(int value);

    float getFcastLambda() const;
    void setFcastLambda(float value);

    double getNobiasLastEarlyYr() const;
    void setNobiasLastEarlyYr(double value);

    double getFullbiasFirstYr() const;
    void setFullbiasFirstYr(double value);

    double getFullbiasLastYr() const;
    void setFullbiasLastYr(double value);

    double getNobiasFirstRecentYr() const;
    void setNobiasFirstRecentYr(double value);

    float getMaxBiasAdjust() const;
    void setMaxBiasAdjust(float value);

    int getRecCycles() const;
    void setRecCycles(int value);

    int getRecDevMin() const;
    void setRecDevMin(int value);

    int getRecDevMax() const;
    void setRecDevMax(int value);

    int getRecDevCode() const;
    void setRecDevCode(int value);

/*    int getNumRecDev() const;
    void setNumRecDev(int value);*/

signals:
    void methodChanged();

private:
    ss_model *parnt;

    float tempSPR;
    int   method; // 2=Ricker; 3=std_B-H; 4=SCAA; 5=Hockey; 6=B-H_flattop; 7=survival_3Parm; 8=Shepard_3Parm
    int   use_steepness; // 0/1 to use steepness in initial equ recruitment calculation
    float env_link;     //
    int   env_target;   // 0=none;1=devs;_2=R0;_3=steepness
    int   rec_dev_code;      // recruitment deviations: 0=none; 1=devvector; 2=simple deviations
    int   rec_dev_start_yr; // first year of main recr_devs
    int   rec_dev_end_yr;   // last year of main recr_devs
    int   rec_dev_phase;    // rec_dev phase
    bool  advanced_opts; // use 13 advanced options
    int   rec_dev_early_start; // 0=none, neg value makes relative to rec_dev_start_yr
    int   rec_dev_early_phase; //
    int   fcast_rec_phase;  // forecast recruitment phase, 0 resets to maxphase + 1
    float fcast_lambda;  //
    double nobias_last_early_yr; //
    double fullbias_first_yr; //
    double fullbias_last_yr; //
    double nobias_first_recent_yr; //
    float max_bias_adjust;  // max bias adjustment in MPD
//    int   rec_cycles;   // period of cycles in recruitment
    int   rec_dev_min;  // min num of rec devs
    int   rec_dev_max;  // max
//    int   num_rec_dev;  // recruitment deviations specified by year

    int distrib_method;
    int distrib_area;
    tablemodel *assignments;
    tablemodel *assignTimings;
    bool doRecruitInteract;
    QStringList header;

    QList<int> parmsUsed;
    QList<int> parmNums;
    longParameterModel *full_parameters;
    int readtvparams;
    timeVaryParameterModel *varParameters;

    longParameterModel *distParams;
    timeVaryParameterModel *distVarParams;
    longParameterModel *interactParams;
    timeVaryParameterModel *interVarParams;
    longParameterModel *cycleParams;
    timeVaryParameterModel *cycleVarParams;

    tablemodel *recruitDevs;


    QString sr_text;

    int futureFeature;
};

#endif // SS_RECRUITMENT_H
