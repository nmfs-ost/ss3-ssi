#include "growth.h"
#include "parametermodel.h"

ss_growth::ss_growth()
{
    num_patterns = 0;
    num_params = 0;
//    paramtable = new parametermodel();
//    paramtable->setRowCount(0);

//    hermaphParams = new parametermodel();
//    hermaphParams->setRowCount(3);

    morphdisttable = new tablemodel();
    morphdisttable->setRowCount(1);

    natMortNumBreakPoints = 0;
    natMortBreakPoints = new tablemodel();
    natMortBreakPoints->setRowCount(1);
    natMortBreakPoints->setRowHeader(0, QString("Age(real) at M Breakpoints"));
    natMortAges = new tablemodel();
    natMortAges->setRowCount(0);
    natMortAges->setColumnCount(1);
    natMort_lorenzen_ref_age = 0;

    matAgeValues = new tablemodel();
    matAgeValues->setRowCount(1);

    maturityParams = new parametermodel();
    maturityParams->setRowCount(2);

    cohortParam = new parametermodel();
    cohortParam->setRowCount(1);

    fracfemaleParams = new parametermodel();
    fracfemaleParams->setRowCount(1);

    customBlock = -1;
    blockParams = new parametermodel();
    blockParams->setRowCount(0);
    customEnvLink = -1;
    environmentParams = new parametermodel();
    environmentParams->setRowCount(0);

    reset();
}

ss_growth::~ss_growth()
{
    setNum_morphs(0);
    setNum_patterns(0);
//    setNum_params(0);
//    delete paramtable;
    delete morphdisttable;
    delete natMortBreakPoints;
    delete natMortAges;
    delete matAgeValues;
    delete maturityParams;
    delete cohortParam;
    delete fracfemaleParams;
    customEnvLink = -1;
    delete environmentParams;
    customBlock = -1;
    delete blockParams;
}

void ss_growth::reset()
{
    setModel(1);

    setNum_patterns(1);
    setAge_for_l1(1.66);
    setAge_for_l2(25);
    setAgeMin_for_K(5);
    setAgeMax_for_K(7);
    setExpDecay(0.2);  // default for 3.24 compatibility
    setSd_add(0);
    setCv_growth_pattern(0);
    setMaturity_option(2);
    setFirst_mature_age(1);

    num_params = 0;
    matAgeValues->setRowCount(1);
    maturityParams->setRowCount(2);
    cohortParam->setRowCount(1);
    fracfemaleParams->setRowCount(1);
    blockParams->setRowCount(0);
    environmentParams->setRowCount(0);

    setNum_morphs(1);
    setMorph_within_ratio(0.7);
    setMorph_dist(0, 1.0);
}

/*
growth_pattern * ss_growth::getPattern(int i) const
{
    growth_pattern * gp = NULL;
    if (i < patterns.count())
        gp = patterns[i];
    return gp;
}

void ss_growth::setPattern(int i, growth_pattern * &value)
{
    if (i >= patterns.count())
        setNum_patterns (i);
    growth_pattern *pat = patterns.takeAt(i);
    pat = new growth_pattern(*value);
    patterns.insert(i-1, pat);
}*/
/*
int ss_growth::getNum_params() const
{
    return num_params;
}*/
/*
void ss_growth::setNum_params(int value)
{
    int i = num_params;

    paramtable->setRowCount(value);*/
/*    for (; i < value; i++)
    {
        params.append(new longParameter());
    }
    while (value < params.count())
    {
        longParameter *lp = params.takeLast();
        delete lp;
    }

    num_params = value;*/
/*}

void ss_growth::setParameter(int index, QStringList data)
{
    if (index >= paramtable->rowCount())
        paramtable->setRowCount(index + 1);
    paramtable->setRowData(index, data);
}

void ss_growth::addParameter(QStringList data)
{
    setParameter(paramtable->rowCount(), data);
}

QStringList ss_growth::getParameter(int index)
{
    return paramtable->getRowData(index);
}*/

/*longParameter *ss_growth::getParam(int i) const
{
    longParameter *lp = NULL;
    if (i < params.count())
        lp = params.at(i);
    return lp;
}

void ss_growth::setParam(int i, longParameter * &value)
{
    if (i >= params.count())
        setNum_params(i);
    longParameter *lp = new longParameter(*value);
    params.insert(i, lp);
    params.takeAt(i+1);
}*/
float ss_growth::getMorph_within_ratio() const
{
    return morph_within_ratio;
}

