#include "dialogchosereport.h"
#include "ui_dialogchosereport.h"

DialogChoseReport::DialogChoseReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChoseReport)
{
    ui->setupUi(this);

    for (int i = 0; i <= 60; i++)
        reports.append(false);

    setMinimReps(true);

//    connect (ui->checkBox_min, SIGNAL(toggled(bool)), this, SLOT(changeMinimReps(bool)));
//    connect (ui->checkBox_all, SIGNAL(toggled(bool)), this, SLOT(changeAllReps(bool)));
//    connect (ui->checkBox_brief, SIGNAL(toggled(bool)), this, SLOT(changeBriefReps(bool)));
    connect (ui->radioButton_Minimal, SIGNAL(toggled(bool)), this, SLOT(changeMinimReps(bool)));
    connect (ui->radioButton_All, SIGNAL(toggled(bool)), this, SLOT(changeAllReps(bool)));
    connect (ui->radioButton_Brief, SIGNAL(toggled(bool)), this, SLOT(changeBriefReps(bool)));
//    connect (ui->radioButton_Custom, SIGNAL(toggled(bool)), this, SLOT(changeCustomReps(bool)));
    connect (ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(clearReports()));

    connectCheckBoxes();
}

DialogChoseReport::~DialogChoseReport()
{
    delete ui;
    reports.clear();
}

