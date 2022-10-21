#include "data_widget.h"
#include "ui_data_widget.h"

data_widget::data_widget(ss_model *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::data_widget)
{
    ui->setupUi(this);

    model_data = model;

    sdYearsView = new tableview();
    sdYearsDelegate = new spinBoxDelegate(this);
    sdYearsDelegate->setRange(1900, 3000);
    sdYearsView->setItemDelegate(sdYearsDelegate);
    sdYearsView->showRow(1);
    sdYearsView->setAcceptDrops(true);
    sdYearsView->setModel(model_data->sdYearsModel);
    sdYearsView->setHeight(1);
    ui->horizontalLayout_sdYears->addWidget(sdYearsView);

/*    mbweightview = new tableview();
    mbweightview->setParent(this);
    mbweightedit = new mbweightdelegate(this);
    mbweightview->setItemDelegate(mbweightedit);
    mbweightview->setModel(model_data->getMeanBwtModel());
    mbweightview->setHeight(model_data->getMeanBwtModel());
    ui->horiz  verticalLayout_mbwt->addWidget(mbweightview);*/

    lengthBins = new tableview();
    lengthBins->setParent(this);
    lBinsDelegate = new spinBoxDelegate(this);
    lBinsDelegate->setRange(0, 200);
    lengthBins->setItemDelegate(lBinsDelegate);
    lengthBins->setAcceptDrops(true);
    lengthBins->showRow(1);
    lengthBins->setModel(model_data->get_length_composition()->getBinsModel());
    lengthBins->setHeight(1);
    ui->horizontalLayout_length_bins->addWidget(lengthBins);
    lengthDirichlet = new tableview();
    lengthDirichlet->setParent(this);
    lengthDirichlet->setModel(model_data->get_length_composition()->getDirichletParamTable());
    ui->verticalLayout_length_dirichlet_parms->addWidget(lengthDirichlet);

    ageBins = new tableview();
    ageBins->setParent(this);
    aBinsDelegate = new spinBoxDelegate(this);
    aBinsDelegate->setRange(0, 50);
    ageBins->setItemDelegate(aBinsDelegate);
    ageBins->setAcceptDrops(true);
    ageBins->showRow(1);
    ageBins->setModel(model_data->get_age_composition()->getBinsModel());
    ageBins->setHeight(1);
    ui->horizontalLayout_age_bins->addWidget(ageBins);
    ageDirichlet = new tableview();
    ageDirichlet->setParent(this);
    ageDirichlet->setModel(model_data->get_age_composition()->getDirichletParamTable());
    ui->verticalLayout_age_dirichlet_parms->addWidget(ageDirichlet);
    ageError = new tableview();
    ageError->setParent(this);
    ageError->setModel(model_data->get_age_composition()->getErrorModel());
    ageError->setHeight(model_data->get_age_composition()->getErrorModel());
    ui->verticalLayout_age_err_matrices->addWidget(ageError);
    ageKeyParams = new tableview();
    ageKeyParams->setParent(this);
    ageKeyParams->setModel(model_data->get_age_composition()->getErrorParameters());
    ageKeyParams->setHeight(model_data->get_age_composition()->getErrorParameters());
    ui->verticalLayout_age_err_params->addWidget(ageKeyParams);
    ageKeyTVParams = new tableview();
    ageKeyTVParams->setParent(this);
    ageKeyTVParams->setModel(model_data->get_age_composition()->getErrorTVParameters());
    ageKeyTVParams->setHeight(model_data->get_age_composition()->getErrorTVParameters());
    ui->verticalLayout_age_err_tv_params->addWidget(ageKeyTVParams);

    genBins = new tableview();
    genBins->setParent(this);
    ui->horizontalLayout_gen_bins->addWidget(genBins);
    current_gen_comp = nullptr;

    tagGroups = new tableview();
    tagGroups->setParent(this);
    tagGroups->setModel(model_data->getTagObservations());
    tagGroups->setHeight(model_data->getTagObservations());
    tagGroups->resizeColumnsToContents();
    ui->horizontalLayout_tag_reldata->addWidget(tagGroups);

    tagInit = new tableview();
    tagInit->setParent(this);
    tagInit->setModel(model_data->getTagLossInit()->getParamTable());
    ui->verticalLayout_tagLossInit->addWidget(tagInit);
    tagInitTV = new tableview();
    tagInitTV->setParent(this);
    tagInitTV->setModel(model_data->getTagLossInitTV()->getVarParamTable());
    ui->verticalLayout_tagInitTV->addWidget(tagInitTV);

    tagChronic = new tableview();
    tagChronic->setParent(this);
    tagChronic->setModel(model_data->getTagLossChronic()->getParamTable());
    ui->verticalLayout_tagLossChronic->addWidget(tagChronic);
    connect (model_data->getTagLossChronic()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagChronicChanged()));
    tagChronicTV = new tableview();
    tagChronicTV->setParent(this);
    tagChronicTV->setModel(model_data->getTagLossChronicTV()->getVarParamTable());
    ui->verticalLayout_tagChronicTV->addWidget(tagChronicTV);

    tagOverdisp = new tableview();
    tagOverdisp->setParent(this);
    tagOverdisp->setModel(model_data->getTagOverdispersion()->getParamTable());
    ui->verticalLayout_tagOverdisp->addWidget(tagOverdisp);
    connect (model_data->getTagOverdispersion()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagOverdispChanged()));
    tagOverdispTV = new tableview();
    tagOverdispTV->setParent(this);
    tagOverdispTV->setModel(model_data->getTagOverdispersionTV()->getVarParamTable());
    ui->verticalLayout_tagOverdispTV->addWidget(tagOverdispTV);

    tagReptFleet = new tableview();
    tagReptFleet->setParent(this);
    tagReptFleet->setModel(model_data->getTagReportFleet()->getParamTable());
    ui->verticalLayout_tagReptFleet->addWidget(tagReptFleet);
    connect (model_data->getTagReportFleet()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptFltChanged()));
    tagReptFleetTV = new tableview();
    tagReptFleetTV->setParent(this);
    tagReptFleetTV->setModel(model_data->getTagReportFleetTV()->getVarParamTable());
    ui->verticalLayout_tagReptFleetTV->addWidget(tagReptFleetTV);

    tagReptFltDecay = new tableview();
    tagReptFltDecay->setParent(this);
    tagReptFltDecay->setModel(model_data->getTagReportDecay()->getParamTable());
    ui->verticalLayout_tagReptDecayFlt->addWidget(tagReptFltDecay);
//    connect (model_data->getTagReportDecay()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptDcayChanged()));
    tagReptDecayFltTV = new tableview();
    tagReptDecayFltTV->setParent(this);
    tagReptDecayFltTV->setModel(model_data->getTagReportDecayTV()->getVarParamTable());
    ui->verticalLayout_tagReptDecayFltTV->addWidget(tagReptDecayFltTV);


    envVariables = new tableview();
    envVariables->setParent(this);
    envVariables->setModel(model_data->getEnvVariables());
    envVariables->setHeight(model_data->getEnvVariables());
    ui->verticalLayout_env_var_obs->addWidget(envVariables);

    timeBlocks = new tableview();
    timeBlocks->setParent(this);
    timeBlocks->setModel(nullptr);
    ui->verticalLayout_block_years->addWidget(timeBlocks);

    lambdaView = new tableview();
    lambdaView->setParent(this);
    lambdaView->setModel(model_data->getLambdaAdjustmentModel());
    ui->verticalLayout_lambdas->addWidget(lambdaView);

    addSdSelexView = new tableview();
    addSdSelexView->setParent(this);
    addSdSelexView->setModel(model_data->getAddSdReporting()->getSelexModel());
    addSdSelexView->setHeight(1);
    ui->verticalLayout_add_sd_selex->addWidget(addSdSelexView);
    addSelexBinsView = new tableview();
    addSelexBinsView->setParent(this);
    addSelexBinsView->setModel(model_data->getAddSdReporting()->getSelexBinModel());
    addSelexBinsView->setHeight(1);
    ui->verticalLayout_add_sd_selex_bins->addWidget(addSelexBinsView);

    addSdGrowthView = new tableview();
    addSdGrowthView->setParent(this);
    addSdGrowthView->setModel(model_data->getAddSdReporting()->getGrowthModel());
    addSdGrowthView->setHeight(1);
    ui->verticalLayout_add_sd_growth->addWidget(addSdGrowthView);
    addGrowthBinsView = new tableview();
    addGrowthBinsView->setParent(this);
    addGrowthBinsView->setModel(model_data->getAddSdReporting()->getGrowthBinModel());
    addGrowthBinsView->setHeight(1);
    ui->verticalLayout_add_sd_growth_bins->addWidget(addGrowthBinsView);

    addSdNumAtAgeView = new tableview();
    addSdNumAtAgeView->setParent(this);
    addSdNumAtAgeView->setModel(model_data->getAddSdReporting()->getNumAtAgeModel());
    addSdNumAtAgeView->setHeight(1);
    ui->verticalLayout_add_sd_natAge->addWidget(addSdNumAtAgeView);
    addNumAtAgeBinsView = new tableview();
    addNumAtAgeBinsView->setParent(this);
    addNumAtAgeBinsView->setModel(model_data->getAddSdReporting()->getNumAtAgeAgeModel());
    addNumAtAgeBinsView->setHeight(1);
    ui->verticalLayout_add_sd_natAge_ages->addWidget(addNumAtAgeBinsView);

    addSdNatMortView = new tableview();
    addSdNatMortView->setParent(this);
    addSdNatMortView->setModel(model_data->getAddSdReporting()->getNatMortModel());
    addSdNatMortView->setHeight(1);
    ui->verticalLayout_add_sd_natMort->addWidget(addSdNatMortView);
    addSdNatMortBinsView = new tableview();
    addSdNatMortBinsView->setParent(this);
    addSdNatMortBinsView->setModel(model_data->getAddSdReporting()->getNatMortAgeModel());
    addSdNatMortBinsView->setHeight(1);
    ui->verticalLayout_add_sd_natMort_ages->addWidget(addSdNatMortBinsView);
    ui->spinBox_add_sd_dynB0->setValue(model_data->getAddSdReporting()->getDynB0());
    ui->spinBox_add_sd_sumBio->setValue(model_data->getAddSdReporting()->getSumBio());

    setAddSdRead(model_data->getAddSdReporting()->getActive());

    ui->spinBox_num_std_yrs->setMaximum(20);
    setNumSdYears(0);

    refresh();

    ui->tabWidget_data->setCurrentIndex(0);
}

