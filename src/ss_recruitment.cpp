#include "ss_recruitment.h"

spawn_recruit::spawn_recruit(ss_model *parent)
{
    parnt = parent;
//    parameters = new parametermodel();
//    parameters->setRowCount(0);
//    parameters->setColumnCount(7);
    full_parameters = new longParameterModel();// parameterModelTV(parnt);
    full_parameters->setNumParams(5);

    tvParameters = new timeVaryParameterModel (parnt);//full_parameters->getParamVarsModel();//getTimeVaryParams();//new shortParameterModel((QObject *)parnt);

    assignments = new tablemodel();
    assignments->setColumnCount(4);
    assignments->setRowCount(0);
    header << "GP" << "seas" << "area" << "Settle_age";
    assignments->setHeader(header);

    cycleParams = new longParameterModel();// parameterModelTV(parnt);
    interactParams = new longParameterModel();// parameterModelTV(parnt);
    distParams = new longParameterModel();// parameterModelTV(parnt);
    distParams->setNumParams(2);

    header.clear();
    header << "Year" << "Input_value";
    recruitDevs = new tablemodel();
    recruitDevs->setHeader(header);
//    tvParameters->setColumnCount(7);

    reset();
/*    method = 3;
    env_link = 0;
    env_target = 0;
    rec_dev = 1;
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
    rec_cycles = 0;
    rec_dev_min = -5;
    rec_dev_max = 5;
    num_rec_dev = 0;*/
}

spawn_recruit::~spawn_recruit()
{
//    delete parameters;
    delete full_parameters;
    delete assignments;
    delete cycleParams;
    delete recruitDevs;
    delete interactParams;
    delete distParams;
}

void spawn_recruit::reset()
{
    distrib_method = 1;
    distrib_area = 1;
    method = 3;
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
}

int spawn_recruit::getNumAssignments()
{
    return assignments->rowCount();
}

void spawn_recruit::setNumAssignments(int rows)
{
    assignments->setRowCount(rows);
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
}

bool spawn_recruit::getDoRecruitInteract() const
{
    return doRecruitInteract;
}

void spawn_recruit::setDoRecruitInteract(bool value)
{
    doRecruitInteract = value;
}

void spawn_recruit::setDoRecruitInteract(int value)
{
    if (value != 0)
        doRecruitInteract = true;
    else
        doRecruitInteract = false;
}

void spawn_recruit::setInteractParam(int index, QStringList data)
{
    if (index >= interactParams->getNumParams())
        interactParams->setNumParams(index + 1);
    interactParams->setParameter(index, data);
}

void spawn_recruit::setDistParam(int index, QStringList data)
{
    if (index >= distParams->getNumParams())
        distParams->setNumParams(index + 1);
    distParams->setParameter(index, data);
}

void spawn_recruit::setCycleParam(int index, QStringList data)
{
    if (index >= cycleParams->getNumParams())
        cycleParams->setNumParams(index + 1);
    cycleParams->setParameter(index, data);
}

void spawn_recruit::setTVParameter(int index, QStringList values)
{
    if (index >= tvParameters->getNumVarParams())
        tvParameters->setNumVarParams(index + 1);
    tvParameters->setVarParameter(index, values);
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

int spawn_recruit::getNobiasLastEarlyYr() const
{
    return nobias_last_early_yr;
}

void spawn_recruit::setNobiasLastEarlyYr(int value)
{
    nobias_last_early_yr = value;
}

int spawn_recruit::getFullbiasFirstYr() const
{
    return fullbias_first_yr;
}

void spawn_recruit::setFullbiasFirstYr(int value)
{
    fullbias_first_yr = value;
}

int spawn_recruit::getFullbiasLastYr() const
{
    return fullbias_last_yr;
}

void spawn_recruit::setFullbiasLastYr(int value)
{
    fullbias_last_yr = value;
}

int spawn_recruit::getNobiasFirstRecentYr() const
{
    return nobias_first_recent_yr;
}

void spawn_recruit::setNobiasFirstRecentYr(int value)
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


