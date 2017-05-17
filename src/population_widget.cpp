#include <QWidget>

#include "population_widget.h"
#include "ui_population_widget.h"
#include "conversions.h"

population_widget::population_widget(ss_model *m_data, QWidget *parent) :
    QWidget(parent), ui(new Ui::population_widget)
{
    ui->setupUi(this);
    model_data = m_data;
    pop = model_data->pPopulation;
    currPattern = NULL;
    spwn_rcr = pop->SR();
    movemnt = pop->Move();
    fecund = pop->Fec();
    grwth = pop->Grow();
    mort = pop->M();

//    ui->verticalLayout_fishingMort_2_detail->addWidget();

    // Growth
    growthMorphDistView = new tableview();
    growthMorphDistView->setParent(this);
//    growthMorphDistView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->horizontalLayout_growth_submorph_dist->addWidget(growthMorphDistView);
    growthParamsView = new tableview();
    growthParamsView->setParent(this);
    ui->verticalLayout_growth_params->addWidget(growthParamsView);
    growthTVParamsView = new tableview();
    growthTVParamsView->setParent(this);
    ui->verticalLayout_growth_time_vary_params->addWidget(growthTVParamsView);

    cvParamsView = new tableview();
    cvParamsView->setParent(this);
    ui->verticalLayout_cv_params->addWidget(cvParamsView);

    // Recruitment
    recruitAssignParamsView = new tableview();
    recruitAssignParamsView->setParent(this);
    ui->horizontalLayout_recr_assigns->addWidget(recruitAssignParamsView);

    recruitDistView = new tableview();
    recruitDistView->setParent(this);
    ui->verticalLayout_recr_dist_params->addWidget(recruitDistView);

    recruitTVParamsView = new tableview();
    recruitTVParamsView->setParent(this);
    ui->verticalLayout_recr_time_vary_params->addWidget(recruitTVParamsView);

    recruitParamsView = new tableview();
    recruitParamsView->setParent(this);
    ui->verticalLayout_recr_params->addWidget(recruitParamsView);

    recruitCyclesParamsView = new tableview();
    recruitCyclesParamsView->setParent(this);
    ui->horizontalLayout_recr_period_params->addWidget(recruitCyclesParamsView);

    recruitDevsView = new tableview();
    recruitDevsView->setParent(this);
    ui->verticalLayout_recr_devs->addWidget(recruitDevsView);

    setRecrArea(2);
    connect (ui->radioButton_area, SIGNAL(clicked()), SLOT(changeRecrArea()));
    connect (ui->radioButton_global, SIGNAL(clicked()), SLOT(changeRecrArea()));
//    setRecrDistParam(1);
    ui->checkBox_recr_interaction->setChecked(false);
    connect (ui->comboBox_recr_spec, SIGNAL(currentIndexChanged(int)), SLOT(changeSpawnRecrSpec(int)));
    connect (ui->spinBox_spwn_recr_time_vary_read, SIGNAL(valueChanged(int)), SLOT(changeSpwnRecReadTimeVary(int)));
    connect (ui->spinBox_recr_num_assigns, SIGNAL(valueChanged(int)), SLOT(changeRecNumAssigns(int)));
    connect (ui->spinBox_recr_dist_params, SIGNAL(valueChanged(int)), SLOT(changeRecrDistParam(int)));
    connect (ui->checkBox_recr_interaction, SIGNAL(toggled(bool)), SLOT(changeRecAssignInteract(bool)));
    connect (ui->spinBox_sr_do_recr_dev, SIGNAL(valueChanged(int)), SLOT(changeRecDoDev(int)));
    connect (ui->spinBox_sr_recr_dev_begin_yr, SIGNAL(valueChanged(int)), SLOT(changeRecrDevStartYr(int)));
    connect (ui->spinBox_sr_recr_dev_end_yr, SIGNAL(valueChanged(int)), SLOT(changeRecrDevEndYr(int)));
    connect (ui->spinBox_sr_recr_dev_phase, SIGNAL(valueChanged(int)), SLOT(changeRecrDevPhase(int)));
    connect (ui->groupBox_recr_dev_adv_opt, SIGNAL(toggled(bool)), SLOT(changeRecrDoAdvancedOpts(bool)));
    connect (ui->spinBox_recr_dev_early_start, SIGNAL(valueChanged(int)), SLOT(changeRecrDevEarlyStart (int)));
    connect (ui->spinBox_recr_dev_early_phase, SIGNAL(valueChanged(int)), SLOT(changeRecrDevEarlyPhase(int)));
    connect (ui->spinBox_recr_dev_fcast_phase, SIGNAL(valueChanged(int)), SLOT(changeRecrFcastPhase(int)));
    connect (ui->lineEdit_recr_dev_fcast_lambda, SIGNAL(textChanged(QString)), SLOT(changeRecrFcastLambda(QString)));
    connect (ui->spinBox_recr_dev_last_nobias, SIGNAL(valueChanged(int)), SLOT(changeRecrNoBiasLastYr(int)));
    connect (ui->spinBox_recr_dev_first_bias, SIGNAL(valueChanged(int)), SLOT(changeRecrFullBiasFirstYr(int)));
    connect (ui->spinBox_recr_dev_last_bias, SIGNAL(valueChanged(int)), SLOT(changeRecrFullBiasLastYr(int)));
    connect (ui->spinBox_recr_dev_first_nobias, SIGNAL(valueChanged(int)), SLOT(changeRecrNoBiasFirstYr(int)));
    connect (ui->lineEdit_recr_dev_max_bias, SIGNAL(textChanged(QString)), SLOT(changeRecrBiasMax(QString)));
    connect (ui->spinBox_recr_dev_cycles, SIGNAL(valueChanged(int)), SLOT(changeRecrCycles(int)));
    connect (ui->lineEdit_recr_dev_min_dev, SIGNAL(textChanged(QString)), SLOT(changeRecrDevMin(QString)));
    connect (ui->lineEdit_recr_dev_max_dev, SIGNAL(textChanged(QString)), SLOT(changeRecrDevMax(QString)));
    connect (ui->spinBox_num_recr_devs, SIGNAL(valueChanged(int)), SLOT(changeRecrNumDevs(int)));

    wtlenParamsView = new tableview();
    wtlenParamsView->setParent(this);
    ui->verticalLayout_wtlen_params->addWidget(wtlenParamsView);
    wtlenTVParamsView = new tableview();
    wtlenTVParamsView->setParent(this);
    ui->verticalLayout_wtlen_time_vary_params->addWidget(wtlenTVParamsView);
    maturityParamsView = new tableview();
    maturityParamsView->setParent(this);
    ui->verticalLayout_cohort_params->addWidget(maturityParamsView);
    maturityTVParamsView = new tableview();
    maturityTVParamsView->setParent(this);
    ui->verticalLayout_cohort_time_vary_params->addWidget(maturityTVParamsView);

//    timeVaryParamsView = new tableview();
//    timeVaryParamsView->setParent(this);
//    ui->verticalLayout_growth_time_vary_params->addWidget(timeVaryParamsView);

    // Maturity

    connect (ui->comboBox_maturity_option, SIGNAL(currentIndexChanged(int)), SLOT(changeMaturityOpt(int)));
    connect (ui->lineEdit_maturity_first_age, SIGNAL(editingFinished()), SLOT(changeFirstMatureAge()));
    connect (ui->checkBox_wtatage, SIGNAL(toggled(bool)), SIGNAL(readWtAtAgeSS(bool)));

    // Fecundity
    fractionFemView = new tableview();
    fractionFemView->setParent(this);
    ui->verticalLayout_fraction_female->addWidget(fractionFemView);

    fecundParamsView = new tableview();
    fecundParamsView->setParent(this);
    ui->verticalLayout_fecund_params->addWidget(fecundParamsView);
    fecundTVParamsView = new tableview();
    fecundTVParamsView->setParent(this);
    ui->verticalLayout_fecund_time_vary_params->addWidget(fecundTVParamsView);

    ui->spinBox_fecund_hermaph_season->setMinimum(-1);

    hermaphParamsView = new tableview();
    hermaphParamsView->setParent(this);
    ui->verticalLayout_hermaph_params->addWidget(hermaphParamsView);

//    connect (ui->lineEdit_fraction_female, SIGNAL(editingFinished()), SLOT(changeFractionFemale()));
    connect (ui->comboBox_fecund_option, SIGNAL(currentIndexChanged(int)), SLOT(changeFecundityOption(int)));
    connect (ui->groupBox_hermaphroditism, SIGNAL(toggled(bool)), SLOT(changeHermaph(bool)));
    connect (ui->spinBox_fecund_hermaph_season, SIGNAL(valueChanged(int)), SLOT(changeHermaphSeas(int)));
    connect (ui->spinBox_fecund_hermaph_male, SIGNAL(valueChanged(int)), SLOT(changeHermaphMales(int)));
    connect (ui->comboBox_fecund_gend_offset, SIGNAL(currentIndexChanged(int)), SLOT(changeFecundityOffsetOption(int)));
//    connect (ui->comboBox_fecund_adj_constraint, SIGNAL(currentIndexChanged(int)), SLOT(changeFecundityAdjustment(int)));


    connect (ui->doubleSpinBox_growth_morph_ratio, SIGNAL(valueChanged(double)), SLOT(changeGrowthMorphRatio(double)));
    connect (ui->spinBox_bio_time_vary_read, SIGNAL(valueChanged(int)), SLOT(changeGrowthTimeVaryRead(int)));
    connect (ui->comboBox_growth_model, SIGNAL(currentIndexChanged(int)), SLOT(changeGrowthModel(int)));
    connect (ui->doubleSpinBox_growth_age_Amin, SIGNAL(valueChanged(double)), SLOT(changeAmin (double)));
    connect (ui->doubleSpinBox_growth_age_Amax, SIGNAL(valueChanged(double)), SLOT(changeAmax (double)));
    connect (ui->doubleSpinBox_exp_decay, SIGNAL(valueChanged(double)), SLOT(changeExpDecay(double)));
    connect (ui->doubleSpinBox_growth_sd_add, SIGNAL(valueChanged(double)), SLOT(changeSDadd(double)));
    connect (ui->comboBox_growth_cv_pattern, SIGNAL(currentIndexChanged(int)), SLOT(changeCVmethod(int)));

    ui->spinBox_growth_pattern->setMinimum(1);
    connect (ui->spinBox_growth_pattern, SIGNAL(valueChanged(int)), SLOT(changeGrowthPattern(int)));
    connect (ui->spinBox_growth_num_patterns, SIGNAL(valueChanged(int)), SLOT(changeNumGrowthPat(int)));
    connect (ui->spinBox_growth_num_submorphs, SIGNAL(valueChanged(int)), SLOT(changeNumSubMorph(int)));

    // Movement
    moveDefsView = new tableview();
    moveDefsView->setParent(this);
    ui->horizontalLayout_move_defs->addWidget(moveDefsView);
    moveParamsView = new tableview();
    moveParamsView->setParent(this);
    ui->verticalLayout_move_params->addWidget(moveParamsView);

    // Mortality
    mortBreakPtsView = new tableview();
    mortBreakPtsView->setParent (this);
    ui->verticalLayout_breakpoints->addWidget(mortBreakPtsView);
    mortAgesView = new tableview();
    mortAgesView->setParent(this);
    ui->horizontalLayout_mort_age_specific->addWidget(mortAgesView);
    mortParamsView = new tableview();
    mortParamsView->setParent (this);
    ui->verticalLayout_mort_params->addWidget(mortParamsView);
    mortTVParamsView = new tableview();
    mortTVParamsView->setParent(this);
    ui->verticalLayout_cohort_time_vary_params->addWidget(mortTVParamsView);

    mortInputsView = new tableview();
    mortInputsView->setParent(this);
    ui->verticalLayout_mort_inputs->addWidget(mortInputsView);
    mortInitialParamsView = new tableview();
    mortInitialParamsView->setParent(this);
    ui->verticalLayout_init_F->addWidget(mortInitialParamsView);

    connect (ui->comboBox_mort_option, SIGNAL(currentIndexChanged(int)), SLOT(changeMortOption(int)));
    connect (ui->spinBox_mort_num_breakpoints, SIGNAL(valueChanged(int)), SLOT(changeMortNumBkpts(int)));

    // Seasonal
    seasonParamsView = new tableview();
    seasonParamsView->setParent(this);
    ui->verticalLayout_seasonal_params->addWidget(seasonParamsView);


    connect (ui->checkBox_seas_femWtLen1, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_femWtLen2, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_fecundity1, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_fecundity2, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_maturity1, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_maturity2, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_maleWtLen1, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_maleWtLen2, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_L1, SIGNAL(toggled(bool)), SLOT(changeSeasParams()));
    connect (ui->checkBox_seas_K, SIGNAL(stateChanged(int)), SLOT(changeSeasParams()));

    connect (ui->spinBox_num_move_defs, SIGNAL(valueChanged(int)), SLOT(changeMoveNumDefs(int)));
    connect (ui->lineEdit_move_age, SIGNAL(editingFinished()), SLOT(changeMoveFirstAge()));

    connect (ui->lineEdit_fishingMort_bpark, SIGNAL(editingFinished()), SLOT(changeFMortBpark()));
    connect (ui->spinBox_fishingMort_bpark_year, SIGNAL(valueChanged(int)), SLOT(changeFMortBparkYr(int)));
    connect (ui->lineEdit_fishingMort_max, SIGNAL(editingFinished()), SLOT(changeFMortMaxF()));
    connect (ui->comboBox_fishingMort_method, SIGNAL(currentIndexChanged(int)), SLOT(changeFMortMethod(int)));
    connect (ui->lineEdit_fishingMort_2_fstart, SIGNAL(editingFinished()), SLOT(changeFMortStartF()));
    connect (ui->spinBox_fishingMort_2_phase, SIGNAL(valueChanged(int)), SLOT(changeFMortPhase(int)));
    connect (ui->spinBox_fishingMort_2_num_detail, SIGNAL(valueChanged(int)), SLOT(changeFMortNumInput(int)));
    connect (ui->spinBox_fishingMort_3_num_iters, SIGNAL(valueChanged(int)), SLOT(changeFMortNumIters(int)));

    set_model(m_data);
    ui->tabWidget->setCurrentIndex(0);
}

