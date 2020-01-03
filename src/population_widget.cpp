#include <QWidget>
#include <cmath>

#include "population_widget.h"
#include "ui_population_widget.h"
#include "conversions.h"
#include "dialogspwnrcrequationview.h"

population_widget::population_widget(ss_model *m_data, QWidget *parent) :
    QWidget(parent), ui(new Ui::population_widget)
{
    ui->setupUi(this);
    ui->radioButton_global->setChecked(true);
    model_data = m_data;
    pop = model_data->pPopulation;
    currPattern = nullptr;

    // Growth - female
    growthMorphDistView = new tableview();
    growthMorphDistView->setParent(this);
    ui->horizontalLayout_growth_submorph_dist->addWidget(growthMorphDistView);
    growthFemParamsView = new tableview();
    growthFemParamsView->setParent(this);
    ui->verticalLayout_f_growth_params->addWidget(growthFemParamsView);
    growthFemTVParamsView = new tableview();
    growthFemTVParamsView->setParent(this);
    ui->verticalLayout_f_growth_time_vary_params->addWidget(growthFemTVParamsView);

    cvFemParamsView = new tableview();
    cvFemParamsView->setParent(this);
    ui->verticalLayout_f_cv_params->addWidget(cvFemParamsView);
    cvFemTVParamsView = new tableview();
    ui->verticalLayout_f_cv_time_vary_parameters->addWidget(cvFemTVParamsView);

    growthMaleParamsView = new tableview();
    ui->verticalLayout_m_growth_params->addWidget(growthMaleParamsView);
    growthMaleTVParamsView = new tableview();
    ui->verticalLayout_m_growth_time_vary_params->addWidget(growthMaleTVParamsView);
    cvMaleParamsView = new tableview();
    ui->verticalLayout_m_cv_params->addWidget(cvMaleParamsView);
    cvMaleTVParamsView = new tableview();
    ui->verticalLayout_m_cv_time_vary_parameters->addWidget(cvMaleTVParamsView);

    // Recruitment
    srEquationView = new DialogSpwnRcrEquationView(this);
    srEquationView->hide();
    recruitAssignParamsView = new tableview();
    recruitAssignParamsView->setParent(this);
    ui->horizontalLayout_recr_assigns->addWidget(recruitAssignParamsView);

    recruitDistParamsView = new tableview();
    recruitDistParamsView->setParent(this);
    ui->verticalLayout_recr_dist_params->addWidget(recruitDistParamsView);

    recruitDistTVParamsView = new tableview();
    recruitDistTVParamsView->setParent(this);
    ui->verticalLayout_recr_dist_time_vary_params->addWidget(recruitDistTVParamsView);

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
//    ui->checkBox_recr_interaction->setChecked(false);
    connect (ui->comboBox_recr_spec, SIGNAL(currentIndexChanged(int)), SLOT(changeSpawnRecrSpec(int)));
    connect (ui->pushButton_equation, SIGNAL(toggled(bool)), SLOT(setSRequationDialogVisible(bool)));
    connect (srEquationView, SIGNAL(hidden()), SLOT(setSRequationDialogVisible()));
    connect (ui->spinBox_spwn_recr_time_vary_read, SIGNAL(valueChanged(int)), SLOT(changeSpwnRecReadTimeVary(int)));
    connect (ui->spinBox_recr_num_assigns, SIGNAL(valueChanged(int)), SLOT(changeRecNumAssigns(int)));
    connect (ui->spinBox_recr_dist_params, SIGNAL(valueChanged(int)), SLOT(changeRecrDistParam(int)));
//    connect (ui->checkBox_recr_interaction, SIGNAL(toggled(bool)), SLOT(changeRecAssignInteract(bool)));
    connect (ui->spinBox_sr_do_recr_dev, SIGNAL(valueChanged(int)), SLOT(changeRecDoDev(int)));
    connect (ui->spinBox_sr_recr_dev_begin_yr, SIGNAL(valueChanged(int)), SLOT(changeRecrDevStartYr(int)));
    connect (ui->spinBox_sr_recr_dev_end_yr, SIGNAL(valueChanged(int)), SLOT(changeRecrDevEndYr(int)));
    connect (ui->spinBox_sr_recr_dev_phase, SIGNAL(valueChanged(int)), SLOT(changeRecrDevPhase(int)));
    connect (ui->groupBox_recr_dev_adv_opt, SIGNAL(toggled(bool)), SLOT(changeRecrDoAdvancedOpts(bool)));
    connect (ui->spinBox_recr_dev_early_start, SIGNAL(valueChanged(int)), SLOT(changeRecrDevEarlyStart (int)));
    connect (ui->spinBox_recr_dev_early_phase, SIGNAL(valueChanged(int)), SLOT(changeRecrDevEarlyPhase(int)));
    connect (ui->spinBox_recr_dev_fcast_phase, SIGNAL(valueChanged(int)), SLOT(changeRecrFcastPhase(int)));
    connect (ui->lineEdit_recr_dev_fcast_lambda, SIGNAL(textChanged(QString)), SLOT(changeRecrFcastLambda(QString)));
    connect (ui->doubleSpinBox_recr_dev_last_nobias, SIGNAL(valueChanged(double)), SLOT(changeRecrNoBiasLastYr(double)));
    connect (ui->doubleSpinBox_recr_dev_first_bias, SIGNAL(valueChanged(double)), SLOT(changeRecrFullBiasFirstYr(double)));
    connect (ui->doubleSpinBox_recr_dev_last_bias, SIGNAL(valueChanged(double)), SLOT(changeRecrFullBiasLastYr(double)));
    connect (ui->doubleSpinBox_recr_dev_first_nobias, SIGNAL(valueChanged(double)), SLOT(changeRecrNoBiasFirstYr(double)));
    connect (ui->lineEdit_recr_dev_max_bias, SIGNAL(textChanged(QString)), SLOT(changeRecrBiasMax(QString)));
    connect (ui->spinBox_recr_dev_cycles, SIGNAL(valueChanged(int)), SLOT(changeRecrCycles(int)));
    connect (ui->lineEdit_recr_dev_min_dev, SIGNAL(textChanged(QString)), SLOT(changeRecrDevMin(QString)));
    connect (ui->lineEdit_recr_dev_max_dev, SIGNAL(textChanged(QString)), SLOT(changeRecrDevMax(QString)));
    connect (ui->spinBox_num_recr_devs, SIGNAL(valueChanged(int)), SLOT(changeRecrNumDevs(int)));

    // Maturity
    wtlenFemParamsView = new tableview();
    wtlenFemParamsView->setParent(this);
    ui->verticalLayout_f_wtlen_params->addWidget(wtlenFemParamsView);
    wtlenFemTVParamsView = new tableview();
    wtlenFemTVParamsView->setParent(this);
    ui->verticalLayout_f_wtlen_time_vary_params->addWidget(wtlenFemTVParamsView);
    maturityParamsView = new tableview();
    maturityParamsView->setParent(this);
    ui->verticalLayout_cohort_params->addWidget(maturityParamsView);
    maturityTVParamsView = new tableview();
    maturityTVParamsView->setParent(this);
    ui->verticalLayout_cohort_time_vary_params->addWidget(maturityTVParamsView);

    wtlenMaleParamsView = new tableview();
    ui->verticalLayout_m_wtlen_params->addWidget(wtlenMaleParamsView);
    wtlenMaleTVParamsView = new tableview();
    ui->verticalLayout_m_wtlen_time_vary_params->addWidget(wtlenMaleTVParamsView);

    connect (ui->comboBox_maturity_option, SIGNAL(currentIndexChanged(int)), SLOT(changeMaturityOpt(int)));
    connect (ui->lineEdit_maturity_first_age, SIGNAL(editingFinished()), SLOT(changeFirstMatureAge()));
    connect (ui->checkBox_wtatage, SIGNAL(toggled(bool)), SIGNAL(readWtAtAgeSS(bool)));

    // Fecundity
    fracFemParamsView = new tableview();
    fracFemParamsView->setParent(this);
    ui->verticalLayout_fraction_female->addWidget(fracFemParamsView);
    fracFemTVParamsView = new tableview();
    fracFemTVParamsView->setParent(this);
    ui->verticalLayout_frac_female_tv_params->addWidget(fracFemTVParamsView);

    fecundParamsView = new tableview();
    fecundParamsView->setParent(this);
    ui->verticalLayout_fecund_params->addWidget(fecundParamsView);
    fecundTVParamsView = new tableview();
    fecundTVParamsView->setParent(this);
    ui->verticalLayout_fecund_time_vary_params->addWidget(fecundTVParamsView);

    ui->spinBox_fecund_hermaph_season->setMinimum(-1);

    // Hermaphroditism
    hermaphParamsView = new tableview();
    hermaphParamsView->setParent(this);
    ui->verticalLayout_hermaph_params->addWidget(hermaphParamsView);
    hermaphTVParamsView = new tableview();
    hermaphTVParamsView->setParent(this);
    ui->verticalLayout_hermaph_tv_params->addWidget(hermaphTVParamsView);

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
    moveTVParamsView = new tableview();
    moveTVParamsView->setParent(this);
    ui->verticalLayout_move_tv_params->addWidget(moveTVParamsView);

    connect (ui->spinBox_num_move_defs, SIGNAL(valueChanged(int)), SLOT(changeMoveNumDefs(int)));
    connect (ui->lineEdit_move_age, SIGNAL(editingFinished()), SLOT(changeMoveFirstAge()));

    // Mortality
    mortBreakPtsView = new tableview();
    mortBreakPtsView->setParent (this);
    ui->verticalLayout_breakpoints->addWidget(mortBreakPtsView);
    mortAgesView = new tableview();
    mortAgesView->setParent(this);
    ui->horizontalLayout_mort_age_specific->addWidget(mortAgesView);
    mortFemParamsView = new tableview();
    mortFemParamsView->setParent (this);
    ui->verticalLayout_mort_f_params->addWidget(mortFemParamsView);
    mortFemTVParamsView = new tableview();
    mortFemTVParamsView->setParent(this);
    ui->verticalLayout_mort_f_tv_params->addWidget(mortFemTVParamsView);
    mortMaleParamsView = new tableview();
    ui->verticalLayout_m_mort_params->addWidget(mortMaleParamsView);
    mortMaleTVParamsView = new tableview();
    ui->verticalLayout_m_mort_params->addWidget(mortMaleTVParamsView);

    ui->verticalLayout_cohort_time_vary_params->addWidget(mortFemTVParamsView);

    mortInputsView = new tableview();
    mortInputsView->setParent(this);
    ui->verticalLayout_mort_inputs->addWidget(mortInputsView);
    mortInitialParamsView = new tableview();
    mortInitialParamsView->setParent(this);
    ui->verticalLayout_init_F->addWidget(mortInitialParamsView);

    connect (ui->comboBox_mort_option, SIGNAL(currentIndexChanged(int)), SLOT(changeMortOption(int)));
    connect (ui->spinBox_mort_num_breakpoints, SIGNAL(valueChanged(int)), SLOT(changeMortNumBkpts(int)));

    connect (ui->lineEdit_fishingMort_bpark, SIGNAL(editingFinished()), SLOT(changeFMortBpark()));
    connect (ui->spinBox_fishingMort_bpark_year, SIGNAL(valueChanged(int)), SLOT(changeFMortBparkYr(int)));
    connect (ui->lineEdit_fishingMort_max, SIGNAL(editingFinished()), SLOT(changeFMortMaxF()));
    connect (ui->comboBox_fishingMort_method, SIGNAL(currentIndexChanged(int)), SLOT(changeFMortMethod(int)));
    connect (ui->lineEdit_fishingMort_2_fstart, SIGNAL(editingFinished()), SLOT(changeFMortStartF()));
    connect (ui->spinBox_fishingMort_2_phase, SIGNAL(valueChanged(int)), SLOT(changeFMortPhase(int)));
    connect (ui->spinBox_fishingMort_2_num_detail, SIGNAL(valueChanged(int)), SLOT(changeFMortNumInput(int)));
    connect (ui->spinBox_fishingMort_3_num_iters, SIGNAL(valueChanged(int)), SLOT(changeFMortNumIters(int)));

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

    set_model(m_data);
    ui->tabWidget->setCurrentIndex(0);
}

