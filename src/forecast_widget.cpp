#include "forecast_widget.h"
#include "ui_forecast_widget.h"

#include <QMessageBox>

#include "metadata.h"
#include "errorfloatdialog.h"
#include "message.h"

forecast_widget::forecast_widget(ss_model *m_data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::forecast_widget)
{
    ui->setupUi(this);
    ui->label_fcast_loops_5->setVisible(false);
    ui->spinBox_fcast_loops_5->setVisible(false);

    model_data = m_data;
    ss_forecast *fcast = model_data->forecast;

    seasFltRelF = new tableview();
    seasFltRelF->setParent(this);
    ui->verticalLayout_seas_flt_relf->addWidget(seasFltRelF);
    seasFltRelF->setHeight(0);

    maxCatchFleets = new tableview();
    maxCatchFleets->setParent(this);
    ui->horizontalLayout_fleet_max->addWidget(maxCatchFleets);
    maxCatchFleets->setHeight(1);

    maxCatchAreas = new tableview();
    maxCatchAreas->setParent(this);
    ui->horizontalLayout_area_max->addWidget(maxCatchAreas);
    maxCatchAreas->setHeight(1);

    allocGrpList = new tableview();
    allocGrpList->setParent(this);
    ui->horizontalLayout_assignment->addWidget(allocGrpList);
    allocGrpList->setHeight(0);

    allocGrpFracts = new tableview();
    allocGrpFracts->setParent(this);
    ui->horizontalLayout_group_fractions->addWidget(allocGrpFracts);
    allocGrpFracts->setHeight(0);

    inputFcastCatch = new tableview();
    inputFcastCatch->setParent(this);
    ui->verticalLayout_catch_basis->addWidget(inputFcastCatch);
    inputFcastCatch->setHeight(0);

    if (fcast->getNumFixedFcastCatch() > 0)
    {
        ui->label_input_seas_relf->setVisible(true);
    }

    refresh();
    ui->tabWidget->setCurrentIndex(0);
}

forecast_widget::~forecast_widget()
{
    delete ui;
}

void forecast_widget::reset()
{
    ui->comboBox_benchmarks->setCurrentIndex(1);
    ui->comboBox_MSY_options->setCurrentIndex(1);
    ui->lineEdit_SPR_target->setText("0.4");
    ui->lineEdit_biomass_target->setText("0.2");
    ui->spinBox_bmark_bio_beg->setValue(0);
    ui->spinBox_bmark_bio_end->setValue(0);
    ui->spinBox_bmark_sel_beg->setValue(0);
    ui->spinBox_bmark_sel_end->setValue(0);
    ui->spinBox_bmark_relf_beg->setValue(0);
    ui->spinBox_bmark_relf_end->setValue(0);
    ui->spinBox_bmark_recr_beg->setValue(0);
    ui->spinBox_bmark_recr_end->setValue(0);
    ui->comboBox_bmark_relF_basis->setCurrentIndex(0);
    ui->comboBox_fcast_options->setCurrentIndex(1);
    ui->spinBox_fcast_yr_num->setValue(4);
    ui->lineEdit_F_scalar->setText("0.2");
    ui->spinBox_fcast_sel_beg->setValue(0);
    ui->spinBox_fcast_sel_end->setValue(0);
    ui->spinBox_fcast_relf_beg->setValue(0);
    ui->spinBox_fcast_relf_end->setValue(0);
    ui->spinBox_fcast_recr_beg->setValue(0);
    ui->spinBox_fcast_recr_end->setValue(0);
    ui->comboBox_control_rule->setCurrentIndex(0);
    ui->lineEdit_ctl_rule_const_F->setText("0.4");
    ui->lineEdit_ctl_rule_no_F->setText("0.1");
    ui->spinBox_num_forecast_loops->setValue(3);
    ui->spinBox_first_loop->setValue(1);
    ui->spinBox_fcast_recr_adjust->setValue(0);
    ui->lineEdit_fcast_recr_adj_value->setText("0.0");
    ui->spinBox_fcast_loops_5->setValue(0);
    ui->spinBox_first_caps_yr->setValue(-1);
    ui->lineEdit_log_sd->setText("0.0");
    ui->spinBox_rebuilder_yinit->setValue(-1);
    ui->spinBox_rebuilder_ydecl->setValue(-1);
    ui->comboBox_fleet_relF->setCurrentIndex(0);
    ui->comboBox_input_catch_basis->setCurrentIndex(0);
//    ui->lineEdit_max_catch_fleet->setText("-1");
//    ui->lineEdit_max_catch_area->setText("-1");
}