population_widget::~population_widget()
{
    delete ui;
}

void population_widget::set_model(ss_model *model)
{
    if (model != NULL)
    {
        if (model_data != NULL)
        {
/*            disconnect (pop->Move()->getMovementDefs(),
                        SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                        SLOT(moveDefsChanged(QModelIndex,QModelIndex)));
*/
        }
        model_data = model;
        pop = model_data->pPopulation;

        // Growth
        connect (pop->Grow()->getWtLenParams(), SIGNAL(dataChanged()),
                 SLOT(changeWtLenParams()));
        connect (pop->Grow()->getWtLenTVParams(), SIGNAL(dataChanged()),
                 SLOT(changeWtLenTVParams()));
        connect (pop->Grow()->getCohortParams(), SIGNAL(dataChanged()),
                 SLOT(changeCohortParams()));
        connect (pop->Grow()->getCohortTVParams(), SIGNAL(dataChanged()),
                 SLOT(changeCohortTVParams()));

        // Movement
        connect (pop->Move()->getMovementDefs(),
                 SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                 SLOT(moveDefsChanged(QModelIndex,QModelIndex)));

        // Recruitment
        recruitParamsView->setModel(pop->SR()->getFullParameters());
        connect (pop->SR()->getFullParameters(), SIGNAL(dataChanged()), SLOT(changeRecrFullParams()));
        recruitTVParamsView->setModel(pop->SR()->getTimeVaryParams());
        connect (pop->SR()->getTimeVaryParams(), SIGNAL(dataChanged()), SLOT(changeRecrTVParams()));
        recruitAssignParamsView->setModel(pop->SR()->getAssignments());
        connect (pop->SR()->getAssignments(), SIGNAL(dataChanged()), SLOT(changeRecrAssignParams()));
        recruitDistView->setModel(pop->SR()->getDistParams());
        connect (pop->SR()->getDistParams(), SIGNAL(dataChanged()), SLOT(changeRecrDistParams()));
        recruitCyclesParamsView->setModel(pop->SR()->getCycleParams());
        connect (pop->SR()->getCycleParams(), SIGNAL(dataChanged()), SLOT(changeRecrCycleParams()));
        recruitDevsView->setModel(pop->SR()->getRecruitDevTable());
        connect (pop->SR()->getRecruitDevTable(), SIGNAL(dataChanged()), SLOT(changeRecrDevParams()));

        //Fec
        fecundParamsView->setModel(pop->Fec()->getFemaleParams());
        connect(pop->Fec()->getFemaleParams(), SIGNAL(dataChanged()),
                SLOT(changeFecundityParams()));
        fecundTVParamsView->setModel(pop->Fec()->getFemaleTVParams());
        connect(pop->Fec()->getFemaleTVParams(), SIGNAL(dataChanged()),
                SLOT(changeFecundityTVParams()));

        reset();
        refresh();
    }
}