population_widget::~population_widget()
{
    delete ui;
}

void population_widget::set_model(ss_model *model)
{
    if (model != nullptr)
    {
        if (model_data != nullptr)
        {
/*            disconnect (pop->Move()->getMovementDefs(),
                        SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                        SLOT(moveDefsChanged(QModelIndex,QModelIndex)));
*/
        }
        model_data = model;
        pop = model_data->pPopulation;

        // Growth
        connect (pop->Grow()->getCohortParams(), SIGNAL(dataChanged()),
                 SLOT(changeCohortParams()));
        connect (pop->Grow()->getCohortTVParams(), SIGNAL(dataChanged()),
                 SLOT(changeCohortTVParams()));

        // Movement
        connect (pop->Move()->getMovementDefs(),
                 SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                 SLOT(moveDefsChanged(QModelIndex,QModelIndex)));
        connect (pop->Move()->getMovementParams(), SIGNAL(dataChanged()),
                 SLOT(changeMoveParams()));
        connect (pop->Move()->getMoveTVParams(), SIGNAL(dataChanged()),
                 SLOT(changeMoveTVParams()));

        // Recruitment
        srEquationView->setPopulation(pop);
        recruitParamsView->setModel(pop->SR()->getFullParameters());
        connect (pop->SR()->getFullParameters(), SIGNAL(dataChanged()), SLOT(changeRecrFullParams()));
        recruitTVParamsView->setModel(pop->SR()->getTimeVaryParams());
        connect (pop->SR()->getTimeVaryParams(), SIGNAL(dataChanged()), SLOT(changeRecrTVParams()));
        changeRecrFullParams();
        recruitAssignParamsView->setModel(pop->SR()->getAssignments());
        connect (pop->SR()->getAssignments(), SIGNAL(dataChanged()), SLOT(changeRecrAssignParams()));
        changeRecrAssignParams();
        recruitDistParamsView->setModel(pop->SR()->getDistParams());
        connect (pop->SR()->getDistParams(), SIGNAL(dataChanged()), SLOT(changeRecrDistParams()));
        recruitDistTVParamsView->setModel(pop->SR()->getDistTVParams());
        connect (pop->SR()->getDistTVParams(), SIGNAL(dataChanged()), SLOT(changeRecrDistTVParams()));
        changeRecrDistParams();

        recruitCyclesParamsView->setModel(pop->SR()->getCycleParams());
        connect (pop->SR()->getCycleParams(), SIGNAL(dataChanged()), SLOT(changeRecrCycleParams()));
        changeRecrCycleParams();
        recruitDevsView->setModel(pop->SR()->getRecruitDevTable());
        connect (pop->SR()->getRecruitDevTable(), SIGNAL(dataChanged()), SLOT(changeRecrDevParams()));
        changeRecrDevParams();

        reset();
        refresh();
    }
}