void forecast_widget::disconnectAll(ss_forecast *fcast)
{
    disconnect (ui->comboBox_benchmarks, SIGNAL(currentIndexChanged(int)), this, SLOT(change_bmarks(int)));
    disconnect (ui->comboBox_MSY_options, SIGNAL(currentIndexChanged(int)), this, SLOT(change_MSY(int)));
    disconnect (ui->lineEdit_SPR_target, SIGNAL(editingFinished()), this, SLOT(change_spr_target()));
    disconnect (ui->lineEdit_biomass_target, SIGNAL(editingFinished()), this, SLOT(change_biomass_target()));
    disconnect (ui->spinBox_bmark_bio_beg, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_bio_begin(int)));
    disconnect (ui->spinBox_bmark_bio_end, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_bio_end(int)));
    disconnect (ui->spinBox_bmark_sel_beg, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_sel_begin(int)));
    disconnect (ui->spinBox_bmark_sel_end, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_sel_end(int)));
    disconnect (ui->spinBox_bmark_relf_beg, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_relf_begin(int)));
    disconnect (ui->spinBox_bmark_relf_end, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_relf_end(int)));
    disconnect (ui->spinBox_bmark_recr_beg, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_recr_begin(int)));
    disconnect (ui->spinBox_bmark_recr_end, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_recr_end(int)));
    disconnect (ui->spinBox_bmark_srparm_beg, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_srparm_begin(int)));
    disconnect (ui->spinBox_bmark_srparm_end, SIGNAL(valueChanged(int)), this, SLOT(set_bmark_srparm_end(int)));

    disconnect (ui->comboBox_bmark_relF_basis, SIGNAL(currentIndexChanged(int)), this, SLOT(change_bmark_relF(int)));
    disconnect (ui->comboBox_fcast_options, SIGNAL(currentIndexChanged(int)), this, SLOT(change_forecast(int)));
    disconnect (ui->spinBox_fcast_yr_num, SIGNAL(valueChanged(int)), fcast, SLOT(set_num_forecast_years(int)));
    disconnect (ui->lineEdit_F_scalar, SIGNAL(editingFinished()), this, SLOT(change_F_mult()));
    disconnect (ui->spinBox_fcast_sel_beg, SIGNAL(valueChanged(int)), this, SLOT(set_fcast_sel_begin(int)));
    disconnect (ui->spinBox_fcast_sel_end, SIGNAL(valueChanged(int)), this, SLOT(set_fcast_sel_end(int)));
    disconnect (ui->spinBox_fcast_relf_beg, SIGNAL(valueChanged(int)), this, SLOT(set_fcast_relf_begin(int)));
    disconnect (ui->spinBox_fcast_relf_end, SIGNAL(valueChanged(int)), this, SLOT(set_fcast_relf_end(int)));
    disconnect (ui->spinBox_fcast_recr_beg, SIGNAL(valueChanged(int)), this, SLOT(set_fcast_recr_begin(int)));
    disconnect (ui->spinBox_fcast_recr_end, SIGNAL(valueChanged(int)), this, SLOT(set_fcast_recr_end(int)));

    disconnect (ui->comboBox_control_rule, SIGNAL(currentIndexChanged(int)), this, SLOT(change_cr_method(int)));
    disconnect (ui->lineEdit_ctl_rule_const_F, SIGNAL(editingFinished()), this, SLOT(set_cr_biomass_const_f()));
    disconnect (ui->lineEdit_ctl_rule_no_F, SIGNAL(editingFinished()), this, SLOT(set_cr_biomass_no_f()));
    disconnect (ui->lineEdit_ctl_tgt_as_fraction, SIGNAL(editingFinished()), this, SLOT(set_cr_target()));

    disconnect (ui->spinBox_num_forecast_loops, SIGNAL(valueChanged(int)), this, SLOT(set_num_forecast_loops(int)));
    disconnect (ui->spinBox_first_loop, SIGNAL(valueChanged(int)), this, SLOT(change_first_forecast_loop(int)));
/*    disconnect (ui->spinBox_fcast_loops_3, SIGNAL(valueChanged(int)), fcast, SLOT(set_forecast_loop_ctl3(int)));
    disconnect (ui->spinBox_fcast_loops_4, SIGNAL(valueChanged(int)), fcast, SLOT(set_forecast_loop_ctl4(int)));
    disconnect (ui->spinBox_fcast_loops_5, SIGNAL(valueChanged(int)), fcast, SLOT(set_forecast_loop_ctl5(int)));*/

    disconnect (ui->spinBox_first_caps_yr, SIGNAL(valueChanged(int)), fcast, SLOT(set_caps_alloc_st_year(int)));
    disconnect (ui->lineEdit_log_sd, SIGNAL(editingFinished()), this, SLOT(set_log_catch_std_dev()));
    disconnect (ui->groupBox_rebuilder, SIGNAL(toggled(bool)), fcast, SLOT(set_do_rebuilder(bool)));
    disconnect (ui->spinBox_rebuilder_ydecl, SIGNAL(valueChanged(int)), this, SLOT(set_rebuilder_first_year(int)));
    disconnect (ui->spinBox_rebuilder_yinit, SIGNAL(valueChanged(int)), this, SLOT(set_rebuilder_curr_year(int)));

    disconnect (ui->comboBox_fleet_relF, SIGNAL(currentIndexChanged(int)), this, SLOT(change_fleet_relF(int)));

    disconnect (ui->comboBox_tuning_basis, SIGNAL(currentIndexChanged(int)), fcast, SLOT(set_combo_box_catch_tuning(int)));

    disconnect (ui->comboBox_input_catch_basis, SIGNAL(currentIndexChanged(int)), this, SLOT(change_fixed_catch(int)));
    disconnect (ui->spinBox_fixed_catch_obs, SIGNAL(valueChanged(int)), this, SLOT(change_num_input_obs(int)));

    disconnect (fcast, SIGNAL(allocGrpsChanged()), this, SLOT(alloc_group_assign_changed()));
    disconnect (ui->groupBox_alloc_groups, SIGNAL(toggled(bool)), this, SLOT(change_use_alloc_groups(bool)));
//    disconnect (ui->lineEdit_alloc_assignments, SIGNAL(editingFinished()), this, SLOT(alloc_group_assign_changed()));

//    disconnect (ui->spinBox_fcast_levels, SIGNAL(valueChanged(int)), fcast, SLOT(set_num_catch_levels(int)));

//    disconnect (ui->lineEdit_max_catch_fleet, SIGNAL(editingFinished()), this, SLOT(change_max_catch_fleet()));
//    disconnect (ui->lineEdit_max_catch_area, SIGNAL(editingFinished()), this, SLOT(change_max_catch_area()));
}