data_widget::~data_widget()
{
    delete sdYearsView;
    delete ui;
}

void data_widget::connectAll()
{
    connect (ui->spinBox_gen_comp, SIGNAL(valueChanged(int)), SLOT(changeGenCompMethod(int)));
    connect (ui->pushButton_gen_new, SIGNAL(released()), SLOT(newGenCompMethod()));
    connect (ui->pushButton_gen_copy, SIGNAL(released()), SLOT(copyGenCompMethod()));
    connect (ui->pushButton_gen_delete, SIGNAL(released()), SLOT(deleteGenCompMethod()));
    connect (ui->spinBox_gen_units, SIGNAL(valueChanged(int)), SLOT(changeGenUnits(int)));
    connect (ui->spinBox_gen_scale, SIGNAL(valueChanged(int)), SLOT(changeGenScale(int)));
    connect (ui->lineEdit_gen_mincomp, SIGNAL(editingFinished()), SLOT(changeGenMinComp()));
    connect (ui->spinBox_genCompErrorType, SIGNAL(valueChanged(int)), SLOT(changeGenCompErrorType(int)));
    connect (ui->spinBox_genCompErrorIndex, SIGNAL(valueChanged(int)), SLOT(changeGenCompErrorIndex(int)));
    connect (ui->spinBox_gen_num_bins, SIGNAL(valueChanged(int)), SLOT(changeGenBins(int)));

    connect (ui->spinBox_do_tags, SIGNAL(valueChanged(int)), SLOT(changeDoTags(int)));
    connect (ui->spinBox_tag_num_groups, SIGNAL(valueChanged(int)), SLOT(changeNumTagGrps(int)));
    connect (ui->spinBox_tag_latency, SIGNAL(valueChanged(int)), model_data, SLOT(set_tag_latency(int)));
    connect (ui->spinBox_tag_max_per, SIGNAL(valueChanged(int)), model_data, SLOT(set_tag_max_periods(int)));
    connect (ui->pushButton_tag_rec_obs, SIGNAL(clicked()), SIGNAL(showRecapObs()));
    connect (ui->spinBox_tag_time_vary, SIGNAL(valueChanged(int)), model_data, SLOT(setTagTimeVaryReadParams(int)));
    connect (ui->spinBox_min_recaps, SIGNAL(valueChanged(int)), model_data, SLOT(setMinRecap(int)));
    connect (model_data->getTagLossInit()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagInitChanged()));
    connect (model_data->getTagLossInitTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagInitTVChanged()));
    tagInitChanged();
    connect (model_data->getTagLossChronic()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagChronicChanged()));
    connect (model_data->getTagLossChronicTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagChronicTVChanged()));
    tagChronicChanged();
    connect (model_data->getTagOverdispersion()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagOverdispChanged()));
    connect (model_data->getTagOverdispersionTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagOverdispTVChanged()));
    tagOverdispChanged();
    connect (model_data->getTagReportFleet()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptFltChanged()));
    connect (model_data->getTagReportFleetTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptFltTVChanged()));
    tagRptFltChanged();
    connect (model_data->getTagReportDecay()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptDcayChanged()));
    connect (model_data->getTagReportDecayTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptDcayTVChanged()));
    tagRptDcayChanged();

    connect (ui->spinBox_env_var_obs, SIGNAL(valueChanged(int)), SLOT(changeNumEnvVarObs(int)));
    connect (ui->spinBox_num_evn_var, SIGNAL(valueChanged(int)), model_data, SLOT(setNumEnvironVars(int)));
    connect (model_data->getEnvVariables(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(changeEnvVarData(QModelIndex,QModelIndex,QVector<int>)));

    connect (model_data->getLambdaAdjustmentModel(), SIGNAL(dataChanged()), SLOT(changeLambdas()));
    changeLambdas();
    connect (ui->spinBox_lambda_max_phase, SIGNAL(valueChanged(int)), SLOT(changeLambdaMaxPhase(int)));
    connect (ui->spinBox_lambda_sd_offset, SIGNAL(valueChanged(int)), SLOT(changeLambdaSdOffset(int)));
    connect (ui->spinBox_lambda_count, SIGNAL(valueChanged(int)), SLOT(changeLambdaCount(int)));

    connect (ui->spinBox_year_start, SIGNAL(valueChanged(int)), model_data, SLOT(set_start_year(int)));
    connect (ui->spinBox_year_start, SIGNAL(valueChanged(int)), SLOT(changeTotalYears()));
    connect (ui->spinBox_year_end, SIGNAL(valueChanged(int)), model_data, SLOT(set_end_year(int)));
    connect (ui->spinBox_year_end, SIGNAL(valueChanged(int)), SLOT(changeTotalYears()));
    connect (ui->spinBox_seas_per_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_seasons(int)));
    connect (ui->spinBox_seas_per_yr, SIGNAL(valueChanged(int)), SLOT(changeMaxSeason(int)));
    connect (ui->spinBox_subseasons, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_subseasons(int)));
    connect (ui->spinBox_season, SIGNAL(valueChanged(int)), SLOT(changeSeason(int)));
    connect (ui->lineEdit_num_mo_season, SIGNAL(textChanged(QString)), SLOT(changeMoPerSeason(QString)));
    connect (ui->doubleSpinBox_spawn_month, SIGNAL(valueChanged(double)), SLOT(changeSpawnMonth(double)));
    connect (ui->spinBox_max_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_ages(int)));
    connect (ui->spinBox_num_areas, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_areas(int)));
    connect (ui->spinBox_num_genders, SIGNAL(valueChanged(int)), SLOT(changeNumGenders(int)));

    connect (ui->checkBox_soft_bounds, SIGNAL(toggled(bool)), model_data, SLOT(set_use_softbounds(bool)));
    connect (ui->checkBox_priors, SIGNAL(toggled(bool)), model_data, SLOT(set_prior_likelihood(bool)));
    connect (ui->spinBox_last_phase, SIGNAL(valueChanged(int)), model_data, SLOT(set_last_estim_phase(int)));
    connect (ui->spinBox_mc_burn, SIGNAL(valueChanged(int)), model_data, SLOT(set_mc_burn(int)));
    connect (ui->spinBox_mc_thin, SIGNAL(valueChanged(int)), model_data, SLOT(set_mc_thin(int)));
    connect (ui->lineEdit_jitter, SIGNAL(editingFinished()), SLOT(changeJitter()));
    connect (ui->lineEdit_alktol, SIGNAL(editingFinished()), SLOT(changeAlkTol()));
    connect (ui->lineEdit_convergence, SIGNAL(editingFinished()), SLOT(changeConvergence()));
    connect (ui->spinBox_retrospect_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_retrospect_year(int)));
    connect (ui->spinBox_f_bmass_min_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_biomass_min_age(int)));
    connect (ui->comboBox_f_dep, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDepletionBase(int)));
    connect (ui->spinBox_f_dep_multi, SIGNAL(valueChanged(int)), this, SLOT(changeDepletionMulti(int)));
    connect (ui->checkBox_f_dep_log, SIGNAL(stateChanged(int)), this, SLOT(changeDepletionLog(int)));
    connect (ui->lineEdit_dep_denom, SIGNAL(editingFinished()), SLOT(changeDepDenom()));
    connect (ui->comboBox_spr_basis, SIGNAL(currentIndexChanged(int)), model_data, SLOT(set_spr_basis(int)));
    connect (ui->comboBox_f_rpt_units, SIGNAL(currentIndexChanged(int)), SLOT(changeFRptUnits(int)));
    connect (ui->spinBox_f_min_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_f_min_age(int)));
    connect (ui->spinBox_f_max_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_f_max_age(int)));
    connect (ui->comboBox_f_rpt_base, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFRptBase(int)));
    connect (ui->spinBox_f_rpt_multi, SIGNAL(valueChanged(int)), this, SLOT(changeFRptMulti(int)));
    connect (ui->checkBox_f_rpt_log, SIGNAL(stateChanged(int)), this, SLOT(changeFRptLog(int)));
    connect (ui->spinBox_sdrpt_min_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_bio_sd_min_year(int)));
    connect (ui->spinBox_sdrpt_max_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_bio_sd_max_year(int)));
    connect (ui->spinBox_num_std_yrs, SIGNAL(valueChanged(int)), SLOT(setNumSdYears(int)));
