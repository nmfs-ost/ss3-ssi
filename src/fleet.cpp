#include "fleet.h"
#include "model.h"
#include "q_ratio.h"

Fleet::Fleet(ss_model *parent) :
    QObject((QObject *)parent)
{
//    ss_model *model = static_cast<ss_model*>(parent);
    i_start_year = parent->get_start_year();
    f_timing = .5;

    set_max_catch(-1);
    retainCatch = new tablemodel(this);
    retainCatch->setColumnCount(5);
    catchHeader << "Year" << "Season" << "Fleet" << "Catch" << "Catch SE";
    retainCatch->setHeader(catchHeader);

    abundModel = new tablemodel (this);
    abundModel->setColumnCount(5);
    abundanceHeader << "Year" << "Month" << "Fleet" << "Observation" << "SE";
    abundModel->setHeader(abundanceHeader);

    discardModel = new tablemodel (this);
    discardModel->setColumnCount(5);
    discardHeader << "Year" << "Month" << "Fleet" << "Observation" << "Std.Err.";
    discardModel->setHeader(discardHeader);

    mbwtObs = new meanBwtObservation();
    lengthComp = new fleet_comp_length(this);
    ageComp = new fleet_comp_age(this);
//    ageObs = new ageObservation();
    saaObs = new saaObservation();
    recapObs = new recapObservation();
//    genObs.append(0);
//    genObs.clear();
    generalComps.append(0);
    generalComps.clear();
    morphComp = new fleet_comp_morph(this);
//    morphObs = new morphObservation();


    lambdaModel = new tablemodel (this);
    lambdaModel->setColumnCount(4);
    abundanceHeader.clear();
    abundanceHeader << "Component" << "Phase" << "Lambda" << "SizeFreq";
    lambdaModel->setHeader(abundanceHeader);

    q_read = false;
    q_R = new q_ratio(parent);
    s_name = new QString("");
    setName(QString("new_fleet"));
    size_selex = new selectivity(parent);
    age_selex = new selectivity(parent);
    reset();
}

Fleet::~Fleet()
{
    reset();
    if (s_name != NULL)
        delete s_name;
    s_name = NULL;
    if (retainCatch != NULL)
        delete retainCatch;
    retainCatch = NULL;
    if (abundModel != NULL)
        delete abundModel;
    abundModel = NULL;
    if (discardModel != NULL)
        delete discardModel;
    discardModel = NULL;
    if (lengthComp != NULL)
        delete lengthComp;
    lengthComp = NULL;
    if (ageComp != NULL)
        delete ageComp;
    ageComp = NULL;
    if (morphComp != NULL)
        delete morphComp;
    morphComp = NULL;
    while (generalComps.count() > 0)
    {
        fleet_comp_general*cg = generalComps.takeLast();
        delete cg;
        cg = NULL;
    }
    if (mbwtObs != NULL)
        delete mbwtObs;
    mbwtObs = NULL;
    if (saaObs != NULL)
        delete saaObs;
    saaObs = NULL;
    if (recapObs != NULL)
        delete recapObs;
    recapObs = NULL;
    if (lambdaModel != NULL)
        delete lambdaModel;
    lambdaModel = NULL;

    delete size_selex;
    size_selex = NULL;
    delete age_selex;
    age_selex = NULL;

    delete q_R;
    q_R = NULL;
    lambdas.clear();
}

void Fleet::setName(QString fname)
{
    if (!s_name->isEmpty())
        s_name->clear();
    s_name->append(fname);
    q_R->setFleetName(fname);
}

QString Fleet::getName()
{
    QString nm("");
    if (s_name == NULL)
        nm.append("None");
    else
        nm.append(*s_name);
    return nm;
}

