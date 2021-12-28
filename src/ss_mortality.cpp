#include "ss_mortality.h"
#include "parametermodel.h"
#include "fileIOgeneral.h"
#include "model.h"

ss_mortality::ss_mortality(QObject *parent, int n_fisheries, int n_surveys)
{
    ssModel = static_cast<ss_model *>(parent);
    QStringList pheader;
    // Method 2
    pheader << "fleet" << "year" << "seas" << "F" << "se" << "phase" ;
    fleetYearValues = new tablemodel(parent);
    fleetYearValues->setHeader(pheader);

    // Method 3
    initialParams = new shortParameterTable(parent);

    // Fleet specific F - Method 4
    pheader.clear();
    pheader << "fleet" << "F" << "phase";
    fleetF = new tablemodel(parent);
    fleetF->setHeader(pheader);

    reset();

    setNumFisheries(n_fisheries);
    numSurveys = n_surveys;
}

void ss_mortality::reset()
{
    setNumFisheries(1);
    numSurveys = 0;
    // Method 2
    fleetYearValues->setRowCount(0);
    // Method 3
    initialParams->setParamCount(0);
    // Fleet specific F - Method 4
    fleetF->setRowCount(0);

    bparkF = 0.3;
    bparkYr = -2001;
    method = 3;
    maxF = 2.9;
    startF = 0;
    phase = 0;
    numInputs = 0;
    numTuningIters = 4;
}

void ss_mortality::setYears(int f_year, int num)
{
    firstYear = f_year;
    numYears = 1;
    if (num < f_year)
        numYears = num;
    else
        numYears = (num - f_year) + 1;
}


int ss_mortality::getNumTuningIters() const
{
    return numTuningIters;
}

void ss_mortality::setNumTuningIters(int value)
{
    numTuningIters = value;
}

void ss_mortality::setStartYear(int value)
{
    firstYear = value;
}

void ss_mortality::setNumYears(int value)
{
    numYears = value;
}

void ss_mortality::setNumSurveys(int value)
{
    numSurveys = value;
}
float ss_mortality::getBparkF() const
{
    return bparkF;
}

void ss_mortality::setBparkF(float value)
{
    bparkF = value;
}
int ss_mortality::getBparkYr() const
{
    return bparkYr;
}

void ss_mortality::setBparkYr(int value)
{
    bparkYr = value;
}
int ss_mortality::getMethod() const
{
    return method;
}

void ss_mortality::setMethod(int value)
{
    method = value;
}
float ss_mortality::getMaxF() const
{
    return maxF;
}

void ss_mortality::setMaxF(float value)
{
    maxF = value;
}
float ss_mortality::getStartF() const
{
    return startF;
}

void ss_mortality::setStartF(float value)
{
    startF = value;
}
int ss_mortality::getPhase() const
{
    return phase;
}

void ss_mortality::setPhase(int value)
{
    phase = value;
}

void ss_mortality::setFleetF(int fleet, float startF, int phaseF)
{
    QStringList f;
    f << QString::number(fleet) << QString::number(startF) << QString::number(phaseF);
    fleetF->setRowData(fleet-1, f);
}