void population_widget::changeGrowthPattern (int num)
{
    growthPattern *gp = pop->Grow()->getPattern(num - 1);
    if (currPattern != gp)
    {
        if (currPattern != NULL)
        {
            // disconnect
        }
    currPattern = gp;
    int numparms;

    growthParamsView->setModel(currPattern->getGrowthParams());
    growthParamsView->setHeight(currPattern->getGrowthParams());
    growthParamsView->resizeColumnsToContents();
    connect (currPattern->getGrowthParams(), SIGNAL(dataChanged()),
             SLOT(changeGrowthParams()));
    growthTVParamsView->setModel(currPattern->getGrowthTVParams());
    numparms = gp->getGrowthTVParams()->rowCount();
//    ui->label_growth_time_vary_params->setVisible(numparms);
//    growthTVParamsView->setEnabled(numparms);
    growthTVParamsView->setHeight(numparms);
    growthTVParamsView->resizeColumnsToContents();
    connect (currPattern->getGrowthTVParams(), SIGNAL(dataChanged()),
             SLOT(changeGrowthTVParams()));
    cvParamsView->setModel(currPattern->getCVParams());
    cvParamsView->setHeight(currPattern->getCVParams());
    cvParamsView->resizeColumnsToContents();
//    ui->label_cv_time_vary_params->setVisible(false);
//    cvTVParamsView->setModel(currPattern->getCVTVParams());
//    numparms = currPattern->getCVTVParams()->rowCount();
//    ui->label_cv_time_vary_params->setVisible(numparms);
//    cvTVParamsView->setVisible(numparms);
//    cvTVParamsView->setHeight(numparms);
//    cvTVParamsView->resizeColumnsToContents();


    fractionFemView->setModel(currPattern->getFractionFemaleParams());
    fractionFemView->setHeight(1);
    fractionFemView->resizeColumnsToContents();

    mortParamsView->setModel(currPattern->getNatMParams());
    mortParamsView->setHeight(currPattern->getNatMParams()->rowCount());
    mortParamsView->resizeColumnsToContents();
    connect (currPattern->getNatMParams(), SIGNAL(dataChanged()),
             SLOT(changeNatMParams()));
    mortTVParamsView->setModel(currPattern->getNatMTVParams());
    numparms = currPattern->getNatMTVParams()->rowCount();
    mortTVParamsView->setVisible(numparms);
    mortTVParamsView->setHeight(numparms);
    mortTVParamsView->resizeColumnsToContents();
    connect (currPattern->getNatMTVParams(), SIGNAL(dataChanged()),
             SLOT(changeNatMTVParams()));
    mortAgesView->setModel(currPattern->getNatMAges());
    mortAgesView->setHeight(2);
    mortAgesView->resizeColumnsToContents();

//    timeVaryParamsView->setModel(currPattern->getTimeVaryParams());
//    timeVaryParamsView->setHeight(currPattern->getTimeVaryParams());
//    timeVaryParamsView->resizeColumnsToContents();

//    maturityParamsView->setModel(currPattern->get);
    }

}