void forecast_widget::connectAll(ss_forecast *fcast)
{
    connect (ui->comboBox_benchmarks, SIGNAL(currentIndexChanged(int)), SLOT(change_bmarks(int)));
    connect (ui->comboBox_MSY_options, SIGNAL(currentIndexChanged(int)), SLOT(change_MSY(int)));
    connect (ui->lineEdit_SPR_target, SIGNAL(editingFinished()), SLOT(change_spr_target()));
    connect (ui->lineEdit_biomass_target, SIGNAL(editingFinished()), SLOT(change_biomass_target()));
    connect (ui->spinBox_bmark_bio_beg, SIGNAL(valueChanged(int)), SLOT(set_bmark_bio_begin(int)));
    connect (ui->spinBox_bmark_bio_end, SIGNAL(valueChanged(int)), SLOT(set_bmark_bio_end(int)));
    connect (ui->spinBox_bmark_sel_beg, SIGNAL(valueChanged(int)), SLOT(set_bmark_sel_begin(int)));
    connect (ui->spinBox_bmark_sel_end, SIGNAL(valueChanged(int)), SLOT(set_bmark_sel_end(int)));
    connect (ui->spinBox_bmark_relf_beg, SIGNAL(valueChanged(int)), SLOT(set_bmark_relf_begin(int)));
    connect (ui->spinBox_bmark_relf_end, SIGNAL(valueChanged(int)), SLOT(set_bmark_relf_end(int)));
    connect (ui->spinBox_bmark_recr_beg, SIGNAL(valueChanged(int)), SLOT(set_bmark_recr_begin(int)));
    connect (ui->spinBox_bmark_recr_end, SIGNAL(valueChanged(int)), SLOT(set_bmark_recr_end(int)));
    connect (ui->spinBox_bmark_srparm_beg, SIGNAL(valueChanged(int)), SLOT(set_bmark_srparm_begin(int)));
    connect (ui->spinBox_bmark_srparm_end, SIGNAL(valueChanged(int)), SLOT(set_bmark_srparm_end(int)));

    connect (ui->comboBox_bmark_relF_basis, SIGNAL(currentIndexChanged(int)), SLOT(change_bmark_relF(int)));
    connect (ui->comboBox_fcast_options, SIGNAL(currentIndexChanged(int)), SLOT(change_forecast(int)));
    connect (ui->spinBox_fcast_yr_num, SIGNAL(valueChanged(int)), fcast, SLOT(set_num_forecast_years(int)));
    connect (ui->lineEdit_F_scalar, SIGNAL(editingFinished()), SLOT(change_F_mult()));
    connect (ui->spinBox_fcast_sel_beg, SIGNAL(valueChanged(int)), SLOT(set_fcast_sel_begin(int)));
    connect (ui->spinBox_fcast_sel_end, SIGNAL(valueChanged(int)), SLOT(set_fcast_sel_end(int)));
    connect (ui->spinBox_fcast_relf_beg, SIGNAL(valueChanged(int)), SLOT(set_fcast_relf_begin(int)));
    connect (ui->spinBox_fcast_relf_end, SIGNAL(valueChanged(int)), SLOT(set_fcast_relf_end(int)));
    connect (ui->spinBox_fcast_recr_beg, SIGNAL(valueChanged(int)), SLOT(set_fcast_recr_begin(int)));
    connect (ui->spinBox_fcast_recr_end, SIGNAL(valueChanged(int)), SLOT(set_fcast_recr_end(int)));

    connect (ui->comboBox_control_rule, SIGNAL(currentIndexChanged(int)), SLOT(change_cr_method(int)));
    connect (ui->lineEdit_ctl_rule_const_F, SIGNAL(editingFinished()), SLOT(set_cr_biomass_const_f()));
    connect (ui->lineEdit_ctl_rule_no_F, SIGNAL(editingFinished()), SLOT(set_cr_biomass_no_f()));
    connect (ui->lineEdit_ctl_tgt_as_fraction, SIGNAL(editingFinished()), SLOT(set_cr_target()));

    connect (ui->spinBox_num_forecast_loops, SIGNAL(valueChanged(int)), SLOT(set_num_forecast_loops(int)));
    connect (ui->spinBox_first_loop, SIGNAL(valueChanged(int)), SLOT(change_first_forecast_loop(int)));
/*    connect (ui->spinBox_fcast_loops_3, SIGNAL(valueChanged(int)), fcast, SLOT(set_forecast_loop_ctl3(int)));
    connect (ui->spinBox_fcast_loops_4, SIGNAL(valueChanged(int)), fcast, SLOT(set_forecast_loop_ctl4(int)));
    connect (ui->spinBox_fcast_loops_5, SIGNAL(valueChanged(int)), fcast, SLOT(set_forecast_loop_ctl5(int)));*/

    connect (ui->spinBox_first_caps_yr, SIGNAL(valueChanged(int)), fcast, SLOT(set_caps_alloc_st_year(int)));
    connect (ui->lineEdit_log_sd, SIGNAL(editingFinished()), SLOT(set_log_catch_std_dev()));
    connect (ui->groupBox_rebuilder, SIGNAL(toggled(bool)), fcast, SLOT(set_do_rebuilder(bool)));
    connect (ui->spinBox_rebuilder_ydecl, SIGNAL(valueChanged(int)), SLOT(set_rebuilder_first_year(int)));
    connect (ui->spinBox_rebuilder_yinit, SIGNAL(valueChanged(int)), SLOT(set_rebuilder_curr_year(int)));

    connect (ui->comboBox_fleet_relF, SIGNAL(currentIndexChanged(int)), SLOT(change_fleet_relF(int)));

    connect (ui->comboBox_tuning_basis, SIGNAL(currentIndexChanged(int)), fcast, SLOT(set_combo_box_catch_tuning(int)));

    connect (ui->comboBox_input_catch_basis, SIGNAL(currentIndexChanged(int)), SLOT(change_fixed_catch(int)));
    connect (ui->spinBox_fixed_catch_obs, SIGNAL(valueChanged(int)), SLOT(change_num_input_obs(int)));

    connect (fcast, SIGNAL(allocGrpsChanged()), SLOT(alloc_group_assign_changed()));
    connect (ui->groupBox_alloc_groups, SIGNAL(toggled(bool)), SLOT(change_use_alloc_groups(bool)));
//    connect (ui->lineEdit_alloc_assignments, SIGNAL(editingFinished()), SLOT(alloc_group_assign_changed()));

//    connect (ui->spinBox_fcast_levels, SIGNAL(valueChanged(int)), fcast, SLOT(set_num_catch_levels(int)));

//    connect (ui->lineEdit_max_catch_fleet, SIGNAL(editingFinished()), SLOT(change_max_catch_fleet()));
//    connect (ui->lineEdit_max_catch_area, SIGNAL(editingFinished()), SLOT(change_max_catch_area()));
}

