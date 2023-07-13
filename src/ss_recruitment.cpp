#include "ss_recruitment.h"
#include "model.h"

spawn_recruit::spawn_recruit(QObject *parent)
    : QObject (parent)
{
    ssModel = static_cast<ss_model *>(parent);

    full_parameters = new longParameterModel(ssModel);
    varParameters = new timeVaryParameterModel (ssModel);

    full_parameters->setNumParams(10);
/*    parmsUsed << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1;
    full_parameters->setParamsUsed(parmsUsed);
    setDefaultParameters();
    setParameterHeaders();*/
    varParameters->setNumParams(10);

    connect (full_parameters, SIGNAL(paramChanged(int,QStringList)),
             varParameters, SLOT(changeVarParamData(int,QStringList)));

    assignments = new tablemodel(parent);
    assignments->setColumnCount(4);
    assignments->setRowCount(1);
    header << "GP" << "Month" << "Area" << "Settle Age";
    assignments->setHeader(header);
    assignTimings = new tablemodel(parent);
    assignTimings->setRowCount(1);
    assignTimings->setColumnCount(1);
    assignTimings->setRowHeader(0, QString("Settle Timings"));

    interactParams = new longParameterModel(ssModel);
    interVarParams = new timeVaryParameterModel(ssModel);
    distParams = new longParameterModel(ssModel);
    distParams->setNumParams(2);
    distVarParams = new timeVaryParameterModel(ssModel);
    distVarParams->setNumParams(2);

    cycleParams = new longParameterModel(ssModel);
    cycleVarParams = new timeVaryParameterModel(ssModel);

    header.clear();
    header << "Year" << "Input_value";
    recruitDevs = new tablemodel(parent);
    recruitDevs->setHeader(header);

    reset();
}


spawn_recruit::~spawn_recruit()
{
//    delete parameters;
    delete full_parameters;
    delete varParameters;
    delete assignments;
    delete interactParams;
    delete distParams;
    delete cycleParams;
    delete recruitDevs;
}

void spawn_recruit::reset()
{
    disconnect (full_parameters, SIGNAL(paramChanged(int,QStringList)),
             varParameters, SLOT(changeVarParamData(int,QStringList)));
    parmsUsed << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1;
    full_parameters->setParamsUsed(parmsUsed);
    setDefaultParameters();
    setParameterHeaders();
    distrib_method = 2;
    distrib_area = 1;
    setMethod(1);
    env_link = 0;
    env_target = 0;
    rec_dev_code = 1;
    rec_dev_start_yr = 1980;
    rec_dev_end_yr = 2009;
    rec_dev_phase = -3;
    advanced_opts = true;
    rec_dev_early_start = 0;
    rec_dev_early_phase = -4;
    fcast_rec_phase = 0;
    fcast_lambda = 1;
    nobias_last_early_yr = 971;
    fullbias_first_yr = 1931;
    fullbias_last_yr = 2010;
    nobias_first_recent_yr = 2011;
    max_bias_adjust = 0;
    setNumCycleParams(0);
//    rec_cycles = 0;
    rec_dev_min = -5;
    rec_dev_max = 5;
    setNumRecDev(0);
//    num_rec_dev = 0;
    use_steepness = 0;
    connect (full_parameters, SIGNAL(paramChanged(int,QStringList)),
             varParameters, SLOT(changeVarParamData(int,QStringList)));
}

