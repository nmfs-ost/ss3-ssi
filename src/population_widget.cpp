#include <QWidget>

#include "population_widget.h"
#include "ui_population_widget.h"

population_widget::population_widget(ss_model *m_data, QWidget *parent) :
    QWidget(parent), ui(new Ui::population_widget)
{
    ui->setupUi(this);
    model_data = NULL;
//    pop = model_data->pPopulation;

//    ui->verticalLayout_fishingMort_2_detail->addWidget();

    // Fecundity
    fractionFemView = new tableview();
    fractionFemView->setParent(this);
    ui->verticalLayout_fraction_female->addWidget(fractionFemView);

    fecundParamsView = new tableview();
    fecundParamsView->setParent(this);
    ui->verticalLayout_fec_params->addWidget(fecundParamsView);

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
    connect (ui->comboBox_fecund_adj_constraint, SIGNAL(currentIndexChanged(int)), SLOT(changeFecundityAdjustment(int)));

    // Recruitment
    assignmentView = new tableview();
    assignmentView->setParent(this);
    ui->horizontalLayout_recr_assigns->addWidget(assignmentView);

//    recruitParamsView = new tableview();
//    recruitParamsView->setParent(this);
//    ui->verticalLayout_recr_params->addWidget(recruitParamsView);

    recruitDevsView = new tableview();
    recruitDevsView->setParent(this);
    ui->verticalLayout_recr_devs->addWidget(recruitDevsView);

    recruitFullParamsView = new tableview();
    recruitFullParamsView->setParent(this);
    ui->verticalLayout_recr_full_params->addWidget(recruitFullParamsView);

    setRecrArea(2);
    connect (ui->radioButton_area, SIGNAL(clicked()), SLOT(changeRecrArea()));
    connect (ui->radioButton_global, SIGNAL(clicked()), SLOT(changeRecrArea()));
//    setRecrDistParam(1);
    ui->checkBox_recr_interaction->setChecked(false);
    connect (ui->spinBox_recr_dist_params, SIGNAL(valueChanged(int)), SLOT(changeRecrDistParam(int)));
    connect (ui->checkBox_recr_interaction, SIGNAL(toggled(bool)), SLOT(changeRecAssignInteract(bool)));
    connect (ui->spinBox_recr_num_assigns, SIGNAL(valueChanged(int)), SLOT(changeRecNumAssigns(int)));
    connect (ui->comboBox_recr_spec, SIGNAL(currentIndexChanged(int)), SLOT(changeSpawnRecrSpec(int)));
    connect (ui->spinBox_num_recr_devs, SIGNAL(valueChanged(int)), SLOT(changeNumRecrDevs(int)));


    // Growth
    growthMorphDistView = new tableview();
    growthMorphDistView->setParent(this);
//    growthMorphDistView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->horizontalLayout_growth_submorph_dist->addWidget(growthMorphDistView);
    growthParamsView = new tableview();
    growthParamsView->setParent(this);
    ui->verticalLayout_growth_params->addWidget(growthParamsView);
    cvParamsView = new tableview();
    cvParamsView->setParent(this);
    ui->verticalLayout_cv_parameters->addWidget(cvParamsView);
    wtlenParamsView = new tableview();
    wtlenParamsView->setParent(this);
    ui->verticalLayout_weightlength->addWidget(wtlenParamsView);
    maturityParamsView = new tableview();
    maturityParamsView->setParent(this);
    ui->verticalLayout_maturity_params->addWidget(maturityParamsView);

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

    // Maturity

    connect (ui->comboBox_maturity_option, SIGNAL(currentIndexChanged(int)), SLOT(changeMaturityOpt(int)));
    connect (ui->lineEdit_maturity_first_age, SIGNAL(editingFinished()), SLOT(changeFirstMatureAge()));


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
            disconnect (ui->checkBox_seas_femWtLen1, SIGNAL(toggled(bool)), pop, SLOT(changeFemWtLn1(bool)));
            disconnect (ui->checkBox_seas_femWtLen2, SIGNAL(toggled(bool)), pop, SLOT(changeFemWtLn2(bool)));
            disconnect (ui->checkBox_seas_fecundity1, SIGNAL(toggled(bool)), pop, SLOT(changeFecundity1(bool)));
            disconnect (ui->checkBox_seas_fecundity2, SIGNAL(toggled(bool)), pop, SLOT(changeFecundity2(bool)));
            disconnect (ui->checkBox_seas_maturity1, SIGNAL(toggled(bool)), pop, SLOT(changeMaturity1(bool)));
            disconnect (ui->checkBox_seas_maturity2, SIGNAL(toggled(bool)), pop, SLOT(changeMaturity2(bool)));
            disconnect (ui->checkBox_seas_maleWtLen1, SIGNAL(toggled(bool)), pop, SLOT(changeMaleWtLn1(bool)));
            disconnect (ui->checkBox_seas_maleWtLen2, SIGNAL(toggled(bool)), pop, SLOT(changeMaleWtLn2(bool)));
            disconnect (ui->checkBox_seas_L1, SIGNAL(toggled(bool)), pop, SLOT(changeL1(bool)));
            disconnect (ui->checkBox_seas_K, SIGNAL(toggled(bool)), pop, SLOT(changeK(bool)));
        }
        model_data = model;
        pop = model_data->pPopulation;
        connect (ui->checkBox_seas_femWtLen1, SIGNAL(toggled(bool)), pop, SLOT(changeFemWtLn1(bool)));
        connect (ui->checkBox_seas_femWtLen2, SIGNAL(toggled(bool)), pop, SLOT(changeFemWtLn2(bool)));
        connect (ui->checkBox_seas_fecundity1, SIGNAL(toggled(bool)), pop, SLOT(changeFecundity1(bool)));
        connect (ui->checkBox_seas_fecundity2, SIGNAL(toggled(bool)), pop, SLOT(changeFecundity2(bool)));
        connect (ui->checkBox_seas_maturity1, SIGNAL(toggled(bool)), pop, SLOT(changeMaturity1(bool)));
        connect (ui->checkBox_seas_maturity2, SIGNAL(toggled(bool)), pop, SLOT(changeMaturity2(bool)));
        connect (ui->checkBox_seas_maleWtLen1, SIGNAL(toggled(bool)), pop, SLOT(changeMaleWtLn1(bool)));
        connect (ui->checkBox_seas_maleWtLen2, SIGNAL(toggled(bool)), pop, SLOT(changeMaleWtLn2(bool)));
        connect (ui->checkBox_seas_L1, SIGNAL(toggled(bool)), pop, SLOT(changeL1(bool)));
        connect (ui->checkBox_seas_K, SIGNAL(toggled(bool)), pop, SLOT(changeK(bool)));
        reset();
        refresh();
    }
}

