#include "model.h"
#include "tablemodel.h"
#include "tableview.h"
#include "composition.h"
#include "mainwindow.h"

#include <QMessageBox>

Season::Season()
{
    reset();
}

void Season::reset()
{
    spawning = false;
    numMonths = 1;
    numSubSeasons = 2;
}

float Season::getNumMonths() const
{
    return numMonths;
}

void Season::setNumMonths(float value)
{
    numMonths = value;
}

int Season::getNumSubSeasons () const
{
    return numSubSeasons;
}

void Season::setNumSubSeasons(int value)
{
    numSubSeasons = value;
}

bool Season::getSpawning() const
{
    return spawning;
}

void Season::setSpawning(bool value)
{
    spawning = value;
}

float Season::getDuration() const
{
    return duration;
}

void Season::setDuration(float value)
{
    duration = value;
}



ss_model::ss_model(QWidget *parent) :
    QObject(parent)
{
    ageData = NULL;
    lengthData = NULL;
    morphData = NULL;
    tag_loss_param = NULL;
    sdYearsModel = new tablemodel(this);
    sdYearsModel->setColumnCount(0);
    sdYearsModel->setRowCount(1);
    sdYearsModel->setRowHeader(0,"");
    additionalSdReporting = new sd_reporting();

    mbweightModel = new tablemodel(this);
    mbweightModel->setColumnCount(6);
    mbweightHeader << "Year" << "Season" << "Type" << "Partition" << "Value" << "CV";
    mbweightModel->setHeader(mbweightHeader);

    obsEnvironVars = new environmentalVars();
    tagData = new tagObservation();

    pPopulation = new population (this);
    forecast = new ss_forecast(1, 1, (QObject *)this);

    doTags = false;
    doMorphComp = false;
    lengthData = new compositionLength(this);

    catchMult = new shortParameterModel(this);
//    catchMult->setColumnCount(7);
    catchMult->setParamCount(1);

    selexAdjustMethod = 1;

    reset();
//    QMessageBox::information(this, "Program Flow", "Model set up.");
}

ss_model::~ss_model()
{
    clear();
    delete additionalSdReporting;
}

void ss_model::clear()
{
    sdYearsModel->setColumnCount(0);
    iSdValues.clear();
    set_num_seasons(0);

    for (int i = 0; i < fleets.count(); i++)
        deleteFleet(i);
    if (pPopulation) delete pPopulation;
    pPopulation = NULL;
 //   populations.clear();
    if (forecast) delete forecast;
    forecast = NULL;
    if (lengthData) delete lengthData;
    lengthData = NULL;
    if (ageData) delete ageData;
    ageData = NULL;
    if (morphData) delete morphData;
    morphData = NULL;
    while (!cListGeneralMethods.isEmpty())
    {
        compositionGeneral *a = cListGeneralMethods.takeFirst();
        delete a;
    }
    set_do_tags(false);
}