//    connect (sdYearsDelegate, SIGNAL(commitData(QWidget*)), SLOT(numSdYearsChanged(QWidget *)));

//    connect (ui->spinBox_length_bin_method, SIGNAL(valueChanged(int)), SLOT(changeLengthCompMethod(int)));
    connect (ui->spinBox_length_num_bins, SIGNAL(valueChanged(int)), SLOT(changeLengthBins(int)));
//    connect (ui->spinBox_length_combine, SIGNAL(valueChanged(int)), SLOT(changeLengthCombine(int)));
//    connect (ui->lineEdit_length_comp_tails, SIGNAL(editingFinished()), SLOT(changeLengthCompress()));
//    connect (ui->lineEdit_length_constant, SIGNAL(editingFinished()), SLOT(changeLengthAdd()));
    connect (ui->pushButton_length_obs, SIGNAL(clicked()), SIGNAL(showLengthObs()));
    connect (ui->pushButton_length_dirichlet_add, SIGNAL(clicked()), SLOT(addLengthDirichlet()));

    connect (ui->spinBox_age_bin_method, SIGNAL(valueChanged(int)), SLOT(changeAgeCompMethod(int)));
    connect (ui->spinBox_age_num_bins, SIGNAL(valueChanged(int)), SLOT(changeAgeBins(int)));
    connect (ui->spinBox_age_error_num, SIGNAL(valueChanged(int)), SLOT(changeAgeError(int)));
    connect (ui->pushButton_age_obs, SIGNAL(clicked()), SIGNAL(showAgeObs()));
    connect (ui->pushButton_saa_obs, SIGNAL(clicked()), SIGNAL(showSAAObs()));
    connect (ui->pushButton_age_dirichlet_add, SIGNAL(clicked()), SLOT(addAgeDirichlet()));

    connect (model_data->get_age_composition()->getErrorModel(), SIGNAL(dataChanged()),
             SLOT(ageErrorChanged()));

    connect (ui->pushButton_gen_obs, SIGNAL(clicked()), SLOT(showGenObs()));

    connect (ui->groupBox_comp_morph, SIGNAL(toggled(bool)), SLOT(changeDoMorphs(bool)));
    connect (ui->spinBox_morph_num_stocks, SIGNAL(valueChanged(int)), SLOT(changeNumMorphs(int)));
    connect (ui->lineEdit_morph_min_comp, SIGNAL(editingFinished()), SLOT(changeMorphMincomp()));
    connect (ui->pushButton_morph_obs, SIGNAL(clicked()), SIGNAL(showMorphObs()));

    connect (ui->spinBox_block_patterns_total, SIGNAL(valueChanged(int)), SLOT(changeNumBlockPatterns(int)));
    connect (ui->spinBox_block_pattern_num, SIGNAL(valueChanged(int)), SLOT(changeBlockPattern(int)));
    connect (ui->spinBox_blocks_count, SIGNAL(valueChanged(int)), SLOT(changeNumBlocks(int)));

    connect (ui->spinBox_add_sd_read, SIGNAL(valueChanged(int)), SLOT(changeAddSdRead(int)));
    connect (ui->spinBox_add_sd_dynB0, SIGNAL(valueChanged(int)), SLOT(changeAddSdDynB0(int)));
    connect (ui->spinBox_add_sd_sumBio, SIGNAL(valueChanged(int)), SLOT(changeAddSdSumBio(int)));
}

void data_widget::disconnectAll()
{
    disconnect (ui->spinBox_gen_comp, SIGNAL(valueChanged(int)), this, SLOT(changeGenCompMethod(int)));
    disconnect (ui->pushButton_gen_new, SIGNAL(released()),this,  SLOT(newGenCompMethod()));
    disconnect (ui->pushButton_gen_copy, SIGNAL(released()), this, SLOT(copyGenCompMethod()));
    disconnect (ui->pushButton_gen_delete, SIGNAL(released()), this, SLOT(deleteGenCompMethod()));
    disconnect (ui->spinBox_gen_units, SIGNAL(valueChanged(int)), this, SLOT(changeGenUnits(int)));
    disconnect (ui->spinBox_gen_scale, SIGNAL(valueChanged(int)), this, SLOT(changeGenScale(int)));
    disconnect (ui->spinBox_genCompErrorType, SIGNAL(valueChanged(int)), this, SLOT(changeGenCompErrorType(int)));
    disconnect (ui->spinBox_genCompErrorIndex, SIGNAL(valueChanged(int)), this, SLOT(changeGenCompErrorIndex(int)));
    disconnect (ui->lineEdit_gen_mincomp, SIGNAL(editingFinished()), this, SLOT(changeGenMinComp()));
    disconnect (ui->spinBox_gen_num_bins, SIGNAL(valueChanged(int)), this, SLOT(changeGenBins(int)));

    disconnect (ui->spinBox_do_tags, SIGNAL(valueChanged(int)), this, SLOT(changeDoTags(int)));
    disconnect (ui->spinBox_tag_num_groups, SIGNAL(valueChanged(int)), this, SLOT(changeNumTagGrps(int)));
    disconnect (ui->spinBox_tag_latency, SIGNAL(valueChanged(int)), model_data, SLOT(set_tag_latency(int)));
    disconnect (ui->spinBox_tag_max_per, SIGNAL(valueChanged(int)), model_data, SLOT(set_tag_max_periods(int)));
    disconnect (ui->pushButton_tag_rec_obs, SIGNAL(clicked()), this, SIGNAL(showRecapObs()));
    disconnect (ui->spinBox_tag_time_vary, SIGNAL(valueChanged(int)), model_data, SLOT(setTagTimeVaryReadParams(int)));
    disconnect (model_data->getTagLossInit()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagInitChanged()));
    disconnect (model_data->getTagLossInitTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagInitTVChanged()));
    disconnect (model_data->getTagLossChronic()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagChronicChanged()));
    disconnect (model_data->getTagLossChronicTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagChronicTVChanged()));
//    tagChronicChanged();
    disconnect (model_data->getTagOverdispersion()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagOverdispChanged()));
    disconnect (model_data->getTagOverdispersionTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagOverdispTVChanged()));
//    tagOverdispChanged();
    disconnect (model_data->getTagReportFleet()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptFltChanged()));
    disconnect (model_data->getTagReportFleetTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptFltTVChanged()));
//    tagRptFltChanged();
    disconnect (model_data->getTagReportDecay()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptDcayChanged()));
    disconnect (model_data->getTagReportDecayTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptDcayTVChanged()));