void ss_growth::setMorph_within_ratio(float value)
{
    morph_within_ratio = value;
}
int ss_growth::getNum_morphs() const
{
    return num_morphs;
}

void ss_growth::setNum_morphs(int value)
{
    if (value <= 1)
    {
        value = 1;
        setMorph_dist(0, 1);
        setMorph_within_ratio(1.0);
        morphdisttable->setColumnCount(value);
    }
    else if (value <= 3)
    {
        value = 3;
        setMorph_within_ratio(0.7);
        morphdisttable->setColumnCount(value);
        setMorph_dist(0, 0.15);
        setMorph_dist(1, 0.70);
        setMorph_dist(2, 0.15);
    }
    else
    {
        value = 5;
        setMorph_within_ratio(0.7);
        morphdisttable->setColumnCount(value);
        setMorph_dist(0, 0.031);
        setMorph_dist(1, 0.237);
        setMorph_dist(2, 0.464);
        setMorph_dist(3, 0.237);
        setMorph_dist(4, 0.031);
    }

    num_morphs = value;
}

void ss_growth::setMorph_dist(QStringList values)
{
    morphdisttable->setRowData(0, values);
}

void ss_growth::setMorph_dist(int index, float value)
{
    if (index + 1 > morphdisttable->columnCount())
        morphdisttable->setColumnCount(index + 1);
    QStringList strlst(morphdisttable->getRowData(0));
    strlst[index] = QString::number(value);
    morphdisttable->setRowData(0, strlst);
}
int ss_growth::getDevPhase() const
{
    return devPhase;
}

void ss_growth::setDevPhase(int value)
{
    devPhase = value;
}
int ss_growth::getCustomBlock() const
{
    return customBlock;
}

void ss_growth::setCustomBlock(int value)
{
    customBlock = value;
}
int ss_growth::getCustomEnvLink() const
{
    return customEnvLink;
}

void ss_growth::setCustomEnvLink(int value)
{
    customEnvLink = value;
}




QStringList ss_growth::getMorphDist_str()
{
    return morphdisttable->getRowData(0);
}

float ss_growth::getMorph_dist(int index)
{
    QStringList qstr(getMorphDist_str());
    float value = qstr.at(index).toFloat();
    return value;
}

float ss_growth::getFraction_female() const
{
    return fraction_female;
}

void ss_growth::setFraction_female(float value)
{
    fraction_female = value;
}

int ss_growth::getNatural_mortality_type() const
{
    return natural_mortality_type;
}

void ss_growth::setNatural_mortality_type(int value)
{
    natural_mortality_type = value;
    switch (natural_mortality_type)
    {
    case 0:
        for (int i = 0; i < num_patterns; i++)
            getPattern(i)->getNatMParams()->setRowCount(2);
        break;
    case 1:
        for (int i = 0; i < num_patterns; i++)
            getPattern(i)->getNatMParams()->setRowCount(2 * natMortNumBreakPoints);
        natMortBreakPoints->setColumnCount(natMortNumBreakPoints);
        break;
    case 2:
        for (int i = 0; i < num_patterns; i++)
            getPattern(i)->getNatMParams()->setRowCount(2);
        break;
    case 3:
    case 4:
        for (int i = 0; i < num_patterns; i++)
            getPattern(i)->getNatMAges()->setColumnCount(num_ages);
        break;
    }
}

int ss_growth::getNatMortNumBreakPts() const
{
    return natMortBreakPoints->columnCount();
}

void ss_growth::setNatMortNumBreakPts(int num)
{
    natMortNumBreakPoints = num;
    natMortBreakPoints->setColumnCount(num);
    for (int i = 0; i < num_patterns; i++)
        getPattern(i)->getNatMParams()->setRowCount(2 * natMortNumBreakPoints);
}

QStringList ss_growth::getNatMortBreakPts ()
{
    QStringList ql (natMortBreakPoints->getRowData(0));
    return ql;
}

void ss_growth::setNatMortBreakPts (QStringList data)
{
    natMortBreakPoints->setRowData(0, data);
}