void population_widget::refresh()
{
    // growth
    growthMorphDistView->setModel(pop->Grow()->getMorphDistModel());
    growthMorphDistView->setHeight(pop->Grow()->getMorphDistModel());
//    growthParamsView->setModel(pop->Grow()->getMaturityParams());
    growthMorphDistView->resizeColumnsToContents();
    setGrowthModel(pop->Grow()->getModel());
    ui->doubleSpinBox_growth_age_Amin->setValue(pop->Grow()->getAge_for_l1());
    ui->doubleSpinBox_growth_age_Amax->setValue(pop->Grow()->getAge_for_l2());
    ui->doubleSpinBox_exp_decay->setValue(pop->Grow()->getExpDecay());
    ui->doubleSpinBox_growth_sd_add->setValue(pop->Grow()->getSd_add());
    ui->comboBox_growth_cv_pattern->setCurrentIndex(pop->Grow()->getCv_growth_pattern());
    ui->spinBox_growth_num_patterns->setValue(pop->Grow()->getNum_patterns());
    ui->spinBox_growth_num_submorphs->setValue(pop->Grow()->getNum_morphs());
    ui->comboBox_growth_time_vary->setCurrentIndex(pop->Grow()->getTimeVaryMethod()-1);
    ui->spinBox_bio_time_vary_read->setValue(pop->Grow()->getTimeVaryReadParams());

    int temp_int = pop->Grow()->getNum_patterns();
    ui->spinBox_growth_num_patterns->setValue(temp_int);
    if (temp_int > 0)
    {
        changeGrowthPattern(1);
        temp_int = pop->Grow()->getNum_morphs();
        ui->spinBox_growth_num_submorphs->setValue(temp_int);
        changeNumSubMorph(temp_int);
    }

    // recruitment
    changeRecrFullParams();
    changeRecrTVParams();
    changeRecrAssignParams();
    changeRecrDistParams();
    changeRecrCycleParams();
    changeRecrDevParams();

    setRecrArea(pop->SR()->getDistribArea());
    setRecrDistParam(pop->SR()->getDistribMethod());
    ui->checkBox_recr_interaction->setChecked(pop->SR()->getDoRecruitInteract());
    ui->spinBox_recr_num_assigns->setValue(pop->SR()->getNumAssignments());
    setSpawnRecrSpec(pop->SR()->getMethod());

    ui->spinBox_sr_do_recr_dev->setValue(pop->SR()->getRecDevCode());
    ui->spinBox_sr_recr_dev_begin_yr->setValue(pop->SR()->getRecDevStartYr());
    ui->spinBox_sr_recr_dev_phase->setValue(pop->SR()->getRecDevPhase());
    ui->spinBox_sr_recr_dev_end_yr->setValue(pop->SR()->getRecDevEndYr());
//    ui->checkBox_recr_dev_adv_opt->setChecked(pop->SR()->advanced_opts);
    ui->groupBox_recr_dev_adv_opt->setChecked(pop->SR()->getAdvancedOpts());
    ui->spinBox_recr_dev_early_start->setValue(pop->SR()->getRecDevEarlyStart());
    ui->spinBox_recr_dev_early_phase->setValue(pop->SR()->getRecDevEarlyPhase());
    ui->spinBox_recr_dev_fcast_phase->setValue(pop->SR()->getFcastRecPhase());
    ui->lineEdit_recr_dev_fcast_lambda->setText(QString::number(pop->SR()->getFcastLambda()));
    ui->spinBox_recr_dev_last_nobias->setValue(pop->SR()->getNobiasLastEarlyYr());
    ui->spinBox_recr_dev_first_bias->setValue(pop->SR()->getFullbiasFirstYr());
    ui->spinBox_recr_dev_last_bias->setValue(pop->SR()->getFullbiasLastYr());
    ui->spinBox_recr_dev_first_nobias->setValue(pop->SR()->getNobiasFirstRecentYr());
    ui->lineEdit_recr_dev_max_bias->setText(QString::number(pop->SR()->getMaxBiasAdjust()));
    ui->spinBox_recr_dev_cycles->setValue(pop->SR()->getRecCycles());
    ui->lineEdit_recr_dev_min_dev->setText(QString::number(pop->SR()->getRecDevMin()));
    ui->lineEdit_recr_dev_max_dev->setText(QString::number(pop->SR()->getRecDevMax()));
    ui->spinBox_num_recr_devs->setValue(pop->SR()->getNumRecDev());
    changeRecrNumDevs(pop->SR()->getNumRecDev());
    ui->spinBox_spwn_recr_time_vary_read->setValue(pop->SR()->getTimeVaryReadParams());

    // Maturity
    ui->checkBox_wtatage->setChecked(model_data->getReadWtAtAge());

    wtlenParamsView->setModel(pop->Grow()->getWtLenParams());
    wtlenParamsView->setHeight(pop->Grow()->getWtLenParams());
    wtlenParamsView->resizeColumnsToContents();
    wtlenTVParamsView->setModel(pop->Grow()->getWtLenTVParams());
    wtlenTVParamsView->setHeight(pop->Grow()->getWtLenTVParams());
    wtlenTVParamsView->resizeColumnsToContents();
    ui->label_wt_len_time_vary_params->setVisible(pop->Grow()->getWtLenTVParams()->rowCount());

    maturityParamsView->setModel(pop->Grow()->getCohortParams());
    maturityParamsView->setHeight(pop->Grow()->getCohortParams());
    maturityParamsView->resizeColumnsToContents();
    setMaturityOpt(pop->Grow()->getMaturity_option());
    ui->lineEdit_maturity_first_age->setText(QString::number(pop->Grow()->getFirst_mature_age()));
    maturityTVParamsView->setModel(pop->Grow()->getCohortTVParams());
    maturityTVParamsView->setHeight(pop->Grow()->getCohortTVParams());
    maturityTVParamsView->resizeColumnsToContents();

    // fecundity
    setFecundityOption(pop->Fec()->getMethod());
    setFecundityOffsetOption(pop->Grow()->getParam_offset_method());
    setFecundityAdjustment(pop->Grow()->getAdjustment_method());
    fecundParamsView->setModel(pop->Fec()->getFemaleParams());
    fecundParamsView->setHeight(pop->Fec()->getFemaleParams());
    fecundParamsView->resizeColumnsToContents();
    fecundTVParamsView->setModel(pop->Fec()->getFemaleTVParams());
    fecundTVParamsView->setHeight(pop->Fec()->getFemaleTVParams());
    fecundTVParamsView->resizeColumnsToContents();

    setHermaphOptions(pop->Fec()->getHermaphroditism());
    hermaphParamsView->setModel(pop->Fec()->getHermParams());
    hermaphParamsView->setHeight(pop->Fec()->getHermParams());
    ui->spinBox_fecund_hermaph_season->setMaximum(model_data->get_num_seasons());
    ui->spinBox_fecund_hermaph_season->setValue(pop->Fec()->getHermSeason());
    ui->spinBox_fecund_hermaph_male->setValue(pop->Fec()->getHermIncludeMales());

    // movement
    ui->spinBox_num_move_defs->setValue(pop->Move()->getNumDefs());
    ui->lineEdit_move_age->setText(QString::number(pop->Move()->getFirstAge()));
    moveDefsView->setModel(pop->Move()->getMovementDefs());
    moveDefsView->setHeight(pop->Move()->getMovementDefs());
    moveDefsView->resizeColumnsToContents();
    moveParamsView->setModel(pop->Move()->getMovementParams());
    moveParamsView->setHeight(pop->Move()->getNumParams());
    moveParamsView->resizeColumnsToContents();

    // Mortality
    mortBreakPtsView->setModel(pop->Grow()->getNatMortValues());
    mortBreakPtsView->setHeight(pop->Grow()->getNatMortValues());
//    mortAgesView->setModel(pop->Grow()->getNatMortAgeValues());
//    mortAgesView->setHeight(pop->Grow()->getNatMortAgeValues());

    mortInputsView->setModel(pop->M()->getInputModel());
    mortInputsView->setHeight(pop->M()->getInputModel());
    mortInputsView->resizeColumnsToContents();
    mortInitialParamsView->setModel(pop->M()->getInitialParams());
    mortInitialParamsView->setHeight(pop->M()->getInitialParams());
    mortInitialParamsView->resizeColumnsToContents();
    setMortOption (pop->Grow()->getNatural_mortality_type());

    // Seasonality
    ui->checkBox_seas_femWtLen1->setChecked(pop->getFemWtLen1());
    ui->checkBox_seas_femWtLen2->setChecked(pop->getFemWtLen2());
    ui->checkBox_seas_fecundity1->setChecked(pop->getFecund1());
    ui->checkBox_seas_fecundity2->setChecked(pop->getFecund2());
    ui->checkBox_seas_maturity1->setChecked(pop->getMaturity1());
    ui->checkBox_seas_maturity2->setChecked(pop->getMaturity2());
    ui->checkBox_seas_maleWtLen1->setChecked(pop->getMaleWtLen1());
    ui->checkBox_seas_maleWtLen2->setChecked(pop->getMaleWtLen2());
    ui->checkBox_seas_L1->setChecked(pop->getL1());
    ui->checkBox_seas_K->setChecked(pop->getK());
    changeSeasParams();

    seasonParamsView->setModel(pop->getSeasonalParams());
    seasonParamsView->setHeight(pop->getNumSeasParams());// SeasonalParams()->rowCount());
    ui->lineEdit_fishingMort_bpark->setText(QString::number(pop->M()->getBparkF()));
    ui->spinBox_fishingMort_bpark_year->setValue(pop->M()->getBparkYr());
    ui->lineEdit_fishingMort_max->setText(QString::number(pop->M()->getMaxF()));
    ui->comboBox_fishingMort_method->setCurrentIndex(pop->M()->getMethod() - 1);
    changeFMortMethod(ui->comboBox_fishingMort_method->currentIndex());
    ui->lineEdit_fishingMort_2_fstart->setText(QString::number(pop->M()->getStartF()));
    ui->spinBox_fishingMort_2_phase->setValue(pop->M()->getPhase());
    ui->spinBox_fishingMort_2_num_detail->setValue(pop->M()->getNumInputs());
    ui->spinBox_fishingMort_3_num_iters->setValue(pop->M()->getNumTuningIters());

    hermaphParamsView->resizeColumnsToContents();
    seasonParamsView->resizeColumnsToContents();
    recruitAssignParamsView->resizeColumnsToContents();
//    recruitParamsView->resizeColumnsToContents();
    recruitTVParamsView->resizeColumnsToContents();
    recruitParamsView->resizeColumnsToContents();
    mortBreakPtsView->resizeColumnsToContents();
    changeMortOption(pop->Grow()->getNatural_mortality_type());
}