void population_widget::changeGrowthPattern (int num)
{
    int totnum = pop->Grow()->getNum_patterns();
    if (num > totnum)
        num = totnum;

    growthPattern *gp = pop->Grow()->getPattern(num - 1);
    if (currPattern != gp)
    {
        if (currPattern != nullptr)
        {
            // disconnect
            disconnect (currPattern->getFemGrowthParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeGrowthParams()));
            disconnect (currPattern->getFemGrowthTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeGrowthTVParams()));
            disconnect (currPattern->getMaleGrowthParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeGrowthParams()));
            disconnect (currPattern->getMaleGrowthTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeGrowthTVParams()));

            disconnect (currPattern->getFemCVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeCVParams()));
            disconnect (currPattern->getFemCVTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeCVTVParams()));
            disconnect (currPattern->getMaleCVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeCVParams()));
            disconnect (currPattern->getMaleCVTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeCVParams()));

            disconnect (currPattern->getFractionFemaleParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeFracFemParams()));
            disconnect (currPattern->getFracFmTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeFracFemTVParams()));

            disconnect (currPattern->getNatMAges(), SIGNAL(dataChanged()),
                     this, SLOT(changeNatMAges()));
            disconnect (currPattern->getFemNatMParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeNatMParams()));
            disconnect (currPattern->getFemNatMTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeNatMTVParams()));

            disconnect (currPattern->getFemWtLenParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeWtLenParams()));
            disconnect (currPattern->getFemWtLenTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeWtLenTVParams()));
            disconnect (currPattern->getMaleWtLenParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeWtLenParams()));
            disconnect (currPattern->getMaleWtLenTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeWtLenTVParams()));

            disconnect (currPattern->getFemMatureParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeFecundityParams()));
            disconnect (currPattern->getFemMatureTVParams(), SIGNAL(dataChanged()),
                     this, SLOT(changeFecundityTVParams()));
        }
    currPattern = gp;

    growthFemParamsView->setModel(currPattern->getFemGrowthParams());
    growthFemTVParamsView->setModel(currPattern->getFemGrowthTVParams());
    connect (currPattern->getFemGrowthParams(), SIGNAL(dataChanged()),
             SLOT(changeGrowthParams()));
    connect (currPattern->getFemGrowthTVParams(), SIGNAL(dataChanged()),
             SLOT(changeGrowthTVParams()));
    growthMaleParamsView->setModel(currPattern->getMaleGrowthParams());
    growthMaleTVParamsView->setModel(currPattern->getMaleGrowthTVParams());
    connect (currPattern->getMaleGrowthParams(), SIGNAL(dataChanged()),
             SLOT(changeGrowthParams()));
    connect (currPattern->getMaleGrowthTVParams(), SIGNAL(dataChanged()),
             SLOT(changeGrowthTVParams()));
    changeGrowthParams();

    cvFemParamsView->setModel(currPattern->getFemCVParams());
    cvFemTVParamsView->setModel(currPattern->getFemCVTVParams());
    connect (currPattern->getFemCVParams(), SIGNAL(dataChanged()),
             SLOT(changeCVParams()));
    connect (currPattern->getFemCVTVParams(), SIGNAL(dataChanged()),
             SLOT(changeCVTVParams()));
    cvMaleParamsView->setModel(currPattern->getMaleCVParams());
    cvMaleTVParamsView->setModel(currPattern->getMaleCVTVParams());
    connect (currPattern->getMaleCVParams(), SIGNAL(dataChanged()),
             SLOT(changeCVParams()));
    connect (currPattern->getMaleCVTVParams(), SIGNAL(dataChanged()),
             SLOT(changeCVParams()));
    changeCVParams();

    fecundParamsView->setModel(currPattern->getFemMatureParams());
    fecundTVParamsView->setModel(currPattern->getFemMatureTVParams());
    connect (currPattern->getFemMatureParams(), SIGNAL(dataChanged()),
             SLOT(changeFecundityParams()));
    connect (currPattern->getFemMatureTVParams(), SIGNAL(dataChanged()),
             SLOT(changeFecundityTVParams()));
    changeFecundityParams();

    fracFemParamsView->setModel(currPattern->getFractionFemaleParams());
    fracFemTVParamsView->setModel(currPattern->getFracFmTVParams());
    connect (currPattern->getFractionFemaleParams(), SIGNAL(dataChanged()),
             SLOT(changeFracFemParams()));
    connect (currPattern->getFracFmTVParams(), SIGNAL(dataChanged()),
             SLOT(changeFracFemTVParams()));
    changeFracFemParams();

    mortFemParamsView->setModel(currPattern->getFemNatMParams());
    mortFemTVParamsView->setModel(currPattern->getFemNatMTVParams());
    connect (currPattern->getFemNatMParams(), SIGNAL(dataChanged()),
             SLOT(changeNatMParams()));
    connect (currPattern->getFemNatMTVParams(), SIGNAL(dataChanged()),
             SLOT(changeNatMTVParams()));
    mortMaleParamsView->setModel(currPattern->getMaleNatMParams());
    mortMaleTVParamsView->setModel(currPattern->getMaleNatMTVParams());
    connect (currPattern->getMaleNatMParams(), SIGNAL(dataChanged()),
             SLOT(changeNatMParams()));
    connect (currPattern->getMaleNatMTVParams(), SIGNAL(dataChanged()),
             SLOT(changeNatMTVParams()));
    changeNatMParams();

    mortAgesView->setModel(currPattern->getNatMAges());
    connect (currPattern->getNatMAges(), SIGNAL(dataChanged()),
             SLOT(changeNatMAges()));
    changeNatMAges();

    wtlenFemParamsView->setModel(gp->getFemWtLenParams());
    wtlenFemTVParamsView->setModel(gp->getFemWtLenTVParams());
    connect (gp->getFemWtLenParams(), SIGNAL(dataChanged()),
             SLOT(changeWtLenParams()));
    connect (gp->getFemWtLenTVParams(), SIGNAL(dataChanged()),
             SLOT(changeWtLenTVParams()));
    wtlenMaleParamsView->setModel(gp->getMaleWtLenParams());
    wtlenMaleTVParamsView->setModel(gp->getMaleWtLenTVParams());
    connect (gp->getMaleWtLenParams(), SIGNAL(dataChanged()),
             SLOT(changeWtLenParams()));
    connect (gp->getMaleWtLenTVParams(), SIGNAL(dataChanged()),
             SLOT(changeWtLenTVParams()));
    changeWtLenParams();

    hermaphParamsView->setModel(gp->getHermaphParams());
    connect (gp->getHermaphParams(), SIGNAL(dataChanged()),
             SLOT(changeHermaphParams()));
    changeHermaphParams();
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
    ui->spinBox_growth_pattern->setMaximum(temp_int);
    if (temp_int > 0)
    {
        changeGrowthPattern(1);
        temp_int = pop->Grow()->getNum_morphs();
        ui->spinBox_growth_num_submorphs->setValue(temp_int);
        changeNumSubMorph(temp_int);
    }

    // recruitment
    changeRecrFullParams();
    changeRecrAssignParams();
    changeRecrDistParams();
    changeRecrCycleParams();
    changeRecrDevParams();

    setRecrArea(pop->SR()->getDistribArea());
    setRecrDistParam(pop->SR()->getDistribMethod());
