#include "population.h"
//#include "model.h"

population::population(QObject *parent) :
    QObject(parent)
{
    QStringList header;
    pop_growth = new ss_growth(parent);
    pop_fecundity = new ss_fecundity(parent);
    pop_recruitment = new spawn_recruit(parent);
    pop_mortality = new ss_mortality(parent);
    pop_movement = new ss_movement(parent);
    meanBwtModel = new tablemodel(parent);
    meanBwtModel->setColumnCount(20);
    header << "Year" << "Seas" << "Type" << "Part" << "Value" << "CV";
    meanBwtModel->setHeader(header);
    seasparamtable = new seasonalEffectsModel(parent);
    header.clear();
    reset();
}

population::~population()
{
    delete meanBwtModel;
    delete pop_growth;
    delete pop_fecundity;
    delete pop_recruitment;
    delete pop_mortality;
    delete pop_movement;
    delete seasparamtable;
}

void population::reset()
{
    Fec()->reset();
    Grow()->reset();
    SR()->reset();
    Move()->reset();
    M()->setMethod(1);
    M()->reset();

    set_frac_female(0.5);

    i_gender = 1;
    i_mean_bwt_deg_freedom = 1;
    i_mean_bwt_count = 1;

    meanBwtModel->setRowCount(0);

    femwtlen1 = false;
    femwtlen2 = false;
    mat1 = false;
    mat2 = false;
    fec1 = false;
    fec2 = false;

    malewtlen1 = false;
    malewtlen2 = false;
    L1 = false;
    K = false;
    setNumSeas(1);
}

void population::setNumSeas(int seas)
{
    // default value since insertSeasParams needs it to be > 0
    iNumSeas = 1;
    seasparamtable->setNumSeasons(seas);
}

void population::setStartYear(int yr)
{
    iStartYear = yr;
    M()->setStartYear(yr);
}

void population::setTotalYears(int yrs)
{
    iNumYears = yrs;
    M()->setNumYears(yrs);
}

void population::setNumAges(int ages)
{
    iNumAges = ages;
    Grow()->setNumAges(ages);
}

float population::get_frac_female() const
{
    return f_frac_female;
}

void population::set_frac_female(float value)
{
    f_frac_female = value;
}

void population::setFractionFemale(QStringList data)
{
    if (!fractionFem.isEmpty())
        fractionFem.clear();
    for (int i = 0; i < data.count(); i++)
        fractionFem.append(data.at(i));
}

QStringList population::getFractionFemale()
{
    return fractionFem;
}

void population::setSeasParamSetup(QStringList values)
{
    seasparamtable->setSetup(values);
}

QStringList population::getSeasParamSetup()
{
    return seasparamtable->getSetup();
}

bool population::getK() const
{
    return seasparamtable->getK();
}

void population::setK(bool value)
{
    seasparamtable->setK(value);
}

bool population::getL1() const
{
    return seasparamtable->getL1();
}

void population::setL1(bool value)
{
    seasparamtable->setL1(value);
}

bool population::getMaleWtLen2() const
{
    return seasparamtable->getMalewtlen2();
}

void population::setMaleWtLen2(bool value)
{
    seasparamtable->setMalewtlen2(value);
}

bool population::getMaleWtLen1() const
{
    return seasparamtable->getMalewtlen1();
}

void population::setMaleWtLen1(bool value)
{
    seasparamtable->setMalewtlen1(value);
}

bool population::getFecund2() const
{
    return seasparamtable->getFec2();
}

void population::setFecund2(bool value)
{
    seasparamtable->setFec2(value);
}

bool population::getFecund1() const
{
    return seasparamtable->getFec1();
}

void population::setFecund1(bool value)
{
    seasparamtable->setFec1(value);
}

bool population::getMaturity2() const
{
    return seasparamtable->getMat2();
}

void population::setMaturity2(bool value)
{
    seasparamtable->setMat2(value);
}

bool population::getMaturity1() const
{
    return seasparamtable->getMat1();
}

void population::setMaturity1(bool value)
{
    seasparamtable->setMat1(value);
}

bool population::getFemWtLen2() const
{
    return seasparamtable->getFemwtlen2();
}

void population::setFemWtLen2(bool value)
{
    seasparamtable->setFemwtlen2(value);
}

bool population::getFemWtLen1() const
{
    return seasparamtable->getFemwtlen1();
}

void population::setFemWtLen1(bool value)
{
    seasparamtable->setFemwtlen1(value);
}

void population::setSeasonalParam(int index, QStringList data)
{
    seasparamtable->setParameter(index, data);
}

QStringList population::getSeasonalParam(int index)
{
    return seasparamtable->getParameter(index);
}

