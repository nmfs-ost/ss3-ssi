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
    catchMultiplierParam = new longParameterModel(this);
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
    generalComps.append(nullptr);
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
    size_selex = new selectivity(parent, SelexType::Size);
    size_selex->setFleet(this);
    age_selex = new selectivity(parent, SelexType::Age);
    age_selex->setFleet(this);
    setName(QString("new_fleet"));
    i_number = 0;
    size_selex->setFleetNum(i_number);
    age_selex->setFleetNum(i_number);

    dar1 = new DAR1();

    reset();
}

Fleet::~Fleet()
{
    reset();
    if (s_name != nullptr)
        delete s_name;
    s_name = nullptr;
    if (retainCatch != nullptr)
        delete retainCatch;
    retainCatch = nullptr;
    if (abundModel != nullptr)
        delete abundModel;
    abundModel = nullptr;
    if (discardModel != nullptr)
        delete discardModel;
    discardModel = nullptr;
    if (lengthComp != nullptr)
        delete lengthComp;
    lengthComp = nullptr;
    if (ageComp != nullptr)
        delete ageComp;
    ageComp = nullptr;
    if (morphComp != nullptr)
        delete morphComp;
    morphComp = nullptr;
    while (generalComps.count() > 0)
    {
        fleet_comp_general*cg = generalComps.takeLast();
        delete cg;
        cg = nullptr;
    }
    if (mbwtObs != nullptr)
        delete mbwtObs;
    mbwtObs = nullptr;
    if (saaObs != nullptr)
        delete saaObs;
    saaObs = nullptr;
    if (recapObs != nullptr)
        delete recapObs;
    recapObs = nullptr;
    if (lambdaModel != nullptr)
        delete lambdaModel;
    lambdaModel = nullptr;

    delete size_selex;
    size_selex = nullptr;
    delete age_selex;
    age_selex = nullptr;

    delete q_R;
    q_R = nullptr;
    lambdas.clear();
}

void Fleet::setName(QString fname)
{
    if (!s_name->isEmpty())
        s_name->clear();
    s_name->append(fname);
    q_R->setFleetName(fname);
    size_selex->setFleetName(fname);
    age_selex->setFleetName(fname);
}

QString Fleet::getName()
{
    QString nm("");
    if (s_name == nullptr)
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
    size_selex->setFleetNum(i_number);
    age_selex->setFleetNum(i_number);

    for (i = 0; i < getNumCatchObs(); i++)
    {
        datalist = getCatchObservation(i);
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setCatchObservation(i, datalist);
    }
    for (i = 0; i < getNumAbundObs(); i++)
    {
        datalist = getAbundanceObs(i);
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setAbundanceObs(i, datalist);
    }
    for (i = 0; i < getDiscardCount(); i++)
    {
        datalist = getDiscard(i);
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setDiscard(i, datalist);
    }

    for (i = 0; i < getMbwtNumObs(); i++)
    {
        datalist = getMbwtObservation(i);
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setMbwtObservation(i, datalist);
    }
    for (i = 0; i < getLengthNumObs(); i++)
    {
        datalist = getLengthObservation(i);
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setLengthObservation(i, datalist);
    }
    for (i = 0; i < getAgeNumObs(); i++)
    {
        datalist = getAgeObservation(i);
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setAgeObservation(i, datalist);
    }
    for (i = 0; i < getSaaNumObs(); i++)
    {
        datalist = getSaaObservation(i);
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setSaaObservation(i, datalist);
    }
    for (i = 0; i < getGenModelTotal(); i++)
    {
        for (int j = 0; j < getGenNumObs(i); j++)
        {
            datalist = getGenObservation(i, j);
            datum = datalist.at(2);
            if (datum.contains("-"))
                datalist[2] = negFltNo;
            else
                datalist[2] = posFltNo;
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
        datum = datalist.at(2);
        if (datum.contains("-"))
            datalist[2] = negFltNo;
        else
            datalist[2] = posFltNo;
        setMorphObservation(i, datalist);
    }

}

void Fleet::reset()
{
//    setName(QString("new_fleet"));
//    setNumber(1);
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
    size_selex->reset();

    //   age selex
    age_selex->reset();

    // 2DAR1
    dar1->reset();

    // Variance
    for (int i = 0; i <= 7; i++)
    {
        inputValueVar[i] = false;
        inputVarianceValue[i] = 0;
    }
    add_to_survey_CV = 0;
    add_to_discard_stddev = 0;
    add_to_bodywt_CV = 0;
    mult_by_lencomp_N = 1;
    mult_by_agecomp_N = 1;
    mult_by_saa_N = 1;
    mult_by_gen_size = 1;

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
    for (int i = 1; i <= oldfl->getNumSeasons(); i++)
        set_catch_equil(i, oldfl->catch_equil(i));
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
    size_selex->setAltBins(model->get_length_composition()->getBins());
    size_selex->setNumAges(model->get_age_composition()->getNumberBins());
    for (i = 0; i < oldfl->getSizeSelectivity()->getNumParameters(); i++)
        size_selex->setParameter(i, oldfl->getSizeSelectivity()->getParameter(i));
    set_size_selex_discard(oldfl->size_selex_discard());
    set_size_selex_male(oldfl->size_selex_male());
    set_size_selex_special(oldfl->size_selex_special());
    set_size_selex_pattern(oldfl->size_selex_pattern());

    //   age selex
    age_selex->setBins(model->get_age_composition()->getBins());
    age_selex->setNumAges(model->get_age_composition()->getNumberBins());
    for (i = 0; i < oldfl->getAgeSelectivity()->getNumParameters(); i++)
        age_selex->setParameter(i, oldfl->getAgeSelectivity()->getParameter(i));
    set_age_selex_gt_lt(oldfl->age_selex_gt_lt());
    set_age_selex_male(oldfl->age_selex_male());
    set_age_selex_special(oldfl->age_selex_special());
    set_age_selex_pattern(oldfl->age_selex_pattern());

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
    d_catch_equil.clear();
    for (int i = 0; i < n_seasons; i++)
        d_catch_equil.append(0);
}

void Fleet::setNumGenders(int num)
{
    i_num_genders = num;
    size_selex->setNumGenders(num);
    age_selex->setNumGenders(num);
}

void Fleet::setCatchMultParam(QStringList& data)
{
    catchMultiplier = QStringList(data);
    catchMultiplierParam->setParameter(0, data);
}

const QStringList& Fleet::getCatchMultParam()
{
    catchMultiplier = catchMultiplierParam->getParameter(0);
    return catchMultiplier;
}

tablemodel *Fleet::getCatchMultParameters()
{
    return catchMultiplierParam->getParamTable();
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
    yearIndexMeasure *yim = nullptr;
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




void Fleet::appendLambda(QStringList values)
{
    int rows = lambdaModel->rowCount();
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

void Fleet::setSelTimeVaryReadParams(int flag)
{
    if (readSelTvParams != flag) {
        readSelTvParams = flag;
        size_selex->setTVautogenerate(flag);
        age_selex->setTVautogenerate(flag);
    }
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