void population_widget::changeGrowthPattern (int num)
{
    growthPattern *gp = pop->Grow()->getPattern(num - 1);

    fractionFemView->setModel(gp->getFractionFemaleParams());
    fractionFemView->setHeight(1);
    fractionFemView->resizeColumnsToContents();

    growthParamsView->setModel(gp->getGrowthParams());
    growthParamsView->setHeight(gp->getGrowthParams());
    growthParamsView->resizeColumnsToContents();

    cvParamsView->setModel(gp->getCVParams());
    cvParamsView->setHeight(gp->getCVParams());
    cvParamsView->resizeColumnsToContents();

//    maturityParamsView->setModel(gp->get);

    mortParamsView->setModel(gp->getNatMParams());
    mortParamsView->setHeight(gp->getNatMParams());
    mortParamsView->resizeColumnsToContents();
    mortAgesView->setModel(gp->getNatMAges());
    mortAgesView->setHeight(2);
    mortAgesView->resizeColumnsToContents();

}

void population_widget::refresh()
{
//    ui->lineEdit_fraction_female->setText(QString::number(pop->get_frac_female()));
    // fecundity
    fecundParamsView->setModel(pop->Fec()->getFemaleParams());
    fecundParamsView->setHeight(pop->Fec()->getFemaleParams());
    setFecundityOption(pop->Fec()->getMethod());

    setHermaphOptions(pop->Fec()->getHermaphroditism());
//    ui->groupBox_hermaphroditism->setChecked(pop->Fec()->getHermaphroditism());
    hermaphParamsView->setModel(pop->Fec()->getHermParams());
    hermaphParamsView->setHeight(pop->Fec()->getHermParams());
    ui->spinBox_fecund_hermaph_season->setMaximum(model_data->num_seasons());
    ui->spinBox_fecund_hermaph_season->setValue(pop->Fec()->getHermSeason());
    ui->spinBox_fecund_hermaph_male->setValue(pop->Fec()->getHermIncludeMales());
    setFecundityOffsetOption(pop->Grow()->getParam_offset_method());
    setFecundityAdjustment(pop->Grow()->getAdjustment_method());

    wtlenParamsView->setModel(pop->Grow()->getMaturityParams());
    wtlenParamsView->setHeight(pop->Grow()->getMaturityParams());
    wtlenParamsView->resizeColumnsToContents();
    maturityParamsView->setModel(pop->Grow()->getCohortParams());
    maturityParamsView->setHeight(pop->Grow()->getCohortParams());
    maturityParamsView->resizeColumnsToContents();
    setMaturityOpt(pop->Grow()->getMaturity_option());
    ui->lineEdit_maturity_first_age->setText(QString::number(pop->Grow()->getFirst_mature_age()));

    int temp_int = pop->Grow()->getNum_patterns();
    ui->spinBox_growth_num_patterns->setValue(temp_int);
    if (temp_int > 0)
    {
        changeNumGrowthPat(temp_int);
        temp_int = pop->Grow()->getNum_morphs();
    }
    ui->spinBox_growth_num_submorphs->setValue(temp_int);
    changeNumSubMorph(temp_int);

    ui->checkBox_seas_femWtLen1->setChecked(pop->getFemwtlen1());
    ui->checkBox_seas_femWtLen2->setChecked(pop->getFemwtlen2());
    ui->checkBox_seas_fecundity1->setChecked(pop->getFec1());
    ui->checkBox_seas_fecundity2->setChecked(pop->getFec2());
    ui->checkBox_seas_maturity1->setChecked(pop->getMat1());
    ui->checkBox_seas_maturity2->setChecked(pop->getMat2());
    ui->checkBox_seas_maleWtLen1->setChecked(pop->getMalewtlen1());
    ui->checkBox_seas_maleWtLen2->setChecked(pop->getMalewtlen2());
    ui->checkBox_seas_L1->setChecked(pop->getL1());
    ui->checkBox_seas_K->setChecked(pop->getK());
    changeSeasParams();

    // recruitment
    assignmentView->setModel(pop->SR()->getAssignments());
    assignmentView->setHeight(pop->SR()->getAssignments());
    recruitDevsView->setModel(pop->SR()->getRecruitDevs()->getObservations());
    recruitDevsView->setHeight(pop->SR()->getRecruitDevs()->getObservations());
    recruitFullParamsView->setModel(pop->SR()->getFullParameterModel());
    recruitFullParamsView->setHeight(pop->SR()->getFullParameterModel());
//    recruitParamsView->setModel(pop->SR()->getSetupModel());
//    recruitParamsView->setHeight(pop->SR()->getSetupModel());
//    recruitParamsView->resizeColumnsToContents();

    setRecrArea(pop->SR()->getDistribArea());
    setRecrDistParam(pop->SR()->getDistribMethod());
    ui->checkBox_recr_interaction->setChecked(pop->SR()->getDoRecruitInteract());
    ui->spinBox_recr_num_assigns->setValue(pop->SR()->getNumAssignments());
    setSpawnRecrSpec(pop->SR()->method);
//    ui->spinBox_sr_env_link->setValue(pop->SR()->env_link);
    ui->spinBox_sr_env_link->setVisible(false);
    ui->label_sr_env_link->setVisible(false);
//    ui->spinBox_sr_env_tgt->setValue(pop->SR()->env_target);
    ui->spinBox_sr_env_tgt->setVisible(false);
    ui->label_sr_env_tgt->setVisible(false);
    ui->spinBox_sr_recr_dev_begin_yr->setValue(pop->SR()->rec_dev_start_yr);
    ui->spinBox_sr_recr_dev_phase->setValue(pop->SR()->rec_dev_phase);
    ui->spinBox_sr_recr_dev_end_yr->setValue(pop->SR()->rec_dev_end_yr);
//    ui->checkBox_recr_dev_adv_opt->setChecked(pop->SR()->advanced_opts);
    ui->groupBox_recr_dev_adv_opt->setChecked(pop->SR()->advanced_opts);
    ui->spinBox_recr_dev_early_start->setValue(pop->SR()->rec_dev_early_start);
    ui->spinBox_recr_dev_early_phase->setValue(pop->SR()->rec_dev_early_phase);
    ui->spinBox_recr_dev_fcast_phase->setValue(pop->SR()->fcast_rec_phase);
    ui->lineEdit_recr_dev_fcast_lambda->setText(QString::number(pop->SR()->fcast_lambda));
    ui->spinBox_recr_dev_last_nobias->setValue(pop->SR()->nobias_last_early_yr);
    ui->spinBox_recr_dev_first_bias->setValue(pop->SR()->fullbias_first_yr);
    ui->spinBox_recr_dev_last_bias->setValue(pop->SR()->fullbias_last_yr);
    ui->spinBox_recr_dev_first_nobias->setValue(pop->SR()->nobias_first_recent_yr);
    ui->lineEdit_recr_dev_max_bias->setText(QString::number(pop->SR()->max_bias_adjust));
    ui->spinBox_recr_dev_cycles->setValue(pop->SR()->rec_cycles);
    ui->lineEdit_recr_dev_min_dev->setText(QString::number(pop->SR()->rec_dev_min));
    ui->lineEdit_recr_dev_max_dev->setText(QString::number(pop->SR()->rec_dev_max));
    ui->spinBox_num_recr_devs->setValue(pop->SR()->getRecruitDevs()->getNumObs());
    changeNumRecrDevs(pop->SR()->getRecruitDevs()->getNumObs());

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
    changeGrowthPattern(1);
    // Maturity


    // movement
    ui->spinBox_num_move_defs->setValue(pop->Move()->getNumDefs());
    ui->lineEdit_move_age->setText(QString::number(pop->Move()->getFirstAge()));
    moveDefsView->setModel(pop->Move()->getMovementDefs());
    moveDefsView->setHeight(pop->Move()->getMovementDefs());
    moveDefsView->resizeColumnsToContents();
    moveParamsView->setModel(pop->Move()->getMovementParams());
    moveParamsView->setHeight(pop->Move()->getMovementParams());
    moveParamsView->resizeColumnsToContents();

    // Mortality
    mortBreakPtsView->setModel(pop->Grow()->getNatMortValues());
    mortBreakPtsView->setHeight(pop->Grow()->getNatMortValues());
//    mortAgesView->setModel(pop->Grow()->getNatMortAgeValues());
//    mortAgesView->setHeight(pop->Grow()->getNatMortAgeValues());

    mortInputsView->setModel(pop->M()->getInputModel());
    mortInputsView->setHeight(pop->M()->getInputModel());
    mortInitialParamsView->setModel(pop->M()->getInitialParams());
    mortInitialParamsView->setHeight(pop->M()->getInitialParams());
    mortInitialParamsView->resizeColumnsToContents();
    setMortOption (pop->Grow()->getNatural_mortality_type());

    // Seasonality
    seasonParamsView->setModel(pop->getSeasonalParams());
    seasonParamsView->setHeight(pop->getSeasonalParams());
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
    assignmentView->resizeColumnsToContents();
//    recruitParamsView->resizeColumnsToContents();
    recruitDevsView->resizeColumnsToContents();
    recruitFullParamsView->resizeColumnsToContents();
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
    ui->comboBox_fecund_adj_constraint->setCurrentIndex(opt - 1);
}