void DialogChoseReport::connectCheckBoxes()
{
    //    1Y  DEFINITIONS report:1
    connect (ui->checkBox_definitions, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    2Y  LIKELIHOOD report:2
    connect (ui->checkBox_likelihood, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    3Y  Input_Variance_Adjustment report:3
    connect (ui->checkBox_inputVarAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    4Y  Parm_devs_detail report:4
    connect (ui->checkBox_parmDevsDetail, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    5Y  PARAMETERS report:5
    connect (ui->checkBox_parameters, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    6Y  DERIVED_QUANTITIES report:6
    connect (ui->checkBox_derivedQuants, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    7Y  MGparm_By_Year_after_adjustments report:7
    connect (ui->checkBox_mgParmAfterAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    8Y  selparm(Size)_By_Year_after_adjustments report:8
    connect (ui->checkBox_sizeSelAfterAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    9Y  selparm(Age)_By_Year_after_adjustments report:9
    connect (ui->checkBox_ageSelAfterAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    10Y  RECRUITMENT_DIST report:10
    connect (ui->checkBox_recruitDist, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    11Y  MORPH_INDEXING report:11
    connect (ui->checkBox_morphIndex, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    12Y  SIZEFREQ_TRANSLATION report:12
    connect (ui->checkBox_sizeFreqTrans, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    13Y  MOVEMENT report:13
    connect (ui->checkBox_movement, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    14Y  EXPLOITATION report:14
    connect (ui->checkBox_exploit, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    15Y  CATCH report:15
    connect (ui->checkBox_catch, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    16Y  TIME_SERIES report:16
    connect (ui->checkBox_timeSeries, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    17Y  SPR_SERIES report:17
    connect (ui->checkBox_sprSeries, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    18Y  Kobe_Plot report:18
    connect (ui->checkBox_kobePlot, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    19Y  SPAWN_RECRUIT report:19
    connect (ui->checkBox_spawnRecruit, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    20Y  SPAWN_RECR_CURVE report:20
    connect (ui->checkBox_spawnRecrCurve, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    21Y  INDEX_1 report:21
    connect (ui->checkBox_index1, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    22Y  INDEX_2 report:22
    connect (ui->checkBox_index2, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    23Y  INDEX_3 report:23
    connect (ui->checkBox_index3, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    24Y  DISCARD_SPECIFICATION report:24
    connect (ui->checkBox_discardSpec, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    25Y  DISCARD_OUTPUT report:25
    connect (ui->checkBox_discardOut, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    26Y  MEAN_BODY_WT_OUTPUT report:26
    connect (ui->checkBox_meanBwtOut, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    27Y  FIT_LEN_COMPS report:27
    connect (ui->checkBox_fitLenComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    28Y  FIT_AGE_COMPS report:28
    connect (ui->checkBox_fitAgeComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    29Y  FIT_SIZE_COMPS report:29
    connect (ui->checkBox_fitSizeComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    30Y  OVERALL_COMPS report:30
    connect (ui->checkBox_overallComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    31Y  LEN_SELEX report:31
    connect (ui->checkBox_lenSelex, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    32Y  AGE_SELEX report:32
    connect (ui->checkBox_ageSelex, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    33Y  ENVIRONMENTAL_DATA report:33
    connect (ui->checkBox_envData, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    34Y  TAG_Recapture report:34
    connect (ui->checkBox_tagRecap, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    35Y  NUMBERS_AT_AGE report:35
    connect (ui->checkBox_numAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    36Y  BIOMASS_AT_AGE report:36
    connect (ui->checkBox_biomAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    37Y  NUMBERS_AT_LENGTH report:37
    connect (ui->checkBox_numAtLen, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    38Y  BIOMASS_AT_LENGTH report:38
    connect (ui->checkBox_biomAtLen, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    39Y  F_AT_AGE report:39
    connect (ui->checkBox_fAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    40Y  CATCH_AT_AGE report:40
    connect (ui->checkBox_catchAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    41Y  DISCARD_AT_AGE report:41
    connect (ui->checkBox_discardAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    42Y  BIOLOGY report:42
    connect (ui->checkBox_biology, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    43Y  Natural_Mortality report:43
    connect (ui->checkBox_natMort, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    44Y  AGE_SPECIFIC_K report:44
    connect (ui->checkBox_ageSpecK, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    45Y  Growth_Parameters report:45
    connect (ui->checkBox_growthParams, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    46Y  Seas_Effects report:46
    connect (ui->checkBox_seasEffects, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    47Y  Biology_at_age_in_endyr report:47
    connect (ui->checkBox_bioAtAgeEndyr, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    48Y  MEAN_BODY_WT(Begin) report:48
    connect (ui->checkBox_meanBwtBegin, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    49Y  MEAN_SIZE_TIMESERIES report:49
    connect (ui->checkBox_meanTimeSeries, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    50Y  AGE_LENGTH_KEY report:50
    connect (ui->checkBox_ageLenKey, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    51Y  AGE_AGE_KEY report:51
    connect (ui->checkBox_ageAgeKey, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    52Y  COMPOSITION_DATABASE report:52
    connect (ui->checkBox_compDbase, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    53Y  SELEX_database report:53
    connect (ui->checkBox_selexDbase, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    54Y  SPR/YPR_Profile report:54
    connect (ui->checkBox_sprYprProfile, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    55Y  GLOBAL_MSY report:55
    connect (ui->checkBox_globalMSY, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    56Y  SS_summary.sso report:56
    connect (ui->checkBox_summary, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    57Y  rebuilder.sso report:57
    connect (ui->checkBox_rebuilder, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    58Y  SIStable.sso report:58
    connect (ui->checkBox_sisTable, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    59Y  Dynamic_Bzero report:59
    connect (ui->checkBox_dynamicBzero, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    60Y  wtatage.ss_new report:60
    connect (ui->checkBox_wtatage, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
}

void DialogChoseReport::disconnectCheckBoxes()
{
    //    1Y  DEFINITIONS report:1
    disconnect (ui->checkBox_definitions, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    2Y  LIKELIHOOD report:2
    disconnect (ui->checkBox_likelihood, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    3Y  Input_Variance_Adjustment report:3
    disconnect (ui->checkBox_inputVarAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    4Y  Parm_devs_detail report:4
    disconnect (ui->checkBox_parmDevsDetail, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    5Y  PARAMETERS report:5
    disconnect (ui->checkBox_parameters, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    6Y  DERIVED_QUANTITIES report:6
    disconnect (ui->checkBox_derivedQuants, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    7Y  MGparm_By_Year_after_adjustments report:7
    disconnect (ui->checkBox_mgParmAfterAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    8Y  selparm(Size)_By_Year_after_adjustments report:8
    disconnect (ui->checkBox_sizeSelAfterAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    9Y  selparm(Age)_By_Year_after_adjustments report:9
    disconnect (ui->checkBox_ageSelAfterAdj, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    10Y  RECRUITMENT_DIST report:10
    disconnect (ui->checkBox_recruitDist, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    11Y  MORPH_INDEXING report:11
    disconnect (ui->checkBox_morphIndex, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    12Y  SIZEFREQ_TRANSLATION report:12
    disconnect (ui->checkBox_sizeFreqTrans, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    13Y  MOVEMENT report:13
    disconnect (ui->checkBox_movement, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    14Y  EXPLOITATION report:14
    disconnect (ui->checkBox_exploit, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    15Y  CATCH report:15
    disconnect (ui->checkBox_catch, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    16Y  TIME_SERIES report:16
    disconnect (ui->checkBox_timeSeries, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    17Y  SPR_SERIES report:17
    disconnect (ui->checkBox_sprSeries, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    18Y  Kobe_Plot report:18
    disconnect (ui->checkBox_kobePlot, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    19Y  SPAWN_RECRUIT report:19
    disconnect (ui->checkBox_spawnRecruit, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    20Y  SPAWN_RECR_CURVE report:20
    disconnect (ui->checkBox_spawnRecrCurve, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    21Y  INDEX_1 report:21
    disconnect (ui->checkBox_index1, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    22Y  INDEX_2 report:22
    disconnect (ui->checkBox_index2, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    23Y  INDEX_3 report:23
    disconnect (ui->checkBox_index3, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    24Y  DISCARD_SPECIFICATION report:24
    disconnect (ui->checkBox_discardSpec, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    25Y  DISCARD_OUTPUT report:25
    disconnect (ui->checkBox_discardOut, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    26Y  MEAN_BODY_WT_OUTPUT report:26
    disconnect (ui->checkBox_meanBwtOut, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    27Y  FIT_LEN_COMPS report:27
    disconnect (ui->checkBox_fitLenComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    28Y  FIT_AGE_COMPS report:28
    disconnect (ui->checkBox_fitAgeComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    29Y  FIT_SIZE_COMPS report:29
    disconnect (ui->checkBox_fitSizeComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    30Y  OVERALL_COMPS report:30
    disconnect (ui->checkBox_overallComps, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    31Y  LEN_SELEX report:31
    disconnect (ui->checkBox_lenSelex, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    32Y  AGE_SELEX report:32
    disconnect (ui->checkBox_ageSelex, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    33Y  ENVIRONMENTAL_DATA report:33
    disconnect (ui->checkBox_envData, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    34Y  TAG_Recapture report:34
    disconnect (ui->checkBox_tagRecap, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    35Y  NUMBERS_AT_AGE report:35
    disconnect (ui->checkBox_numAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    36Y  BIOMASS_AT_AGE report:36
    disconnect (ui->checkBox_biomAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    37Y  NUMBERS_AT_LENGTH report:37
    disconnect (ui->checkBox_numAtLen, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    38Y  BIOMASS_AT_LENGTH report:38
    disconnect (ui->checkBox_biomAtLen, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    39Y  F_AT_AGE report:39
    disconnect (ui->checkBox_fAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    40Y  CATCH_AT_AGE report:40
    disconnect (ui->checkBox_catchAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    41Y  DISCARD_AT_AGE report:41
    disconnect (ui->checkBox_discardAtAge, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    42Y  BIOLOGY report:42
    disconnect (ui->checkBox_biology, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    43Y  Natural_Mortality report:43
    disconnect (ui->checkBox_natMort, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    44Y  AGE_SPECIFIC_K report:44
    disconnect (ui->checkBox_ageSpecK, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    45Y  Growth_Parameters report:45
    disconnect (ui->checkBox_growthParams, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    46Y  Seas_Effects report:46
    disconnect (ui->checkBox_seasEffects, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    47Y  Biology_at_age_in_endyr report:47
    disconnect (ui->checkBox_bioAtAgeEndyr, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    48Y  MEAN_BODY_WT(Begin) report:48
    disconnect (ui->checkBox_meanBwtBegin, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    49Y  MEAN_SIZE_TIMESERIES report:49
    disconnect (ui->checkBox_meanTimeSeries, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    50Y  AGE_LENGTH_KEY report:50
    disconnect (ui->checkBox_ageLenKey, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    51Y  AGE_AGE_KEY report:51
    disconnect (ui->checkBox_ageAgeKey, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    52Y  COMPOSITION_DATABASE report:52
    disconnect (ui->checkBox_compDbase, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    53Y  SELEX_database report:53
    disconnect (ui->checkBox_selexDbase, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    54Y  SPR/YPR_Profile report:54
    disconnect (ui->checkBox_sprYprProfile, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    55Y  GLOBAL_MSY report:55
    disconnect (ui->checkBox_globalMSY, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    56Y  SS_summary.sso report:56
    disconnect (ui->checkBox_summary, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    57Y  rebuilder.sso report:57
    disconnect (ui->checkBox_rebuilder, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    58Y  SIStable.sso report:58
    disconnect (ui->checkBox_sisTable, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    59Y  Dynamic_Bzero report:59
    disconnect (ui->checkBox_dynamicBzero, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
    //    60Y  wtatage.ss_new report:60
    disconnect (ui->checkBox_wtatage, SIGNAL(toggled(bool)), this, SLOT(changeCheckbox(bool)));
}

void DialogChoseReport::close()
{
    hide();
}

void DialogChoseReport::clearReports()
{
    setReports(false);
    ui->radioButton_Custom->setChecked(true);
}

int DialogChoseReport::getRepDetail() const
{
    return repDetail;
/*    int value = 0;
    if (ui->checkBox_min->isChecked()) {
        value = 0;
    }
    else if (ui->checkBox_all->isChecked()) {
        value = 1;
    }
    else if (ui->checkBox_brief->isChecked()) {
        value = 2;
    }
    else {
        value = 3;
    }
    return value;*/
}

void DialogChoseReport::setRepDetail(int value)
{
    if (repDetail != value) {
        clearReports();
        repDetail = value;
        switch(value) {
        case 0:
            setMinimReps(true);
            break;
        case 1:
            setAllReps(true);
            break;
        case 2:
            setBriefReps(true);
            break;
        default:
            ui->radioButton_Custom->setChecked(true);
        }
    }
}

bool DialogChoseReport::getMinimReps() const
{
    bool value = ui->radioButton_Minimal->isChecked();
    return value;
}

void DialogChoseReport::setMinimReps(bool value)
{
    clearReports();
    changeMinimReps(value);
}

bool DialogChoseReport::getAllReps() const
{
    bool value = ui->radioButton_All->isChecked();
    return value;
}

void DialogChoseReport::setAllReps(bool value)
{
    clearReports();
    changeAllReps(value);
}

bool DialogChoseReport::getBriefReps() const
{
    bool value = ui->radioButton_Brief->isChecked();
    return value;
}

void DialogChoseReport::setBriefReps(bool value)
{
    clearReports();
    changeBriefReps(value);
}

bool DialogChoseReport::getCustmReps() const
{
    return ui->radioButton_Custom->isChecked();
}

void DialogChoseReport::setCustmReps(bool value)
{
    ui->radioButton_Custom->setChecked(value);
}

void DialogChoseReport::setCustomReport(int value)
{
    if (value == 100) {
        changeMinimReps(true);
    }
    else if (value == 101) {
        changeAllReps(true);
    }
    else if (value == 102) {
        changeBriefReps(true);
    }
    else {
        reports[value] = true;
        setCheckboxes();
    }
}

void DialogChoseReport::setReports(bool value)
{
    for (int i = 0; i < reports.count(); i++)
        reports[i] = value;
    setCheckboxes();
}

QList<bool> &DialogChoseReport::getReports()
{
//    reports[1] = ui->checkBox_definitions->isChecked();

    return reports;
}

void DialogChoseReport::changeMinimReps(bool value)
{
    if (minimReps != value) {
        minimReps = value;
        if (value) {
            // set minimum reports to true
            repDetail = 0;
            //1Y  DEFINITIONS report:1
            reports[1] = true;
            //2Y  LIKELIHOOD report:2
            reports[2] = true;
            //5Y  PARAMETERS report:5
            reports[5] = true;
            //6Y  DERIVED_QUANTITIES report:6
            reports[6] = true;
            //14Y  EXPLOITATION report:14
            reports[14] = true;
            //15Y  CATCH report:15
            reports[15] = true;
            //16Y  TIME_SERIES report:16
            reports[16] = true;

            setCheckboxes();
        }
    }
}

void DialogChoseReport::changeAllReps(bool value)
{
    if (allReps != value) {
        allReps = value;
        if (value) {
            // set all reports to true
            setReports(true);
        }
    }
}

void DialogChoseReport::changeBriefReps(bool value)
{
    if (briefReps != value) {
        briefReps = value;
        if (value) {
            // set brief reports to true
            //1Y  DEFINITIONS report:1
            reports[1] = true;
            //2Y  LIKELIHOOD report:2
            reports[2] = true;
            //3Y  Input_Variance_Adjustment report:3
            reports[3] = true;
            //4Y  Parm_devs_detail report:4
            reports[4] = true;
            //5Y  PARAMETERS report:5
            reports[5] = true;
            //6Y  DERIVED_QUANTITIES report:6
            reports[6] = true;
            //9Y  selparm(Age)_By_Year_after_adjustments report:9
            reports[9] = true;
            //10Y  RECRUITMENT_DIST report:10
            reports[10] = true;
            //13Y  MOVEMENT report:13
            reports[13] = true;
            //14Y  EXPLOITATION report:14
            reports[14] = true;
            //15Y  CATCH report:15
            reports[15] = true;
            //16Y  TIME_SERIES report:16
            reports[16] = true;
            //17N  SPR_SERIES report:17
            //18N  Kobe_Plot report:18
            //19Y  SPAWN_RECRUIT report:19
            reports[19] = true;
            //20Y  SPAWN_RECR_CURVE report:20
            reports[20] = true;
            //21Y  INDEX_1 report:21
            reports[21] = true;
            //22Y  INDEX_2 report:22
            reports[22] = true;
            //23Y  INDEX_3 report:23
            reports[23] = true;
            //24N  DISCARD_SPECIFICATION report:24
            //25N  DISCARD_OUTPUT report:25
            //26N  MEAN_BODY_WT_OUTPUT report:26
            //27N  FIT_LEN_COMPS report:27
            //28Y  FIT_AGE_COMPS report:28
            reports[28] = true;
            //29N  FIT_SIZE_COMPS report:29
            //30Y  OVERALL_COMPS report:30
            reports[30] = true;
            //31N  LEN_SELEX report:31
            //32Y  AGE_SELEX report:32
            reports[32] = true;
            //33N  ENVIRONMENTAL_DATA report:33
            //34N  TAG_Recapture report:34
            //35Y  NUMBERS_AT_AGE report:35
            reports[35] = true;
            //36Y  BIOMASS_AT_AGE report:36
            reports[36] = true;
            //37N  NUMBERS_AT_LENGTH report:37
            //38N  BIOMASS_AT_LENGTH report:38
            //39Y  F_AT_AGE report:39
            reports[39] = true;
            //40Y  CATCH_AT_AGE report:40
            reports[40] = true;
            //41Y  DISCARD_AT_AGE report:41
            reports[41] = true;
            //42Y  BIOLOGY report:42
            reports[42] = true;
            //43Y  Natural_Mortality report:43
            reports[43] = true;
            //44N  AGE_SPECIFIC_K report:44
            //45N  Growth_Parameters report:45
            //46N  Seas_Effects report:46
            //47N  Biology_at_age_in_endyr report:47
            //48N  MEAN_BODY_WT(Begin) report:48
            //49N  MEAN_SIZE_TIMESERIES report:49
            //50N  AGE_LENGTH_KEY report:50
            //51Y  AGE_AGE_KEY report:51
            reports[51] = true;
            //52Y  COMPOSITION_DATABASE report:52
            reports[52] = true;
            //53N  SELEX_database report:53
            //54Y  SPR/YPR_Profile report:54
            reports[54] = true;
            //55Y  GLOBAL_MSY report:55
            reports[55] = true;
            //56Y  SS_summary.sso report:56
            reports[56] = true;
            //57N  rebuilder.sso report:57
            //58N  SIStable.sso report:58
            //59Y  Dynamic_Bzero report:59
            reports[59] = true;
            //60Y  wtatage.ss_new report:60
            reports[60] = true;

            setCheckboxes();
        }
    }
}


void DialogChoseReport::setCheckboxes()
{
    disconnectCheckBoxes();
    //    1Y  DEFINITIONS report:1
    ui->checkBox_definitions->setChecked(reports.at(1));
    //    2Y  LIKELIHOOD report:2
    ui->checkBox_likelihood->setChecked(reports.at(2));
    //    3Y  Input_Variance_Adjustment report:3
    ui->checkBox_inputVarAdj->setChecked(reports.at(3));
    //    4Y  Parm_devs_detail report:4
    ui->checkBox_parmDevsDetail->setChecked(reports.at(4));
    //    5Y  PARAMETERS report:5
    ui->checkBox_parameters->setChecked(reports.at(5));
    //    6Y  DERIVED_QUANTITIES report:6
    ui->checkBox_derivedQuants->setChecked(reports.at(6));
    //    7Y  MGparm_By_Year_after_adjustments report:7
    ui->checkBox_mgParmAfterAdj->setChecked(reports.at(7));
    //    8Y  selparm(Size)_By_Year_after_adjustments report:8
    ui->checkBox_sizeSelAfterAdj->setChecked(reports.at(8));
    //    9Y  selparm(Age)_By_Year_after_adjustments report:9
    ui->checkBox_ageSelAfterAdj->setChecked(reports.at(9));
    //    10Y  RECRUITMENT_DIST report:10
    ui->checkBox_recruitDist->setChecked(reports.at(10));
    //    11Y  MORPH_INDEXING report:11
    ui->checkBox_morphIndex->setChecked(reports.at(11));
    //    12Y  SIZEFREQ_TRANSLATION report:12
    ui->checkBox_sizeFreqTrans->setChecked(reports.at(12));
    //    13Y  MOVEMENT report:13
    ui->checkBox_movement->setChecked(reports.at(13));
    //    14Y  EXPLOITATION report:14
    ui->checkBox_exploit->setChecked(reports.at(14));
    //    15Y  CATCH report:15
    ui->checkBox_catch->setChecked(reports.at(15));
    //    16Y  TIME_SERIES report:16
    ui->checkBox_timeSeries->setChecked(reports.at(16));
    //    17Y  SPR_SERIES report:17
    ui->checkBox_sprSeries->setChecked(reports.at(17));
    //    18Y  Kobe_Plot report:18
    ui->checkBox_kobePlot->setChecked(reports.at(18));
    //    19Y  SPAWN_RECRUIT report:19
    ui->checkBox_spawnRecruit->setChecked(reports.at(19));
    //    20Y  SPAWN_RECR_CURVE report:20
    ui->checkBox_spawnRecrCurve->setChecked(reports.at(20));
    //    21Y  INDEX_1 report:21
    ui->checkBox_index1->setChecked(reports.at(21));
    //    22Y  INDEX_2 report:22
    ui->checkBox_index2->setChecked(reports.at(22));
    //    23Y  INDEX_3 report:23
    ui->checkBox_index3->setChecked(reports.at(23));
    //    24Y  DISCARD_SPECIFICATION report:24
    ui->checkBox_discardSpec->setChecked(reports.at(24));
    //    25Y  DISCARD_OUTPUT report:25
    ui->checkBox_discardOut->setChecked(reports.at(25));
    //    26Y  MEAN_BODY_WT_OUTPUT report:26
    ui->checkBox_meanBwtOut->setChecked(reports.at(26));
    //    27Y  FIT_LEN_COMPS report:27
    ui->checkBox_fitLenComps->setChecked(reports.at(27));
    //    28Y  FIT_AGE_COMPS report:28
    ui->checkBox_fitAgeComps->setChecked(reports.at(28));
    //    29Y  FIT_SIZE_COMPS report:29
    ui->checkBox_fitSizeComps->setChecked(reports.at(29));
    //    30Y  OVERALL_COMPS report:30
    ui->checkBox_overallComps->setChecked(reports.at(30));
    //    31Y  LEN_SELEX report:31
    ui->checkBox_lenSelex->setChecked(reports.at(31));
    //    32Y  AGE_SELEX report:32
    ui->checkBox_ageSelex->setChecked(reports.at(32));
    //    33Y  ENVIRONMENTAL_DATA report:33
    ui->checkBox_envData->setChecked(reports.at(33));
    //    34Y  TAG_Recapture report:34
    ui->checkBox_tagRecap->setChecked(reports.at(34));
    //    35Y  NUMBERS_AT_AGE report:35
    ui->checkBox_numAtAge->setChecked(reports.at(35));
    //    36Y  BIOMASS_AT_AGE report:36
    ui->checkBox_biomAtAge->setChecked(reports.at(36));
    //    37Y  NUMBERS_AT_LENGTH report:37
    ui->checkBox_numAtLen->setChecked(reports.at(37));
    //    38Y  BIOMASS_AT_LENGTH report:38
    ui->checkBox_biomAtLen->setChecked(reports.at(38));
    //    39Y  F_AT_AGE report:39
    ui->checkBox_fAtAge->setChecked(reports.at(39));
    //    40Y  CATCH_AT_AGE report:40
    ui->checkBox_catchAtAge->setChecked(reports.at(40));
    //    41Y  DISCARD_AT_AGE report:41
    ui->checkBox_discardAtAge->setChecked(reports.at(41));
    //    42Y  BIOLOGY report:42
    ui->checkBox_biology->setChecked(reports.at(42));
    //    43Y  Natural_Mortality report:43
    ui->checkBox_natMort->setChecked(reports.at(43));
    //    44Y  AGE_SPECIFIC_K report:44
    ui->checkBox_ageSpecK->setChecked(reports.at(44));
    //    45Y  Growth_Parameters report:45
    ui->checkBox_growthParams->setChecked(reports.at(45));
    //    46Y  Seas_Effects report:46
    ui->checkBox_seasEffects->setChecked(reports.at(46));
    //    47Y  Biology_at_age_in_endyr report:47
    ui->checkBox_bioAtAgeEndyr->setChecked(reports.at(47));
    //    48Y  MEAN_BODY_WT(Begin) report:48
    ui->checkBox_meanBwtBegin->setChecked(reports.at(48));
    //    49Y  MEAN_SIZE_TIMESERIES report:49
    ui->checkBox_meanTimeSeries->setChecked(reports.at(49));
    //    50Y  AGE_LENGTH_KEY report:50
    ui->checkBox_ageLenKey->setChecked(reports.at(50));
    //    51Y  AGE_AGE_KEY report:51
    ui->checkBox_ageAgeKey->setChecked(reports.at(51));
    //    52Y  COMPOSITION_DATABASE report:52
    ui->checkBox_compDbase->setChecked(reports.at(52));
    //    53Y  SELEX_database report:53
    ui->checkBox_selexDbase->setChecked(reports.at(53));
    //    54Y  SPR/YPR_Profile report:54
    ui->checkBox_sprYprProfile->setChecked(reports.at(54));
    //    55Y  GLOBAL_MSY report:55
    ui->checkBox_globalMSY->setChecked(reports.at(55));
    //    56Y  SS_summary.sso report:56
    ui->checkBox_summary->setChecked(reports.at(56));
    //    57Y  rebuilder.sso report:57
    ui->checkBox_rebuilder->setChecked(reports.at(57));
    //    58Y  SIStable.sso report:58
    ui->checkBox_sisTable->setChecked(reports.at(58));
    //    59Y  Dynamic_Bzero report:59
    ui->checkBox_dynamicBzero->setChecked(reports.at(59));
    //    60Y  wtatage.ss_new report:60
    ui->checkBox_wtatage->setChecked(reports.at(60));
    setRadioButtons();
    connectCheckBoxes();
}

void DialogChoseReport::changeCheckbox(bool value)
{
    reports[0] = value;
    //    1Y  DEFINITIONS report:1
    reports[1] = ui->checkBox_definitions->isChecked();
    //    2Y  LIKELIHOOD report:2
    reports[2] = ui->checkBox_likelihood->isChecked();
    //    3Y  Input_Variance_Adjustment report:3
    reports[3] = ui->checkBox_inputVarAdj->isChecked();
    //    4Y  Parm_devs_detail report:4
    reports[4] = ui->checkBox_parmDevsDetail->isChecked();
    //    5Y  PARAMETERS report:5
    reports[5] = ui->checkBox_parameters->isChecked();
    //    6Y  DERIVED_QUANTITIES report:6
    reports[6] = ui->checkBox_derivedQuants->isChecked();
    //    7Y  MGparm_By_Year_after_adjustments report:7
    reports[7] = ui->checkBox_mgParmAfterAdj->isChecked();
    //    8Y  selparm(Size)_By_Year_after_adjustments report:8
    reports[8] = ui->checkBox_sizeSelAfterAdj->isChecked();
    //    9Y  selparm(Age)_By_Year_after_adjustments report:9
    reports[9] = ui->checkBox_ageSelAfterAdj->isChecked();
    //    10Y  RECRUITMENT_DIST report:10
    reports[10] = ui->checkBox_recruitDist->isChecked();
    //    11Y  MORPH_INDEXING report:11
    reports[11] = ui->checkBox_morphIndex->isChecked();
    //    12Y  SIZEFREQ_TRANSLATION report:12
    reports[12] = ui->checkBox_sizeFreqTrans->isChecked();
    //    13Y  MOVEMENT report:13
    reports[13] = ui->checkBox_movement->isChecked();
    //    14Y  EXPLOITATION report:14
    reports[14] = ui->checkBox_exploit->isChecked();
    //    15Y  CATCH report:15
    reports[15] = ui->checkBox_catch->isChecked();
    //    16Y  TIME_SERIES report:16
    reports[16] = ui->checkBox_timeSeries->isChecked();
    //    17Y  SPR_SERIES report:17
    reports[17] = ui->checkBox_sprSeries->isChecked();
    //    18Y  Kobe_Plot report:18
    reports[18] = ui->checkBox_kobePlot->isChecked();
    //    19Y  SPAWN_RECRUIT report:19
    reports[19] = ui->checkBox_spawnRecruit->isChecked();
    //    20Y  SPAWN_RECR_CURVE report:20
    reports[20] = ui->checkBox_spawnRecrCurve->isChecked();
    //    21Y  INDEX_1 report:21
    reports[21] = ui->checkBox_index1->isChecked();
    //    22Y  INDEX_2 report:22
    reports[22] = ui->checkBox_index2->isChecked();
    //    23Y  INDEX_3 report:23
    reports[23] = ui->checkBox_index3->isChecked();
    //    24Y  DISCARD_SPECIFICATION report:24
    reports[24] = ui->checkBox_discardSpec->isChecked();
    //    25Y  DISCARD_OUTPUT report:25
    reports[25] = ui->checkBox_discardOut->isChecked();
    //    26Y  MEAN_BODY_WT_OUTPUT report:26
    reports[26] = ui->checkBox_meanBwtOut->isChecked();
    //    27Y  FIT_LEN_COMPS report:27
    reports[27] = ui->checkBox_fitLenComps->isChecked();
    //    28Y  FIT_AGE_COMPS report:28
    reports[28] = ui->checkBox_fitAgeComps->isChecked();
    //    29Y  FIT_SIZE_COMPS report:29
    reports[29] = ui->checkBox_fitSizeComps->isChecked();
    //    30Y  OVERALL_COMPS report:30
    reports[30] = ui->checkBox_overallComps->isChecked();
    //    31Y  LEN_SELEX report:31
    reports[31] = ui->checkBox_lenSelex->isChecked();
    //    32Y  AGE_SELEX report:32
    reports[32] = ui->checkBox_ageSelex->isChecked();
    //    33Y  ENVIRONMENTAL_DATA report:33
    reports[33] = ui->checkBox_envData->isChecked();
    //    34Y  TAG_Recapture report:34
    reports[34] = ui->checkBox_tagRecap->isChecked();
    //    35Y  NUMBERS_AT_AGE report:35
    reports[35] = ui->checkBox_numAtAge->isChecked();
    //    36Y  BIOMASS_AT_AGE report:36
    reports[36] = ui->checkBox_biomAtAge->isChecked();
    //    37Y  NUMBERS_AT_LENGTH report:37
    reports[37] = ui->checkBox_numAtLen->isChecked();
    //    38Y  BIOMASS_AT_LENGTH report:38
    reports[38] = ui->checkBox_biomAtLen->isChecked();
    //    39Y  F_AT_AGE report:39
    reports[39] = ui->checkBox_fAtAge->isChecked();
    //    40Y  CATCH_AT_AGE report:40
    reports[40] = ui->checkBox_catchAtAge->isChecked();
    //    41Y  DISCARD_AT_AGE report:41
    reports[41] = ui->checkBox_discardAtAge->isChecked();
    //    42Y  BIOLOGY report:42
    reports[42] = ui->checkBox_biology->isChecked();
    //    43Y  Natural_Mortality report:43
    reports[43] = ui->checkBox_natMort->isChecked();
    //    44Y  AGE_SPECIFIC_K report:44
    reports[44] = ui->checkBox_ageSpecK->isChecked();
    //    45Y  Growth_Parameters report:45
    reports[45] = ui->checkBox_growthParams->isChecked();
    //    46Y  Seas_Effects report:46
    reports[46] = ui->checkBox_seasEffects->isChecked();
    //    47Y  Biology_at_age_in_endyr report:47
    reports[47] = ui->checkBox_bioAtAgeEndyr->isChecked();
    //    48Y  MEAN_BODY_WT(Begin) report:48
    reports[48] = ui->checkBox_meanBwtBegin->isChecked();
    //    49Y  MEAN_SIZE_TIMESERIES report:49
    reports[49] = ui->checkBox_meanTimeSeries->isChecked();
    //    50Y  AGE_LENGTH_KEY report:50
    reports[50] = ui->checkBox_ageLenKey->isChecked();
    //    51Y  AGE_AGE_KEY report:51
    reports[51] = ui->checkBox_ageAgeKey->isChecked();
    //    52Y  COMPOSITION_DATABASE report:52
    reports[52] = ui->checkBox_compDbase->isChecked();
    //    53Y  SELEX_database report:53
    reports[53] = ui->checkBox_selexDbase->isChecked();
    //    54Y  SPR/YPR_Profile report:54
    reports[54] = ui->checkBox_sprYprProfile->isChecked();
    //    55Y  GLOBAL_MSY report:55
    reports[55] = ui->checkBox_globalMSY->isChecked();
    //    56Y  SS_summary.sso report:56
    reports[56] = ui->checkBox_summary->isChecked();
    //    57Y  rebuilder.sso report:57
    reports[57] = ui->checkBox_rebuilder->isChecked();
    //    58Y  SIStable.sso report:58
    reports[58] = ui->checkBox_sisTable->isChecked();
    //    59Y  Dynamic_Bzero report:59
    reports[59] = ui->checkBox_dynamicBzero->isChecked();
    //    60Y  wtatage.ss_new report:60
    reports[60] = ui->checkBox_wtatage->isChecked();

    setRadioButtons();
}

bool DialogChoseReport::isMinim() const
{
    bool value = true;

    // check if minimum reports
    //1Y  DEFINITIONS report:1
    if (reports[1] != true)
        value = false;
    //2Y  LIKELIHOOD report:2
    else if (reports[2] != true)
        value = false;
    //3N  Input_Variance_Adjustment report:3
    else if (reports.at(3))
        value = false;
    //4N  Parm_devs_detail report:4
    else if (reports.at(4))
        value = false;
    //5Y  PARAMETERS report:5
    else if (reports[5] != true)
        value = false;
    //6Y  DERIVED_QUANTITIES report:6
    else if (reports[6] != true)
        value = false;
    //7N  MGparm_By_Year_after_adjustments report:7
    else if (reports.at(7))
        value = false;
    //8N  selparm(Size)_By_Year_after_adjustments report:8
    else if (reports.at(8))
        value = false;
    //9N  selparm(Age)_By_Year_after_adjustments report:9
    else if (reports.at(9))
        value = false;
    //10N  RECRUITMENT_DIST report:10
    else if (reports.at(10))
        value = false;
    //11N  MORPH_INDEXING report:11
    else if (reports.at(11))
        value = false;
    //12N  SIZEFREQ_TRANSLATION report:12
    else if (reports.at(12))
        value = false;
    //13N  MOVEMENT report:13
    else if (reports.at(13))
        value = false;
    //14Y  EXPLOITATION report:14
    else if (reports[14] != true)
        value = false;
    //15Y  CATCH report:15
    else if (reports[15] != true)
        value = false;
    //16Y  TIME_SERIES report:16
    else if (reports[16] != true)
        value = false;
    // all the rest are false
    for (int i = 17; i < reports.count(); i++) {
        if (reports.at(i)) {
            value = false;
            break;
        }
    }
    return value;
}

bool DialogChoseReport::isAll() const
{
    bool value = true;
    for (int i = 1; i < reports.count(); i++) {
        if (!reports.at(i)) {
            value = false;
            break;
        }
    }
    return value;
}

bool DialogChoseReport::isBrief() const
{
    bool value = true;

    // check if brief reports
    //1Y  DEFINITIONS report:1
    if (!reports[1])
        value = false;
    //2Y  LIKELIHOOD report:2
    else if (!reports[2])
        value = false;
    //3Y  Input_Variance_Adjustment report:3
    else if (!reports[3])
        value = false;
    //4Y  Parm_devs_detail report:4
    else if (!reports[4])
        value = false;
    //5Y  PARAMETERS report:5
    else if (!reports[5])
        value = false;
    //6Y  DERIVED_QUANTITIES report:6
    else if (!reports[6])
        value = false;
    //7N  MGparm_By_Year_after_adjustments report:7
    else if (reports.at(7))
        value = false;
    //8N  selparm(Size)_By_Year_after_adjustments report:8
    else if (reports.at(8))
        value = false;
    //9Y  selparm(Age)_By_Year_after_adjustments report:9
    else if (!reports[9])
        value = false;
    //10Y  RECRUITMENT_DIST report:10
    else if (!reports[10])
        value = false;
    //11N  MORPH_INDEXING report:11
    else if (reports.at(11))
        value = false;
    //12N  SIZEFREQ_TRANSLATION report:12
    else if (reports.at(12))
        value = false;
    //13Y  MOVEMENT report:13
    else if (!reports[13])
        value = false;
    //14Y  EXPLOITATION report:14
    else if (!reports[14])
        value = false;
    //15Y  CATCH report:15
    else if (!reports[15])
        value = false;
    //16Y  TIME_SERIES report:16
    else if (!reports[16])
        value = false;
    //17N  SPR_SERIES report:17
    else if (reports.at(17))
        value = false;
    //18N  Kobe_Plot report:18
    else if (reports.at(18))
        value = false;
    //19Y  SPAWN_RECRUIT report:19
    else if (!reports[19])
        value = false;
    //20Y  SPAWN_RECR_CURVE report:20
    else if (!reports[20])
        value = false;
    //21Y  INDEX_1 report:21
    else if (!reports[21])
        value = false;
    //22Y  INDEX_2 report:22
    else if (!reports[22])
        value = false;
    //23Y  INDEX_3 report:23
    else if (!reports[23])
        value = false;
    //24N  DISCARD_SPECIFICATION report:24
    else if (reports.at(24))
        value = false;
    //25N  DISCARD_OUTPUT report:25
    else if (reports.at(25))
        value = false;
    //26N  MEAN_BODY_WT_OUTPUT report:26
    else if (reports.at(26))
        value = false;
    //27N  FIT_LEN_COMPS report:27
    else if (reports.at(27))
        value = false;
    //28Y  FIT_AGE_COMPS report:28
    else if (!reports[28])
        value = false;
    //29N  FIT_SIZE_COMPS report:29
    else if (reports.at(29))
        value = false;
    //30Y  OVERALL_COMPS report:30
    else if (!reports[30])
        value = false;
    //31N  LEN_SELEX report:31
    else if (reports.at(31))
        value = false;
    //32Y  AGE_SELEX report:32
    else if (!reports[32])
        value = false;
    //33N  ENVIRONMENTAL_DATA report:33
    else if (reports.at(33))
        value = false;
    //34N  TAG_Recapture report:34
    else if (reports.at(34))
        value = false;
    //35Y  NUMBERS_AT_AGE report:35
    else if (!reports[35])
        value = false;
    //36Y  BIOMASS_AT_AGE report:36
    else if (!reports[36])
        value = false;
    //37N  NUMBERS_AT_LENGTH report:37
    else if (reports.at(37))
        value = false;
    //38N  BIOMASS_AT_LENGTH report:38
    else if (reports.at(38))
        value = false;
    //39Y  F_AT_AGE report:39
    else if (!reports[39])
        value = false;
    //40Y  CATCH_AT_AGE report:40
    else if (!reports[40])
        value = false;
    //41Y  DISCARD_AT_AGE report:41
    else if (!reports[41])
        value = false;
    //42Y  BIOLOGY report:42
    else if (!reports[42])
        value = false;
    //43Y  Natural_Mortality report:43
    else if (!reports[43] )
        value = false;
    //44N  AGE_SPECIFIC_K report:44
    else if (reports.at(44))
        value = false;
    //45N  Growth_Parameters report:45
    else if (reports.at(45))
        value = false;
    //46N  Seas_Effects report:46
    else if (reports.at(46))
        value = false;
    //47N  Biology_at_age_in_endyr report:47
    else if (reports.at(47))
        value = false;
    //48N  MEAN_BODY_WT(Begin) report:48
    else if (reports.at(48))
        value = false;
    //49N  MEAN_SIZE_TIMESERIES report:49
    else if (reports.at(49))
        value = false;
    //50N  AGE_LENGTH_KEY report:50
    else if (reports.at(50))
        value = false;
    //51Y  AGE_AGE_KEY report:51
    else if (!reports[51])
        value = false;
    //52Y  COMPOSITION_DATABASE report:52
    else if (!reports[52])
        value = false;
    //53N  SELEX_database report:53
    else if (reports.at(53))
        value = false;
    //54Y  SPR/YPR_Profile report:54
    else if (!reports[54])
        value = false;
    //55Y  GLOBAL_MSY report:55
    else if (!reports[55])
        value = false;
    //56Y  SS_summary.sso report:56
    else if (!reports[56])
        value = false;
    //57N  rebuilder.sso report:57
    else if (reports.at(57))
        value = false;
    //58N  SIStable.sso report:58
    else if (reports.at(58))
        value = false;
    //59Y  Dynamic_Bzero report:59
    else if (!reports[59])
        value = false;
    //60Y  wtatage.ss_new report:60
    else if (!reports[60])
        value = false;

    return value;
}

void DialogChoseReport::setRadioButtons()
{
    if (isMinim()) {
        minimReps = true;
        allReps = false;
        briefReps = false;
        custmReps = false;
        repDetail = 0;
        ui->radioButton_Minimal->setChecked(true);
    }
    else if (isAll()) {
        minimReps = false;
        allReps = true;
        briefReps = false;
        custmReps = false;
        repDetail = 1;
        ui->radioButton_All->setChecked(true);
    }
    else if (isBrief()) {
        minimReps = false;
        allReps = false;
        briefReps = true;
        custmReps = false;
        repDetail = 2;
        ui->radioButton_Brief->setChecked(true);
    }
    else {
        minimReps = false;
        allReps = false;
        briefReps = false;
        custmReps = true;
        repDetail = 3;
        ui->radioButton_Custom->setChecked(true);
    }
    emit detailChanged(repDetail);
}