void population_widget::reset()
{
}

void population_widget::setFecundityOption(int opt)
{
    int index = opt - 1;
    ui->comboBox_fecund_option->setCurrentIndex(index);
}

void population_widget::changeFecundityOption(int opt)
{
    int num = opt + 1;
    pop->Fec()->setMethod(num);
    fecundParamsView->setHeight(2);
    fecundParamsView->resizeColumnsToContents();
}

int population_widget::getFecundityOption()
{
    int num = ui->comboBox_fecund_option->currentIndex() + 1;
    return num;
}

void population_widget::setFecundityOffsetOption(int opt)
{
    ui->comboBox_fecund_gend_offset->setCurrentIndex(opt - 1);
}

void population_widget::changeFecundityOffsetOption(int opt)
{
    int num = opt + 1;
    pop->Grow()->setParam_offset_method(num);
}

int population_widget::getFecundityOffsetOption()
{
    return (ui->comboBox_fecund_gend_offset->currentIndex() + 1);
}

void population_widget::setFecundityAdjustment(int opt)
{
//    ui->comboBox_fecund_adj_constraint->setCurrentIndex(opt - 1);
}

void population_widget::changeFecundityAdjustment(int opt)
{
    int num = opt + 1;
    pop->Grow()->setAdjustment_method(num);
}

int population_widget::getFecundityAdjustment()
{
//    return (ui->comboBox_fecund_adj_constraint->currentIndex() + 1);
    return 0;
}

void population_widget::setMortOption(int opt)
{
    int numgen = model_data->get_num_genders() > 1? 2: 1;
    ui->comboBox_mort_option->setCurrentIndex(opt);
    ui->widget_mort_breakpoints->setVisible(false);
    ui->widget_mort_lorenz->setVisible(false);
    ui->label_mort_age_specific->setVisible(false);
    mortAgesView->setVisible(false);
    mortParamsView->setVisible(true);
    switch (opt)
    {
    case 0:
        mortParamsView->setHeight(numgen);
        break;
    case 1: // breakpoints
        ui->widget_mort_breakpoints->setVisible(true);
        ui->spinBox_mort_num_breakpoints->setValue(pop->Grow()->getNatMortNumBreakPts());
        mortParamsView->setHeight(pop->Grow()->getPattern(0)->getNatMParams());
        break;
    case 2: // lorenzen
        ui->widget_mort_lorenz->setVisible(true);
        ui->spinBox_mort_lorenz_int->setValue(pop->Grow()->getNaturalMortLorenzenRef());
        mortParamsView->setHeight(numgen);
        break;
    case 3: // specific age
    case 4:
        ui->label_mort_age_specific->setVisible(true);
        mortAgesView->setVisible(true);
        ui->label_mort_params->setVisible(false);
        mortParamsView->setVisible(false);

        break;
    }
}