void population_widget::changeFecundityAdjustment(int opt)
{
    int num = opt + 1;
    pop->Grow()->setAdjustment_method(num);
}

int population_widget::getFecundityAdjustment()
{
    return (ui->comboBox_fecund_adj_constraint->currentIndex() + 1);
}

void population_widget::setMortOption(int opt)
{
    ui->comboBox_mort_option->setCurrentIndex(opt);
    ui->widget_mort_breakpoints->setVisible(false);
    ui->widget_mort_lorenz->setVisible(false);
    ui->label_mort_age_specific->setVisible(false);
    mortAgesView->setVisible(false);
    mortParamsView->setVisible(true);
    switch (opt)
    {
    case 0:
        mortParamsView->setHeight(2);
        break;
    case 1: // breakpoints
        ui->widget_mort_breakpoints->setVisible(true);
        ui->spinBox_mort_num_breakpoints->setValue(pop->Grow()->getNatMortNumBreakPts());
        mortParamsView->setHeight(pop->Grow()->getPattern(0)->getNatMParams());
        break;
    case 2: // lorenzen
        ui->widget_mort_lorenz->setVisible(true);
        ui->spinBox_mort_lorenz_int->setValue(pop->Grow()->getNaturalMortLorenzenRef());
        mortParamsView->setHeight(2);
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
        mortParamsView->setHeight(2);
        break;
    case 1:
        ui->widget_mort_breakpoints->setVisible(true);
        ui->spinBox_mort_num_breakpoints->setValue(pop->Grow()->getNatMortNumBreakPts());
        mortParamsView->setHeight(pop->Grow()->getPattern(0)->getNatMParams());
        break;
    case 2:
        ui->widget_mort_lorenz->setVisible(true);
        ui->spinBox_mort_lorenz_int->setValue(pop->Grow()->getNaturalMortLorenzenRef());
        mortParamsView->setHeight(2);
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
    if (nmph != num)
    {
        if (num > 1)
        {
            vis = true;
            ui->doubleSpinBox_growth_morph_ratio->setValue(pop->Grow()->getMorph_within_ratio());
        }
        pop->Grow()->setNum_morphs(num);
    }
    ui->label_growth_submorph_ratio->setVisible(vis);
    ui->doubleSpinBox_growth_morph_ratio->setVisible(vis);
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

void population_widget::changeNumRecrDevs(int num)
{
    pop->SR()->recruitDeviations->setNumRecruitDevs(num);
    recruitDevsView->setHeight(num);
}

void population_widget::setMaturityOpt(int opt)
{
    ui->comboBox_maturity_option->setCurrentIndex(opt - 1);
    changeMaturityOpt(ui->comboBox_maturity_option->currentIndex());
}

void population_widget::changeMaturityOpt(int opt)
{
    bool vis = false;
    pop->Grow()->setMaturity_option(opt + 1);
    if (opt == 2 || opt == 3)
        vis = true;
//    ui->frame_growth_age_spec->setVisible(vis);
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
//    bool showtable = false;
    int num = pop->getNumSeasParams() + 1;
//    if (num > 0)
//        showtable = true;
    seasonParamsView->setHeight(num);
//    seasonParamsView->setVisible(showtable);
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
    assignmentView->setHeight(num);
//    assignmentView->setVisible(vis);
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
    int old_mthd = pop->SR()->method;
    if (mthd != old_mthd)
    {
        pop->SR()->method = mthd;
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
                QStringList lst = pop->SR()->getFullParameter(2);
                pop->SR()->insertFullParameter(3);
            }
            break;
        }
    }
    recruitFullParamsView->setHeight(pop->SR()->full_parameters);
}

void population_widget::changeMoveNumDefs(int value)
{
    pop->Move()->setNumDefs(value);
    moveDefsView->setHeight(value);
    moveDefsView->resizeColumnsToContents();
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
}

void population_widget::changeFMortNumIters (int num)
{
    pop->M()->setNumTuningIters(num);
}