void ss_model::reset()
{
    // set defaults
    setReadMonths(true);
    set_use_softbounds(true);
    set_last_estim_phase(8);
    set_mc_burn(10);
    set_mc_thin(2);
    set_jitter_param(0);
    set_bio_sd_min_year(-1);
    set_bio_sd_max_year(-1);
    set_num_std_years(0);
    set_convergence_criteria(.0001);
    set_retrospect_year(0);
    set_biomass_min_age(2);
    set_depletion_basis(1);
    set_depletion_denom(.40);
    set_spr_basis(1);
    set_f_units(4);
    set_f_min_age(13);
    set_f_max_age(17);
    set_f_basis(2);
    setALKTol(0.0);
//    QMessageBox::information(this, "Program Flow", "Model basic data set up.");

    set_start_year(1980);
    set_end_year(2009);
    set_num_seasons(1);
    seasons.at(0)->setNumMonths(12);
    seasons.at(0)->setSpawning(true);
    seasons.at(0)->setNumSubSeasons(2);
    set_spawn_month(3.5);
    set_num_genders(2);
    set_num_std_years(0);
 //   set_f;
//    QMessageBox::information(this, "Program Flow", "Model seasons set up.");
    set_num_fleets(1);
    getActiveFleet(1)->reset();
    set_num_areas(1);
//    QMessageBox::information(this, "Program Flow", "Model fleets set up.");

    setNumBlockPatterns(0);

    forecast->reset();

    setReadWtAtAge(false);
    pPopulation->reset();
    iNumRuns = 0;
//    QMessageBox::information(this, "Program Flow", "Model Forecast and Population reset.");

    if (ageData == NULL)
        ageData = new compositionAge(this);
    ageData->setNumberBins(0);
    ageData->setNumberObs(0);
    ageData->set_num_error_defs(0);
    ageData->set_number_saa_observations(0);
    ageData->set_number_ages(0);
//    QMessageBox::information(this, "Program Flow", "Model age comp set up.");

    if (lengthData == NULL)
        lengthData = new compositionLength();
    lengthData->setNumberBins(0);
    lengthData->setNumberObs(0);
    lengthData->setNumberAltBins(0);
//    QMessageBox::information(this, "Program Flow", "Model length comp set up.");

    set_mean_body_wt_df(0);

    set_num_ages(0);
    set_num_std_years(0);

    setCustomSelParmDevPhase(0);
    setCustomBlockSetup(0);
    setCustomEnviroLink(0);
    setCustomSelParmDevAdjust(0);

    setDoMorphComp(false);
    if (morphData)
        delete morphData;
    morphData = NULL;
    while (!cListGeneralMethods.isEmpty())
    {
        compositionGeneral *a = cListGeneralMethods.takeFirst();
        delete a;
    }

    setAddVarSetupFromText(QString("0 0 0 0 0 0 0 0 0"));
    setAddVariance(0);
    setInputValueVariance(0);

    set_do_tags(false);
    set_num_tag_groups(0);
    setTagLoss(0);
}

void ss_model::set_start_year(int year)
{
    iStartYr = year;
    for (int i = 0; i < get_num_fleets(); i++)
    {
        getFleet(i)->setStartYear (iStartYr);
    }
    getPopulation()->setStartYear (iStartYr);
}

void ss_model::set_end_year(int year)
{
    iEndYr = year;
    iTotalYears = iEndYr - iStartYr + 1;
    for (int i = 0; i < get_num_fleets(); i++)
    {
        getFleet(i)->setTotalYears(iTotalYears);
    }
    getPopulation()->setTotalYears(iTotalYears);
}

void ss_model::set_num_std_years(int num_yrs)
{
    sdYearsModel->setColumnCount(num_yrs);
}

void ss_model::set_std_years_text(QString txt)
{
//    int i;
    QStringList ql (txt.split(' ', QString::SkipEmptyParts));
    sdYearsModel->setRowData(0, ql);
}

void ss_model::set_std_year(int index, QString year)
{
    QStringList newdata, data = sdYearsModel->getRowData(0);
    if (index >= sdYearsModel->columnCount())
        sdYearsModel->setColumnCount(index + 1);
    for (int i = 0; i < sdYearsModel->columnCount(); i++)
    {
        if (i < data.count())
            newdata.append(data.at(i));
        else
            newdata.append("0");
    }
    newdata[index] = year;
    sdYearsModel->setRowData(0, newdata);
}

/*    for (i = 0; i < iSdValues.count(); i++)
    {
        iSdValues[i] = 0;
    }
    for (i = 0; i < ql.count(); i++)
    {
        if (i == iSdValues.count())
            break;
        iSdValues[i] = ql.at(i).toInt();
    }*/

QStringList ss_model::get_std_years ()
{
    return sdYearsModel->getRowData(0);
}