void forecast_widget::set_model(ss_model *m_data)
{
    ss_forecast *fcast = model_data->forecast;
    disconnectAll(fcast);
//    ss_model *old_model = model_data;
    model_data = m_data;
//    delete old_model;

    refresh ();
}

void forecast_widget::refresh()
{
    QString txt("");
    ss_forecast *fcast = model_data->forecast;

    disconnectAll(fcast);
    set_combo_box_bmarks(fcast->get_benchmarks());//ui->comboBox_benchmarks->setCurrentIndex(fcast->get_benchmarks());
    set_combo_box_MSY(fcast->get_MSY());//(ui->comboBox_MSY_options, fcast->get_MSY());
    ui->lineEdit_SPR_target->setText(QString::number(fcast->get_spr_target()));
    ui->lineEdit_biomass_target->setText(QString::number(fcast->get_biomass_target()));
    ui->spinBox_bmark_bio_beg->setValue(fcast->get_benchmark_year(0));
    set_bmark_bio_begin(fcast->get_benchmark_year(0));
    ui->spinBox_bmark_bio_end->setValue(fcast->get_benchmark_year(1));
    set_bmark_bio_end(fcast->get_benchmark_year(1));
    ui->spinBox_bmark_sel_beg->setValue(fcast->get_benchmark_year(2));
    set_bmark_sel_begin(fcast->get_benchmark_year(2));
    ui->spinBox_bmark_sel_end->setValue(fcast->get_benchmark_year(3));
    set_bmark_sel_end(fcast->get_benchmark_year(3));
    ui->spinBox_bmark_relf_beg->setValue(fcast->get_benchmark_year(4));
    set_bmark_relf_begin(fcast->get_benchmark_year(4));
    ui->spinBox_bmark_relf_end->setValue(fcast->get_benchmark_year(5));
    set_bmark_relf_end(fcast->get_benchmark_year(5));
    ui->spinBox_bmark_recr_beg->setValue(fcast->get_benchmark_year(6));
    set_bmark_recr_begin(fcast->get_benchmark_year(6));
    ui->spinBox_bmark_recr_end->setValue(fcast->get_benchmark_year(7));
    set_bmark_recr_end(fcast->get_benchmark_year(7));
    ui->spinBox_bmark_srparm_beg->setValue(fcast->get_benchmark_year(8));
    set_bmark_srparm_begin(fcast->get_benchmark_year(8));
    ui->spinBox_bmark_srparm_end->setValue(fcast->get_benchmark_year(9));
    set_bmark_srparm_end(fcast->get_benchmark_year(9));
    set_combo_box_bmark_relF(fcast->get_benchmark_rel_f());//(ui->comboBox_bmark_relF_basis, fcast->get_benchmark_rel_f());

    set_combo_box_fcast(fcast->get_forecast());//(ui->comboBox_fcast_options, fcast->get_forecast());
    ui->spinBox_fcast_yr_num->setValue(fcast->get_num_forecast_years());
    ui->lineEdit_F_scalar->setText(QString::number(fcast->get_f_scalar()));
    ui->spinBox_fcast_sel_beg->setValue(fcast->get_forecast_year(0));
    set_fcast_sel_begin(fcast->get_forecast_year(0));
    ui->spinBox_fcast_sel_end->setValue(fcast->get_forecast_year(1));
    set_fcast_sel_end(fcast->get_forecast_year(1));
    ui->spinBox_fcast_relf_beg->setValue(fcast->get_forecast_year(2));
    set_fcast_relf_begin(fcast->get_forecast_year(2));
    ui->spinBox_fcast_relf_end->setValue(fcast->get_forecast_year(3));
    set_fcast_relf_end(fcast->get_forecast_year(3));
    ui->spinBox_fcast_recr_beg->setValue(fcast->get_forecast_year(4));
    set_fcast_recr_begin(fcast->get_forecast_year(4));
    ui->spinBox_fcast_recr_end->setValue(fcast->get_forecast_year(5));
    set_fcast_recr_end(fcast->get_forecast_year(5));

    set_fcast_selex(fcast->getSelectivity());
    set_combo_box_cr_method(fcast->get_cr_method());//(ui->comboBox_control_rule, fcast->get_cr_method());
    ui->lineEdit_ctl_rule_const_F->setText(QString::number(fcast->get_cr_biomass_const_f()));
    ui->lineEdit_ctl_rule_no_F->setText(QString::number(fcast->get_cr_biomass_no_f()));
    ui->lineEdit_ctl_tgt_as_fraction->setText(QString::number(fcast->get_cr_buffer()));

    int numloops = fcast->get_num_forecast_loops();
    int firstloop = fcast->get_forecast_loop_first();
    ui->spinBox_num_forecast_loops->setValue(numloops);
    ui->spinBox_first_loop->setMaximum(numloops);
    ui->spinBox_first_loop->setValue(firstloop);
    ui->spinBox_fcast_recr_adjust->setValue(fcast->get_forecast_recr_adjust());
    ui->lineEdit_fcast_recr_adj_value->setText(QString::number(fcast->get_forecast_recr_adj_value()));
    ui->spinBox_fcast_loops_5->setValue(fcast->get_forecast_loop_ctl5());

    ui->spinBox_first_caps_yr->setValue(fcast->get_caps_alloc_st_year());
    ui->lineEdit_log_sd->setText(QString::number(fcast->get_log_catch_std_dev()));
    ui->groupBox_rebuilder->setChecked(fcast->get_do_rebuilder());
    ui->spinBox_rebuilder_ydecl->setValue(fcast->get_rebuilder_first_year());
    ui->spinBox_rebuilder_yinit->setValue(fcast->get_rebuilder_curr_year());

    ui->label_input_seas_relf->setVisible(false);
    set_combo_box_fleet_relF(fcast->get_fleet_rel_f());//(ui->comboBox_relF, );
//    change_bmark_relF(ui->comboBox_relF->currentIndex());

    set_combo_box_tuning_basis(fcast->get_catch_tuning_basis());//(ui->comboBox_tuning_basis, );
//    set_max_catch_fleet();
//    set_max_catch_area();

    seasFltRelF->setModel(fcast->getSeasFleetRelFTable());
    seasFltRelF->resizeColumnsToContents();

    maxCatchFleets->setModel(fcast->getMaxCatchFleetTable());
    maxCatchFleets->resizeColumnsToContents();

    maxCatchAreas->setModel(fcast->getMaxCatchAreaTable());
    maxCatchAreas->resizeColumnsToContents();

    set_allocation_groups();
    set_allocation_group_assign();
    allocGrpList->setModel(fcast->getAllocGrps());
    allocGrpList->setHeight(1);
    allocGrpList->resizeColumnsToContents();
    allocGrpFracts->setModel(fcast->getAllocFractModel());
    allocGrpFracts->setHeight(fcast->getAllocFractModel());
    allocGrpFracts->resizeColumnsToContents();
    change_use_alloc_groups (ui->groupBox_alloc_groups->isChecked());

    ui->spinBox_fixed_catch_obs->setValue(fcast->getFixedFcastCatchModel()->rowCount());
    inputFcastCatch->setModel(fcast->getFixedFcastCatchModel());
    inputFcastCatch->setHeight(fcast->getFixedFcastCatchModel());
    inputFcastCatch->resizeColumnsToContents();

//    ui->spinBox_fcast_levels->setValue(fcast->num_catch_levels());
    set_combo_box_catch_basis(fcast->get_input_catch_basis());
//    set_combo_box_fixed_catch(ui->comboBox_input_catch_basis->currentIndex());

    connectAll(fcast);
    ui->tabWidget->setCurrentIndex(0);
}

