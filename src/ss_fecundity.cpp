#include "ss_fecundity.h"

ss_fecundity::ss_fecundity()
{
    method = 0;
    hermSeason = 1;
    hermIncludeMales = 0;

    hermaphParams = new parametermodel();
    hermaphParams->setRowCount(3);
    hermaphParams->setRowHeader(0, QString("Hermaph_inflect_age"));
    hermaphParams->setRowHeader(1, QString("Hermaph_std_dev"));
    hermaphParams->setRowHeader(2, QString("Hermaph_asymp_rate"));
    femaleParams = new parametermodel();
    femaleParams->setRowCount(4);
}

ss_fecundity::~ss_fecundity()
{
    delete hermaphParams;
    delete femaleParams;
}

int ss_fecundity::getMethod() const
{
    return method;
}

void ss_fecundity::setMethod(int value)
{
    method = value;
}

void ss_fecundity::setHermParam(int index, QStringList data)
{
    if (index >= hermaphParams->rowCount())
        hermaphParams->setRowCount(index + 1);
    hermaphParams->setRowData(index, data);
}
int ss_fecundity::getHermIncludeMales() const
{
    return hermIncludeMales;
}

void ss_fecundity::setHermIncludeMales(int value)
{
    hermIncludeMales = value;
}
int ss_fecundity::getHermSeason() const
{
    return hermSeason;
}

void ss_fecundity::setHermSeason(int value)
{
    hermSeason = value;
}
bool ss_fecundity::getHermaphroditism() const
{
    return hermaphroditism;
}

void ss_fecundity::setHermaphroditism(int value)
{
    hermaphroditism = value;
}

void ss_fecundity::setFemaleParam(int index, QStringList data)
{
    if (index >= femaleParams->rowCount())
        femaleParams->setRowCount(index + 1);
    femaleParams->setRowData(index, data);
}