QString ss_model::get_std_years_text()
{
    QString txt("");
    QStringList ql (get_std_years());// sdYearsModel->getRowData(0));
    for (int i = 0; i < ql.count(); i++)
    {
        if (ql.at(i).isEmpty())
            txt.append(" 0");
        else
            txt.append(QString(" %1").arg(ql.at(i)));
    }

//    for (int i = 0; i < iSdValues.count(); i++)
 //       txt.append(QString(" %1").arg(QString::number(iSdValues.at(i))));

    return txt;
}

void ss_model::setALKTol (float val)
{
    if (val < 0.0)
        val = 0.0;
    if (val > 0.1)
        val = 0.1;

    fALKTol = val;
}


Season *ss_model::getSeason (int index)
{
    Season *season = NULL;
    if (index > 0 && index <= seasons.count())
        season = seasons.at(index - 1);
    return season;
}

int ss_model::getSeasonByMonth(float month)
{
    int mon = static_cast<int>(month);
    int index, months = 0;
    for (index = 0; index < seasons.count(); index++)
    {
        months += seasons.at(index)->getNumMonths();
        if (months >= mon)
            break;
    }
    return index + 1;
}

float ss_model::getMonthBySeasonFleet(int seas, int fleet)
{
    float month = 0.0;
    Season *seasn;
    Fleet *flt;
    for (int i = 1; i < seas; i++)
    {
        seasn = getSeason(i);
        if (seasn != NULL)
            month += seasn->getNumMonths();
    }
    seasn = getSeason(seas);
    flt = getFleet(fleet);
    month += ((float)seasn->getNumMonths() * flt->getSeasTiming()) + 1;
    return month;
}

void ss_model::set_num_seasons(int seasns)
{
    int i;
    Season *seasn;

    if (seasons.isEmpty())
    {
        i = 0;
    }
    else
    {
        i = seasons.count();
    }

    if (i < seasns)
    {
        for (i = seasons.count(); i < seasns; i++)
        {
            seasn = new Season();
            seasn->setNumMonths(0);
            seasn->setSpawning(false);
            seasons.append(seasn);
        }
    }
    else
    {
        while (seasons.count() > seasns)
        {
            seasn = seasons.takeLast();
            delete seasn;
        }
    }
    forecast->set_num_seasons (seasns);
    pPopulation->setNumSeas(seasns);
}

void ss_model::set_months_per_season(int seasn, float months)
{
    if (months > 0.0 && months < 12.1)
    {
        seasons.at(seasn-1)->setNumMonths(months);
    }
}

void ss_model::rescale_months_per_season()
{
    float sum = 0;
    float dur = 0;
    float months = 0;
    for (int i = 0; i < get_num_seasons(); i++)
        sum += get_months_per_season(i);
    if (sum > 0.0)
    {
        if (sum < 11.9)
            sum = 12;
        for (int i = 0; i < get_num_seasons(); i++)
        {
            months = get_months_per_season(i);
            dur = months / sum;
            seasons.at(i)->setDuration(dur);
            months = dur * 12.0;
            seasons.at(i)->setNumMonths(months);
        }
    }
}

int ss_model::get_num_subseasons () const
{
    int num = 2;
    if (seasons.count() > 0)
        num = seasons.at(0)->getNumSubSeasons();
    return num;
}

void ss_model::set_num_subseasons(int value)
{
    for (int i = 0; i < seasons.count(); i++)
        seasons.at(i)->setNumSubSeasons(value);
}

int ss_model::getTotalMonths()
{
    int total = 0;
    for (int i = 0; i < get_num_seasons(); i++)
    {
        total += seasons.at(i)->getNumMonths();
    }
    return total;
}

void ss_model::set_spawn_month(float month)
{
    fSpawnMonth = month;
    set_spawn_season(find_season(month));
}

float ss_model::get_spawn_month()
{
    return fSpawnMonth;
}

void ss_model::set_spawn_season(int seasn)
{
    if (seasn > 0 && seasn <= seasons.count())
    {
        for (int i = 0; i < seasons.count(); i++)
            seasons.at(i)->setSpawning(false);
        seasons.at(seasn - 1)->setSpawning(true);
    }
}