void forecast_widget::set_combo_box(QComboBox *cbox, int value)
{
    QString entry;
    QString val(QString::number(value));
    val.append(' ');
    int i;
    for (i = 0; i < cbox->count(); i++)
    {
        entry = cbox->itemText(i);
        if (entry.contains (val))
            break;
    }
    if (i == cbox->count())
        i = 0;
    cbox->setCurrentIndex(i);
}

void forecast_widget::set_combo_box_bmarks(int value)
{
    ui->comboBox_benchmarks->setCurrentIndex(value);
}

void forecast_widget::change_bmarks(int value)
{
    model_data->forecast->set_benchmarks(value);
}

void forecast_widget::set_combo_box_MSY(int value)
{
    ui->comboBox_MSY_options->setCurrentIndex(value-1);
}

void forecast_widget::change_MSY(int value)
{
    model_data->forecast->set_MSY(value+1);
}

void forecast_widget::change_spr_target()
{
    QString value(ui->lineEdit_SPR_target->text());
    double spr = checkdoublevalue(value);
    model_data->forecast->set_spr_target(spr);
    ui->lineEdit_SPR_target->setText(QString::number(spr));
}

void forecast_widget::change_biomass_target()
{
    QString value(ui->lineEdit_biomass_target->text());
    double bmt = checkdoublevalue(value);
    model_data->forecast->set_biomass_target(bmt);
    ui->lineEdit_biomass_target->setText(QString::number(bmt));
}

void forecast_widget::set_combo_box_bmark_relF(int value)
{
    ui->comboBox_bmark_relF_basis->setCurrentIndex(value);
}

void forecast_widget::change_bmark_relF (int value)
{
    model_data->forecast->set_benchmark_rel_f(value);
}

void forecast_widget::set_combo_box_fcast(int value)
{
    ui->comboBox_fcast_options->setCurrentIndex(value+1);
}

void forecast_widget::change_forecast(int fcast)
{
    int i_fcast = fcast - 1;
    bool showScalar = (i_fcast == 5);
    model_data->forecast->set_forecast(i_fcast);
    ui->label_F_scalar->setVisible(showScalar);
    ui->lineEdit_F_scalar->setVisible(showScalar);
}

void forecast_widget::change_F_mult()
{
    QString value(ui->lineEdit_F_scalar->text());
    double fsc = checkdoublevalue(value);
    model_data->forecast->set_f_mult(fsc);
    ui->lineEdit_F_scalar->setText(QString::number(fsc));
}

void forecast_widget::set_fcast_selex(int value)
{
    ui->comboBox_fcast_selex->setCurrentIndex(value);
}

void forecast_widget::change_fcast_selex(int value)
{
    model_data->forecast->setSelectivity(value);
}

void forecast_widget::set_combo_box_cr_method(int value)
{
    ui->comboBox_control_rule->setCurrentIndex(value);
}

void forecast_widget::change_cr_method(int value)
{
    model_data->forecast->set_cr_method(value);
}

void forecast_widget::set_cr_biomass_const_f()
{
    QString value(ui->lineEdit_ctl_rule_const_F->text());
    double val = checkdoublevalue(value);
    model_data->forecast->set_cr_biomass_const_f(val);
    ui->lineEdit_ctl_rule_const_F->setText(QString::number(val));
}
void forecast_widget::set_cr_biomass_no_f()
{
    QString value(ui->lineEdit_ctl_rule_no_F->text());
    double val = checkdoublevalue(value);
    model_data->forecast->set_cr_biomass_no_f(val);
    ui->lineEdit_ctl_rule_no_F->setText(QString::number(val));

}
void forecast_widget::set_cr_target()
{
    QString value(ui->lineEdit_ctl_tgt_as_fraction->text());
    double val = checkdoublevalue(value);
    model_data->forecast->set_cr_buffer(val);
    ui->lineEdit_ctl_tgt_as_fraction->setText(QString::number(val));

}

