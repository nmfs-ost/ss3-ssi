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
    ui->horizontalLayout_age_error->addWidget(ageError);

    genBins = new tableview();
    genBins->setParent(this);
    ui->horizontalLayout_gen_bins->addWidget(genBins);
    current_gen_comp = nullptr;
    connect (ui->spinBox_gen_comp, SIGNAL(valueChanged(int)), SLOT(changeGenCompMethod(int)));
    connect (ui->pushButton_gen_new, SIGNAL(released()), SLOT(newGenCompMethod()));
    connect (ui->pushButton_gen_copy, SIGNAL(released()), SLOT(copyGenCompMethod()));
    connect (ui->pushButton_gen_delete, SIGNAL(released()), SLOT(deleteGenCompMethod()));
    connect (ui->spinBox_gen_units, SIGNAL(valueChanged(int)), SLOT(changeGenUnits(int)));
    connect (ui->spinBox_gen_scale, SIGNAL(valueChanged(int)), SLOT(changeGenScale(int)));
    connect (ui->lineEdit_gen_mincomp, SIGNAL(editingFinished()), SLOT(changeGenMinComp()));
    connect (ui->spinBox_gen_num_bins, SIGNAL(valueChanged(int)), SLOT(changeGenBins(int)));

    tagGroups = new tableview();
    tagGroups->setParent(this);
    tagGroups->setModel(model_data->get_tag_observations());
    tagGroups->setHeight(model_data->get_tag_observations());
    tagGroups->resizeColumnsToContents();
    ui->horizontalLayout_tag_reldata->addWidget(tagGroups);
    connect (ui->groupBox_tag, SIGNAL(toggled(bool)), SLOT(changeDoTags(bool)));
    connect (ui->spinBox_tag_num_groups, SIGNAL(valueChanged(int)), SLOT(changeNumTagGrps(int)));
    connect (ui->spinBox_tag_latency, SIGNAL(valueChanged(int)), model_data, SLOT(set_tag_latency(int)));
    connect (ui->spinBox_tag_max_per, SIGNAL(valueChanged(int)), model_data, SLOT(set_tag_max_periods(int)));
    connect (ui->pushButton_tag_rec_obs, SIGNAL(clicked()), SIGNAL(showRecapObs()));
    connect (ui->spinBox_tag_time_vary, SIGNAL(valueChanged(int)), model_data, SLOT(setTagTimeVaryReadParams(int)));

    tagInit = new tableview();
    tagInit->setParent(this);
    tagInit->setModel(model_data->getTagLossInit()->getParamTable());
    ui->verticalLayout_tagLossInit->addWidget(tagInit);
    connect (model_data->getTagLossInit()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagInitChanged()));
    tagInitTV = new tableview();
    tagInitTV->setParent(this);
    tagInitTV->setModel(model_data->getTagLossInitTV()->getVarParamTable());
    ui->verticalLayout_tagInitTV->addWidget(tagInitTV);
    connect (model_data->getTagLossInitTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagInitTVChanged()));
    tagInitChanged();

    tagChronic = new tableview();
    tagChronic->setParent(this);
    tagChronic->setModel(model_data->getTagLossChronic()->getParamTable());
    ui->verticalLayout_tagLossChronic->addWidget(tagChronic);
    connect (model_data->getTagLossChronic()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagChronicChanged()));
    tagChronicTV = new tableview();
    tagChronicTV->setParent(this);
    tagChronicTV->setModel(model_data->getTagLossChronicTV()->getVarParamTable());
    ui->verticalLayout_tagChronicTV->addWidget(tagChronicTV);
    connect (model_data->getTagLossChronicTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagChronicTVChanged()));
    tagChronicChanged();

    tagOverdisp = new tableview();
    tagOverdisp->setParent(this);
    tagOverdisp->setModel(model_data->getTagOverdispersion()->getParamTable());
    ui->verticalLayout_tagOverdisp->addWidget(tagOverdisp);
    connect (model_data->getTagOverdispersion()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagOverdispChanged()));
    tagOverdispTV = new tableview();
    tagOverdispTV->setParent(this);
    tagOverdispTV->setModel(model_data->getTagOverdispersionTV()->getVarParamTable());
    ui->verticalLayout_tagOverdispTV->addWidget(tagOverdispTV);
    connect (model_data->getTagOverdispersionTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagOverdispTVChanged()));
    tagOverdispChanged();

    tagReptFleet = new tableview();
    tagReptFleet->setParent(this);
    tagReptFleet->setModel(model_data->getTagReportFleet()->getParamTable());
    ui->verticalLayout_tagReptFleet->addWidget(tagReptFleet);
    connect (model_data->getTagReportFleet()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptFltChanged()));
    tagReptFleetTV = new tableview();
    tagReptFleetTV->setParent(this);
    tagReptFleetTV->setModel(model_data->getTagReportFleetTV()->getVarParamTable());
    ui->verticalLayout_tagReptFleetTV->addWidget(tagReptFleetTV);
    connect (model_data->getTagReportFleetTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptFltTVChanged()));
    tagRptFltChanged();

    tagReptFltDecay = new tableview();
    tagReptFltDecay->setParent(this);
    tagReptFltDecay->setModel(model_data->getTagReportDecay()->getParamTable());
    ui->verticalLayout_tagReptDecayFlt->addWidget(tagReptFltDecay);
    connect (model_data->getTagReportDecay()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptDcayChanged()));
    tagReptDecayFltTV = new tableview();
    tagReptDecayFltTV->setParent(this);
    tagReptDecayFltTV->setModel(model_data->getTagReportDecayTV()->getVarParamTable());
    ui->verticalLayout_tagReptDecayFltTV->addWidget(tagReptDecayFltTV);
    connect (model_data->getTagReportDecayTV()->getVarParamTable(), SIGNAL(dataChanged()), this, SLOT(tagRptDcayTVChanged()));
    tagRptDcayChanged();


    envVariables = new tableview();
    envVariables->setParent(this);
    envVariables->setModel(model_data->getEnvVariables());
    envVariables->setHeight(model_data->getEnvVariables());
    ui->verticalLayout_env_var_obs->addWidget(envVariables);
    connect (ui->spinBox_env_var_obs, SIGNAL(valueChanged(int)), SLOT(changeNumEnvVarObs(int)));
    connect (ui->spinBox_num_evn_var, SIGNAL(valueChanged(int)), model_data, SLOT(setNumEnvironVars(int)));
    connect (model_data->getEnvVariables(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
             SLOT(changeEnvVarData(QModelIndex,QModelIndex,QVector<int>)));

    timeBlocks = new tableview();
    timeBlocks->setParent(this);
    timeBlocks->setModel(nullptr);
    ui->verticalLayout_block_years->addWidget(timeBlocks);

    lambdaView = new tableview();
    lambdaView->setParent(this);
    lambdaView->setModel(model_data->getLambdaAdjustmentModel());
    ui->verticalLayout_lambdas->addWidget(lambdaView);
    connect (model_data->getLambdaAdjustmentModel(), SIGNAL(dataChanged()), SLOT(changeLambdas()));
    changeLambdas();
    connect (ui->spinBox_lambda_max_phase, SIGNAL(valueChanged(int)), SLOT(changeLambdaMaxPhase(int)));
    connect (ui->spinBox_lambda_sd_offset, SIGNAL(valueChanged(int)), SLOT(changeLambdaSdOffset(int)));
    connect (ui->spinBox_lambda_count, SIGNAL(valueChanged(int)), SLOT(changeLambdaCount(int)));

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
    ui->verticalLayout_add_sd_NatAge->addWidget(addSdNumAtAgeView);
    addNumAtAgeBinsView = new tableview();
    addNumAtAgeBinsView->setParent(this);
    addNumAtAgeBinsView->setModel(model_data->getAddSdReporting()->getNumAtAgeAgeModel());
    addNumAtAgeBinsView->setHeight(1);
    ui->verticalLayout_add_sd_NatAge_ages->addWidget(addNumAtAgeBinsView);

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
    connect (ui->comboBox_f_dep, SIGNAL(currentIndexChanged(int)), model_data, SLOT(set_depletion_basis(int)));
    connect (ui->lineEdit_dep_denom, SIGNAL(editingFinished()), SLOT(changeDepDenom()));
    connect (ui->comboBox_spr_basis, SIGNAL(currentIndexChanged(int)), model_data, SLOT(set_spr_basis(int)));
    connect (ui->comboBox_f_rpt_units, SIGNAL(currentIndexChanged(int)), SLOT(setFRptUnits(int)));
    connect (ui->spinBox_f_min_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_f_min_age(int)));
    connect (ui->spinBox_f_max_age, SIGNAL(valueChanged(int)), model_data, SLOT(set_f_max_age(int)));
    connect (ui->comboBox_f_report_basis, SIGNAL(currentIndexChanged(int)), model_data, SLOT(set_f_basis(int)));
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

    connect (ui->pushButton_gen_obs, SIGNAL(clicked()), SLOT(showGenObs()));

    connect (ui->groupBox_comp_morph, SIGNAL(toggled(bool)), SLOT(changeDoMorphs(bool)));
    connect (ui->spinBox_morph_num_stocks, SIGNAL(valueChanged(int)), SLOT(changeNumMorphs(int)));
    connect (ui->lineEdit_morph_min_comp, SIGNAL(editingFinished()), SLOT(changeMorphMincomp()));
    connect (ui->pushButton_morph_obs, SIGNAL(clicked()), SIGNAL(showMorphObs()));

    connect (ui->spinBox_block_patterns_total, SIGNAL(valueChanged(int)), SLOT(changeNumBlockPatterns(int)));
    connect (ui->spinBox_block_pattern_num, SIGNAL(valueChanged(int)), SLOT(changeBlockPattern(int)));
    connect (ui->spinBox_blocks_count, SIGNAL(valueChanged(int)), SLOT(changeNumBlocks(int)));

    connect (ui->spinBox_add_sd_read, SIGNAL(valueChanged(int)), SLOT(changeAddSdRead(int)));

    ui->spinBox_num_std_yrs->setMaximum(20);
    setNumSdYears(0);

    refresh();

    ui->tabWidget->setCurrentIndex(0);
}