int ss_model::get_spawn_season()
{
    int seasn;
    for (seasn = 0; seasn < seasons.count(); seasn++)
    {
        if (seasons.at(seasn)->getSpawning())
            break;
    }
    return (seasn + 1);
}

int ss_model::find_season(float month)
{
    int itr = 0, seasn = 0;
//    int mn = (int)month;
    for (seasn = 0; seasn < seasons.count(); seasn++)
    {
        itr += seasons.at(seasn)->getNumMonths();
        if (itr >= month)
            break;
    }
    return (seasn + 1);
}

float ss_model::find_month(int fleet, int seas)
{
    int i;
    float month = 0;
    Fleet *flt = fleets.at(fleet);
    float time = flt->getSeasTiming();
    for (i = 0; i < (seas-1); i++)
        month += seasons.at(i)->getNumMonths();
    month += seasons.at(i)->getNumMonths() * time + 1.0;
    return month;
}


int ss_model::getTotalSeasons()
{
    return seasons.count();
}

void ss_model::addFleet(Fleet *flt)
{
    if (flt != NULL)
    {
        flt->setStartYear(get_start_year());
        flt->setTotalYears(getTotalYears());
        flt->setNumSeasons(getTotalSeasons());
        fleets.append(flt);
        assignFleetNumbers();
    }
    forecast->set_num_fleets(get_num_fleets());
}

Fleet * ss_model::getFleet(int index)
{
    Fleet * flt = NULL;
    if (index >= 0 && index < fleets.count())
    {
        flt = fleets.at(index);
    }
    else
    {
        flt = fleets.at(0);
    }
    return flt;
}

Fleet * ss_model::newFleet(QString name)
{
    Fleet *newfl = new Fleet(this);
//    newfl->Q()->setParentModel(this);
    if (name.isEmpty())
            name = QString("New_Fleet");
    newfl->setName(name);
    newfl->setType(Fleet::Fishing);
    newfl->setActive(true);
/*    fleets.append(newfl);
    newfl->setStartYear(get_start_year());
    newfl->setTotalYears(getTotalYears());
    newfl->setNumSeasons(getTotalSeasons());
    addFleet(newfl);
    assignFleetNumbers();
    forecast->set_num_fleets(get_num_fleets());*/
    return newfl;
}

void ss_model::deleteFleet(int index)
{
    Fleet * flt = NULL;
    if (index >= 0 && index < fleets.count())
    {
        flt = fleets.takeAt(index);
        delete flt;
        flt = NULL;
        assignFleetNumbers();
    }
    forecast->set_num_fleets(get_num_fleets());
}

Fleet * ss_model::duplicateFleet(Fleet *oldfl)
{
    Fleet *newfl = new Fleet(this);
    newfl->copy (oldfl);
    addFleet(newfl);
    assignFleetNumbers();
    forecast->set_num_fleets(get_num_fleets());
    return newfl;
}

void ss_model::set_num_fleets(int n_fleets)
{
    int index = n_fleets;
//    Fleet *fleet;

    if (fleets.isEmpty())
        fleets.append(new Fleet(this));

    while (fleets.count() > n_fleets)
    {
        index = n_fleets - 1;
        deleteFleet(index);
    }
    while (n_fleets > fleets.count())
    {
        addFleet(newFleet());
    }
    assignFleetNumbers();

    forecast->set_num_fleets(n_fleets);
}


int ss_model::get_num_fisheries()
{
    int i = 0, num = 0;
    for (; i < fleets.count(); i++)
        if (fleets.at(i)->getType() == Fleet::Fishing)
            num++;
    iNumFisheries = num;
    return num;
}

int ss_model::get_num_surveys()
{
    int i = 0, num = 0;
    for (; i < fleets.count(); i++)
        if (fleets.at(i)->getType() == Fleet::Survey)
            num++;
    iNumSurveys = num;
    return num;
}