void ss_growth::setNatMortAgesHeader(int ages)
{
    int i;
    natMortAges->setRowCount(ages);
    for (i = 0; i < num_patterns; i++)
        natMortAges->setRowHeader(i, QString("F_GP%1").arg(QString::number(i+1)));
    for (; i < ages; i++)
        natMortAges->setRowHeader(i, QString("M_GP%1").arg(QString::number(i+1)));
}

QStringList ss_growth::getNatMortAges()
{
    QStringList ql;
    for (int i = 0; i < natMortAges->rowCount(); i++)
        ql << natMortAges->getRowData(i).at(0);
    return ql;
}

void ss_growth::setNatMortAges(QStringList data)
{
    setNatMortAgesHeader(data.count());
    for (int i = 0; i < data.count(); i++)
        natMortAges->setRowData(i, data.at(i));
}

int ss_growth::getModel() const
{
    return model;
}

void ss_growth::setModel(int value)
{
    model = value;
}

float ss_growth::getAge_for_l1() const
{
    return age_for_l1;
}

void ss_growth::setAge_for_l1(float value)
{
    age_for_l1 = value;
}

float ss_growth::getAge_for_l2() const
{
    return age_for_l2;
}

void ss_growth::setAge_for_l2(float value)
{
    age_for_l2 = value;
}

float ss_growth::getExpDecay ()
{
    return age_exp_decay;
}

void ss_growth::setExpDecay(float value)
{
    age_exp_decay = value;
}

float ss_growth::getFirst_mature_age() const
{
    return first_mature_age;
}

void ss_growth::setFirst_mature_age(float value)
{
    first_mature_age = value;
}

float ss_growth::getSd_add() const
{
    return sd_add;
}

void ss_growth::setSd_add(float value)
{
    sd_add = value;
}

int ss_growth::getCv_growth_pattern() const
{
    return cv_growth_pattern;
}

void ss_growth::setCv_growth_pattern(int value)
{
    cv_growth_pattern = value;
}

int ss_growth::getMaturity_option() const
{
    return maturity_option;
}

void ss_growth::setMaturity_option(int value)
{
    maturity_option = value;
}

void ss_growth::setMaturityParam(int index, QStringList data)
{
    if (index >= maturityParams->rowCount())
        maturityParams->setRowCount(index + 1);
    maturityParams->setRowData(index, data);
}

/*
int ss_growth::getHermaphroditism() const
{
    return hermaphroditism;
}

void ss_growth::setHermaphroditism(int value)
{
    hermaphroditism = value;
}*/

int ss_growth::getParam_offset_method() const
{
    return param_offset_method;
}

void ss_growth::setParam_offset_method(int value)
{
    param_offset_method = value;
}

int ss_growth::getAdjustment_method() const
{
    return adjustment_method;
}

void ss_growth::setAdjustment_method(int value)
{
    adjustment_method = value;
}

int ss_growth::getParam_dev_phase() const
{
    return param_dev_phase;
}

void ss_growth::setParam_dev_phase(int value)
{
    param_dev_phase = value;
}

int ss_growth::getNum_patterns() const
{
    return num_patterns;
}

void ss_growth::setNum_patterns(int value)
{
    int i;
    if (patterns.isEmpty())
    {
        patterns.append(new growthPattern());
    }
    i = patterns.count();
    for (; i < value; i++)
    {
        growthPattern *new_gp = new growthPattern();
        patterns.append(new_gp);
    }
    while (value < patterns.count())
    {
        growthPattern *old = patterns.takeLast();
        delete old;
    }
    num_patterns = patterns.count();
}

int ss_growth::getNumDevParams()
{
    int num = 0;
    QStringList data;
    for (int i = 0; i < maturityParams->rowCount(); i++)
    {
        if (maturityParams->useDev(i))
            num++;
    }
    return num;
}

int ss_growth::getNumEnvLinkParams()
{
    int num = 0;
    QStringList data;
    for (int i = 0; i < maturityParams->rowCount(); i++)
    {
        if (maturityParams->envLink(i))
            num++;
    }
    return num;
}

int ss_growth::getNumBlockParams()
{
    int num = 0;
    QStringList data;
    for (int i = 0; i < maturityParams->rowCount(); i++)
    {
        if (maturityParams->useBlock(i))
            num++;
    }
    return num;
}