//    tagRptDcayChanged();

    disconnect (ui->spinBox_env_var_obs, SIGNAL(valueChanged(int)), this, SLOT(changeNumEnvVarObs(int)));
    disconnect (ui->spinBox_num_evn_var, SIGNAL(valueChanged(int)), model_data, SLOT(setNumEnvironVars(int)));
    disconnect (model_data->getEnvVariables(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             this, SLOT(changeEnvVarData(QModelIndex,QModelIndex,QVector<int>)));

    disconnect (model_data->getLambdaAdjustmentModel(), SIGNAL(dataChanged()), this, SLOT(changeLambdas()));
//    changeLambdas();
    disconnect (ui->spinBox_lambda_max_phase, SIGNAL(valueChanged(int)), this, SLOT(changeLambdaMaxPhase(int)));
    disconnect (ui->spinBox_lambda_sd_offset, SIGNAL(valueChanged(int)), this, SLOT(changeLambdaSdOffset(int)));
    disconnect (ui->spinBox_lambda_count, SIGNAL(valueChanged(int)), this, SLOT(changeLambdaCount(int)));

    disconnect (ui->spinBox_year_start, SIGNAL(valueChanged(int)), model_data, SLOT(set_start_year(int)));
    disconnect (ui->spinBox_year_start, SIGNAL(valueChanged(int)), this, SLOT(changeTotalYears()));
    disconnect (ui->spinBox_year_end, SIGNAL(valueChanged(int)), model_data, SLOT(set_end_year(int)));
    disconnect (ui->spinBox_year_end, SIGNAL(valueChanged(int)), this, SLOT(changeTotalYears()));
    disconnect (ui->spinBox_seas_per_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_seasons(int)));
    disconnect (ui->spinBox_seas_per_yr, SIGNAL(valueChanged(int)), this, SLOT(changeMaxSeason(int)));
    disconnect (ui->spinBox_subseasons, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_subseasons(int)));
    disconnect (ui->spinBox_season, SIGNAL(valueChanged(int)), this, SLOT(changeSeason(int)));
    disconnect (ui->lineEdit_num_mo_season, SIGNAL(textChanged(QString)), this, SLOT(changeMoPerSeason(QString)));
    disconnect (ui->doubleSpinBox_spawn_month, SIGNAL(valueChanged(double)), this, SLOT(changeSpawnMonth(double)));
    disconnect (ui->spinBox_max_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_ages(int)));
    disconnect (ui->spinBox_num_areas, SIGNAL(valueChanged(int)), model_data, SLOT(set_num_areas(int)));
    disconnect (ui->spinBox_num_genders, SIGNAL(valueChanged(int)), this, SLOT(changeNumGenders(int)));

    disconnect (ui->checkBox_soft_bounds, SIGNAL(toggled(bool)), model_data, SLOT(set_use_softbounds(bool)));
    disconnect (ui->checkBox_priors, SIGNAL(toggled(bool)), model_data, SLOT(set_prior_likelihood(bool)));
    disconnect (ui->spinBox_last_phase, SIGNAL(valueChanged(int)), model_data, SLOT(set_last_estim_phase(int)));
    disconnect (ui->spinBox_mc_burn, SIGNAL(valueChanged(int)), model_data, SLOT(set_mc_burn(int)));
    disconnect (ui->spinBox_mc_thin, SIGNAL(valueChanged(int)), model_data, SLOT(set_mc_thin(int)));
    disconnect (ui->lineEdit_jitter, SIGNAL(editingFinished()), this, SLOT(changeJitter()));
    disconnect (ui->lineEdit_alktol, SIGNAL(editingFinished()), this, SLOT(changeAlkTol()));
    disconnect (ui->lineEdit_convergence, SIGNAL(editingFinished()), this, SLOT(changeConvergence()));
    disconnect (ui->spinBox_retrospect_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_retrospect_year(int)));
    disconnect (ui->spinBox_f_bmass_min_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_biomass_min_age(int)));
    disconnect (ui->comboBox_f_dep, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDepletionBase(int)));
    disconnect (ui->spinBox_f_dep_multi, SIGNAL(valueChanged(int)), this, SLOT(changeDepletionMulti(int)));
    disconnect (ui->checkBox_f_dep_log, SIGNAL(stateChanged(int)), this, SLOT(changeDepletionLog(int)));
    disconnect (ui->lineEdit_dep_denom, SIGNAL(editingFinished()), this, SLOT(changeDepDenom()));
    disconnect (ui->comboBox_spr_basis, SIGNAL(currentIndexChanged(int)), model_data, SLOT(set_spr_basis(int)));
    disconnect (ui->comboBox_f_rpt_units, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFRptUnits(int)));
    disconnect (ui->spinBox_f_min_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_f_min_age(int)));
    disconnect (ui->spinBox_f_max_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_f_max_age(int)));
    disconnect (ui->comboBox_f_rpt_base, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFRptBase(int)));
    disconnect (ui->spinBox_f_rpt_multi, SIGNAL(valueChanged(int)), this, SLOT(changeFRptMulti(int)));
    disconnect (ui->checkBox_f_rpt_log, SIGNAL(stateChanged(int)), this, SLOT(changeFRptLog(int)));
    disconnect (ui->spinBox_sdrpt_min_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_bio_sd_min_year(int)));
    disconnect (ui->spinBox_sdrpt_max_yr, SIGNAL(valueChanged(int)), model_data, SLOT(set_bio_sd_max_year(int)));
    disconnect (ui->spinBox_num_std_yrs, SIGNAL(valueChanged(int)), this, SLOT(setNumSdYears(int)));
//    disconnect (sdYearsDelegate, SIGNAL(commitData(QWidget*)), this, SLOT(numSdYearsChanged(QWidget *)));

//    disconnect (ui->spinBox_length_bin_method, SIGNAL(valueChanged(int)), this, SLOT(changeLengthCompMethod(int)));
    disconnect (ui->spinBox_length_num_bins, SIGNAL(valueChanged(int)), this, SLOT(changeLengthBins(int)));
//    disconnect (ui->spinBox_length_combine, SIGNAL(valueChanged(int)), this, SLOT(changeLengthCombine(int)));
//    disconnect (ui->lineEdit_length_comp_tails, SIGNAL(editingFinished()), this, SLOT(changeLengthCompress()));
//    disconnect (ui->lineEdit_length_constant, SIGNAL(editingFinished()), this, SLOT(changeLengthAdd()));
    disconnect (ui->pushButton_length_obs, SIGNAL(clicked()), this, SIGNAL(showLengthObs()));
    disconnect (ui->pushButton_length_dirichlet_add, SIGNAL(clicked()), this, SLOT(addLengthDirichlet()));

    disconnect (ui->spinBox_age_bin_method, SIGNAL(valueChanged(int)), this, SLOT(changeAgeCompMethod(int)));
    disconnect (ui->spinBox_age_num_bins, SIGNAL(valueChanged(int)), this, SLOT(changeAgeBins(int)));
    disconnect (ui->spinBox_age_error_num, SIGNAL(valueChanged(int)), this, SLOT(changeAgeError(int)));
    disconnect (ui->pushButton_age_obs, SIGNAL(clicked()), this, SIGNAL(showAgeObs()));
    disconnect (ui->pushButton_saa_obs, SIGNAL(clicked()), this, SIGNAL(showSAAObs()));
    disconnect (ui->pushButton_age_dirichlet_add, SIGNAL(clicked()), this, SLOT(addAgeDirichlet()));

    disconnect (model_data->get_age_composition()->getErrorModel(), SIGNAL(dataChanged()),
             this, SLOT(ageErrorChanged()));

    disconnect (ui->pushButton_gen_obs, SIGNAL(clicked()), this, SLOT(showGenObs()));

    disconnect (ui->groupBox_comp_morph, SIGNAL(toggled(bool)), this, SLOT(changeDoMorphs(bool)));
    disconnect (ui->spinBox_morph_num_stocks, SIGNAL(valueChanged(int)), this, SLOT(changeNumMorphs(int)));
    disconnect (ui->lineEdit_morph_min_comp, SIGNAL(editingFinished()), this, SLOT(changeMorphMincomp()));
    disconnect (ui->pushButton_morph_obs, SIGNAL(clicked()), this, SIGNAL(showMorphObs()));

    disconnect (ui->spinBox_block_patterns_total, SIGNAL(valueChanged(int)), this, SLOT(changeNumBlockPatterns(int)));
    disconnect (ui->spinBox_block_pattern_num, SIGNAL(valueChanged(int)), this, SLOT(changeBlockPattern(int)));
    disconnect (ui->spinBox_blocks_count, SIGNAL(valueChanged(int)), this, SLOT(changeNumBlocks(int)));

    disconnect (ui->spinBox_add_sd_read, SIGNAL(valueChanged(int)), this, SLOT(changeAddSdRead(int)));
    disconnect (ui->spinBox_add_sd_dynB0, SIGNAL(valueChanged(int)), this, SLOT(changeAddSdDynB0(int)));
    disconnect (ui->spinBox_add_sd_sumBio, SIGNAL(valueChanged(int)), this, SLOT(changeAddSdSumBio(int)));
}

void data_widget::set_model(ss_model *m_data)
{
    reset();
    {
        model_data = m_data;
    }
    refresh();
}

void data_widget::reset()
{

}