int ss_model::getNumLinesCatch()
{
    int num = 0;
    for (int i = 0; i < get_num_fleets(); i++)
    {
        num += getFleet(i)->getNumCatchObs();
    }
    return num;
}

void ss_model::add_fleet_catch_per_season(int fleet, int yr, int season, double num, double se)
{
    getFleet(fleet)->add_catch_per_season(yr, season, num, se);
}
/*
double ss_model::fleet_catch_per_season(int fleet, int yr, int season)
{
    return getFleet(fleet)->catch_per_season(yr, season);
}*/

void ss_model::set_fleet_units_err_type(int fleet, int units, int err_type)
{
//    int i_fleet = fleet - 1;
    if (fleet < get_num_fleets())
    {
        getFleet(fleet)->setAbundUnits(units);
        getFleet(fleet)->setAbundErrType(err_type);
    }
}

int ss_model::fleet_units(int fleet)
{
    int units = 0;
    if (fleet < get_num_fleets())
    {
        units = getFleet(fleet)->getAbundUnits();
    }
    return units;
}

int ss_model::fleet_err_type(int fleet)
{
    int err = 0;
    if (fleet < get_num_fleets())
    {
        err = getFleet(fleet)->getAbundErrType();
    }
    return err;
}

void ss_model::set_fleet_abundance(int fleet, int year, int month, float obs, float err)
{
    getFleet(fleet)->set_abundance(year, month, obs);
    getFleet(fleet)->set_abundance_error(year, month, err);
}

float ss_model::fleet_abundance(int fleet, int year, int month)
{
    return getFleet(fleet)->abundance(year, month);
}

float ss_model::fleet_abund_err(int fleet, int year, int month)
{
    return getFleet(fleet)->abundance_error(year, month);
}

void ss_model::set_fleet_discard_units_err_type(int fleet, int units, int err_type)
{
    //int i_fleet = fleet - 1;
    fleets.at(fleet)->setDiscardUnits(units);
    fleets.at(fleet)->setDiscardErrType(err_type);
}

int ss_model::fleet_discard_units(int fleet)
{
    int units = 0, i_fleet = fleet - 1;
    if (i_fleet < get_num_fleets())
    {
        units = getFleet(fleet)->getDiscardUnits();
    }
    return units;
}

int ss_model::fleet_discard_err_type(int fleet)
{
    int err = 0, i_fleet = fleet - 1;
    if (i_fleet < get_num_fleets())
    {
        err = getFleet(fleet)->getDiscardErrType();
    }
    return err;
}

int ss_model::fleet_discard_count()
{
    int i = 0, count = 0;
    for (i = 0; i < get_num_fleets(); i++)
    {
        if (getFleet(i)->getDiscardCount() > 0)
            count++;
    }
    return count;
}

int ss_model::fleet_discard_obs_count()
{
    int i = 0, count = 0;
    for (i = 0; i < get_num_fleets(); i++)
    {
        count += getFleet(i)->getDiscardCount();
    }
    return count;
}

void ss_model::set_num_genders(int genders)
{
    int num = (genders == 1)? 1: 2;
    iNumGenders = genders;
    getPopulation()->set_gender(genders);
    for (int i = 0; i < get_num_fleets(); i++)
    {
        getFleet(i)->setNumGenders(num);
    }
    forecast->set_num_genders(num);
}

void ss_model::set_num_tag_groups(int num)
{
    tagData->setNumTagGroups(num);
    if (num == 0)
        doTags = false;
}

void ss_model::setNumBlockPatterns(int num)
{
    iNumBlockPatterns = num;
    if (blockPatterns.isEmpty())
    {
        for (int i = 0; i < num; i++)
        {
            BlockPattern *blk = new BlockPattern();
            blockPatterns.append(blk);
        }
    }
    else if (blockPatterns.count() < num)
    {
        for (int i = blockPatterns.count(); i < num; i++)
        {
            BlockPattern *blk = new BlockPattern();
            blockPatterns.append(blk);
        }
    }
    else
    {
        while (blockPatterns.count() > num)
        {
            BlockPattern *blk = blockPatterns.takeLast();
            delete blk;
        }
    }
}