void population_widget::changeMortOption(int opt)
{
    int numgen = model_data->get_num_genders() > 1? 2: 1;
    pop->Grow()->setNatural_mortality_type(opt);
    ui->widget_mort_breakpoints->setVisible(false);
    ui->widget_mort_lorenz->setVisible(false);
    ui->label_mort_age_specific->setVisible(false);
    mortAgesView->setVisible(false);
    ui->label_mort_params->setVisible(true);
    mortParamsView->setVisible(true);
    switch (opt)
    {
    case 0:
        mortParamsView->setHeight(numgen);
        break;
    case 1:
        ui->widget_mort_breakpoints->setVisible(true);
        ui->spinBox_mort_num_breakpoints->setValue(pop->Grow()->getNatMortNumBreakPts());
        mortParamsView->setHeight(pop->Grow()->getPattern(0)->getNatMParams());
        break;
    case 2:
        ui->widget_mort_lorenz->setVisible(true);
        ui->spinBox_mort_lorenz_int->setValue(pop->Grow()->getNaturalMortLorenzenRef());
        mortParamsView->setHeight(numgen);
        break;
    case 3:
    case 4:
        ui->label_mort_age_specific->setVisible(true);
        mortAgesView->setVisible(true);
        ui->label_mort_params->setVisible(false);
        mortParamsView->setVisible(false);
        break;
    }

}

void population_widget::changeMortNumBkpts(int num)
{
    int pat = ui->spinBox_growth_pattern->value() - 1;
    pop->Grow()->setNatMortNumBreakPts(num);
    mortBreakPtsView->resizeColumnsToContents();
    mortParamsView->setHeight(pop->Grow()->getPattern(pat)->getNatMParams());
    mortParamsView->resizeColumnsToContents();
}

void population_widget::changeNumGrowthPat(int num)
{
    bool vis = (bool)num;
    pop->Grow()->setNum_patterns(num);
    ui->spinBox_growth_pattern->setMaximum(num);
    ui->label_growth_num_submorphs->setVisible(vis);
    ui->spinBox_growth_num_submorphs->setVisible(vis);
    if (vis)
        changeNumSubMorph(ui->spinBox_growth_num_submorphs->value());
    else
        changeNumSubMorph(1);
}

void population_widget::changeNumSubMorph(int num)
{
    bool vis = false;
    int nmph = pop->Grow()->getNum_morphs();
    float value, within, between;
    if (nmph != num)
    {
        if (num > 1)
        {
            value = pop->Grow()->getMorph_within_ratio();
            if (value == 1.0)
            {
                between = .000001;
                within = 1.0;
            }
            else
            {
                between = sqrt(1. / (1. + value * value));
                within = value * between;
            }
            vis = true;
            ui->doubleSpinBox_growth_morph_ratio->setValue(value);
            ui->doubleSpinBox_growth_morph_between->setValue(between);
            ui->doubleSpinBox_growth_morph_within->setValue(within);
        }
        pop->Grow()->setNum_morphs(num);
    }
    ui->label_growth_submorph_ratio->setVisible(vis);
    ui->doubleSpinBox_growth_morph_ratio->setVisible(vis);
    ui->label_growth_morph_within->setVisible(vis);
    ui->doubleSpinBox_growth_morph_within->setVisible(vis);
    ui->label_growth_morph_between->setVisible(vis);
    ui->doubleSpinBox_growth_morph_between->setVisible(vis);
    ui->label_growth_submorph_dist->setVisible(vis);
    growthMorphDistView->setVisible(vis);
    growthMorphDistView->resizeColumnsToContents();
}

void population_widget::setGrowthModel(int opt)
{
    ui->comboBox_growth_model->setCurrentIndex(opt - 1);
    changeGrowthModel(ui->comboBox_growth_model->currentIndex());
}

void population_widget::changeGrowthModel(int opt)
{
    bool vis = false;
    pop->Grow()->setModel(opt + 1);
    if (opt == 2)
    {
        vis = true;
        ui->doubleSpinBox_growth_age_spec_min->setValue(pop->Grow()->getAgeMin_for_K());
        ui->doubleSpinBox_growth_age_spec_max->setValue(pop->Grow()->getAgeMax_for_K());
    }
    ui->label_growth_age_spec_min->setVisible(vis);
    ui->doubleSpinBox_growth_age_spec_min->setVisible(vis);
    ui->label_growth_age_spec_max->setVisible(vis);
    ui->doubleSpinBox_growth_age_spec_max->setVisible(vis);
//    ui->frame_growth_age_spec_k->setVisible(vis);

}

int population_widget::getGrowthModel()
{
    return ui->comboBox_growth_model->currentIndex() + 1;
}

void population_widget::changeAmin (double val)
{
    pop->Grow()->setAge_for_l1(val);
}

void population_widget::changeAmax (double val)
{
    pop->Grow()->setAge_for_l2(val);
}

void population_widget::changeKageMin (double val)
{
    pop->Grow()->setAgeMin_for_K(val);
}

void population_widget::changeKageMax (double val)
{
    pop->Grow()->setAgeMax_for_K(val);
}

void population_widget::changeExpDecay (double val)
{
    pop->Grow()->setExpDecay(val);
}

void population_widget::changeSDadd (double val)
{
    pop->Grow()->setSd_add(val);
}

void population_widget::changeCVmethod (int num)
{
    pop->Grow()->setCv_growth_pattern(num);
}

void population_widget::changeTimeVaryMethod(int num)
{
    grwth->setTimeVaryMethod(num+1);
}

void population_widget::changeGrowthMorphRatio(double ratio)
{
    double between, within;
    pop->Grow()->setMorph_within_ratio(ratio);

    if (ui->spinBox_growth_num_submorphs->value() == 1)
    {
        pop->Grow()->setMorph_within_ratio(1.0);
        between = .000001;
        within = 1.0;
    }
    else
    {
        between = sqrt(1. / (1. + ratio * ratio));
        within = ratio * between;
    }
    ui->doubleSpinBox_growth_morph_between->setValue(between);
    ui->doubleSpinBox_growth_morph_within->setValue(within);
}

void population_widget::changeGrowthTimeVaryRead(int flag)
{
    bool vis = (flag != 0);
    grwth->setTimeVaryReadParams(flag);
//    ui->label_growth_time_vary_params->setVisible(vis);
    growthTVParamsView->setEnabled(vis);
//    ui->label_wt_len_time_vary_params->setVisible(vis);
    wtlenTVParamsView->setEnabled(vis);
//    ui->label_maturity_time_vary_params->setVisible(vis);

    ui->label_fec_time_vary_params->setVisible(vis);
}

void population_widget::changeGrowthParams()
{
    growthParamsView->setHeight(currPattern->getGrowthParams());
    growthParamsView->resizeColumnsToContents();
}

void population_widget::changeGrowthTVParams()
{
    growthTVParamsView->setHeight(currPattern->getGrowthTVParams());
    growthTVParamsView->resizeColumnsToContents();
}

void population_widget::changeNatMParams()
{
    mortParamsView->setHeight(currPattern->getNatMParams());
    mortParamsView->resizeColumnsToContents();
}