void data_widget::refresh()
{
    disconnectAll();
    if (model_data != nullptr)
    {
        ui->spinBox_year_start->setValue(model_data->get_start_year());
        ui->spinBox_year_end->setValue(model_data->get_end_year());
        changeTotalYears();
        ui->spinBox_seas_per_yr->setValue(model_data->get_num_seasons());
        ui->spinBox_subseasons->setValue(model_data->get_num_subseasons());

        ui->doubleSpinBox_spawn_month->setValue(model_data->get_spawn_month());
        ui->spinBox_season->setValue(1);
        setMoPerSeason(1);
        setTotalMonths();
        ui->spinBox_num_fisheries->setValue(model_data->get_num_fisheries());
        ui->spinBox_num_surveys->setValue(model_data->get_num_surveys());
        ui->spinBox_total_fleets->setValue(model_data->get_num_fleets());
        ui->spinBox_num_areas->setValue(model_data->get_num_areas());
        ui->spinBox_max_age->setValue(model_data->get_num_ages());
        ui->spinBox_num_genders->setValue(model_data->get_num_genders());

        ui->checkBox_soft_bounds->setChecked(model_data->get_use_softbounds());
        ui->checkBox_priors->setChecked(model_data->get_prior_likelihood());
        ui->spinBox_last_phase->setValue(model_data->get_last_estim_phase());
        ui->spinBox_mc_burn->setValue(model_data->mc_burn());
        ui->spinBox_mc_thin->setValue(model_data->mc_thin());
        setRandSeed(model_data->getRandSeed());
        ui->lineEdit_jitter->setText(QString::number(model_data->jitter_param()));
        ui->lineEdit_alktol->setText(QString::number(model_data->getALKTol()));
        ui->lineEdit_convergence->setText(QString::number(model_data->get_convergence_criteria()));
        ui->spinBox_f_bmass_min_age->setValue(model_data->get_biomass_min_age());
        setDepletionBasis(model_data->get_depletion_basis());
        ui->comboBox_f_dep->setCurrentIndex(model_data->get_depletion_basis());
        ui->lineEdit_dep_denom->setText(QString::number(model_data->get_depletion_denom()));
        ui->comboBox_spr_basis->setCurrentIndex(model_data->get_spr_basis());
        setFRptUnits (model_data->get_f_units());
        ui->spinBox_f_min_age->setValue(model_data->get_f_min_age());
        ui->spinBox_f_max_age->setValue(model_data->get_f_max_age());
        setFRptBasis(model_data->get_f_basis());

        ui->spinBox_sdrpt_min_yr->setValue(model_data->bio_sd_min_year());
        ui->spinBox_sdrpt_max_yr->setValue(model_data->bio_sd_max_year());
        ui->spinBox_num_std_yrs->setValue(model_data->get_num_std_years());
        sdYearsView->setModel(model_data->sdYearsModel);
        sdYearsView->resizeColumnsToContents();

        setLengthCompMethod(model_data->get_length_composition()->getAltBinMethod());
        setLengthBins (model_data->get_length_composition()->getNumberBins());
        lengthBins->setModel(model_data->get_length_composition()->getBinsModel());
        lengthBins->setHeight(1);
        lengthBins->resizeColumnsToContents();

        lengthDirichlet->setModel(model_data->get_length_composition()->getDirichletParamTable());
        lengthDirichlet->setHeight(model_data->get_length_composition()->getDirichletParamTable());
        lengthDirichlet->resizeColumnsToContents();


        ui->spinBox_age_bin_method->setValue(model_data->get_age_composition()->getAltBinMethod());
        setAgeBins(model_data->get_age_composition()->getNumberBins());
        setAgeError(model_data->get_age_composition()->number_error_defs());
        ageBins->setModel(model_data->get_age_composition()->getBinsModel());
        ageBins->setHeight(40);
        ageBins->resizeColumnsToContents();

        ageError->setModel(model_data->get_age_composition()->getErrorModel());
        ageError->setHeight(model_data->get_age_composition()->getErrorModel());
        ageError->resizeColumnsToContents();

        ageDirichlet->setModel(model_data->get_age_composition()->getDirichletParamTable());
        ageDirichlet->setHeight(model_data->get_age_composition()->getDirichletParamTable());
        ageDirichlet->resizeColumnsToContents();

        ui->label_gen_comp_total->setText(QString::number(model_data->getNumGeneralCompMethods()));
        setGenCompMethod(0);

        setDoMorphs(model_data->getDoMorphComp());

        setDoTags(model_data->getDoTags());
        ui->spinBox_tag_time_vary->setValue(model_data->getTagTimeVaryReadParams());

        setLambdaMaxPhase(model_data->getLambdaMaxPhase());
        setLambdaSdOffset(model_data->getLambdaSdOffset());
        setLambdaCount(model_data->getNumLambdaAdjustments());

        ui->spinBox_env_var_obs->setValue(model_data->getNumEnvironVarObs());
        ui->spinBox_num_evn_var->setValue(model_data->getNumEnvironVars());

        int num = model_data->getNumBlockPatterns();
        ui->spinBox_block_patterns_total->setValue(num);
        if (num > 0) {
            setBlockPattern(1);
            changeBlockPattern(1);
        }

        ui->spinBox_add_sd_read->setValue (model_data->getAddSdReporting()->getActive());
        setAddSdRead(model_data->getAddSdReporting()->getActive());
        ui->spinBox_add_sd_dynB0->setValue(model_data->getAddSdReporting()->getDynB0());
        ui->spinBox_add_sd_sumBio->setValue(model_data->getAddSdReporting()->getSumBio());

        set2DAR1(model_data->getUse2DAR1());
        connectAll();
    }
}

void data_widget::changeAddSdRead(int value) {
    model_data->getAddSdReporting()->setActive(value);
    setAddSdRead(value);
}

void data_widget::setAddSdRead(int value) {
    if (ui->spinBox_add_sd_read->value() != value) {
        ui->spinBox_add_sd_read->setValue(value);
    }
    // set everthing invisible
    addSdSelexView->setVisible(false);
    addSelexBinsView->setVisible(false);

    addSdGrowthView->setVisible(false);
    addGrowthBinsView->setVisible(false);

    addSdNumAtAgeView->setVisible(false);
    addNumAtAgeBinsView->setVisible(false);

    ui->label_add_sd_natMort->setVisible(false);
    addSdNatMortView->setVisible(false);
    addSdNatMortBinsView->setVisible(false);

    ui->label_add_sd_dynB0->setVisible(false);
    ui->spinBox_add_sd_dynB0->setVisible(false);
    ui->label_add_sd_sumBio->setVisible(false);
    ui->spinBox_add_sd_sumBio->setVisible(false);

    // turn on correct elements
    if (value > 0) {
        addSdSelexView->setVisible(true);
        addSdSelexView->resizeColumnsToContents();
        addSelexBinsView->setVisible(true);
        addSelexBinsView->resizeColumnsToContents();

        if (model_data->getReadWtAtAge() == 0) {
            addSdGrowthView->setVisible(true);
            addSdGrowthView->resizeColumnsToContents();
            addGrowthBinsView->setVisible(true);
            addGrowthBinsView->resizeColumnsToContents();
        }

        addSdNumAtAgeView->setVisible(true);
        addSdNumAtAgeView->resizeColumnsToContents();
        addNumAtAgeBinsView->setVisible(true);
        addNumAtAgeBinsView->resizeColumnsToContents();

        if (value == 2) {
            ui->label_add_sd_natMort->setVisible(true);
            addSdNatMortView->setVisible(true);
            addSdNatMortView->resizeColumnsToContents();
            addSdNatMortBinsView->setVisible(true);
            addSdNatMortBinsView->resizeColumnsToContents();
            ui->label_add_sd_dynB0->setVisible(true);
            ui->spinBox_add_sd_dynB0->setVisible(true);
            ui->label_add_sd_sumBio->setVisible(true);
            ui->spinBox_add_sd_sumBio->setVisible(true);
        }

    }
}

void data_widget::changeAddSdDynB0(int value)
{
    model_data->getAddSdReporting()->setDynB0(value);
}

void data_widget::changeAddSdSumBio(int value)
{
    model_data->getAddSdReporting()->setSumBio(value);
}

void data_widget::changeMaxSeason(int num)
{
    if (num < 1)
        num = 1;
    ui->spinBox_season->setMaximum(num);
    ui->spinBox_season->setMaximum(num);
}

void data_widget::changeSeason(int seas)
{
    setMoPerSeason(seas);
//    setNumSubSeasons();
}

void data_widget::setMoPerSeason(int seas)
{
//    int seas = ui->spinBox_season->value();
    float months = model_data->getSeason(seas)->getNumMonths();
    ui->lineEdit_num_mo_season->setText(QString::number(months));
}

void data_widget::changeMoPerSeason(QString txt)
{
    int seas = ui->spinBox_season->value();
    float months = txt.toFloat();
    model_data->getSeason(seas)->setNumMonths(months);
//    model_data->rescale_months_per_season();
    setTotalMonths();
}

float data_widget::setTotalMonths()
{
    int num_seas = model_data->get_num_seasons();
    float tot_months = 0;
    for (int i = 1; i <= num_seas; i++)
        tot_months += model_data->getSeason(i)->getNumMonths();
    ui->label_total_months_value->setText(QString::number(tot_months, 'g', 2));
    return tot_months;
}

void data_widget::changeSpawnMonth(double month)
{
    model_data->set_spawn_month((float)month);
}

void data_widget::changeSpawnSeason(float seas)
{
    int season = static_cast<int>(seas + .5);
    if (season > ui->spinBox_seas_per_yr->value())
    {
        season = setTotalMonths();//ui->spinBox_seas_per_yr->value();
//        ui->doubleSpinBox_spawn_season->setValue(seas);
    }
    model_data->set_spawn_season(season);
}

void data_widget::changeTotalYears()
{
    int start = ui->spinBox_year_start->value();
    int end = ui->spinBox_year_end->value();
    ui->spinBox_year_total->setValue(end - start + 1);
}