data_widget::~data_widget()
{
    delete sdYearsView;
    delete ui;
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
    if (model_data != nullptr)
    {
        ui->spinBox_year_start->setValue(model_data->get_start_year());
        ui->spinBox_year_end->setValue(model_data->get_end_year());
        changeTotalYears();
        ui->spinBox_seas_per_yr->setValue(model_data->get_num_seasons());
        ui->spinBox_subseasons->setValue(model_data->get_num_subseasons());
//
        ui->doubleSpinBox_spawn_month->setValue(model_data->get_spawn_month());
        ui->spinBox_season->setValue(1);
        setMoPerSeason(1);
        setTotalMonths();
        ui->spinBox_num_fisheries->setValue(model_data->get_num_fisheries());
        ui->spinBox_num_surveys->setValue(model_data->get_num_surveys());
        ui->spinBox_total_fleets->setValue(model_data->get_num_fleets());
        ui->spinBox_num_areas->setValue(model_data->get_num_areas());
//        ui->spinBox_num_ages->setValue(model_data->num_ages());
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
        ui->comboBox_f_dep->setCurrentIndex(model_data->get_depletion_basis());
        ui->lineEdit_dep_denom->setText(QString::number(model_data->get_depletion_denom()));
        ui->comboBox_spr_basis->setCurrentIndex(model_data->get_spr_basis());
        ui->comboBox_f_rpt_units->setCurrentIndex(model_data->get_f_units());
        ui->spinBox_f_min_age->setValue(model_data->get_f_min_age());
        ui->spinBox_f_max_age->setValue(model_data->get_f_max_age());
        ui->comboBox_f_report_basis->setCurrentIndex(model_data->get_f_basis());

        ui->spinBox_sdrpt_min_yr->setValue(model_data->bio_sd_min_year());
        ui->spinBox_sdrpt_max_yr->setValue(model_data->bio_sd_max_year());
        ui->spinBox_num_std_yrs->setValue(model_data->get_num_std_years());
        sdYearsView->setModel(model_data->sdYearsModel);
        sdYearsView->resizeColumnsToContents();

/*        ui->spinBox_mbwt_df->setValue(model_data->mean_body_wt_df());
        setMBWTObs(model_data->mean_body_wt_count());
        mbweightview->setModel(model_data->getMeanBwtModel());
        mbweightview->resizeColumnsToContents();*/

        setLengthCompMethod(model_data->get_length_composition()->getAltBinMethod());
        setLengthBins (model_data->get_length_composition()->getNumberBins());
        lengthBins->setModel(model_data->get_length_composition()->getBinsModel());
        lengthBins->setHeight(1);
        lengthBins->resizeColumnsToContents();
//        lengthObs->setModel(model_data->get_length_composition()->getObsModel());
//        lengthObs->resizeColumnsToContents();
        lengthDirichlet->setModel(model_data->get_length_composition()->getDirichletParamTable());
        lengthDirichlet->setHeight(model_data->get_length_composition()->getDirichletParamTable());
        lengthDirichlet->resizeColumnsToContents();


        ui->spinBox_age_bin_method->setValue(model_data->get_age_composition()->getAltBinMethod());
        setAgeBins(model_data->get_age_composition()->getNumberBins());
        setAgeError(model_data->get_age_composition()->number_error_defs());
        ageBins->setModel(model_data->get_age_composition()->getBinsModel());
        ageBins->setHeight(1);
        ageBins->resizeColumnsToContents();
        QSize size (ageBins->size());
        size.setHeight(40);
        ageBins->resize(size);
        ageError->setModel(model_data->get_age_composition()->getErrorModel());
        ageError->setHeight(model_data->get_age_composition()->getErrorModel());
        ageError->resizeColumnsToContents();
//        ageObs->setModel(model_data->get_age_composition()->getObsModel());
//        ageObs->resizeColumnsToContents();
        ageDirichlet->setModel(model_data->get_age_composition()->getDirichletParamTable());
        ageDirichlet->setHeight(model_data->get_age_composition()->getDirichletParamTable());
        ageDirichlet->resizeColumnsToContents();

        ui->label_gen_comp_total->setText(QString::number(model_data->getNumGeneralCompMethods()));
        setGenCompMethod(0);

        setDoMorphs(model_data->getDoMorphComp());

        setDoTags(model_data->get_do_tags());
        ui->spinBox_tag_time_vary->setValue(model_data->getTagTimeVaryReadParams());

        setLambdaMaxPhase(model_data->getLambdaMaxPhase());
        setLambdaSdOffset(model_data->getLambdaSdOffset());
        setLambdaCount(model_data->getNumLambdaAdjustments());

        ui->spinBox_env_var_obs->setValue(model_data->getNumEnvironVarObs());
        ui->spinBox_num_evn_var->setValue(model_data->getNumEnvironVars());

        ui->spinBox_block_patterns_total->setValue(model_data->getNumBlockPatterns());
        setBlockPattern(1);

//        ui->groupBox_add_sd->setChecked(model_data->getAddSdReporting()->getActive());
        ui->spinBox_add_sd_read->setValue (model_data->getAddSdReporting()->getActive());
        changeAddSdRead(model_data->getAddSdReporting()->getActive());

/*        addSdSelexView->setModel(model_data->getAddSdReporting()->getSpecModel());
        addSdSelexView->setHeight(model_data->getAddSdReporting()->getSpecModel());
        addSdSelexView->resizeColumnsToContents();
        ui->verticalLayout_add_sd_spec->addWidget(addSdSelexView);
        addSelexBinsView->setModel(model_data->getAddSdReporting()->getNumAtAgeAgeModel());
        addSelexBinsView->setHeight(model_data->getAddSdReporting()->getNumAtAgeAgeModel());
        addSelexBinsView->resizeColumnsToContents();
        ui->verticalLayout_add_sd_bins->addWidget(addSelexBinsView);*/
        set2DAR1(model_data->getUse2DAR1());
    }
}

