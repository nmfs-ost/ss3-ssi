/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef FORECAST_WIDGET_H
#define FORECAST_WIDGET_H

#include <QWidget>

#include"dialog_fileview.h"
#include "input_file.h"
#include "model.h"

#define REBUILDER_FILE "rebuild.dat"

namespace Ui {
class forecast_widget;
}

class forecast_widget : public QWidget
{
    Q_OBJECT

public:
    explicit forecast_widget(ss_model *m_data, QWidget *parent = nullptr);
    ~forecast_widget();

    void disconnectAll(ss_forecast *fcast);
    void connectAll(ss_forecast *fcast);

public slots:
    void set_model (ss_model * m_data);

//    void set_fixed_catch_text(ssObservation *obs, QString txt);
//    void set_fixed_catch_text (int i, QString txt);
//    QString fixed_catch_text (int i);

    void reset();
    void refresh();

    void set_combo_box_bmarks(int value);
    void change_bmarks(int value);
    void bmarks_changed();
    void set_combo_box_MSY(int value);
    void change_MSY(int value);
    void change_spr_target();
    void change_biomass_target();
    void set_blimit(double value);
    void change_blimit();

    void set_bmark_bio_begin(int yr);
    void set_bmark_bio_begin(QString yrStr);
    void set_bmark_bio_end(int yr);
    void set_bmark_sel_begin(int yr);
    void set_bmark_sel_end (int yr);
    void set_bmark_relf_begin(int yr);
    void set_bmark_relf_end(int yr);
    void set_bmark_recr_begin(int yr);
    void set_bmark_recr_end(int yr);
    void set_bmark_srparm_begin(int yr);
    void set_bmark_srparm_end(int yr);
    void set_combo_box_bmark_relF(int value);
    void change_bmark_relF (int value);

    void set_combo_box_fcast(int value);
    void change_forecast(int fcast);
//    void set_num_fcast_years(int value);
//    void change_num_fcast_years(int value);
//    void set_F_mult();
    void change_F_mult();
    void set_fcast_sel_begin(int yr);
    void set_fcast_sel_end (int yr);
    void set_fcast_relf_begin(int yr);
    void set_fcast_relf_end (int yr);
    void set_fcast_recr_begin(int yr);
    void set_fcast_recr_end(int yr);
    void change_fixed_catch (int);

    void set_fcast_selex(int value);
    void change_fcast_selex(int value);
    void set_combo_box_cr_method(int value);
    void change_cr_method(int value);
    void set_cr_biomass_const_f();
    void set_cr_biomass_no_f();
    void set_cr_target();
    void set_num_forecast_loops (int num);
    void change_first_forecast_loop (int num);
    void set_log_catch_std_dev();

    void change_use_alloc_groups (bool flag);
    void set_num_alloc_groups (int num);
    void set_allocation_groups();
    void set_allocation_group_assign();
    void alloc_group_assign_changed ();
    void set_allocation_group_fract();
    void alloc_group_fract_changed ();
    void set_rebuilder (int option);
    void change_rebuilder (bool option);
    void set_rebuilder_first_year(int yr);
    void change_rebuilder_first_yr(int yr);
    void set_rebuilder_curr_year(int yr);
    void change_rebuilder_curr_yr(int yr);
    void view_rebuilder_file();

    void set_combo_box_fleet_relF(int value);
    void change_fleet_relF(int value);
    void set_combo_box_tuning_basis(int value);
    void change_tuning_basis(int value);
    void set_max_catch_fleet ();
    void change_max_catch_fleet ();
    void set_max_catch_area ();
    void change_max_catch_area ();

    void set_combo_box_catch_basis(int value);
    void change_catch_basis(int value);
    void set_num_input_obs (int num);
    void change_num_input_obs (int num);

signals:

private:
    Ui::forecast_widget *ui;

    ss_model *model_data;

    void set_combo_box (QComboBox *cbox, int value);
    QList <QLineEdit *> edit_fixed_catch_list;

    tableview *msyCostsView;
    tableview *seasFltRelF;
    tableview *maxCatchFleets;
    tableview *maxCatchAreas;
    tableview *allocGrpList;
    tableview *allocGrpFracts;
    tableview *inputFcastCatch;

    Dialog_fileView *fileViewer;
};

#endif // FORECAST_WIDGET_H