//    ui->checkBox_recr_interaction->setChecked(pop->SR()->getDoRecruitInteract());
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
    ui->doubleSpinBox_recr_dev_last_nobias->setValue(pop->SR()->getNobiasLastEarlyYr());
    ui->doubleSpinBox_recr_dev_first_bias->setValue(pop->SR()->getFullbiasFirstYr());
    ui->doubleSpinBox_recr_dev_last_bias->setValue(pop->SR()->getFullbiasLastYr());
    ui->doubleSpinBox_recr_dev_first_nobias->setValue(pop->SR()->getNobiasFirstRecentYr());
    ui->lineEdit_recr_dev_max_bias->setText(QString::number(pop->SR()->getMaxBiasAdjust()));
    ui->spinBox_recr_dev_cycles->setValue(pop->SR()->getRecCycles());
    ui->lineEdit_recr_dev_min_dev->setText(QString::number(pop->SR()->getRecDevMin()));
    ui->lineEdit_recr_dev_max_dev->setText(QString::number(pop->SR()->getRecDevMax()));
    ui->spinBox_num_recr_devs->setValue(pop->SR()->getNumRecDev());
    changeRecrNumDevs(pop->SR()->getNumRecDev());
    ui->spinBox_spwn_recr_time_vary_read->setValue(pop->SR()->getTimeVaryReadParams());

    // Maturity
    setMaturityOpt(pop->Grow()->getMaturity_option());
    ui->checkBox_wtatage->setChecked(model_data->getReadWtAtAge());
    ui->lineEdit_maturity_first_age->setText(QString::number(pop->Grow()->getFirst_mature_age()));

    maturityParamsView->setModel(pop->Grow()->getCohortParams());
    maturityTVParamsView->setModel(pop->Grow()->getCohortTVParams());
    changeCohortParams();

    // fecundity
    setFecundityOption(pop->Fec()->getMethod());
    setFecundityOffsetOption(pop->Grow()->getParam_offset_method());
    setFecundityAdjustment(pop->Grow()->getAdjustment_method());
    setHermaphOptions(pop->Fec()->getHermaphroditism());
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
    connect (pop->Move()->getMovementParams(), SIGNAL(dataChanged()), SLOT(changeMoveParams()));
    moveTVParamsView->setModel(pop->Move()->getMoveTVParams());
    connect (pop->Move()->getMoveTVParams(), SIGNAL(dataChanged()), SLOT(changeMoveTVParams()));
    changeMoveParams();

    // Mortality
    mortBreakPtsView->setModel(pop->Grow()->getNatMortValues());
    mortBreakPtsView->setHeight(pop->Grow()->getNatMortValues());

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
    seasonParamsView->setHeight(pop->getNumSeasParams());

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
    Q_UNUSED(opt);
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
    int numgen = (model_data->get_num_genders() > 1)? 2: 1;
    ui->comboBox_mort_option->setCurrentIndex(opt);
    ui->widget_mort_breakpoints->setVisible(false);
    ui->widget_mort_lorenz->setVisible(false);
    ui->label_mort_age_specific->setVisible(false);
    mortAgesView->setVisible(false);
    mortFemParamsView->setVisible(true);
    switch (opt)
    {
    case 0:
        mortFemParamsView->setHeight(numgen);
        break;
    case 1: // breakpoints
        ui->widget_mort_breakpoints->setVisible(true);
        ui->spinBox_mort_num_breakpoints->setValue(pop->Grow()->getNatMortNumBreakPts());
        mortFemParamsView->setHeight(pop->Grow()->getPattern(0)->getFemNatMParams());
        break;
    case 2: // lorenzen
        ui->widget_mort_lorenz->setVisible(true);
        ui->spinBox_mort_lorenz_int->setValue(pop->Grow()->getNaturalMortLorenzenRef());
        mortFemParamsView->setHeight(numgen);
        break;
    case 3: // specific age
    case 4:
        ui->label_mort_age_specific->setVisible(true);
        mortAgesView->setVisible(true);
        ui->label_mort_f_params->setVisible(false);
        mortFemParamsView->setVisible(false);
        ui->label_mort_m_params->setVisible(false);
        mortMaleParamsView->setVisible(false);

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
    ui->label_mort_f_params->setVisible(true);
    mortFemParamsView->setVisible(true);
    switch (opt)
    {
    case 0:
        mortFemParamsView->setHeight(numgen);
        break;
    case 1:
        ui->widget_mort_breakpoints->setVisible(true);
        ui->spinBox_mort_num_breakpoints->setValue(pop->Grow()->getNatMortNumBreakPts());
        mortFemParamsView->setHeight(pop->Grow()->getPattern(0)->getFemNatMParams());
        break;
    case 2:
        ui->widget_mort_lorenz->setVisible(true);
        ui->spinBox_mort_lorenz_int->setValue(pop->Grow()->getNaturalMortLorenzenRef());
        mortFemParamsView->setHeight(numgen);
        break;
    case 3:
    case 4:
        ui->label_mort_age_specific->setVisible(true);
        mortAgesView->setVisible(true);
        ui->label_mort_f_params->setVisible(false);
        mortFemParamsView->setVisible(false);
        break;
    }

}