void data_widget::changeAddSdRead(int value) {
    model_data->getAddSdReporting()->setActive(value);
    if (value == 0) {
        ui->label_add_sd_selex->setEnabled(false);
        addSdSelexView->setEnabled(false);
        addSelexBinsView->setEnabled(false);

        ui->label_add_sd_growth->setEnabled(false);
        addSdGrowthView->setEnabled(false);
        addGrowthBinsView->setEnabled(false);

        ui->label_add_sd_NatAge->setEnabled(false);
        addSdNumAtAgeView->setEnabled(false);
        addNumAtAgeBinsView->setEnabled(false);

        ui->label_add_sd_natMort->setEnabled(false);
        addSdNatMortView->setEnabled(false);
        addSdNatMortBinsView->setEnabled(false);
    }
    else {
        ui->label_add_sd_selex->setEnabled(true);
        addSdSelexView->setEnabled(true);
        addSdSelexView->resizeColumnsToContents();
        addSelexBinsView->setEnabled(true);
        addSelexBinsView->resizeColumnsToContents();

        if (model_data->getReadWtAtAge() == 0) {
            ui->label_add_sd_growth->setEnabled(true);
            ui->label_add_sd_growth->setVisible(true);
            addSdGrowthView->setEnabled(true);
            addSdGrowthView->setVisible(true);
            addSdGrowthView->resizeColumnsToContents();
            addGrowthBinsView->setEnabled(true);
            addGrowthBinsView->setVisible(true);
            addGrowthBinsView->resizeColumnsToContents();
        }
        else {
            ui->label_add_sd_growth->setVisible(false);
            addSdGrowthView->setVisible(false);
            addGrowthBinsView->setVisible(false);
        }

        ui->label_add_sd_NatAge->setEnabled(true);
        addSdNumAtAgeView->setEnabled(true);
        addSdNumAtAgeView->resizeColumnsToContents();
        addNumAtAgeBinsView->setEnabled(true);
        addNumAtAgeBinsView->resizeColumnsToContents();

        if (value > 1) {
            ui->label_add_sd_natMort->setVisible(true);
            ui->label_add_sd_natMort->setEnabled(true);
            addSdNatMortView->setEnabled(true);
            addSdNatMortView->setVisible(true);
            addSdNatMortView->resizeColumnsToContents();
            addSdNatMortBinsView->setEnabled(true);
            addSdNatMortBinsView->setVisible(true);
            addSdNatMortBinsView->resizeColumnsToContents();
        }
        else {
            ui->label_add_sd_natMort->setVisible(false);
            addSdNatMortView->setVisible(false);
            addSdNatMortBinsView->setVisible(false);
        }
    }
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
    if (seas > ui->spinBox_seas_per_yr->value())
    {
        seas = setTotalMonths();//ui->spinBox_seas_per_yr->value();
//        ui->doubleSpinBox_spawn_season->setValue(seas);
    }
    model_data->set_spawn_season(seas);
}