void data_widget::changeFRptVis(bool flag)
{
    ui->label_f_min_age->setVisible(flag);
    ui->spinBox_f_min_age->setVisible(flag);
    ui->label_f_max_age->setVisible(flag);
    ui->spinBox_f_max_age->setVisible(flag);
}

void data_widget::setFRptUnits(int val)
{
    ui->comboBox_f_rpt_units->setCurrentIndex(val);
    changeFRptVis(val > 3);
}

void data_widget::changeFRptUnits(int val)
{
    model_data->set_f_units(val);
    changeFRptVis(val > 3);
}

void data_widget::setNumSdYears(int val)
{
    model_data->set_num_std_years(val);
    model_data->sdYearsModel->setColumnCount(val);
    ui->label_std_yrs->setVisible(val);
    sdYearsView->setVisible(val);
}

void data_widget::numSdYearsChanged()
{

}

void data_widget::changeNumGenders(int val)
{
    model_data->set_num_genders(val);
//    int posGender = abs(val);
    for (int i = 0; i < model_data->get_num_fleets(); i++)
    {
        int bins = model_data->get_length_composition()->getNumberBins();
        if (bins > 0)
            model_data->getFleet(i)->setLengthNumBins(bins);
        bins = model_data->get_age_composition()->getNumberBins();
        if (bins > 0)
            model_data->getFleet(i)->setAgeNumBins(bins);
        for (int j = 0; j < model_data->getNumGeneralCompMethods(); j++)
            model_data->getFleet(i)->setGenNumBins(j, model_data->getGeneralCompMethod(j)->getNumberBins());
    }
}

/*void data_widget::setMBWTObs(int count)
{
    ui->spinBox_mbwt_num_obs->setValue(count);
    if (count > 0)
        mbweightview->show();
    else
        mbweightview->hide();
}

void data_widget::changeMBWTObs(int count)
{
    model_data->set_mean_body_wt_obs_count(count);
}*/

void data_widget::setLengthCompMethod(int method)
{
//    ui->spinBox_length_bin_method->setValue(method);
    ui->spinBox_length_num_bins->setValue(model_data->get_length_composition()->getNumberBins());
    if (method == 1)
    {
        ui->label_length_num_bins->setVisible(false);
        ui->label_length_bins->setVisible(false);
        ui->spinBox_length_num_bins->setVisible(false);

    }
    else
    {
        ui->label_length_num_bins->setVisible(true);
        ui->label_length_bins->setVisible(true);
        ui->spinBox_length_num_bins->setVisible(true);

    }
}

void data_widget::changeLengthCompMethod(int method)
{
    model_data->get_length_composition()->setAltBinMethod(method);
}

void data_widget::setLengthBins(int numBins)
{
    ui->spinBox_length_num_bins->setValue(numBins);
    if (numBins > 0)
    {
        lengthBins->show();
    }
    else
    {
        lengthBins->hide();
    }
}

void data_widget::changeLengthBins(int numBins)
{
    if (numBins != model_data->get_length_composition()->getNumberBins())
    {
        model_data->get_length_composition()->setNumberBins(numBins);
        for (int i = 0; i < model_data->get_num_fleets(); i++)
            model_data->getFleet(i)->setLengthNumBins(numBins);
    }
    setLengthBins(numBins);
}

void data_widget::changeLengthCompress()
{
//    double comp = ui->lineEdit_length_comp_tails->text().toDouble();
//    model_data->get_length_composition()->set_compress_tails(comp);
}

void data_widget::changeLengthAdd()
{
//    double add = ui->lineEdit_length_constant->text().toDouble();
//    model_data->get_length_composition()->set_add_to_compression(add);
}

void data_widget::changeLengthCombine()
{
//    model_data->get_length_composition()->set_combine_genders(gen);
}

void data_widget::setAgeCompMethod(int method)
{
    ui->spinBox_age_bin_method->setValue(method);
}

void data_widget::changeAgeCompMethod(int method)
{
    model_data->get_age_composition()->setAltBinMethod(method);
}

void data_widget::setAgeBins(int numBins)
{
    ui->spinBox_age_num_bins->setValue(numBins);
    if (numBins > 0)
    {
        ageBins->show();
        ageErrorChanged();
    }
    else
    {
        ageBins->hide();
        ui->label_agekey_params->setVisible(false);
        ui->label_agekey_tv_params->setVisible(false);
        ageKeyParams->setVisible(false);
        ageKeyTVParams->setVisible(false);
    }
}

void data_widget::changeAgeBins(int numBins)
{
    if (numBins != model_data->get_age_composition()->getNumberBins())
    {
        model_data->get_age_composition()->setNumberBins(numBins);
        for (int i = 0; i < model_data->get_num_fleets(); i++)
            model_data->getFleet(i)->setAgeNumBins(numBins);
    }
    setAgeBins(numBins);
}

void data_widget::setAgeError(int numDefs)
{
    ui->spinBox_age_error_num->setValue(numDefs);
    if (numDefs > 0)
    {

    }
    else
    {

    }
}

void data_widget::changeAgeError(int numDefs)
{
    model_data->get_age_composition()->set_num_error_defs(numDefs);
}

void data_widget::ageErrorChanged()
{
    bool usingParams = false;
    QStringList ql;
    int index = 1;
    for (int i = 0; i < ui->spinBox_age_error_num->value(); i++)
    {
        index = i * 2 + 1;
        ql = model_data->get_age_composition()->getErrorParam(index);
        if (ql.isEmpty())
            usingParams = false;
        else if (ql.at(0).toFloat() < 0)
        {
            usingParams = true;
            break;
        }
    }
    ui->label_agekey_params->setVisible(usingParams);
    ui->label_agekey_tv_params->setVisible(usingParams);
    ageKeyParams->setVisible(usingParams);
    ageKeyTVParams->setVisible(usingParams);
    if (usingParams)
        ui->label_agekey_tv_params->setVisible(model_data->get_age_composition()->getErrorTVParameters()->rowCount()>0);
}

void data_widget::changeAgeCombine()
{
//    model_data->get_age_composition()->set_combine_genders(gen);
}

void data_widget::changeNumEnvVarObs(int num)
{
    model_data->setNumEnvironVarObs(num);
    envVariables->setHeight(num);
}

void data_widget::changeEnvVarData(QModelIndex tl, QModelIndex br, QVector<int> data)
{
    tablemodel *tm = model_data->getEnvVariables();
    int numEnvVar = checkNumEnvVars();//model_data->getNumEnvironVars();
    int item;
    int firstrow = tl.row();
    int lastrow = br.row();
    QStringList rowdata;
    Q_UNUSED(data);

    for (int i = firstrow; i <= lastrow; i++)
    {
        rowdata = tm->getRowData(i);
        item = QString(rowdata.at(1)).toInt();
        if (item > numEnvVar)
        {
            model_data->setNumEnvironVars(item);
            ui->spinBox_num_evn_var->setValue(item);
        }
    }
}

int data_widget::checkNumEnvVars()
{
    tablemodel *tm = model_data->getEnvVariables();
    int numEnvVar = model_data->getNumEnvironVars();
    int numVars = 0;
    int item;
    QStringList rowdata;
    for (int i = 0; i <= tm->rowCount(); i++)
    {
        rowdata = tm->getRowData(i);
        item = QString(rowdata.at(1)).toInt();
        if (item > numVars)
            numVars = item;
    }
    if (numVars != numEnvVar)
    {
        model_data->setNumEnvironVars(numVars);
        ui->spinBox_num_evn_var->setValue(numVars);
    }
    return numVars;
}

void data_widget::setGenCompMethod(int method)
{
    ui->spinBox_gen_comp->setValue(method);
    changeGenCompMethod(method);
}

void data_widget::changeGenCompMethod(int method)
{
    int total = model_data->getNumGeneralCompMethods();
    if (total > 0)
    {
        if (method < 1)
            ui->spinBox_gen_comp->setValue(1);
        else if (method > total)
            ui->spinBox_gen_comp->setValue(total);

        else
        {
            ui->label_gen_comp_total->setText(QString::number(total));
            current_gen_comp = model_data->getGeneralCompMethod(method - 1);
            ui->spinBox_gen_units->setValue(current_gen_comp->getUnits());
            ui->spinBox_gen_scale->setValue(current_gen_comp->getScale());
            ui->lineEdit_gen_mincomp->setText(QString::number(current_gen_comp->getMinComp()));
            ui->spinBox_gen_num_bins->setValue(current_gen_comp->getNumberBins());
            genBins->setModel(current_gen_comp->getBinsModel());
            genBins->setHeight(1);
            genBins->resizeColumnsToContents();
        }
    }
    else
    {
        ui->spinBox_gen_comp->setValue(0);
        ui->label_gen_comp_total->setText("0");
        current_gen_comp = nullptr;
        ui->spinBox_gen_units->setValue(0);
        ui->spinBox_gen_scale->setValue(0);
        ui->lineEdit_gen_mincomp->setText(" ");
        ui->spinBox_gen_num_bins->setValue(0);
        genBins->setModel(nullptr);
    }
}