void ss_model::setBlockPattern(int index, BlockPattern *bp)
{
    blockPatterns.takeAt(index);
    blockPatterns.insert(index, bp);
}

void ss_model::setTagLoss(int flag)
{
    tag_loss = flag;
    if (tag_loss == 1)
    {
        if (tag_loss_param == NULL)
            tag_loss_param =  new longParameter();
    }
    else
    {
        if (tag_loss_param != NULL)
            delete tag_loss_param;
        tag_loss_param = NULL;
    }
}

void ss_model::set_num_ages(int ages)
{
    iNumAges = ages;
    get_age_composition()->set_number_ages(ages);
    pPopulation->setNumAges(ages);
}

void ss_model::addEnvironVarObs(QStringList txtlst)
{
    int num = obsEnvironVars->getNumObs();
    obsEnvironVars->setNumObs(num + 1);
    setEnvironVarObs(num, txtlst);
}

void ss_model::setEnvironVarObs(int index, QStringList txtlst)
{
    obsEnvironVars->setObservation(index, txtlst);
}

void ss_model::setNumGeneralCompMethods(int num)
{
    if (cListGeneralMethods.count() != num)
    {
        if (cListGeneralMethods.isEmpty())
            cListGeneralMethods.append(new compositionGeneral());
        while (cListGeneralMethods.count() < num)
            cListGeneralMethods.append(new compositionGeneral());
        while (num < cListGeneralMethods.count())
        {
            compositionGeneral *cg = cListGeneralMethods.takeLast();
            deleteGeneralCompMethod(cg);
        }
    }
    for (int i = 0; i < fleets.count(); i++)
        getFleet(i)->setGenModelTotal(num);
}

void ss_model::copyGeneralCompMethod(compositionGeneral *method)
{
    compositionGeneral *method_new = new compositionGeneral(this);
    addGeneralCompMethod(method_new);
    method_new->setUnits(method->getUnits());
    method_new->setNumber(method->getNumber());
    method_new->setNumberBins(method->getNumberBins());
    method_new->setNumberAltBins(method->getNumberAltBins());
    method_new->setBins(method->getBins());
    method_new->setAltBins(method->getAltBins());
    method_new->setAltBinMax(method->getAltBinMax());
    method_new->setAltBinMethod(method->getAltBinMethod());
    method_new->setScale(method->getScale());

}

void ss_model::deleteGeneralCompMethod(compositionGeneral *method)
{
    delete method;
}

void ss_model::deleteGeneralCompMethod(int index)
{
    compositionGeneral *method = cListGeneralMethods.takeAt(index);
    deleteGeneralCompMethod(method);
}

void ss_model::setTagLossParameter(longParameter *lp)
{
    setTagLossParameter(lp->toText());
}

void ss_model::setTagLossParameter(QString text)
{
    tag_loss_param->fromText(text);
}

int ss_model::getLambdaNumChanges()
{
    int num = 0;
    for (int i = 0; i < get_num_fleets(); i++)
        num += getFleet(i)->getNumLambdas();
    return num;
}

int ss_model::getAddVariance() const
{
    return i_add_variance;
}

void ss_model::setAddVariance(int value)
{
    additionalSdReporting->setActive(value);
    if (add_var_setup.isEmpty())
    {
        add_var_setup.append(0);
        add_var_setup.clear();
    }
    if (value == 1)
    {
        int i;
        if (add_var_setup.isEmpty())
        {
            i = 0;
        }
        else
        {
            i = add_var_setup.count();
        }
        for (; i < 9; i++)
            add_var_setup.append(0);
        i_add_variance = 1;
    }
    else
    {
        if (!add_var_setup.isEmpty())
            add_var_setup.clear();
        if (!add_var_slx_bins.isEmpty())
            add_var_slx_bins.clear();
        if (!add_var_age_bins.isEmpty())
            add_var_age_bins.clear();
        if (!add_var_Naa_bins.isEmpty())
            add_var_Naa_bins.clear();
        i_add_variance = 0;
    }
}