void forecast_widget::set_num_forecast_loops(int num)
{
    model_data->forecast->set_num_forecast_loops(num);
    ui->spinBox_first_loop->setMaximum(num);
}

void forecast_widget::change_first_forecast_loop(int num)
{
    model_data->forecast->set_forecast_loop_first(num);
}

void forecast_widget::set_log_catch_std_dev()
{
    QString value(ui->lineEdit_log_sd->text());
    double val = checkdoublevalue(value);
    model_data->forecast->set_log_catch_std_dev(val);
    ui->lineEdit_log_sd->setText(QString::number(val));

}

void forecast_widget::set_combo_box_fleet_relF(int value)
{
    if (value < 1) value = 1;
    if (value > 2) value = 2;
    ui->comboBox_fleet_relF->setCurrentIndex(value-1);
}

void forecast_widget::change_fleet_relF(int value)
{
    model_data->forecast->set_fleet_rel_f(value+1);
    if (value == 1)
    {
        seasFltRelF->setVisible(true);
        seasFltRelF->setHeight(model_data->get_num_seasons());
        ui->label_input_seas_relf->setVisible(true);
    }
    else
    {
        seasFltRelF->setVisible(false);
        seasFltRelF->setHeight(0);
        ui->label_input_seas_relf->setVisible(false);
    }
}

void forecast_widget::set_combo_box_tuning_basis(int value)
{
    int index = 0;
    switch (value)
    {
    case 2:
        index = 0;
        break;
    case 3:
        index = 1;
        break;
    case 5:
        index = 2;
        break;
    case 6:
        index = 3;
    }
    ui->comboBox_tuning_basis->setCurrentIndex(index);
}

void forecast_widget::change_tuning_basis(int value)
{
    int set = 2;
    switch (value)
    {
    case 0:
        set = 2;
        break;
    case 1:
        set = 3;
        break;
    case 2:
        set = 5;
        break;
    case 3:
        set = 6;
        break;
    }
    model_data->forecast->set_catch_tuning_basis(set);
}

void forecast_widget::set_allocation_groups()
{
    set_num_alloc_groups(model_data->forecast->get_num_alloc_groups());
}

void forecast_widget::set_num_alloc_groups(int num)
{
    if (num != ui->spinBox_num_alloc_groups->value())
    {
        ui->spinBox_num_alloc_groups->setValue(num);
        if (num > 0)
            ui->groupBox_alloc_groups->setChecked(true);
        else
            ui->groupBox_alloc_groups->setChecked(false);
    }
}

void forecast_widget::set_allocation_group_assign()
{
    int num = model_data->forecast->get_num_fleets();
    QString txt("");
    if (ui->spinBox_num_alloc_groups->value() > 0)
    {
        for (int i = 0; i < num; i++)
        {
            txt.append(QString("%1 ").arg(
                     QString::number(model_data->getFleet(i)->getAllocGroup())));//forecast->alloc_group(i))));
        }
    }
    alloc_group_assign_changed ();
//    ui->lineEdit_alloc_assignments->setText(txt);
}

void forecast_widget::alloc_group_assign_changed ()
{
/*    QString txt (ui->lineEdit_alloc_assignments->text());
    bool okay = true;
    int num = -1;
    QStringList ql(txt.split(' ', QString::SkipEmptyParts));

    if (ql.count() == 1 && ql.at(0).toInt() < 1)
    {
        ui->groupBox_alloc_groups->setChecked(false);
        model_data->forecast->set_num_alloc_groups(0);
        set_num_alloc_groups(0);
        allocGrpFracts->setHeight(0);
    }
    else if (ql.count() < model_data->num_fleets())
    {
        showInputMessage(QString("You must input a group for each fleet, total %1.").arg(model_data->num_fleets()));
    }
    else if (ql.count() > model_data->num_fleets())
    {
        showInputMessage(QString("Too many values. Max number is %1").arg(model_data->num_fleets()));
    }
    else
    {
        for (int i = 0; i < ql.count(); i++)
        {
            int val = ql.at(i).toInt(&okay);
            if (!okay)
            {
                ErrorFloatDialog efd (this, "Error in values", "Not all integer values", ql.count(), false);
                efd.fromText(txt);
                efd.exec();
                txt = efd.toText();
                okay = true;
                ui->lineEdit_alloc_assignments->setText(txt);
                alloc_group_assign_changed();
                return;
            }
            else
            {
                model_data->getFleet(i)->setAllocGroup(val);
                if (val > num)
                    num = val;
            }
        }
        set_num_alloc_groups(num);
        model_data->forecast->set_num_alloc_groups(num);
    }*/
    set_num_alloc_groups(model_data->forecast->getAllocFractModel()->columnCount() - 1);
    allocGrpList->setHeight(model_data->forecast->getAllocGrps());
    allocGrpList->resizeColumnsToContents();
    allocGrpFracts->setHeight(model_data->forecast->getAllocFractModel());
    allocGrpFracts->resizeColumnsToContents();
}

void forecast_widget::change_use_alloc_groups(bool flag)
{
    ui->label_alloc_assignment->setVisible(flag);
    allocGrpList->setVisible(flag);
    ui->label_alloc_fraction->setVisible(flag);
    allocGrpFracts->setVisible(flag);
}

void forecast_widget::set_allocation_group_fract()
{
/*    int num = model_data->forecast->num_alloc_groups();
    QString txt("");
    for (int i = 0; i < num; i++)
    {
        txt.append(QString("%1 ").arg(QString::number(model_data->forecast->alloc_fraction(0, i))));
    }
    ui->lineEdit_alloc_fractions->setText(txt);*/
}