void data_widget::newGenCompMethod()
{
    int index = ui->spinBox_gen_comp->value();
    int total = model_data->getNumGeneralCompMethods();
    compositionGeneral *genMethod = new compositionGeneral(model_data);
//    genMethod->set
    model_data->addGeneralCompMethod(genMethod);
    model_data->getGeneralCompMethod(index)->setNumber(index+1);
    for (int j = 0; j < model_data->get_num_fleets(); j++)
        model_data->getFleet(j)->setGenModelTotal(total+1);
    ui->spinBox_gen_comp->setValue(index+1);
}

void data_widget::copyGenCompMethod()
{
    int index = ui->spinBox_gen_comp->value();
    int total = model_data->getNumGeneralCompMethods();
    if (total == 0)
        newGenCompMethod();
    else
    {
        model_data->copyGeneralCompMethod(model_data->getGeneralCompMethod(index-1));
    }
    ui->spinBox_gen_comp->setValue(total+1);
}

void data_widget::deleteGenCompMethod()
{
    int index = ui->spinBox_gen_comp->value();
    int total = model_data->getNumGeneralCompMethods();
    if (total != 0)
        model_data->deleteGeneralCompMethod(index - 1);
    index--; if (index < 0) index = 0;
    total--; if (total < 0) total = 0;
    if (index >= 0)
    {
        if (index < total)
            ui->spinBox_gen_comp->setValue(index);
        else
            ui->spinBox_gen_comp->setValue(total);
    }
}

void data_widget::changeGenUnits(int units)
{
    if (units < 1)
        ui->spinBox_gen_units->setValue(1);
    else if (units > 2)
        ui->spinBox_gen_units->setValue(2);

    if (current_gen_comp != nullptr)
        current_gen_comp->setUnits(units);
}

void data_widget::changeGenScale(int scale)
{
    if (scale < 1)
        ui->spinBox_gen_scale->setValue(1);
    else if (scale > 4)
        ui->spinBox_gen_scale->setValue(4);

    if (current_gen_comp != nullptr)
        current_gen_comp->setScale(scale);
}

void data_widget::changeGenCompErrorType(int type)
{
    if (current_gen_comp != nullptr)
        current_gen_comp->setCompErrorType(type);
}

void data_widget::changeGenCompErrorIndex(int index)
{
    if (current_gen_comp != nullptr)
        current_gen_comp->setCompErrorIndex(index);
}

void data_widget::changeGenMinComp()
{
    float temp = ui->lineEdit_gen_mincomp->text().toFloat();
    if (current_gen_comp != nullptr)
        current_gen_comp->setMinComp(temp);
}

void data_widget::changeGenBins(int numBins)
{
    if (numBins < 0)
        ui->spinBox_gen_num_bins->setValue(0);
    else if (current_gen_comp != nullptr)
    {
        int method = ui->spinBox_gen_comp->value();
        current_gen_comp->setNumberBins(numBins);
        for (int i = 0; i < model_data->get_num_fleets(); i++)
            model_data->getFleet(i)->setGenNumBins(method-1, numBins);
    }
}

void data_widget::showGenObs()
{
    int in = ui->spinBox_gen_comp->value();
    emit showGenObs(in);
}

void data_widget::setDoMorphs(bool flag)
{
    ui->groupBox_comp_morph->setChecked(flag);
    changeDoMorphs(flag);
}

void data_widget::changeDoMorphs(bool flag)
{
    model_data->setDoMorphComp(flag);
    if (flag)
    {
        ui->groupBox_comp_morph->setChecked(true);
        ui->spinBox_morph_num_stocks->setValue(model_data->get_morph_composition()->getNumberMorphs());
//        ui->lineEdit_morph_min_comp->setText(QString::number(model_data->get_morph_composition()->mincomp()));
    }
    else
    {
        ui->groupBox_comp_morph->setChecked(false);
        ui->spinBox_morph_num_stocks->setValue(0);
        ui->lineEdit_morph_min_comp->setText("0");
    }
}

void data_widget::changeNumMorphs(int num)
{
    model_data->get_morph_composition()->setNumberMorphs(num);
    for (int i = 0; i < model_data->get_num_fleets(); i++)
        model_data->getFleet(i)->setMorphNumMorphs(num);
}

void data_widget::changeMorphMincomp()
{
    QString val = ui->lineEdit_morph_min_comp->text();
    for (int i = 0; i < model_data->get_num_fleets(); i++)
        model_data->getFleet(i)->setMorphMinTailComp(val);
}

void data_widget::setDoTags(int flag)
{
    ui->spinBox_do_tags->setValue(flag);
    changeDoTags(flag);
}

void data_widget::changeDoTags(int flag)
{
    model_data->setDoTags(flag);
    if (flag > 0)
    {
        ui->groupBox_tag->setEnabled(true);
        ui->scrollArea_tagParams->setEnabled(true);
        ui->spinBox_tag_num_groups->setValue(model_data->getNumTagGroups());
        ui->spinBox_tag_latency->setValue(model_data->getTagLatency());
        ui->spinBox_tag_max_per->setValue(model_data->getTagMaxPeriods());
        tagGroups->setModel(model_data->getTagObservations());
        tagGroups->setHeight(model_data->getTagObservations());
        tagGroups->resizeColumnsToContents();
        ui->scrollAreaWidgetContents_tagParams->setEnabled(true);
        if (flag > 1)
        {
            ui->spinBox_min_recaps->setValue(model_data->getMinRecap());
            ui->spinBox_min_recaps->setVisible(true);
            ui->label_min_recaps->setVisible(true);
            ui->label_min_recaps_info->setVisible(true);
        }
        else
        {
            ui->spinBox_min_recaps->setVisible(false);
            ui->label_min_recaps->setVisible(false);
            ui->label_min_recaps_info->setVisible(false);
        }
    }
    else
    {
        ui->groupBox_tag->setEnabled(false);
        ui->scrollArea_tagParams->setEnabled(false);
        ui->spinBox_tag_num_groups->setValue(0);
        ui->spinBox_tag_latency->setValue(0);
        ui->spinBox_tag_max_per->setValue(0);
        tagGroups->setModel(nullptr);
        ui->scrollAreaWidgetContents_tagParams->setEnabled(false);
        ui->spinBox_min_recaps->setVisible(false);
        ui->label_min_recaps->setVisible(false);
        ui->label_min_recaps_info->setVisible(false);
    }
}

void data_widget::changeNumTagGrps(int num)
{
    model_data->setNumTagGroups(num);
    tagGroups->setHeight(num);
}

void data_widget::changeLambdas()
{
    int count = model_data->getLambdaAdjustmentModel()->rowCount();
    lambdaView->setHeight(model_data->getNumLambdaAdjustments());
    lambdaView->resizeColumnsToContents();
    setLambdaCount(count);
}

void data_widget::setLambdaMaxPhase(int value)
{
    ui->spinBox_lambda_max_phase->setValue(value);
}
void data_widget::changeLambdaMaxPhase(int value)
{
    model_data->setLambdaMaxPhase(value);
}
void data_widget::setLambdaSdOffset(int value)
{
    ui->spinBox_lambda_sd_offset->setValue(value);
}
void data_widget::changeLambdaSdOffset(int value)
{
    model_data->setLambdaSdOffset(value);
}
void data_widget::setLambdaCount(int value)
{
    ui->spinBox_lambda_count->setValue(value);
}
void data_widget::changeLambdaCount(int value)
{
    model_data->setNumLambdaAdjustments(value);
    model_data->setLambdaNumChanges(value);
}

void data_widget::setBlockPattern(int num)
{
    if (model_data->getNumBlockPatterns() > 0)
    {
        ui->spinBox_block_pattern_num->setValue(num);
    }
    else
    {
        ui->spinBox_block_pattern_num->setValue(0);
        ui->spinBox_blocks_count->setValue(0);
    }
}

void data_widget::changeNumBlockPatterns(int num)
{
    model_data->setNumBlockPatterns(num);
//    setBlockPattern(num);
}

void data_widget::changeBlockPattern(int num)
{
    int numpat = model_data->getNumBlockPatterns();
    if (numpat > 0)
    {
        if (num > numpat)
            ui->spinBox_block_pattern_num->setValue(numpat);
        else if (num < 1)
            ui->spinBox_block_pattern_num->setValue(1);
        else
        {
            timeBlocks->setModel(model_data->getBlockPattern(num-1)->getBlocks());
            timeBlocks->setHeight(model_data->getBlockPattern(num-1)->getBlocks());
            ui->spinBox_blocks_count->setValue(model_data->getBlockPattern(num-1)->getNumBlocks());
        }
    }
    else
    {
        ui->spinBox_block_patterns_total->setValue(0);
        ui->spinBox_block_pattern_num->setValue(0);
    }
}