void ss_model::setAddVarSetupFromText(QString txt)
{
    QStringList ql (txt.split(' ', QString::SkipEmptyParts));
    setAddVariance(1);
    setAddVarSetupFleet(ql[0].toInt());
    setAddVarSetupLenAge(ql[1].toInt());
    setAddVarSetupYear(ql[2].toInt());
    setAddVarSetupNSlxBins(ql[3].toInt());
    setAddVarSetupGPatt(ql[4].toInt());
    setAddVarSetupNGAges(ql[5].toInt());
    setAddVarSetupArNaa(ql[6].toInt());
    setAddVarSetupYrNaa(ql[7].toInt());
    setAddVarSetupNaaBins(ql[8].toInt());
}

void ss_model::setAddVarSetupNSlxBins(int val)
{
    if (!add_var_slx_bins.isEmpty())
        add_var_slx_bins.clear();
    for (int i = 0; i < val; i++)
    {
        add_var_slx_bins.append(0.0);
    }
    add_var_setup[3] = val;
}

void ss_model::setAddVarSetupNGAges(int val)
{
    if (!add_var_age_bins.isEmpty())
        add_var_age_bins.clear();
    for (int i = 0; i < val; i++)
    {
        add_var_age_bins.append(0.0);
    }
    add_var_setup[5] = val;
}

void ss_model::setAddVarSetupNaaBins(int val)
{
    if (!add_var_Naa_bins.isEmpty())
        add_var_Naa_bins.clear();
    for (int i = 0; i < val; i++)
    {
        add_var_Naa_bins.append(0.0);
    }
    add_var_setup[8] = val;
}

QString ss_model::getAddVarSetupToText()
{
    QString txt("");
    QStringList specs(additionalSdReporting->getSpecs());
    for (int i = 0; i < 9; i++)
    {
        txt.append(' ');
        txt.append(specs.at(i));
//        txt.append(QString::number(add_var_setup.at(i)));
    }
    return txt;
}

void ss_model::setAddVarSelexBins(int index, float val)
{
    if (index >= 0 && index < add_var_slx_bins.count())
        add_var_slx_bins[index] = val;
}

void ss_model::setAddVarGrwthBins(int index, float val)
{
    if (index >= 0 && index < add_var_age_bins.count())
        add_var_age_bins[index] = val;
}

void ss_model::setAddVarNumAaBins(int index, float val)
{
    if (index >= 0 && index < add_var_Naa_bins.count())
        add_var_Naa_bins[index] = val;
}

void ss_model::incrementYear()
{
}

int ss_model::checkyearvalue(QString str)
{
    bool ok;
    int value = str.toInt(&ok);
    if (!ok)
        value = get_start_year();
    return checkyearvalue(value);
}

int ss_model::checkyearvalue(int value)
{
    int val = value;
    int totNeg = get_start_year() - get_end_year();
    if (value == -999)
        ;
    else if (value <= totNeg)
        val = totNeg;
    else if (value > 0 && value < get_start_year())
        ;//val = get_start_year();
    else if (value > get_end_year())
        val = get_end_year();
    return val;
}

int ss_model::refyearvalue(int value)
{
    int year = value;
    if (value == -999)
        year = get_start_year();
    else if (value <= 0)
        year = get_end_year() + value;

    return year;
}
int ss_model::getCustomSelParmDevAdjust() const
{
    return customSelParmDevAdjust;
}

void ss_model::setCustomSelParmDevAdjust(int value)
{
    customSelParmDevAdjust = value;
}

QString ss_model::getDataFileName()
{
    MainWindow *mw = (MainWindow *)parent();
    return mw->getDataFile();
}

QString ss_model::getControlFileName()
{
    MainWindow *mw = (MainWindow *)parent();
    return mw->getControlFile();
}