void population_widget::changeMortNumBkpts(int num)
{
    int pat = ui->spinBox_growth_pattern->value() - 1;
    pop->Grow()->setNatMortNumBreakPts(num);
    mortBreakPtsView->resizeColumnsToContents();
    mortFemParamsView->setHeight(pop->Grow()->getPattern(pat)->getFemNatMParams());
    mortFemParamsView->resizeColumnsToContents();
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
                between = std::sqrt(1. / (1. + value * value));
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
    pop->Grow()->setTimeVaryMethod(num+1);
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
        between = std::sqrt(1. / (1. + ratio * ratio));
        within = ratio * between;
    }
    ui->doubleSpinBox_growth_morph_between->setValue(between);
    ui->doubleSpinBox_growth_morph_within->setValue(within);
}

void population_widget::changeGrowthTimeVaryRead(int flag)
{
    bool vis = (flag != 0);
    pop->Grow()->setTimeVaryReadParams(flag);
    growthFemTVParamsView->setEnabled(vis);
    growthMaleTVParamsView->setEnabled(vis);
    wtlenFemTVParamsView->setEnabled(vis);
    wtlenMaleTVParamsView->setEnabled(vis);
    cvFemTVParamsView->setEnabled(vis);
    cvMaleTVParamsView->setEnabled(vis);
    maturityTVParamsView->setEnabled(vis);
    fecundTVParamsView->setEnabled(vis);
    moveTVParamsView->setEnabled(vis);
    mortFemTVParamsView->setEnabled(vis);
    mortMaleTVParamsView->setEnabled(vis);
}