void spawn_recruit::setDefaultParameters()
{
    QStringList parmvalues;
//    parmvalues = full_parameters->getParameter(0);  // SR_R(0)
    parmvalues << "3.0" << "31.0" << "8.85" << "10.3" << "10" << "0" << "1" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(0, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(1);  // SR_steepness
    parmvalues << "0.2" << "1.0" << "0.598" << "0.7" << "0.05" << "1" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(1, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(2);  // SR_Rmin (hockey stick)
    parmvalues << "0.0" << "1.0" << "0.0" << "1" << "0.5" << "6" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(2, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(3);  // SR_Zfrac (survival)
    parmvalues << "0.0" << "3.0" << "1.0" << "0.8" << "0.5" << "1"  << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(3, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(4);  // SR_Beta (survival)
    parmvalues << "0.0" << "2.0" << "0.8" << "1" << "0.5"  << "6" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(4, parmvalues);
    parmvalues.clear();
 //   parmvalues = full_parameters->getParameter(5);  // SR_Shep_C (shepherd)
    parmvalues << "-0.5" << "5.0" << "1.166" << "1" << "0.5" << "6" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(5, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(6);  // SR_RkrPwr_gamma
    parmvalues << "-0.5" << "5.0" << "1.0" << "1" << "0.5" << "6" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(6, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(7);  // SigmaR
    parmvalues << "0" << "2" << "0.6" << "0.8" << "0.8" << "0" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(7, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(8);  // regime
    parmvalues << "-5" << "5" << "0" << "0" << "1" << "0" << "4" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(8, parmvalues);
    parmvalues.clear();
//    parmvalues = full_parameters->getParameter(9);  // autocorr
    parmvalues << "0" << "0" << "0" << "0" << "0" << "0" << "-99" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    full_parameters->setParameter(9, parmvalues);
}

void spawn_recruit::setParameterHeaders()
{
    full_parameters->setParamHeader(0, QString("SR_LN(R0)"));
    full_parameters->setParamHeader(1, QString("SR_steep"));
    full_parameters->setParamHeader(2, QString("SR_R_min"));
    full_parameters->setParamHeader(3, QString("SR_Zfrac"));
    full_parameters->setParamHeader(4, QString("SR_Beta"));
    full_parameters->setParamHeader(5, QString("SR_shape"));
    full_parameters->setParamHeader(6, QString("SR_Rick_pow"));
    full_parameters->setParamHeader(7, QString("SR_sigmaR"));
    full_parameters->setParamHeader(8, QString("SR_regime"));
    full_parameters->setParamHeader(9, QString("SR_autocorr"));
}

void spawn_recruit::setOption(int option)
{
    setMethod (1);
    if (1 != option)
        setMethod(option);
}

void spawn_recruit::setMethod(int value)
{
    method = value;
    QStringList param (full_parameters->getParameter(1));
    param[0] = QString::number(.2, 'g', 3);
    full_parameters->setParamData(1, param);

    switch (value)
    {
    case 1: // null
        parmsUsed.clear();
        parmsUsed << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        break;
    case 2: // Ricker
        parmsUsed.clear();
        parmsUsed << 1 << 1 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        break;
    case 3: // Beverton-Holt
    case 6: // Beverton-Holt flattop
        parmsUsed.clear();
        parmsUsed << 1 << 1 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        break;
    case 4: // Ignore steepness, no bias
        parmsUsed.clear();
        parmsUsed << 1 << 1 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        break;
    case 5: // Hockey stick
        parmsUsed.clear();
        parmsUsed << 1 << 1 << 1 << 0 << 0 << 0 << 0 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        param = full_parameters->getParameter(1);
        param[0] = QString::number(.01, 'g', 3);
        full_parameters->setParamData(1, param);
        break;
    case 7: // Survivorship
        parmsUsed.clear();
        parmsUsed << 1 << 0 << 0 << 1 << 1 << 0 << 0 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        break;
    case 8: // Shepherd
        parmsUsed.clear();
        parmsUsed << 1 << 1 << 0 << 0 << 0 << 1 << 0 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        break;
    case 9: // Ricker power
        parmsUsed.clear();
        parmsUsed << 1 << 1 << 0 << 0 << 0 << 0 << 1 << 1 << 1 << 1;
        full_parameters->setParamsUsed(parmsUsed);
        setParamNums ();
        break;
    }
}

void spawn_recruit::setParamNums()
{
    parmNums.clear();
    for (int i = 0; i < parmsUsed.count(); i++)
    {
        if (parmsUsed.at(i) > 0)
            parmNums.append(i);
    }
}

QStringList spawn_recruit::getParameter (int num)
{
    return getFullParameters()->getRowData(num);
}
void spawn_recruit::setParameter (int num, QStringList data)
{
    getFullParameters()->setRowData(num, data);
}
QString spawn_recruit::getParameterHeader (int num)
{
    return getFullParameters()->getRowHeader(num);
}
void spawn_recruit::setParameterHeader (int num, QString hdr)
{
    getFullParameters()->setRowHeader(num, hdr);
}

void spawn_recruit::setFullParameter(int index, QStringList values)
{
//    int i = parmNums.at(index);
    full_parameters->setParamData(index, values);
    varParameters->setParameter(index, values);
}

void spawn_recruit::setFullParameterHeader(int index, QString hdr)
{
//    int i = parmNums.at(index);
    full_parameters->setParamHeader(index, hdr);
    varParameters->setTableTitle(index, hdr);
}

QStringList spawn_recruit::getFullParameter(int index)
{
//    int i = parmNums.at(index);
    return full_parameters->getParameter(index);
}

void spawn_recruit::setDistribMethod(int value)
{
    distrib_method = value;
}

int spawn_recruit::getNumAssignments()
{
    return assignments->rowCount();
}

void spawn_recruit::setNumAssignments(int rows)
{
    if (rows < assignments->rowCount())
    {
        assignments->setRowCount(rows);
        setAssignTimings();
    }
    else
    {
        assignments->setRowCount(rows);
    }
}

tablemodel *spawn_recruit::getAssignments() const
{
    return assignments;
}

QStringList spawn_recruit::getAssignment(int row)
{
    return assignments->getRowData(row);
}

void spawn_recruit::setAssignment(int row, QStringList data)
{
    if (row >= assignments->rowCount())
        assignments->setRowCount(row + 1);
    assignments->setRowData(row, data);
    setAssignTimings();
}

int spawn_recruit::getNumAssignTimings() const
{
    return assignTimings->columnCount();
}

QStringList spawn_recruit::getAssignTimings() const
{
    return assignTimings->getRowData(0);
}

void spawn_recruit::setAssignTimings()
{
    int num = getNumAssignments();
    int settle, timings = 0;
    QString month;
    QStringList datalist;
    QStringList assignment;

    if (num == 0)
    {
        datalist.append(QString::number(1.0,'g',1));
    }
    else
    {
        timings = 1;
        datalist.append (getAssignment(0).at(1));

        for (settle = 1; settle < num; settle++)
        {
            assignment = getAssignment (settle);
            month = assignment.at(1);
            if (!datalist.contains(month) && !month.isEmpty())
            {
                datalist.append(month);
                timings++;
            }
        }
    }
    assignTimings->setColumnCount(datalist.count());
    assignTimings->setRowData(0, datalist);
    if (distrib_method == 3)
        distParams->setTotalNumParams(datalist.count());
}

int spawn_recruit::getDoRecruitInteract() const
{
    return doRecruitInteract;
}

void spawn_recruit::setDoRecruitInteract(bool value)
{
    doRecruitInteract = value? 1: 0;
}

void spawn_recruit::setDoRecruitInteract(int value)
{
    doRecruitInteract = value;
}

void spawn_recruit::setInteractParam(int index, QStringList data)
{
    if (index >= interactParams->getNumParams())
        interactParams->setNumParams(index + 1);
    interactParams->setParameter(index, data);
}

void spawn_recruit::setNumDistParams(int num)
{
    distParams->setNumParams(num);
    distVarParams->setTotalNumVarTables(num);
}

void spawn_recruit::setDistParam(int index, QStringList data)
{
    if (index >= distParams->getNumParams())
        setNumDistParams(index + 1);
    distParams->setParameter(index, data);
    distVarParams->setParameter(index, data);
}

void spawn_recruit::setDistTVParam(int index, QStringList values)
{
    if (index < distVarParams->getNumVarParams())
        distVarParams->setVarParameter(index, values);
}

void spawn_recruit::setTimeVaryReadParams(int flag)
{
    readtvparams = flag;
    varParameters->setAutoGenerate(flag);
    distVarParams->setAutoGenerate(flag);
    interVarParams->setAutoGenerate(flag);
    cycleVarParams->setAutoGenerate(flag);

}

void spawn_recruit::setCycleParam(int index, QStringList data)
{
    if (index >= cycleParams->getNumParams())
        cycleParams->setNumParams(index + 1);
    cycleParams->setParameter(index, data);
}

void spawn_recruit::setTVParameter(int index, QStringList values)
{
    if (index >= varParameters->getNumVarParams())
        varParameters->setNumVarParams(index + 1);
    varParameters->setVarParameter(index, values);
}

int spawn_recruit::getRecDevStartYr() const
{
    return rec_dev_start_yr;
}

void spawn_recruit::setRecDevStartYr(int value)
{
    rec_dev_start_yr = value;
}

int spawn_recruit::getRecDevEndYr() const
{
    return rec_dev_end_yr;
}

void spawn_recruit::setRecDevEndYr(int value)
{
    rec_dev_end_yr = value;
}

bool spawn_recruit::getAdvancedOpts() const
{
    return advanced_opts;
}

void spawn_recruit::setAdvancedOpts(bool value)
{
    advanced_opts = value;
}

int spawn_recruit::getRecDevEarlyStart() const
{
    return rec_dev_early_start;
}

void spawn_recruit::setRecDevEarlyStart(int value)
{
    rec_dev_early_start = value;
}

int spawn_recruit::getRecDevEarlyPhase() const
{
    return rec_dev_early_phase;
}

void spawn_recruit::setRecDevEarlyPhase(int value)
{
    rec_dev_early_phase = value;
}

int spawn_recruit::getFcastRecPhase() const
{
    return fcast_rec_phase;
}

void spawn_recruit::setFcastRecPhase(int value)
{
    fcast_rec_phase = value;
}

float spawn_recruit::getFcastLambda() const
{
    return fcast_lambda;
}

void spawn_recruit::setFcastLambda(float value)
{
    fcast_lambda = value;
}

double spawn_recruit::getNobiasLastEarlyYr() const
{
    return nobias_last_early_yr;
}

void spawn_recruit::setNobiasLastEarlyYr(double value)
{
    nobias_last_early_yr = value;
}

double spawn_recruit::getFullbiasFirstYr() const
{
    return fullbias_first_yr;
}

void spawn_recruit::setFullbiasFirstYr(double value)
{
    fullbias_first_yr = value;
}

double spawn_recruit::getFullbiasLastYr() const
{
    return fullbias_last_yr;
}

void spawn_recruit::setFullbiasLastYr(double value)
{
    fullbias_last_yr = value;
}

double spawn_recruit::getNobiasFirstRecentYr() const
{
    return nobias_first_recent_yr;
}

void spawn_recruit::setNobiasFirstRecentYr(double value)
{
    nobias_first_recent_yr = value;
}

float spawn_recruit::getMaxBiasAdjust() const
{
    return max_bias_adjust;
}

void spawn_recruit::setMaxBiasAdjust(float value)
{
    max_bias_adjust = value;
}

int spawn_recruit::getRecCycles() const
{
    return cycleParams->getNumParams();
//    return rec_cycles;
}

void spawn_recruit::setRecCycles(int value)
{
    cycleParams->setNumParams(value);
//    rec_cycles = value;
}

int spawn_recruit::getRecDevMin() const
{
    return rec_dev_min;
}

void spawn_recruit::setRecDevMin(int value)
{
    rec_dev_min = value;
}

int spawn_recruit::getRecDevMax() const
{
    return rec_dev_max;
}

void spawn_recruit::setRecDevMax(int value)
{
    rec_dev_max = value;
}

int spawn_recruit::getRecDevCode() const
{
    return rec_dev_code;
}

void spawn_recruit::setRecDevCode(int value)
{
    rec_dev_code = value;
}

/*int spawn_recruit::getNumRecDev() const
{
    return num_rec_dev;
}

void spawn_recruit::setNumRecDev(int value)
{
    num_rec_dev = value;
}*/

int spawn_recruit::getRecDevPhase() const
{
    return rec_dev_phase;
}

void spawn_recruit::setRecDevPhase(int value)
{
    rec_dev_phase = value;
}