void forecast_widget::alloc_group_fract_changed ()
{
/*    QString txt (ui->lineEdit_alloc_fractions->text());
    bool okay;
    QString err_title("Allocation Group Fractions Error"), err_msg;
    QStringList ql(txt.split(' ', QString::SkipEmptyParts));
    for (int i = 0; i < ql.count(); i++)
    {
        float val = ql.at(i).toFloat(&okay);
        if (!okay)
        {
            ErrorFloatDialog efd (this, "Error in values", "Not all float values", ql.count(), true);
            efd.setNumValues(ui->spinBox_num_alloc_groups->value());
            efd.fromText(txt);
            efd.setSumValue(1.0);
            efd.exec();
            txt = efd.toText();
            okay = true;
            ui->lineEdit_alloc_fractions->setText(txt);
            alloc_group_fract_changed();
            break;
        }
        else
        {
            model_data->forecast->set_alloc_fraction(0, i, val);
        }
    }*/
}

void forecast_widget::set_max_catch_fleet ()
{
    QString vals;
    for (int i = 0; i < model_data->get_num_fleets(); i++)
    {
        vals.append(QString(" %1").arg(QString::number(model_data->forecast->max_catch_fleet(i))));
    }
//    ui->lineEdit_max_catch_fleet->setText(vals);
}

void forecast_widget::change_max_catch_fleet ()
{
/*    QString txt (ui->lineEdit_max_catch_fleet->text());
    QStringList vals(txt.split(' ', QString::SkipEmptyParts));
    for (int i = 0; i < vals.count(), i < model_data->num_fleets(); i++)
    {
        int val = checkintvalue(vals.at(i));
        model_data->forecast->set_max_catch_fleet(i, val);
    }
    set_max_catch_fleet();*/
}

void forecast_widget::set_max_catch_area ()
{
    QString vals;
    for (int i = 0; i < model_data->get_num_areas(); i++)
    {
        vals.append(QString(" %1").arg(QString::number(model_data->forecast->max_catch_area(i))));
    }

//    ui->lineEdit_max_catch_area->setText(vals);
}

void forecast_widget::change_max_catch_area ()
{
/*    QString txt (ui->lineEdit_max_catch_area->text());
    QStringList vals(txt.split(' ', QString::SkipEmptyParts));
    for (int i = 0; i < vals.count(), i < model_data->num_areas(); i++)
    {
        int val = checkintvalue(vals.at(i));
        model_data->forecast->set_max_catch_area(i, val);
    }
    set_max_catch_area();*/
}

void forecast_widget::set_combo_box_catch_basis(int value)
{
    int index = 0;
    switch (value) {
    case -1:
        index = 0;
        break;
    case 2:
        index = 1;
        break;
    case 3:
        index = 2;
        break;
    case 99:
        index = 3;
        break;
    }
    ui->comboBox_input_catch_basis->setCurrentIndex(index);
}

void forecast_widget::change_catch_basis(int value)
{
    int set = -1;
    switch (value) {
    case 0:
        set = -1;
        break;
    case 1:
        set = 2;
        break;
    case 2:
        set = 3;
        break;
    case 3:
        set = 99;
        break;
    }
    model_data->forecast->set_input_catch_basis(set);
}

void forecast_widget::set_num_input_obs(int num)
{
    ui->spinBox_fixed_catch_obs->setValue(num);
}

void forecast_widget::change_num_input_obs(int num)
{
    model_data->forecast->setNumFixedFcastCatch(num);
    inputFcastCatch->setHeight(num);
}

void forecast_widget::change_fixed_catch(int value)
{
    model_data->forecast->set_combo_box_catch_input(value);
    if (value > 0)
    {
        model_data->forecast->getFixedFcastCatchModel()->setColumnCount(4);
        if (value == 3)
            model_data->forecast->getFixedFcastCatchModel()->setColumnHeader(3, QString("Hrate"));
        else
            model_data->forecast->getFixedFcastCatchModel()->setColumnHeader(3, QString("Catch"));
//        ui->label_fixed_catch_obs->setVisible(false);
//        ui->spinBox_fixed_catch_obs->setVisible(false);
//        inputFcastCatch->setVisible(false);
//        ui->label_input_fixed_catch->setVisible(false);
    }
    else
    {
        model_data->forecast->getFixedFcastCatchModel()->setColumnCount(5);
        model_data->forecast->getFixedFcastCatchModel()->setColumnHeader(3, QString("Catch"));
        model_data->forecast->getFixedFcastCatchModel()->setColumnHeader(4, QString("Basis"));
//        ui->label_fixed_catch_obs->setVisible(true);
//        ui->spinBox_fixed_catch_obs->setVisible(true);
//        inputFcastCatch->setVisible(true);
//        ui->label_input_fixed_catch->setVisible(true);
    }
}

void forecast_widget::set_bmark_bio_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
        set_bmark_bio_begin(year);
    else
    {
        QString yrStr(QString::number(year));
        model_data->forecast->set_benchmark_bio_beg(year);
        ui->spinBox_bmark_bio_beg->setValue(year);
        year = model_data->refyearvalue(year);
        yrStr =  QString::number(year);
        ui->label_bmark_bio_beg_yr_ref->setText (yrStr);
    }
}

void forecast_widget::set_bmark_bio_begin(QString yrStr)
{
    int yr = yrStr.toInt();
    set_bmark_bio_begin(yr);
}

void forecast_widget::set_bmark_bio_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
        set_bmark_bio_end(year);
//        ui->spinBox_bmark_bio_end->setValue(year);
    else
    {
        QString yrStr(QString::number(year));
        model_data->forecast->set_benchmark_bio_end(year);
        ui->spinBox_bmark_bio_end->setValue(year);
        year = model_data->refyearvalue(year);
        yrStr =  QString::number(year);
        ui->label_bmark_bio_end_yr_ref->setText (yrStr);
    }
}