void population_widget::changeGrowthParams()
{
    growthFemParamsView->setHeight(currPattern->getFemGrowthParams());
    growthFemParamsView->resizeColumnsToContents();
    int num = currPattern->getMaleGrowthParams()->rowCount();
    growthMaleParamsView->setHeight(num);
    growthMaleParamsView->resizeColumnsToContents();
    growthMaleParamsView->setVisible(num);
    changeGrowthTVParams();
}

void population_widget::changeGrowthTVParams()
{
    int num = currPattern->getFemGrowthTVParams()->rowCount();
    growthFemTVParamsView->setVisible(num);
    ui->label_growth_time_vary_params->setVisible(num);
    growthFemTVParamsView->setHeight(num);
    growthFemTVParamsView->resizeColumnsToContents();
    num = currPattern->getMaleGrowthTVParams()->rowCount();
    growthMaleTVParamsView->setVisible(num);
    ui->label_growth_time_vary_params_2->setVisible(num);
    growthMaleTVParamsView->setHeight(num);
    growthMaleTVParamsView->resizeColumnsToContents();
}

void population_widget::changeCVParams()
{
    cvFemParamsView->setHeight(currPattern->getFemCVParams());
    cvFemParamsView->resizeColumnsToContents();
    int num = currPattern->getMaleCVParams()->rowCount();
    cvMaleParamsView->setHeight(num);
    cvMaleParamsView->resizeColumnsToContents();
    cvMaleParamsView->setVisible(num);
    changeCVTVParams();
}