bool ss_model::getReadWtAtAge() const
{
    return readWtAtAge;
}

void ss_model::setReadWtAtAge(bool value)
{
    readWtAtAge = value;
}

int ss_model::getSelexAdjustMethod() const
{
    return selexAdjustMethod;
}

void ss_model::setSelexAdjustMethod(int value)
{
    selexAdjustMethod = value;
}

int ss_model::getCustomSelParmDevPhase() const
{
    return customSelParmDevPhase;
}

void ss_model::setCustomSelParmDevPhase(int value)
{
    customSelParmDevPhase = value;
}

int ss_model::getCustomBlockSetup() const
{
    return customBlockSetup;
}

void ss_model::setCustomBlockSetup(int value)
{
    customBlockSetup = value;
}

int ss_model::getCustomEnviroLink() const
{
    return customEnviroLink;
}

void ss_model::setCustomEnviroLink(int value)
{
    customEnviroLink = value;
}


void ss_model::assignFleetNumbers()
{
    int num = 1;
    Fleet *flt;
    iNumFisheries = 0;
    iNumSurveys = 0;
    i_num_predators = 0;
    for (int i = 0; i < get_num_fleets(); i++)
    {
        flt = getFleet(i);
        if (flt->getType() == Fleet::Fishing)
        {
            iNumFisheries++;
            if (flt->isActive())
                flt->setNumber(num++);
        }
    }
    for (int i = 0; i < get_num_fleets(); i++)
    {
        flt = getFleet(i);
        if (flt->getType() == Fleet::Survey)
        {
            iNumSurveys++;
            if (flt->isActive())
                flt->setNumber(num++);
        }
    }
    for (int i = 0; i < get_num_fleets(); i++)
    {
        flt = getFleet(i);
        if (flt->getType() == Fleet::Predator)
        {
            i_num_predators++;
            if (flt->isActive())
                flt->setNumber(num++);
        }
    }
}


int ss_model::getNumActiveFleets()
{
    int num = 0;
    for (int i = 0; i < get_num_fleets(); i++)
    {
        if (getFleet(i)->isActive())
        {
            num += 1;
        }
    }
    return num;
}

Fleet *ss_model::getActiveFleet(int num)
{
    Fleet *flt = NULL;
    for (int i = 0; i < get_num_fleets(); i++)
    {
        flt = getFleet(i);
        if (flt->isActive())
        {
            if (flt->getNumber() == num)
            {
                break;
            }
        }
    }
    return flt;
}

void ss_model::set_num_areas(int n_areas)
{
    iNumAreas = n_areas;
    getForecast()->set_num_areas(n_areas);
    getPopulation()->Move()->setNumAreas(n_areas);
}

int checkintvalue(QString value)
{
    bool okay = true;
    int chk = value.toInt(&okay);
    while (!okay)
    {
        value.truncate(value.count() - 1);
        chk = value.toInt(&okay);
    }
    return chk;
}

float checkfloatvalue(QString value)
{
    bool okay = true;
    float chk = value.toFloat(&okay);
    while (!okay)
    {
        value.truncate(value.count() - 1);
        chk = value.toFloat(&okay);
    }
    return chk;
}

double checkdoublevalue(QString value)
{
    bool okay = true;
    double chk = value.toDouble(&okay);
    while (!okay)
    {
        value.truncate(value.count() - 1);
        chk = value.toDouble(&okay);
    }
    return chk;
}

bool floatEquals(float a, float b)
{
    bool equals = false;
    float aa = abs(a);
    float bb = abs(b);
    if ((a < 0 && b < 0) ||
            (a >= 0 && b >= 0))
        if ((aa > bb - .0000001) && (aa < bb + .0000001))
            equals = true;

    return equals;
}

bool ss_model::getReadSelectivityPriors()
{
    return readSelxPriors;
}

void ss_model::setReadSelectivityPriors(bool flag)
{
    readSelxPriors = flag;
}