void data_widget::changeTotalYears()
{
    int start = ui->spinBox_year_start->value();
    int end = ui->spinBox_year_end->value();
    ui->spinBox_year_total->setValue(end - start + 1);
}

void data_widget::setFRptUnits(int val)
{
    bool flag = false;
    if (val == 4)
    {
        flag = true;
    }
    ui->label_f_min_age->setVisible(flag);
    ui->spinBox_f_min_age->setVisible(flag);
    ui->label_f_max_age->setVisible(flag);
    ui->spinBox_f_max_age->setVisible(flag);
    model_data->set_f_units(val);
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
    }
    else
    {
        ageBins->hide();
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

void data_widget::setDoTags(bool flag)
{
    ui->groupBox_tag->setChecked(flag);
    changeDoTags(flag);
    ui->scrollArea_tagParams->setEnabled(flag);
}

void data_widget::changeDoTags(bool flag)
{
    model_data->set_do_tags(flag);
    if (flag)
    {
        ui->spinBox_tag_num_groups->setValue(model_data->get_num_tag_groups());
        ui->spinBox_tag_latency->setValue(model_data->get_tag_latency());
        ui->spinBox_tag_max_per->setValue(model_data->get_tag_max_periods());
        tagGroups->setModel(model_data->get_tag_observations());
        tagGroups->setHeight(model_data->get_tag_observations());
        tagGroups->resizeColumnsToContents();
    }
    else
    {
        ui->spinBox_tag_num_groups->setValue(0);
        ui->spinBox_tag_latency->setValue(0);
        ui->spinBox_tag_max_per->setValue(0);
        tagGroups->setModel(nullptr);
    }
    ui->scrollAreaWidgetContents_tagParams->setEnabled(flag);
}

void data_widget::changeNumTagGrps(int num)
{
    model_data->set_num_tag_groups(num);
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
//    int seed = ui->spinBox_rseed->value();
    model_data->setRandSeed(seed);
}

void data_widget::setRandSeed(int seed)
{
    ui->spinBox_rseed->setValue(seed);
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
