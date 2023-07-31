#include "growth.h"
#include "parametermodel.h"
#include "setupmodel.h"
#include "model.h"

#include <QMessageBox>

ss_growth::ss_growth(QObject *parent)
    : QObject(parent)
{
    ssModel = static_cast<ss_model *>(parent);
    num_patterns = 0;
    num_params = 0;

    growthBins = new tablemodel(parent);
    growthBins->setRowCount(1);

    morphdisttable = new tablemodel(parent);
    morphdisttable->setRowCount(1);
    morphdiststdevtable = new tablemodel(parent);
    morphdiststdevtable->setColumnCount(14);
    morphdiststdevtable->setRowCount(1);

    setNatural_mortality_type(0);
    natMortNumBreakPoints = 1;
    natMortBreakPoints = new tablemodel(parent);
    natMortBreakPoints->setRowCount(1);
    natMortBreakPoints->setRowHeader(0, QString("Age(real) at M Breakpoints"));
    natMortAges = new tablemodel(parent);
    natMortAges->setRowCount(0);
    natMortAges->setColumnCount(1);
    natMort_lorenzen_ref_age1 = 0;
    natMort_lorenzen_ref_age2 = 1;

    matAgeValues = new tablemodel(parent);
    matAgeValues->setRowCount(1);

    timeVaryMethod = 1;
    timeVaryReadParams = 0;

    cohortParam = new longParameterModel(ssModel);
    cohortParam->setNumParams(1);
    cohortParam->setParamHeader(0, QString("CohortGrowDev"));
    cohortVarParam = new timeVaryParameterModel(ssModel);
    cohortVarParam->setTotalNumVarTables(1);
    cohortVarParam->setTableTitle(0, QString("CohortGrowDev"));
    connect (cohortParam, SIGNAL(paramChanged(int,QStringList)),
             cohortVarParam, SLOT(changeVarParamData(int,QStringList)));


    reset();
}

ss_growth::~ss_growth()
{
    setNum_morphs(0);
    setNum_patterns(0);

    delete growthBins;
    delete morphdisttable;
    delete morphdiststdevtable;
    delete natMortBreakPoints;
    delete natMortAges;
    delete matAgeValues;
     delete cohortParam;
    delete cohortVarParam;
    customEnvLink = -1;
//    delete environmentParams;
//    delete environmentVarParams;
    customBlock = -1;
//    delete blockParams;
//    delete blockVarParams;
}

void ss_growth::reset()
{
    setModel(1);

    setNum_patterns(1);
    setAge_for_l1(1.66);
    setAge_for_l2(25);
    setNumKmults(0);
    setAgeMin_for_K(5);
    setAgeMid_for_K(0);
    setAgeMax_for_K(7);
    setExpDecay(0.2);  // default for 3.24 compatibility
    setSd_add(0);
    setCv_growth_pattern(0);
    setMaturity_option(2);
    setFirst_mature_age(1);

    num_params = 0;
    matAgeValues->setRowCount(1);
    setTimeVaryMethod(1);
    setTimeVaryReadParams(0);
    cohortParam->setNumParams(1);
//    blockParams->setNumParams(0);
//    environmentParams->setNumParams(0);
    growthBinMethod = 1;
    growthBinMin = 0;
    growthBinMax = 0;
    growthBinStep = 0;

    setNum_morphs(1);
    setMorph_within_ratio(1.0);

}

