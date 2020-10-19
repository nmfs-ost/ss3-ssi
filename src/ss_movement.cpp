#include "ss_movement.h"
#include "parametermodel.h"
#include "model.h"

ss_movement::ss_movement(QObject *parent, int n_fleets)
{
    ssModel = static_cast<ss_model *>(parent);
    movement_defs = new tablemodel(parent);
    defHeader << "seas" << "GP" << "source_area" << "dest_area" << "minage" << "maxage";
    movement_defs->setColumnCount(defHeader.count());
    movement_defs->setHeader(defHeader);
    movement_defs->setRowCount(0);
    movement_parms = new longParameterModel(parent);
    movement_parms->setNumParams(0);
    moveTVParams = new timeVaryParameterModel(parent);
    moveTVParams->setNumParams(0);
    num_fleets = n_fleets;
    first_year = 0;
    num_years = 0;
    numAreas = 0;
    method = 0;
}

ss_movement::~ss_movement()
{
    delete movement_defs;
    delete movement_parms;
    delete moveTVParams;
}

void ss_movement::reset()
{
    movement_defs->setRowCount(0);
    movement_parms->setNumParams(0);
    first_year = 0;
    num_years = 0;
    numAreas = 0;
    method = 0;
}

void ss_movement::setYears(int f_yr, int num)
{
    first_year = f_yr;
    if (num < first_year)
        num_years = num;
    else
        num_years = (num - first_year) + 1;
}

QStringList ss_movement::getDefinition(int index)
{
    return movement_defs->getRowData(index);
}

void ss_movement::setDefinition(int index, QStringList valuelist)
{
    if (index >= movement_defs->rowCount())
        movement_defs->setRowCount(index + 1);
    movement_defs->setRowData(index, valuelist);
}

void ss_movement::setParameter(int index, QStringList valuelist)
{
    if (index >= movement_parms->getNumParams())
    {
        movement_parms->setNumParams(index + 1);
        moveTVParams->setNumParams(index + 1);
    }
    movement_parms->setParameter(index, valuelist);
    moveTVParams->setParameter(index, valuelist);
}

int ss_movement::getMethod() const
{
    return method;
}

void ss_movement::setMethod(int value)
{
    method = value;
}

int ss_movement::getNumAreas() const
{
    return numAreas;
}

void ss_movement::setNumAreas(int value)
{
    numAreas = value;
}

float ss_movement::getFirstAge() const
{
    return firstAge;
}

void ss_movement::setFirstAge(float value)
{
    firstAge = value;
}