void Fleet::setNumber (int n)
{
    int i;
    QString datum;
    QStringList datalist;
    QString posFltNo (QString::number(n));
    QString negFltNo (QString("-%1").arg(posFltNo));
    i_number = n;

    for (i = 0; i < getNumCatchObs(); i++)
    {
        datalist = getCatchObservation(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setCatchObservation(i, datalist);
    }
    for (i = 0; i < getNumAbundObs(); i++)
    {
        datalist = getAbundanceObs(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setAbundanceObs(i, datalist);
    }
    for (i = 0; i < getDiscardCount(); i++)
    {
        datalist = getDiscard(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setDiscard(i, datalist);
    }

    for (i = 0; i < getMbwtNumObs(); i++)
    {
        datalist = getMbwtObservation(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setMbwtObservation(i, datalist);
    }
    for (i = 0; i < getLengthNumObs(); i++)
    {
        datalist = getLengthObservation(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setLengthObservation(i, datalist);
    }
    for (i = 0; i < getAgeNumObs(); i++)
    {
        datalist = getAgeObservation(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setAgeObservation(i, datalist);
    }
    for (i = 0; i < getSaaNumObs(); i++)
    {
        datalist = getSaaObservation(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setSaaObservation(i, datalist);
    }
    for (i = 0; i < getGenModelTotal(); i++)
    {
        for (int j = 0; j < getGenNumObs(i); j++)
        {
            datalist = getGenObservation(i, j);
            datum = datalist.takeAt(3);
            if (datum.contains("-"))
                datalist.insert(3, negFltNo);
            else
                datalist.insert(3, posFltNo);
            setGenObservation(i, j, datalist);
        }
    }
/*    for (i = 0; i < getRecapNumEvents(); i++)
    {
        datalist = getRecapObservation(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(3, fltno.prepend("-"));
        else
            datalist.insert(3, fltno);
        setRecapObservation(i, datalist);
    }*/
    for (i = 0; i < getMorphNumObs(); i++)
    {
        datalist = getMorphObservation(i);
        datum = datalist.takeAt(2);
        if (datum.contains("-"))
            datalist.insert(2, negFltNo);
        else
            datalist.insert(2, posFltNo);
        setMorphObservation(i, datalist);
    }

}

void Fleet::reset()
{
    setName(QString("new_fleet"));
    setNumber(1);
    setActive(true);
    setType(Fleet::Fishing);
    setArea(1);
    setSeasTiming(0.5);
    setBycatchDead(1);
    setBycatchF(1);
    setBycFirstYr(QString("1993"));
    setBycLastYr(QString("1996"));
    setBycUnused(QString("1"));
    setTotalYears(1);
    setNumSeasons(1);
    setNumGenders(2);
    setAllocGroup(0);

    // catch
    setCatchUnits(2);
    set_equ_catch_se(0.1);
    set_catch_se(0.1);
    setCatchMultiplier(0);
    set_catch_equil(0.0);
    setNumCatchObs(0);

    // surveys
    setAbundUnits(0);
    setAbundErrType(0);

    // abundance
    setNumAbundObs(0);

    // discard
    setDiscardUnits(0);
    setDiscardErrType(0);
    setNumDiscardObs(0);

    // mean bwt
    setMbwtDF(0);
    setMbwtNumObs(0);

    // sizecomp data
    setLengthMinTailComp("0.0");
    setLengthAddToData("0.0");
    setLengthCombineGen(0);
    setLengthCompressBins(0);
    setLengthCompError(0);
    setLengthCompErrorParm(0);
    setLengthNumObs(0);
    setLengthNumBins(0);

    // agecomp data
    setAgeMinTailComp("0.0");
    setAgeAddToData("0.0000001");
    setAgeCombineGen(0);
    setAgeCompressBins(0);
    setAgeCompError(0);
    setAgeCompErrorParm(0);
    setAgeNumObs(0);
    setAgeNumBins(0);
    ageComp->set_num_error_defs(0);

    // saa data
    setSaaNumObs(0);
    setSaaNumBins(0);

    // general size data
    while (generalComps.count() > 0)
    {
        fleet_comp_general*cg = generalComps.takeLast();
        cg->setNumberObs(0);
        delete cg;
    }
    setGenModelTotal(0);

    // recapture data
    setRecapNumEvents(0);

    // morph data
    setMorphMinTailComp("0.0");
    setMorphAddToData("0.0");
    setMorphCombineGen(0);
    setMorphCompressBins(0);
    setMorphCompError(0);
    setMorphCompErrorParm(0);
    setMorphNumObs(0);
    setMorphNumMorphs(0);

    //  q_section
    q_read = false;
    q_R->reset();

    //   size selex
    size_selex->setPattern(0);

    //   age selex
    age_selex->setPattern(0);

    // Variance
    add_to_survey_CV = 0;
    add_to_discard_stddev = 0;
    add_to_bodywt_CV = 0;
    mult_by_lencomp_N = 1;
    mult_by_agecomp_N = 1;
    mult_by_saa_N = 1;

    resetLambdas();
}

Fleet *Fleet::copy(Fleet *oldfl)
{
    ss_model *model = (ss_model *)oldfl->parent();
    int i;
    QStringList strList;
    setName (QString("%1_Copy").arg(oldfl->getName()));
    setActive(true);
    setNumber(0);
    setType (oldfl->getType());
    setArea(oldfl->getArea());
    setSeasTiming(oldfl->getSeasTiming());
    setStartYear(oldfl->getStartYear());
    setTotalYears(oldfl->getTotalYears());
    setNumSeasons(oldfl->getNumSeasons());

    // catch
    setCatchUnits(oldfl->getCatchUnits());
    set_catch_se(oldfl->catch_se());
    set_equ_catch_se(oldfl->equ_catch_se());
    set_catch_equil(oldfl->catch_equil());
    setCatchMultiplier(oldfl->getCatchMultiplier());
    setNumCatchObs(oldfl->getNumCatchObs());
    for (int i = 0; i < retainCatch->rowCount(); i++)
    {
        setCatchObservation(i, oldfl->getCatchObservation(i));
    }

    // abundance
    setAbundUnits(oldfl->getAbundUnits());
    setAbundErrType(oldfl->getAbundErrType());
    setNumAbundObs(oldfl->getAbundanceCount());
    for (i = 0; i < abundModel->rowCount(); i++)
        setAbundanceObs(i, oldfl->getAbundanceObs(i));

    // discard
    setDiscardUnits(oldfl->getDiscardUnits());
    setDiscardErrType(oldfl->getDiscardErrType());
    setNumDiscardObs(oldfl->getDiscardCount());
    for (i = 0; i < discardModel->rowCount(); i++)
        setDiscard(i, oldfl->getDiscard(i));

    // mean bwt
    setMbwtDF(oldfl->getMbwtDF());
    setMbwtNumObs(oldfl->getMbwtNumObs());
    for (i = 0; i < getMbwtNumObs(); i++)
        setMbwtObservation(i, oldfl->getMbwtObservation(i));

    // sizecomp data
    setLengthMinTailComp(oldfl->getLengthMinTailComp());
    setLengthAddToData(oldfl->getLengthAddToData());
    setLengthCombineGen(oldfl->getLengthCombineGen());
    setLengthCompressBins(oldfl->getLengthCompressBins());
    setLengthCompError(oldfl->getLengthCompError());
    setLengthCompErrorParm(oldfl->getLengthCompErrorParm());
    setLengthNumBins(model->get_length_composition()->getNumberBins());
    setLengthNumObs(oldfl->getLengthNumObs());
    for (i = 0; i < getLengthNumObs(); i++)
        setLengthObservation(i, oldfl->getLengthObservation(i));

    // agecomp data
    setAgeMinTailComp(oldfl->getAgeMinTailComp());
    setAgeAddToData(oldfl->getAgeAddToData());
    setAgeCombineGen(oldfl->getAgeCombineGen());
    setAgeCompressBins(oldfl->getAgeCompressBins());
    setAgeCompError(oldfl->getAgeCompError());
    setAgeCompErrorParm(oldfl->getAgeCompErrorParm());
    setAgeNumBins(model->get_age_composition()->getNumberBins());
    setAgeNumObs(oldfl->getAgeNumObs());
    for (i = 0; i < getAgeNumObs(); i++)
        setAgeObservation(i, oldfl->getAgeObservation(i));

    // saa data
    setSaaNumBins(model->get_age_composition()->getNumberBins());
    setSaaNumObs(oldfl->getSaaNumObs());
    for (i = 0; i < getSaaNumObs(); i++)
        setSaaObservation(i, oldfl->getSaaObservation(i));

    // general size data
    setGenModelTotal(oldfl->getGenModelTotal());
    for (int mod = 0; mod < getGenModelTotal(); mod++)
    {
        setGenMinTailComp(mod, oldfl->getGenMinTailComp(mod));
        setGenAddToData(mod, oldfl->getGenAddToData(mod));
        setGenCombineGen(mod, oldfl->getGenCombineGen(mod));
        setGenCompressBins(mod, oldfl->getGenCompressBins(mod));
        setGenCompError(mod, oldfl->getGenCompError(mod));
        setGenCompErrorParm(mod, oldfl->getGenCompErrorParm(mod));
        setGenNumBins(mod, model->getGeneralCompMethod(mod)->getNumberBins());
        setGenNumObs(mod, oldfl->getGenNumObs(mod));
        for (i = 0; i < getGenNumObs(mod); i++)
            setGenObservation(mod, i, oldfl->getGenObservation(mod, i));
    }

    // recapture data
    setRecapNumEvents(oldfl->getRecapNumEvents());
    for (i = 0; i < getRecapNumEvents(); i++)
        setRecapObservation(i, oldfl->getRecapObservation(i));

    // morph data
    if (model->getDoMorphComp())
    {
        setMorphMinTailComp(oldfl->getMorphMinTailComp());
        setMorphAddToData(oldfl->getMorphAddToData());
        setMorphCombineGen(oldfl->getMorphCombineGen());
        setMorphCompressBins(oldfl->getMorphCompressBins());
        setMorphCompError(oldfl->getMorphCompError());
        setMorphCompErrorParm(oldfl->getMorphCompErrorParm());
        setMorphNumMorphs(model->get_morph_composition()->getNumberMorphs());
        setMorphNumObs(oldfl->getMorphNumObs());
        for (i = 0; i < getMorphNumObs(); i++)
            setMorphObservation(i, oldfl->getMorphObservation(i));
    }

    //  q_section
    Q()->setSetup(oldfl->Q()->getSetup());
/*    Q()->setDoPower(oldfl->Q()->getDoPower());
    Q()->setDoEnvLink(oldfl->Q()->getDoEnvLink());
    Q()->setDoExtraSD(oldfl->Q()->getDoExtraSD());
    Q()->setType(oldfl->Q()->getType());*/
/*    set_q_do_power(oldfl->q_do_power());
    set_q_do_env_lnk(oldfl->q_do_env_lnk());
    set_q_do_extra_sd(oldfl->q_do_extra_sd());
    set_q_type(oldfl->q_type());*/
    for (i = 0; i < oldfl->Q()->getNumParams(); i++)
    {
        strList = oldfl->Q()->getParameter(i);
        Q()->setParameter(i, strList);
    }
    for (i = 0; i < oldfl->Q()->getNumTVParams(); i++)
    {
        strList = oldfl->Q()->getTVParam(i);
        Q()->setTVParam(i, strList);
    }

    //   size selex
    set_size_selex_pattern(oldfl->size_selex_pattern());
    set_size_selex_discard(oldfl->size_selex_discard());
    set_size_selex_male(oldfl->size_selex_male());
    set_size_selex_special(oldfl->size_selex_special());
    size_selex->setPattern(size_selex_pattern());
    for (i = 0; i < oldfl->getSizeSelectivity()->getNumParameters(); i++)
        size_selex->setParameter(i, oldfl->getSizeSelectivity()->getParameter(i));

    //   age selex
    set_age_selex_pattern(oldfl->age_selex_pattern());
    set_age_selex_gt_lt(oldfl->age_selex_gt_lt()); // <>
    set_age_selex_male(oldfl->age_selex_male());
    set_age_selex_special(oldfl->age_selex_special());
    age_selex->setPattern(age_selex_pattern());
    for (i = 0; i < oldfl->getAgeSelectivity()->getNumParameters(); i++)
        age_selex->setParameter(i, oldfl->getAgeSelectivity()->getParameter(i));

    // lambdas
    {
        int lams = oldfl->getNumLambdas();
        setNumLambdas(0);
        for (i = 0; i < lams; i++)
        {
            appendLambda(oldfl->getLambda(i));
        }
    }

    return this;
}

void Fleet::setTotalYears(int n_years)
{
    i_num_years = n_years;
}
void Fleet::add_catch_per_season(int yr, int seas, double value, double se)
{
    int index = retainCatch->rowCount();
    set_catch_per_season(index, yr, seas, value, se);
}

void Fleet::set_catch_per_season(int index, int yr, int seas, double value, double se)
{
    QVector<double> rowdata;
    rowdata.append((double)yr);
    rowdata.append((double)seas);
    rowdata.append(value);
    rowdata.append(se);

    if (retainCatch->rowCount() <= index)
        retainCatch->setRowCount(index + 1);

    retainCatch->setRowData(index, rowdata);
}


void Fleet::addCatchObservation(QStringList data)
{
    int index = retainCatch->rowCount();
    setCatchObservation(index, data);
}

void Fleet::setCatchObservation(int index, QStringList data)
{
    if (retainCatch->rowCount() <= index)
        retainCatch->setRowCount(index + 1);
    retainCatch->setRowData(index, data);
}

QStringList Fleet::getCatchObservation (int index)
{
    return retainCatch->getRowData(index);
}



void Fleet::setNumDiscardObs (int num)
{
    discardModel->setRowCount(num);
}

void Fleet::setDiscardMonth (int year, float month, float obs, float err)
{
    QString yr(QString::number(year));
    QString mo(QString::number(month));
    int row = getYearMonthRow(discardModel, yr, mo);
    QStringList values;
    values << yr << mo << QString::number(obs) << QString::number(err);
    discardModel->setRowData(row, values);
}

int Fleet::getDiscardCount ()
{
    return discardModel->rowCount();
}

void Fleet::setDiscard(int i, QStringList data)
{
    if (i >= getDiscardCount())
        discardModel->setRowCount(i+1);
    discardModel->setRowData(i, data);
}

void Fleet::addDiscard(QStringList data)
{
    setDiscard(getDiscardCount(), data);
}

QStringList Fleet::getDiscard (int row)
{
    QStringList values(discardModel->getRowData(row));
    return values;
}

float Fleet::getDiscardObs (int year, float month)
{
    QString yr(QString::number(year));
    QString mo(QString::number(month));
    int row = getYearMonthRow(discardModel, yr, mo);
    QStringList values(discardModel->getRowData(row));
    return values.at(2).toFloat();
}

float Fleet::getDiscardErr (int year, float month)
{
    QString yr(QString::number(year));
    QString mo(QString::number(month));
    int row = getYearMonthRow(discardModel, yr, mo);
    QStringList values(discardModel->getRowData(row));
    return values.at(3).toFloat();
}

void Fleet::setGenModelTotal(int num)
{
    while (getGenModelTotal() < num)
        generalComps.append(new fleet_comp_general(this));
    while (num < getGenModelTotal())
        delete generalComps.takeLast();
}

int Fleet::getGenModelTotal()
{
    int num = 0;
    if (!generalComps.isEmpty())
        num = generalComps.count();
    return num;
}

void Fleet::addGenObservation(int index, QStringList data)
{
    generalComps.at(index)->addObservation(data);
}

void Fleet::setGenObservation(int index, int row, QStringList data)
{
    generalComps.at(index)->setObservation(row, data);
}

void Fleet::setNumSeasons(int n_seasons)
{
    i_num_seasons = n_seasons;
}

int Fleet::getNumSeasons()
{
    return i_num_seasons;
}

int Fleet::getYearMonthRow(tablemodel *tm, QString year, QString month)
{
    int row;
    for (row = 0; row < tm->rowCount(); row++)
    {
        QStringList check (tm->getRowData(row));
        if (check.at(0) == year)
        {
            if (check.at(1) == month)
                break;
        }
    }
    if (row == tm->rowCount())
        tm->setRowCount(row + 1);
    return row;
}

void Fleet::setNumAbundObs(int num)
{
    abundModel->setRowCount(num);
}

void Fleet::addAbundanceObs(QStringList data)
{
    setAbundanceObs(getAbundanceCount(), data);
}

void Fleet::setAbundanceObs(int index, QStringList data)
{
    if (abundModel->rowCount() <= index)
        abundModel->setRowCount(index + 1);
    abundModel->setRowData(index, data);
}

QStringList Fleet::getAbundanceObs(int row)
{
    return abundModel->getRowData(row);
}

void Fleet::addAbundByMonth(int year, float month, float obs, float err)
{
    int index = getAbundanceCount();
    QStringList values;
    values << QString::number(year);
    values << QString::number(month);
    values << QString::number(obs);
    values << QString::number(err);
    setAbundanceObs(index, values);
}

void Fleet::setAbundMonth(int year, float month, float obs, float err)
{
    QStringList values;
    int row = (year - i_start_year) * i_num_seasons + (int)month;
    values << QString::number(year);
    values << QString::number(month);
    values << QString::number(obs);
    values << QString::number(err);
//    row = getYearMonthRow(abundModel, values.at(0), values.at(1));
    setAbundanceObs(row, values);
}

void Fleet::set_abundance(int year, int season, float obs)
{
//    yearIndexMeasure *yim = getYearIndexMeasure(f_abundance, year, season);
//    yim->setValue(obs);
}

float Fleet::abundance(int year, int month)
{
    double val = 0.0;
 //   yearIndexMeasure *yim = NULL;
 //   yim = getYearIndexMeasure(f_abundance, year, month);
 //   if (yim)
 //       val = yim->getValue();
    return val;
}

void Fleet::set_abundance_error(int i_year, int month, float err)
{
 //   yearIndexMeasure *yim = getYearIndexMeasure(f_abundance_error, i_year, month);

 //   yim->setValue(err);
}

float Fleet::abundance_error(int year, int month)
{
    double val = 0.0;
//    yearIndexMeasure *yim = NULL;
//    yim = getYearIndexMeasure(f_abundance_error, year, month);
 //   if (yim)
 //       val = yim->getValue();
    return val;
}

int Fleet::getAbundanceCount()
{
    return abundModel->rowCount();
}

float Fleet::getAbundanceAmt(int yr, float mn)
{
    float dat = yr+mn/12.0;
    return dat;
}

float Fleet::getAbundanceErr(int yr, float mn)
{
    float dat = yr + mn / 12.0;
    return dat;
}

int Fleet::abundance_count()
{
    int count = 0;
    QStringList values;
    for (int i = 0; i < abundModel->rowCount(); i++)
    {
        values = abundModel->getRowData(i);
        if (!values.at(0).isEmpty())
        {
            if (values.at(2).toFloat() > 0.0001)
                count ++;
        }
    }
    return count;
/*    int count = 0;
    yearIndexMeasure *yim = NULL;
    for (int i = 0; i < f_abundance.count(); i++)
    {
        yim = f_abundance.at(i);
        if (yim->getValue() > 0.0001)
            count ++;
    }
    return count;*/
}

void Fleet::setQSetupRead(bool flag)
{
    if (q_read != flag)
    {
        if (!flag)
        {
            if (    Q()->getDoPower() ||
                    Q()->getDoExtraSD())// ||
 //                   Q()->getOffset() > 0)
                flag = true;
            else if (abundModel->rowCount() > 0)
                flag = true;
        }
        q_read = flag;
        qSetupChanged();
    }
}

void Fleet::qSetupChanged()
{
    Q()->changeSetup(getAbundErrType());//setupChanged(getAbundErrType());
}


/*
void Fleet::resetLambdas()
{
    if (!lambdas.isEmpty())
        lambdas.clear();
    numLambdas = 0;
}

void Fleet::setLambda (int cmp, int phs, float lmb, int szfq)
{
    bool append = true;

    fleetLambda lamb (cmp, phs, lmb, szfq);
    fleetLambda cur;

    if (numLambdas > 0)
    {
        for (int i = 0; i < numLambdas; i++)
        {
            cur = lambdas.at(i);
            if (cur == lamb)
            {
                append = false;
                break;
            }
        }
    }
    if (append)
    {
        lambdas.append(lamb);
        numLambdas = lambdas.count();
    }

}

void Fleet::setLambdaComponent (int index, int cmp)
{
    if (index >= 0 && index < numLambdas)
    {
        lambdas[index].setComponent(cmp);
    }
}

int Fleet::getLambdaComponent (int index)
{
    int cmp = 0;
    if (index >= 0 && index < numLambdas)
    {
        cmp = lambdas[index].getComponent();
    }
    return cmp;
}

void Fleet::setLambdaPhase (int index, int phs)
{
    if (index >= 0 && index < numLambdas)
    {
        lambdas[index].setPhase(phs);
    }
}

int Fleet::getLambdaPhase (int index)
{
    int phs = -1;
    if (index >= 0 && index < numLambdas)
    {
        phs = lambdas[index].getPhase();
    }
    return phs;
}

void Fleet::setLambdaValue (int index, float lmb)
{
    if (index >= 0 && index < numLambdas)
    {
        lambdas[index].setLambda(lmb);
    }
}

float Fleet::getLambdaValue (int index)
{
    float val = 1.0;
    if (index >= 0 && index < numLambdas)
    {
        val = lambdas[index].getLambda();
    }
    return val;
}

void Fleet::setLambdaSizeFreq(int index, int szfq)
{
    if (index >= 0 && index < numLambdas)
    {
        lambdas[index].setSizeFreq(szfq);
    }
}

int Fleet::getLambdaSizeFreq(int index)
{
    int val = 0;
    if (index >= 0 && index < numLambdas)
    {
        val = lambdas[index].getSizeFreq();
    }
    return val;
}
*/

void Fleet::appendLambda(QStringList values)
{
    int rows = lambdaModel->rowCount();
    lambdaModel->setRowCount(rows + 1);
    lambdaModel->setRowData(rows, values);
}

QString Fleet::getBycUnused() const
{
    return s_byc_unused;
}

void Fleet::setBycUnused(const QString &value)
{
    s_byc_unused = value;
}

QString Fleet::getBycLastYr() const
{
    return s_byc_lastYr;
}

void Fleet::setBycLastYr(const QString &value)
{
    s_byc_lastYr = value;
}

QString Fleet::getBycFirstYr() const
{
    return s_byc_firstYr;
}

void Fleet::setBycFirstYr(const QString &value)
{
    s_byc_firstYr = value;
}

int Fleet::getBycatchF() const
{
    return i_bycatch_f;
}

void Fleet::setBycatchF(int value)
{
    i_bycatch_f = value;
}

int Fleet::getBycatchDead() const
{
    return i_bycatch_dead;
}

void Fleet::setBycatchDead(int value)
{
    i_bycatch_dead = value;
}

int Fleet::getAr1SelSmoother()
{
    return ar1SelSmoother;
}

void Fleet::setAr1SelSmoother(int value)
{
    ar1SelSmoother = value;
}

float Fleet::get_max_catch() const
{
    return f_max_catch;
}

void Fleet::set_max_catch(float value)
{
    f_max_catch = value;
}

bool Fleet::getActive() const
{
    return active;
}

void Fleet::setActive(bool value)
{
    active = value;
}