void population_widget::changeNatMTVParams()
{
    mortTVParamsView->setHeight(currPattern->getNatMTVParams());
    mortTVParamsView->resizeColumnsToContents();
}

void population_widget::changeWtLenParams()
{
    wtlenParamsView->setHeight(pop->Grow()->getWtLenParams());
    wtlenParamsView->resizeColumnsToContents();
}

void population_widget::changeWtLenTVParams()
{
    wtlenTVParamsView->setHeight(pop->Grow()->getWtLenTVParams());
    wtlenTVParamsView->resizeColumnsToContents();
}

void population_widget::changeCohortParams()
{
    maturityParamsView->setHeight(pop->Grow()->getCohortParams());
    maturityParamsView->resizeColumnsToContents();
}

void population_widget::changeCohortTVParams()
{
    maturityTVParamsView->setHeight(pop->Grow()->getCohortTVParams());
    maturityTVParamsView->resizeColumnsToContents();
}

void population_widget::changeFecundityParams()
{
    fecundParamsView->setHeight(pop->Fec()->getFemaleParams());
    fecundParamsView->resizeColumnsToContents();
}

void population_widget::changeFecundityTVParams()
{
    fecundTVParamsView->setHeight(pop->Fec()->getFemaleTVParams());
    fecundTVParamsView->resizeColumnsToContents();
}

void population_widget::changeFractionFemale()
{
//    float value = ui->lineEdit_fraction_female->text().toFloat();
//    pop->set_frac_female(value);
}

void population_widget::setHermaphOptions(int herm)
{
    if (herm != 0)
    {
        ui->groupBox_hermaphroditism->setChecked(true);
        if (herm == 1)
            ui->radioButton_herm_MtF->setChecked(true);
        else
            ui->radioButton_herm_FtM->setChecked(true);
    }
    else
    {
        ui->groupBox_hermaphroditism->setChecked(false);
    }
}

void population_widget::changeHermaph(bool flag)
{
    int doherm = 0;
    if (flag)
    {
        doherm = -1;
        if (ui->radioButton_herm_MtF->isChecked())
            doherm = 1;
     }
    pop->Fec()->setHermaphroditism(doherm);
}

void population_widget::changeHermaphSeas(int seas)
{
    pop->Fec()->setHermSeason(seas);
}

void population_widget::changeHermaphMales(int opt)
{
    pop->Fec()->setHermIncludeMales(opt);
}

void population_widget::changeRecrNumDevs(int num)
{
    pop->SR()->setNumRecDev(num);
    recruitDevsView->setHeight(num);
}

void population_widget::changeSpwnRecReadTimeVary(int flag)
{
    bool vis = (flag != 0);
    spwn_rcr->setTimeVaryReadParams(flag);
//    ui->label_recr_time_vary_params->setVisible(vis);
    recruitTVParamsView->setEnabled(vis);
}

void population_widget::setMaturityOpt(int opt)
{
    ui->comboBox_maturity_option->setCurrentIndex(opt - 1);
    changeMaturityOpt(ui->comboBox_maturity_option->currentIndex());
}

void population_widget::changeMaturityOpt(int opt)
{
//    bool vis = true;
    pop->Grow()->setMaturity_option(opt + 1);
/*    if (opt == 2 || opt == 3)
        vis = true;
    ui->frame_growth_age_spec->setVisible(vis);*/
}

int population_widget::getMaturityOpt()
{
    return ui->comboBox_maturity_option->currentIndex() + 1;
}

void population_widget::changeFirstMatureAge()
{
    float value = ui->lineEdit_maturity_first_age->text().toFloat();
    pop->Grow()->setFirst_mature_age(value);
}

void population_widget::changeSeasParams()
{
    int num;
    pop->setFemWtLen1(ui->checkBox_seas_femWtLen1->isChecked());
    pop->setFemWtLen2(ui->checkBox_seas_femWtLen2->isChecked());
    pop->setFecund1(ui->checkBox_seas_fecundity1->isChecked());
    pop->setFecund2(ui->checkBox_seas_fecundity2->isChecked());
    pop->setMaturity1(ui->checkBox_seas_maturity1->isChecked());
    pop->setMaturity2(ui->checkBox_seas_maturity2->isChecked());
    pop->setMaleWtLen1(ui->checkBox_seas_maleWtLen1->isChecked());
    pop->setMaleWtLen2(ui->checkBox_seas_maleWtLen2->isChecked());
    pop->setL1(ui->checkBox_seas_L1->isChecked());
    pop->setK(ui->checkBox_seas_K->isChecked());

    num = pop->getNumSeasParams();// + 1;
    seasonParamsView->setHeight(num);
}

void population_widget::setRecrArea(int value)
{
    if (value == 1)
        ui->radioButton_global->setChecked(true);
    else if (value == 2)
        ui->radioButton_area->setChecked(true);
}

void population_widget::changeRecrArea()
{
    int area = getRecrArea();
    pop->SR()->setDistribArea(area);
}

int population_widget::getRecrArea()
{
    int value = 0;
    if (ui->radioButton_global->isChecked())
        value = 1;
    else if (ui->radioButton_area->isChecked())
        value = 2;
    return value;
}

void population_widget::changeRecrFullParams()
{
    recruitParamsView->setHeight(pop->SR()->getFullParameters());
    recruitParamsView->resizeColumnsToContents();
}
void population_widget::changeRecrTVParams()
{
    recruitTVParamsView->setHeight(pop->SR()->getTimeVaryParams());
    recruitTVParamsView->resizeColumnsToContents();
}
void population_widget::changeRecrAssignParams()
{
    recruitAssignParamsView->setHeight(pop->SR()->getAssignments());
    recruitAssignParamsView->resizeColumnsToContents();
}
void population_widget::changeRecrDistParams()
{
    recruitDistView->setHeight(pop->SR()->getDistParams());
    recruitDistView->resizeColumnsToContents();
}
void population_widget::changeRecrCycleParams()
{
    recruitCyclesParamsView->setHeight(pop->SR()->getCycleParams());
    recruitCyclesParamsView->resizeColumnsToContents();
}
void population_widget::changeRecrDevParams()
{
    recruitDevsView->setHeight(pop->SR()->getRecruitDevTable());
    recruitDevsView->resizeColumnsToContents();
}

void population_widget::setRecrDistParam(int method)
{
    ui->spinBox_recr_dist_params->setValue(method);
    changeRecrDistParam(method);
}

void population_widget::changeRecrDistParam(int method)
{
    pop->SR()->setDistribMethod(method);
    if (method == 1)
        ui->checkBox_recr_interaction->setVisible(true);
    else
        ui->checkBox_recr_interaction->setVisible(false);
}

int population_widget::getRecrDistParam()
{
    return ui->spinBox_recr_dist_params->value();
}