void population_widget::changeCVTVParams()
{
    int num = currPattern->getFemCVTVParams()->rowCount();
    cvFemTVParamsView->setVisible(num);
    ui->label_cv_time_vary_parameters->setVisible(num);
    cvFemTVParamsView->setHeight(num);
    cvFemTVParamsView->resizeColumnsToContents();
    num = currPattern->getMaleCVTVParams()->rowCount();
    cvMaleTVParamsView->setVisible(num);
    ui->label_cv_time_vary_parameters_2->setVisible(num);
    cvMaleTVParamsView->setHeight(num);
    cvMaleTVParamsView->resizeColumnsToContents();
}

void population_widget::changeNatMAges()
{
    mortAgesView->setHeight(currPattern->getNatMAges());
    mortAgesView->resizeColumnsToContents();
}

void population_widget::changeNatMParams()
{
    mortFemParamsView->setHeight(currPattern->getFemNatMParams());
    mortFemParamsView->resizeColumnsToContents();
    int num = currPattern->getMaleNatMParams()->rowCount();
    mortMaleParamsView->setHeight(num);
    mortMaleParamsView->resizeColumnsToContents();
    mortMaleParamsView->setVisible(num);
    changeNatMTVParams();
}

void population_widget::changeNatMTVParams()
{
    int num = currPattern->getFemNatMTVParams()->rowCount();
    mortFemTVParamsView->setVisible(num);
    ui->label_mort_f_tv_params->setVisible(num);
    mortFemTVParamsView->setHeight(num);
    mortFemTVParamsView->resizeColumnsToContents();
    num = currPattern->getMaleNatMTVParams()->rowCount();
    mortMaleTVParamsView->setVisible(num);
    ui->label_mort_m_tv_params->setVisible(num);
    mortMaleTVParamsView->setHeight(num);
    mortMaleTVParamsView->resizeColumnsToContents();
}

void population_widget::changeWtLenParams()
{
    wtlenFemParamsView->setHeight(currPattern->getFemWtLenParams());
    wtlenFemParamsView->resizeColumnsToContents();
    int num = currPattern->getMaleWtLenParams()->rowCount();
    wtlenMaleParamsView->setHeight(num);
    wtlenMaleParamsView->resizeColumnsToContents();
    wtlenMaleParamsView->setVisible(num);
    changeWtLenTVParams();
}

void population_widget::changeWtLenTVParams()
{
    int num = currPattern->getFemWtLenTVParams()->rowCount();
    wtlenFemTVParamsView->setVisible(num);
    ui->label_wt_len_f_time_vary_params->setVisible(num);
    wtlenFemTVParamsView->setHeight(num);
    wtlenFemTVParamsView->resizeColumnsToContents();
    num = currPattern->getMaleWtLenTVParams()->rowCount();
    wtlenMaleTVParamsView->setVisible(num);
    ui->label_wt_len_m_time_vary_params->setVisible(num);
    wtlenMaleTVParamsView->setHeight(num);
    wtlenMaleTVParamsView->resizeColumnsToContents();
}

void population_widget::changeCohortParams()
{
    maturityParamsView->setHeight(pop->Grow()->getCohortParams());
    maturityParamsView->resizeColumnsToContents();
    changeCohortTVParams();
}

void population_widget::changeCohortTVParams()
{
    int num = pop->Grow()->getCohortTVParams()->rowCount();
    maturityTVParamsView->setHeight(num);
    maturityTVParamsView->resizeColumnsToContents();
    maturityTVParamsView->setVisible(num);
    ui->label_maturity_time_vary_params->setVisible(num);
}

void population_widget::changeFecundityParams()
{
    fecundParamsView->setHeight(currPattern->getFemMatureParams());
    fecundParamsView->resizeColumnsToContents();
    changeFecundityTVParams();
}

void population_widget::changeFecundityTVParams()
{
    int num = currPattern->getFemMatureTVParams()->rowCount();
    fecundTVParamsView->setHeight(num);
    fecundTVParamsView->resizeColumnsToContents();
    fecundTVParamsView->setVisible(num);
    ui->label_fec_time_vary_params->setVisible(num);
}

void population_widget::changeFracFemParams()
{
    fracFemParamsView->setHeight(1);
    fracFemParamsView->resizeColumnsToContents();
    changeFracFemTVParams();
}
void population_widget::changeFracFemTVParams()
{
    int num = currPattern->getNumFracFmTVParams();
    fracFemTVParamsView->setHeight(num);
    fracFemTVParamsView->resizeColumnsToContents();
    fracFemTVParamsView->setVisible(num);
    ui->label_frac_tfemale_tv_params->setVisible(num);
}

