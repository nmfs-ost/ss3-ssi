#include "ss_fecundity.h"

ss_fecundity::ss_fecundity(ss_model *parent)
{
    parnt = parent;
    method = 0;
    hermSeason = 1;
    hermIncludeMales = 0;

    hermaphParams = new longParameterModel((QObject*)parnt);
    hermaphParams->setParamCount(3);
    hermaphParams->setParamHeader(0, QString("Hermaph_inflect_age"));
    hermaphParams->setParamHeader(1, QString("Hermaph_std_dev"));
    hermaphParams->setParamHeader(2, QString("Hermaph_asymp_rate"));
    femaleParams = new longParameterModel((QObject*)parnt);
    femaleParams->setParamCount(4);
}

ss_fecundity::~ss_fecundity()
{
    delete hermaphParams;
    delete femaleParams;
}

void ss_fecundity::reset()
{
    method = 0;
    hermSeason = 1;
    hermIncludeMales = 0;
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
    if (index >= hermaphParams->getParamCount())
        hermaphParams->setParamCount(index + 1);
    hermaphParams->setParameter(index, data);
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
    if (index >= femaleParams->getParamCount())
        femaleParams->setParamCount(index + 1);
    femaleParams->setParameter(index, data);
}