void data_widget::changeNumBlocks(int num)
{
    int bpnum = ui->spinBox_block_pattern_num->value();
    if (bpnum > 0)
    {
        BlockPattern *bp = model_data->getBlockPattern(bpnum - 1);
        bp->setNumBlocks(num);
        timeBlocks->setHeight(model_data->getBlockPattern(bpnum-1)->getBlocks());
    }
}

void data_widget::changeJitter()
{
    QString value(ui->lineEdit_jitter->text());
    bool okay = true;
    double fl = value.toDouble(&okay);
    while (!okay)
    {
        value.truncate(value.count() - 1);
        fl = value.toDouble(&okay);
    }
    model_data->set_jitter_param(fl);
    ui->lineEdit_jitter->setText(QString::number(fl));
}

void data_widget::changeAlkTol()
{
    QString value(ui->lineEdit_alktol->text());
    bool okay = true;
    float fl = value.toFloat(&okay);
    while (!okay)
    {
        value.truncate(value.count() - 1);
        fl = value.toFloat(&okay);
    }
    model_data->setALKTol(fl);
    ui->lineEdit_alktol->setText(QString::number(fl));
}

void data_widget::changeConvergence()
{
    QString value(ui->lineEdit_convergence->text());
    bool okay = true;
    double fl = value.toDouble(&okay);
    if (okay)
        model_data->set_convergence_criteria(fl);
    else
        while (!okay)
        {
            value.truncate(value.count() - 1);
            fl = value.toDouble(&okay);
        }
    ui->lineEdit_convergence->setText(QString::number(fl));
}

void data_widget::changeDepDenom()
{
    QString value(ui->lineEdit_dep_denom->text());
    double db = checkdoublevalue(value);
    model_data->set_depletion_denom(db);
    ui->lineEdit_dep_denom->setText(QString::number(db));
}

void data_widget::changeRandSeed(int seed)
{
    model_data->setRandSeed(seed);
}

void data_widget::setRandSeed(int seed)
{
    ui->spinBox_rseed->setValue(seed);
}

void data_widget::changeDepletionBasis(int base, int multi, bool uselog)
{
    int log = uselog? 100: 0;
    int basis = log + (multi * 10) + base;
    model_data->set_depletion_basis(basis);
    ui->spinBox_f_dep->setValue(basis);
}

void data_widget::changeDepletionBase(int base)
{
//    int base = ui->comboBox_f_dep->currentIndex();
    int multi = ui->spinBox_f_dep_multi->value();
    bool uselog = ui->checkBox_f_dep_log->isChecked();
    changeDepletionBasis(base, multi, uselog);
}

void data_widget::setDepletionBasis(int basis)
{
    int multi = 0;
    int base = basis - 100;
    bool uselog = false;
    ui->spinBox_f_dep->setValue(basis);
    if (base > 0)
    {
        uselog = true;
        basis = base;
    }
    if (basis > 9)
    {
        multi = basis / 10;
        basis = basis - multi;
    }
    ui->comboBox_f_dep->setCurrentIndex(basis);
    ui->spinBox_f_dep_multi->setValue(multi);
    ui->checkBox_f_dep_log->setChecked(uselog);
}

void data_widget::changeDepletionMulti(int multi)
{
    int base = ui->comboBox_f_dep->currentIndex();
//    int multi = ui->spinBox_f_dep_multi->value();
    bool uselog = ui->checkBox_f_dep_log->isChecked();
    changeDepletionBasis(base, multi, uselog);
}

void data_widget::changeDepletionLog(int state)
{
    int base = ui->comboBox_f_dep->currentIndex();
    int multi = ui->spinBox_f_dep_multi->value();
    bool uselog = ui->checkBox_f_dep_log->isChecked();
    changeDepletionBasis(base, multi, uselog);
}

void data_widget::changeFRptBasis(int base, int multi, bool uselog)
{
    int log = uselog? 100: 0;
    int basis = log + (multi * 10) + base;
    model_data->set_f_basis(basis);
    ui->spinBox_f_rpt_basis->setValue(basis);
}

void data_widget::changeFRptBase(int base)
{
//    int base = ui->comboBox_f_rpt_base->currentIndex();
    int multi = ui->spinBox_f_rpt_multi->value();
    bool uselog = ui->checkBox_f_rpt_log->isChecked();
    changeFRptBasis(base, multi, uselog);
}

void data_widget::setFRptBasis(int basis)
{
    int multi = 0;
    int base = basis;
    bool uselog = ((basis - 100) > 0);
    ui->spinBox_f_rpt_basis->setValue(basis);
    if (uselog)
    {
        basis -= 100;
    }
    if (basis > 9)
    {
        multi = basis / 10;
        base = basis - multi;
    }
    ui->comboBox_f_rpt_base->setCurrentIndex(base);
    ui->spinBox_f_rpt_multi->setValue(multi);
    ui->checkBox_f_rpt_log->setChecked(uselog);
}

void data_widget::changeFRptMulti(int multi)
{
    int base = ui->comboBox_f_rpt_base->currentIndex();
//    int multi = ui->spinBox_f_rpt_multi->value();
    bool uselog = ui->checkBox_f_rpt_log->isChecked();
    changeFRptBasis(base, multi, uselog);
}

void data_widget::changeFRptLog(int state)
{
    int base = ui->comboBox_f_rpt_base->currentIndex();
    int multi = ui->spinBox_f_rpt_multi->value();
    bool uselog = ui->checkBox_f_rpt_log->isChecked();
    changeFRptBasis(base, multi, uselog);
}

void data_widget::addLengthDirichlet()
{
    QStringList ql;
    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    int num = model_data->get_length_composition()->getNumDirichletParams();
    model_data->get_length_composition()->setDirichletParam(num, ql);
    lengthDirichlet->setHeight(num+1);
}

void data_widget::addAgeDirichlet()
{
    QStringList ql;
    ql << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
    int num = model_data->get_age_composition()->getNumDirichletParams();
    model_data->get_age_composition()->setDirichletParam(num, ql);
    ageDirichlet->setHeight(num+1);
}

void data_widget::tagInitChanged()
{
    tagInit->setHeight(model_data->getTagLossInit()->getParamTable());
    tagInit->resizeColumnsToContents();
    tagInitTVChanged();
}
void data_widget::tagInitTVChanged()
{
    int num = model_data->getTagLossInitTV()->getNumParams();
    ui->label_tagInitTV->setVisible(num > 0);
    tagInitTV->setVisible(num > 0);
    tagInitTV->setHeight(num);
    tagInitTV->resizeColumnsToContents();
}

void data_widget::tagChronicChanged()
{
    tagChronic->setHeight(model_data->getTagLossChronic()->getParamTable());
    tagChronic->resizeColumnsToContents();
    tagChronicTVChanged();
}
void data_widget::tagChronicTVChanged()
{
    int num = model_data->getTagLossChronicTV()->getNumParams();
    ui->label_tagChronicTV->setVisible(num > 0);
    tagChronicTV->setVisible(num > 0);
    tagChronicTV->setHeight(num);
    tagChronicTV->resizeColumnsToContents();
}

void data_widget::tagOverdispChanged()
{
    tagOverdisp->setHeight(model_data->getTagOverdispersion()->getParamTable());
    tagOverdisp->resizeColumnsToContents();
    tagOverdispTVChanged();
}
void data_widget::tagOverdispTVChanged()
{
    int num = model_data->getTagOverdispersionTV()->getNumParams();
    ui->label_tagOverdispTV->setVisible(num > 0);
    tagOverdispTV->setVisible(num > 0);
    tagOverdispTV->setHeight(num);
    tagOverdispTV->resizeColumnsToContents();
}
void data_widget::tagRptFltChanged()
{
    tagReptFleet->setHeight(model_data->getTagReportFleet()->getParamTable());
    tagReptFleet->resizeColumnsToContents();
    tagRptFltTVChanged();
}
void data_widget::tagRptFltTVChanged()
{
    int num = model_data->getTagReportFleetTV()->getNumParams();
    ui->label_tagReptFleetTV->setVisible(num > 0);
    tagReptFleetTV->setVisible(num > 0);
    tagReptFleetTV->setHeight(num);
    tagReptFleetTV->resizeColumnsToContents();
}
void data_widget::tagRptDcayChanged()
{
    tagReptFltDecay->setHeight(model_data->getTagReportDecay()->getParamTable());
    tagReptFltDecay->resizeColumnsToContents();
    tagRptDcayTVChanged();
}
void data_widget::tagRptDcayTVChanged()
{
    int num = model_data->getTagReportDecayTV()->getNumParams();
    ui->label_tagReptDecayFltTV->setVisible(num > 0);
    tagReptDecayFltTV->setVisible(num > 0);
    tagReptDecayFltTV->setHeight(num);
    tagReptDecayFltTV->resizeColumnsToContents();
}

bool data_widget::get2DAR1() {
    return ui->checkBox_2DAR1->isChecked();
}

void data_widget::set2DAR1(bool use) {
    ui->checkBox_2DAR1->setChecked(use);
}

void data_widget::change2DAR1(bool use) {
    model_data->setUse2DAR1(use);
}