void population_widget::changeHermaphParams()
{
    int num = currPattern->getNumHermaphParams();
    hermaphParamsView->setHeight(num);
    hermaphParamsView->resizeColumnsToContents();
    hermaphParamsView->setVisible(num);
    changeHermaphTVParams();
}
void population_widget::changeHermaphTVParams()
{
    int num = currPattern->getNumHermaphTVParams();
    hermaphTVParamsView->setHeight(num);
    hermaphTVParamsView->resizeColumnsToContents();
    hermaphTVParamsView->setVisible(num);
    ui->label_hermaph_tv_params->setVisible(num);
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
    pop->SR()->setTimeVaryReadParams(flag);
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
    int option = opt + 1;
//    bool vis = true;
    pop->Grow()->setMaturity_option(option);
    if (option == 5)
        ui->checkBox_wtatage->setChecked(true);
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

void population_widget::changeMoveParams()
{
    int num = pop->Move()->getNumParams();
    moveParamsView->setHeight(num);
    moveParamsView->resizeColumnsToContents();
    moveParamsView->setVisible(num);
    ui->label_movement_params->setVisible(num);
    changeMoveTVParams();
}

void population_widget::changeMoveTVParams()
{
    int num = pop->Move()->getNumTVParams();
    moveTVParamsView->setHeight(num);
    moveTVParamsView->resizeColumnsToContents();
    moveTVParamsView->setVisible(num);
    ui->label_move_tv_params->setVisible(num);
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
    recruitParamsView->setHeight(pop->SR()->getNumFullParameters());
    recruitParamsView->resizeColumnsToContents();
    changeRecrTVParams();
}
void population_widget::changeRecrTVParams()
{
    int num = pop->SR()->getNumTVParameters();
    recruitTVParamsView->setHeight(num);
    recruitTVParamsView->resizeColumnsToContents();
    recruitTVParamsView->setVisible(num);
    ui->label_recr_time_vary_params->setVisible(num);
}
void population_widget::changeRecrAssignParams()
{
    int num = pop->SR()->getNumAssignments();
    recruitAssignParamsView->setHeight(num);
    recruitAssignParamsView->resizeColumnsToContents();
    pop->SR()->setAssignTimings();
    changeRecrDistAssignments(pop->SR()->getDistribMethod());
}
void population_widget::changeRecrDistParams()
{
    int num = pop->SR()->getNumDistParams();
    recruitDistParamsView->setHeight(num);
    recruitDistParamsView->resizeColumnsToContents();
    changeRecrDistTVParams();
}
void population_widget::changeRecrDistTVParams()
{
    int num = pop->SR()->getNumDistTVParams();
    recruitDistTVParamsView->setHeight(num);
    recruitDistTVParamsView->resizeColumnsToContents();
    recruitDistTVParamsView->setVisible(num);
    ui->label_recr_dist_time_vary_params->setVisible(num);
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
    changeRecrDistAssignments(method);
}

void population_widget::changeRecrDistParam(int method)
{
    if (method != pop->SR()->getDistribMethod())
    {
        pop->SR()->setDistribMethod(method);
        changeRecrDistAssignments(method);
        refresh();
    }
//    if (method == 1)  // no longer used
//        ui->checkBox_recr_interaction->setVisible(true);
//    else
//        ui->checkBox_recr_interaction->setVisible(false);
}

void population_widget::changeRecrDistAssignments(int method)
{
    int i, num = 0;
    if (method == 2)
    {
        num = pop->Grow()->getNum_patterns() +
                model_data->get_num_areas() +
                pop->SR()->getNumAssignTimings();
        pop->SR()->setNumDistParams(num);
        num = 0;
        for (i = 0; i < pop->Grow()->getNum_patterns(); i++, num++)
            pop->SR()->getDistParams()->setRowHeader(num, QString("RecrDist_GP_%1").arg(QString::number(i+1)));
        for (i = 0; i < model_data->get_num_areas(); i++, num++)
            pop->SR()->getDistParams()->setRowHeader(num, QString("RecrDist_Area_%1").arg(QString::number(i+1)));
        for (i = 0; i < pop->SR()->getNumAssignTimings(); i++, num++)
            pop->SR()->getDistParams()->setRowHeader(num, QString("RecrDist_Timing_%1").arg(QString::number(i+1)));
    }
    else if (method == 3)
    {
        pop->SR()->setNumDistParams(pop->SR()->getNumAssignments());
        for (i = 0; i < pop->SR()->getNumAssignments(); i++)
            pop->SR()->getDistParams()->setRowHeader(i, QString("RecrDist_Assignmnt_%1").arg(QString::number(i+1)));
    }
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
    pop->SR()->setAssignTimings();
    changeRecrDistAssignments(pop->SR()->getDistribMethod());
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

void population_widget::changeRecrNoBiasLastYr (double yr)
{
    pop->SR()->setNobiasLastEarlyYr(yr);
}

void population_widget::changeRecrFullBiasFirstYr (double yr)
{
    pop->SR()->setFullbiasFirstYr(yr);
}

void population_widget::changeRecrFullBiasLastYr (double yr)
{
    pop->SR()->setFullbiasLastYr(yr);
}

void population_widget::changeRecrNoBiasFirstYr (double yr)
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
    if (spec > 10) spec = 10;
    ui->comboBox_recr_spec->setCurrentIndex(spec - 1);
    srEquationView->setEquationNumber(spec);
}

void population_widget::changeSpawnRecrSpec(int num)
{
    int mthd = num + 1;
    int old_mthd = pop->SR()->getMethod();
    if (mthd != old_mthd)
    {
        pop->SR()->setMethod(mthd);
        recruitParamsView->setHeight(pop->SR()->getNumFullParameters());
        srEquationView->changeEquationNumber(mthd);
    }
}

void population_widget::setSRequationDialogVisible(bool checked)
{
    ui->pushButton_equation->setChecked(checked);
    if (checked) {
        srEquationView->show();
        ui->pushButton_equation->setText("Hide Curve");
    }
    else {
        ui->pushButton_equation->setText("Show Curve");
    }
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