/*
growth_pattern * ss_growth::getPattern(int i) const
{
    growth_pattern * gp = nullptr;
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
    longParameter *lp = nullptr;
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
tablemodel *ss_growth::getGrowthBinTable()
{
    return growthBins;
}
QStringList ss_growth::getGrowthBins ()
{
    return growthBins->getRowData(0);
}
void ss_growth::setGrowthBins(QStringList data)
{
    if (data.count() > 1) {
        growthBins->setRowData(0, data);
        float min = data.first().toFloat();
        float max = data.last().toFloat();
        float wid = data.at(1).toFloat() - min;
        if (growthBinMethod == 3) {
            growthBinMin = min;
            growthBinMax = max;
            growthBinStep = wid;
        }
    }
}


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
        value = 1;
    else if (value <= 3)
        value = 3;
    else
        value = 5;
    num_morphs = value;
    morphdisttable->setColumnCount(value);
    setMorphDist();
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

void ss_growth::setMorphDist()
{
    switch (num_morphs)
    {
        case 1:
            setMorph_dist(0, 1);
            break;
        case 3:
            setMorph_dist(0, 0.15);
            setMorph_dist(1, 0.70);
            setMorph_dist(2, 0.15);
            break;
        case 5:
            setMorph_dist(0, 0.031);
            setMorph_dist(1, 0.237);
            setMorph_dist(2, 0.464);
            setMorph_dist(3, 0.237);
            setMorph_dist(4, 0.031);
    }
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

int ss_growth::getTimeVaryMethod() const
{
    return timeVaryMethod;
}

void ss_growth::setTimeVaryMethod(int value)
{
    timeVaryMethod = value;
}

int ss_growth::getTimeVaryReadParams() const
{
    return timeVaryReadParams;
}

void ss_growth::setTimeVaryReadParams(int value)
{
    timeVaryReadParams = value;
    if (value == 0) // autogenerate
        autogenerateTimeVaryParams();
    for (int i = 0; i < getNum_patterns(); i++) {
        getPattern(i)->setTimeVaryReadParams (value);
    }
}

void ss_growth::autogenerateTimeVaryParams()
{
    cohortVarParam->setAutoGenerate(timeVaryReadParams);
    cohortVarParam->changeVarParamData(0, getCohortParam());
}

void ss_growth::setMorphDistStDev (QStringList &stdev)
{
    morphdiststdevtable->setRowData(0, stdev);
}

tablemodel *ss_growth::getMorphDistStDevModel() const
{
    return morphdiststdevtable;
}

QStringList ss_growth::getMorphDistStDev() const
{
    return morphdiststdevtable->getRowData(0);
}

void ss_growth::setGrowthBinStep(float value)
{
    growthBinStep = value;
}

float ss_growth::getGrowthBinWidth() const
{
    return growthBinStep;
}

float ss_growth::getGrowthBinMax() const
{
    return growthBinMax;
}

void ss_growth::setGrowthBinMax(float value)
{
    growthBinMax = value;
}

float ss_growth::getGrowthBinMin() const
{
    return growthBinMin;
}

void ss_growth::setGrowthBinMin(float value)
{
    growthBinMin = value;
}

void ss_growth::generateGrowthBins()
{
    QStringList data;
    float length;
    if (growthBinMax != 0 && growthBinStep != 0) {
        for (length = growthBinMin; length <= growthBinMax;
             length += growthBinStep) {
            data.append(QString::number(length, 'g', 2));
        }
        setNumGrowthBins(data.count());
        setGrowthBins(data);
    }
    else {
        QString msg ("Could not generate population growth bins from values entered in data file.");
        QMessageBox::information((QWidget *)this, tr("Input Error"), tr(msg.toUtf8()));
    }
}

int ss_growth::getGrowthBinMethod() const
{
    return growthBinMethod;
}

void ss_growth::setGrowthBinMethod(int value)
{
    growthBinMethod = value;
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
        for (int i = 0; i < num_patterns; i++) {
            getPattern(i)->getFemNatMParams()->setRowCount(1);
            getPattern(i)->getMaleNatMParams()->setRowCount(1);
        }
        break;
    case 1:
        setNatMortNumBreakPts(natMortNumBreakPoints);
        break;
    case 2:
        setNaturalMortLorenzenRef(4);
        break;
    case 3:
    case 4:
        for (int i = 0; i < num_patterns; i++) {
            getPattern(i)->getNatMAges()->setRowCount(num_genders);
            getPattern(i)->getNatMAges()->setColumnCount(num_ages+1);
        }
        break;
    case 5:
        setNatMortMaunderOption(1);
        break;
    case 6:
        setNaturnalMortLorenzenRefMin(0);
        setNaturnalMortLorenzenRefMax(10);
        break;
    }
}

int ss_growth::getNatMortNumBreakPts() const
{
    return natMortNumBreakPoints;
}

void ss_growth::setNatMortNumBreakPts(int num)
{
    natMortNumBreakPoints = num;
    natMortBreakPoints->setColumnCount(num);
    for (int i = 0; i < num_patterns; i++) {
        getPattern(i)->getFemNatMParams()->setRowCount(natMortNumBreakPoints);
        getPattern(i)->getMaleNatMParams()->setRowCount(natMortNumBreakPoints);
    }
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

void ss_growth::setNaturnalMortLorenzenRefMin(int value)
{
    natMort_lorenzen_ref_age1 = value;
    QStringList datalist;
    int numparams = 1;
    datalist << "0.0" << "0.0" << "0.0" << "0.0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    if (getNatural_mortality_type() == 5)
    {
        if (value == 3)
        {
            numparams = 6;
        }
        else
        {
            numparams = 4;
        }
    }

    for (int i = 0; i < num_patterns; i++)
    {
        getPattern(i)->getFemNatMParams()->setRowCount(numparams);
        getPattern(i)->getMaleNatMParams()->setRowCount(numparams);
        for (int j = 0; j < numparams; j++)
        {
            getPattern(i)->getFemNatMParams()->setRowData(j, datalist);
            getPattern(i)->getFemNatMParams()->setRowHeader(j, QString("natM_p_%1_Fem_GP_%2").arg(j+1).arg(i+1));
            getPattern(i)->getMaleNatMParams()->setRowData(j, datalist);
            getPattern(i)->getMaleNatMParams()->setRowHeader(j, QString("natM_p_%1_Mal_GP_%2").arg(j+1).arg(i+1));
        }
    }
}

void ss_growth::setNumGenders(int num)
{
    num_genders = num;
    for (int i = 0; i < num_patterns; i++)
        patterns.at(i)->setNumGenders(num);
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

void ss_growth::setCohortParam (QStringList data)
{
    cohortParam->setParamData(0, data);
    cohortVarParam->setParameter(0, data);
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
        patterns.append(new growthPattern(ssModel));
    }
    i = patterns.count();
    for (; i < value; i++)
    {
        growthPattern *new_gp = new growthPattern(ssModel);
        patterns.append(new_gp);
    }
    while (value < patterns.count())
    {
        growthPattern *old = patterns.takeLast();
        delete old;
    }
    num_patterns = patterns.count();
}

growthPattern *ss_growth::getPattern(int index)
{
    growthPattern *gp = nullptr;
    if (index < 0) index = 0;
    if (patterns.count() > index)
        gp = patterns[index];
    return gp;
}
/*
int ss_growth::getNumDevParams()
{
    int num = 0;
    QStringList data;
    for (int i = 0; i < wtLenParams->getNumParams(); i++)
    {
        if (wtLenParams->useDevs(i))
            num++;
    }
    return num;
}

int ss_growth::getNumEnvLinkParams()
{
    int num = 0;
    QStringList data;
    for (int i = 0; i < wtLenParams->getNumParams(); i++)
    {
        if (wtLenParams-> envLink(i))
            num++;
    }
    return num;
}

int ss_growth::getNumBlockParams()
{
    int num = 0;
    QStringList data;
    for (int i = 0; i < wtLenParams->getNumParams(); i++)
    {
        if (wtLenParams->useBlock(i))
            num++;
    }
    return num;
}
*/
