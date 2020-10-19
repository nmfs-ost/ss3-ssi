#include "ss_fecundity.h"
#include "model.h"

ss_fecundity::ss_fecundity(QObject *parent)
    : QObject(parent)
{
    ssModel = static_cast<ss_model *>(parent);
    method = 0;
    hermSeason = 1;
    hermIncludeMales = 0;
}

ss_fecundity::~ss_fecundity()
{
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