void forecast_widget::set_bmark_sel_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_sel_begin(year);
//        ui->spinBox_bmark_sel_beg->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_sel_beg(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_sel_beg_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_bmark_sel_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_sel_end(year);
        //ui->spinBox_bmark_sel_end->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_sel_end(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_sel_end_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_bmark_relf_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_relf_begin(year);
        //ui->spinBox_bmark_relf_beg->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_relf_beg(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_relf_beg_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_bmark_relf_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_relf_end(year);
        //ui->spinBox_bmark_relf_end->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_relf_end(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_relf_end_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_bmark_recr_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_recr_begin(year);
//        ui->spinBox_bmark_recr_beg->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_rec_beg(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_recr_beg_yr_ref->setText(QString::number(year));
    }
}

void forecast_widget::set_bmark_recr_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_recr_end(year);
//        ui->spinBox_bmark_recr_end->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_rec_end(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_recr_end_yr_ref->setText(QString::number(year));
    }
}

void forecast_widget::set_bmark_srparm_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_srparm_begin(year);
//       ui->spinBox_bmark_srparm_beg->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_srpar_beg(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_srparm_beg_yr_ref->setText(QString::number(year));
    }
}

void forecast_widget::set_bmark_srparm_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_bmark_srparm_end(year);
//        ui->spinBox_bmark_srparm_end->setValue(year);
    }
    else
    {
        model_data->forecast->set_benchmark_srpar_end(year);
        year = model_data->refyearvalue(year);
        ui->label_bmark_srparm_end_yr_ref->setText(QString::number(year));
    }
}

void forecast_widget::set_fcast_sel_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_fcast_sel_begin(year);
//        ui->spinBox_fcast_sel_beg->setValue(year);
    }
    else
    {
        model_data->forecast->set_forecast_sel_beg(year);
        year = model_data->refyearvalue(year);
        ui->label_fcast_sel_beg_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_fcast_sel_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_fcast_sel_end(year);
//        ui->spinBox_fcast_sel_end->setValue(year);
    }
    else
    {
        model_data->forecast->set_forecast_sel_end(year);
        year = model_data->refyearvalue(year);
        ui->label_fcast_sel_end_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_fcast_relf_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_fcast_relf_end(year);
//        ui->spinBox_fcast_relf_beg->setValue(year);
    }
    else
    {
        model_data->forecast->set_forecast_relf_beg(year);
        year = model_data->refyearvalue(year);
        ui->label_fcast_relf_beg_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_fcast_relf_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_fcast_relf_end(year);
//        ui->spinBox_fcast_relf_end->setValue(year);
    }
    else
    {
        model_data->forecast->set_forecast_relf_end(yr);
        year = model_data->refyearvalue(year);
        ui->label_fcast_relf_end_yr_ref->setText (QString::number(year));
    }
}

void forecast_widget::set_fcast_recr_begin(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_fcast_recr_begin(year);
//        ui->spinBox_fcast_recr_beg->setValue(year);
    }
    else
    {
        model_data->forecast->set_forecast_rec_beg(year);
        year = model_data->refyearvalue(year);
        ui->label_fcast_recr_beg_yr_ref->setText(QString::number(year));
    }
}

void forecast_widget::set_fcast_recr_end(int yr)
{
    int year = model_data->checkyearvalue(yr);
    if (yr != year)
    {
        set_fcast_recr_end(year);
//        ui->spinBox_fcast_recr_end->setValue(year);
    }
    else
    {
        model_data->forecast->set_forecast_rec_end(year);
        year = model_data->refyearvalue(year);
        ui->label_fcast_recr_end_yr_ref->setText(QString::number(year));
    }
}

void forecast_widget::set_rebuilder(bool flag)
{
    model_data->forecast->set_do_rebuilder(flag);
}

void forecast_widget::set_rebuilder_first_year(int yr)
{
    int year = yr;
    if (yr == -1)
    {
        year = 1999;
    }
    else if (yr < model_data->get_start_year())
    {
        year = model_data->get_start_year();
//        yr = year;
//        model_data->forecast->set_rebuilder_first_year(year);
    }
    else if (yr > model_data->get_end_year())
    {
        year = model_data->get_end_year();
//        yr = year;
//        model_data->forecast->set_rebuilder_first_year(year);
    }
    ui->spinBox_rebuilder_ydecl->setValue(year);
    ui->label_rebuilder_yeardecl->setText(QString::number(yr));
}

void forecast_widget::change_rebuilder_first_yr(int yr)
{
    if (yr < model_data->get_start_year())
    {
        yr = model_data->get_start_year();
    }
    else if (yr > model_data->get_end_year())
    {
        yr = model_data->get_end_year();
    }
    model_data->forecast->set_rebuilder_first_year(yr);
    ui->label_rebuilder_yeardecl->setText(QString::number(yr));
}

void forecast_widget::set_rebuilder_curr_year(int yr)
{
    int year = yr;
    if (yr == -1)
    {
        year = model_data->get_end_year() + 1;
    }
    else if (yr < model_data->get_start_year())
    {
        year = model_data->get_start_year();
//        yr = year;
//        model_data->forecast->set_rebuilder_curr_year(year);
    }
    else if (yr > (model_data->get_end_year() + 1))
    {
        year = model_data->get_end_year() + 1;
//        yr = year;
//        model_data->forecast->set_rebuilder_curr_year(year);
    }
    ui->spinBox_rebuilder_yinit->setValue(year);
    ui->label_rebuilder_yearinit->setText(QString::number(yr));
}

void forecast_widget::change_rebuilder_curr_yr(int yr)
{
    if (yr < model_data->get_start_year())
    {
        yr = model_data->get_start_year() + 1;
    }
    else if (yr > model_data->get_end_year())
    {
        yr = model_data->get_end_year() + 1;
    }
    model_data->forecast->set_rebuilder_curr_year(yr);
    ui->label_rebuilder_yearinit->setText(QString::number(yr));
}