void population_widget::changeRecAssignInteract(bool flag)
{
    pop->SR()->setDoRecruitInteract(flag);
}

void population_widget::changeRecNumAssigns(int num)
{
//    bool vis = (num != 0);
    pop->SR()->setNumAssignments(num);
//    ui->label_recr_num_assigns->setVisible(vis);
    recruitAssignParamsView->setHeight(num);
//    assignmentView->setVisible(vis);
}

void population_widget::changeRecDoDev(int num)
{
    pop->SR()->setRecDevCode(num);
}

void population_widget::changeRecrDevStartYr (int yr)
{
    pop->SR()->setRecDevStartYr(yr);
}

void population_widget::changeRecrDevEndYr (int yr)
{
    pop->SR()->setRecDevEndYr(yr);
}

void population_widget::changeRecrDevPhase (int phase)
{
    pop->SR()->setRecDevPhase(phase);
}

void population_widget::changeRecrDoAdvancedOpts(bool flag)
{
    pop->SR()->setAdvancedOpts(flag);
}

void population_widget::changeRecrDevEarlyStart (int yr)
{
    pop->SR()->setRecDevEarlyStart(yr);
}

void population_widget::changeRecrDevEarlyPhase (int phase)
{
    pop->SR()->setRecDevEarlyPhase(phase);
}

void population_widget::changeRecrFcastPhase (int phase)
{
    pop->SR()->setFcastRecPhase(phase);
}

void population_widget::changeRecrFcastLambda (QString s_lam)
{
    float lam = stringToFloat(s_lam);
    pop->SR()->setFcastLambda(lam);
}

void population_widget::changeRecrNoBiasLastYr (int yr)
{
    pop->SR()->setNobiasLastEarlyYr(yr);
}

void population_widget::changeRecrFullBiasFirstYr (int yr)
{
    pop->SR()->setFullbiasFirstYr(yr);
}

void population_widget::changeRecrFullBiasLastYr (int yr)
{
    pop->SR()->setFullbiasLastYr(yr);
}

void population_widget::changeRecrNoBiasFirstYr (int yr)
{
    pop->SR()->setNobiasFirstRecentYr(yr);
}

void population_widget::changeRecrBiasMax (QString s_max)
{
    float max = stringToFloat(s_max);
    pop->SR()->setMaxBiasAdjust(max);
}

void population_widget::changeRecrCycles (int cycles)
{
    pop->SR()->setRecCycles(cycles);
}

void population_widget::changeRecrDevMin (QString s_min)
{
    float min = stringToFloat(s_min);
    pop->SR()->setRecDevMin(min);
}

void population_widget::changeRecrDevMax (QString s_max)
{
    float max = stringToFloat(s_max);
    pop->SR()->setRecDevMax(max);
}

void population_widget::setSpawnRecrSpec(int spec)
{
    if (spec < 1) spec = 1;
    if (spec > 7) spec = 7;
    ui->comboBox_recr_spec->setCurrentIndex(spec - 1);
}

void population_widget::changeSpawnRecrSpec(int num)
{
    int mthd = num + 1;
    int old_mthd = pop->SR()->getMethod();
    if (mthd != old_mthd)
    {
        pop->SR()->setMethod(mthd);
        switch (mthd)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
            if (old_mthd == 5 ||
                    old_mthd == 7 ||
                    old_mthd == 8)
            {
                pop->SR()->removeFullParameter(3);
            }
            break;
        case 5:
        case 7:
        case 8:
            if (old_mthd < 5 ||
                    old_mthd == 6)
            {
                pop->SR()->insertFullParameter(3);
            }
            break;
        }
    }
    recruitParamsView->setHeight(pop->SR()->getFullParameters());
}

void population_widget::changeMoveNumDefs(int value)
{
    pop->Move()->setNumDefs(value);
    moveDefsView->setHeight(value);
    pop->Move()->setNumParams(value * 2);
    moveParamsView->setHeight(value * 2);
    setMoveParamTitles();
}

void population_widget::moveDefsChanged(QModelIndex tl, QModelIndex br)
{
    int r1 = tl.row();
    int r2 = br.row();
    moveDefsView->resizeColumnsToContents();
    for (int i = r1; i <= r2; i++)
        setMoveParamTitle(i);
}

void population_widget::setMoveParamTitles()
{
    int def, num;
    num = pop->Move()->getNumDefs();
    ui->spinBox_num_move_defs->setValue(num);

    for (def = 0; def < num; def++)
    {
        setMoveParamTitle(def);
    }
    moveParamsView->resizeColumnsToContents();
}

void population_widget::setMoveParamTitle(int def)
{
    QStringList d_list = pop->Move()->getDefinition(def);
    int index = def * 2;
    pop->Move()->setParamHeader
            (index, QString("MoveParm_A_seas_%1_GP_%2from_%3to_%4").arg(
                 d_list.at(0), d_list.at(1), d_list.at(2), d_list.at(3)));
    pop->Move()->setParamHeader
            (index + 1, QString("MoveParm_B_seas_%1_GP_%2from_%3to_%4").arg(
                 d_list.at(0), d_list.at(1), d_list.at(2), d_list.at(3)));
}

void population_widget::changeMoveFirstAge()
{
    float age = ui->lineEdit_move_age->text().toFloat();
    pop->Move()->setFirstAge(age);
}

void population_widget::changeFMortMethod (int opt)
{
    pop->M()->setMethod(opt + 1);
    switch (opt)
    {
    case 0:
        ui->frame_fishingMort_2->setVisible(false);
        ui->frame_fishingMort_3->setVisible(false);
        break;
    case 1:
        ui->frame_fishingMort_2->setVisible(true);
        ui->frame_fishingMort_3->setVisible(false);
        break;
    case 2:
        ui->frame_fishingMort_2->setVisible(false);
        ui->frame_fishingMort_3->setVisible(true);
        break;
    }

}

void population_widget::changeFMortBpark ()
{
    QString txt = ui->lineEdit_fishingMort_bpark->text();
    pop->M()->setBparkF(txt.toFloat());
}

void population_widget::changeFMortBparkYr (int yr)
{
    pop->M()->setBparkYr(yr);
}

void population_widget::changeFMortMaxF ()
{
    QString txt = ui->lineEdit_fishingMort_max->text();
    pop->M()->setMaxF(txt.toFloat());
}

void population_widget::changeFMortStartF ()
{
    QString txt = ui->lineEdit_fishingMort_2_fstart->text();
    pop->M()->setStartF(txt.toFloat());
}

void population_widget::changeFMortPhase (int phs)
{
    pop->M()->setPhase(phs);
}

void population_widget::changeFMortNumInput (int num)
{
    pop->M()->setNumInputs(num);
    mortInputsView->setHeight(num);
}

void population_widget::changeFMortNumIters (int num)
{
    pop->M()->setNumTuningIters(num);
}

