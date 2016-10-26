#ifndef SS_FORECAST_H
#define SS_FORECAST_H

#include <QObject>
#include <QList>

#include "ss_observation.h"
#include "tablemodel.h"

class ss_forecast : public QObject
{
    Q_OBJECT
public:
    explicit ss_forecast(int fleets = 1, int seasons = 1, QObject *parent = 0);
    ~ss_forecast();

signals:
    void allocGrpsChanged();

public slots:
    void set_benchmarks (int bmark) {i_bmark = bmark;}
    void set_benchmarks (bool flag) {i_bmark = flag? 1: 0;}
    int get_benchmarks () {return i_bmark;}
    void set_MSY (int msy) {i_msy = msy;}
    void set_combo_box_MSY (int msy);
    int get_MSY () {return i_msy;}
    void set_spr_target (double spr) {f_spr_tgt = (float)spr;}
    float get_spr_target () {return f_spr_tgt;}
    void set_biomass_target (double bmss) {f_bmass_tgt = (float)bmss;}
    float get_biomass_target () {return f_bmass_tgt;}
    void set_benchmark_years (int i, int yr) {i_bmark_yrs[i] = yr;}
    void set_benchmark_bio_beg (int yr) {i_bmark_yrs[0] = yr;}
    void set_benchmark_bio_end (int yr) {i_bmark_yrs[1] = yr;}
    void set_benchmark_sel_beg (int yr) {i_bmark_yrs[2] = yr;}
    void set_benchmark_sel_end (int yr) {i_bmark_yrs[3] = yr;}
    void set_benchmark_relf_beg (int yr) {i_bmark_yrs[4] = yr;}
    void set_benchmark_relf_end (int yr) {i_bmark_yrs[5] = yr;}
    void set_benchmark_rec_beg (int yr) {i_bmark_yrs[6] = yr;}
    void set_benchmark_rec_end (int yr) {i_bmark_yrs[7] = yr;}
    int get_benchmark_year (int i) {return i_bmark_yrs[i];}
    void set_benchmark_rel_f (int relf) {i_bmark_rel_f = relf;}
    void set_combo_box_relf_basis (int relf);
    int get_benchmark_rel_f () {return i_bmark_rel_f;}
    void set_forecast (int fcast) {i_method = fcast;}
    void set_combo_box_forecast(int fcast);
    int get_forecast () {return i_method;}
    void set_num_forecast_years (int yrs);// {i_num_fcast_yrs = yrs;}
    int get_num_forecast_years () {return i_num_fcast_yrs;}
    void set_forecast_year (int i, int yr) {i_fcast_yrs[i] = yr;}
    void set_forecast_sel_beg (int yr) {i_fcast_yrs[0] = yr;}
    void set_forecast_sel_end (int yr) {i_fcast_yrs[1] = yr;}
    void set_forecast_relf_beg (int yr) {i_fcast_yrs[2] = yr;}
    void set_forecast_relf_end (int yr) {i_fcast_yrs[3] = yr;}
    void set_forecast_rec_beg (int yr) {i_fcast_yrs[4] = yr;}
    void set_forecast_rec_end (int yr) {i_fcast_yrs[5] = yr;}
    int get_forecast_year (int i) {return i_fcast_yrs[i];}
    void set_cr_method (int ctl) {i_ctl_rule_method = ctl;}
    void set_combo_box_cr_method (int ctl);
    int get_cr_method () {return i_ctl_rule_method;}
    void set_f_scalar (double f) {f_f_scalar = (float)f;}
    float get_f_scalar () {return f_f_scalar;}
    void set_cr_biomass_const_f (double cr) {f_ctl_rule_bmass_const_f = (float)cr;}
    float get_cr_biomass_const_f () {return f_ctl_rule_bmass_const_f;}
    void set_cr_biomass_no_f (double cr) {f_ctl_rule_bmass_no_f = (float)cr;}
    float get_cr_biomass_no_f () {return f_ctl_rule_bmass_no_f;}
    void set_cr_target (double targ) {f_ctl_rule_tgt = (float)targ;}
    float get_cr_target () {return f_ctl_rule_tgt;}
    void set_num_forecast_loops (int loops) {i_num_fcast_loops = loops;}
    int get_num_forecast_loops () {return i_num_fcast_loops;}
    void set_forecast_loop_recruitment (int loop) {i_fcast_loop_stch_recruit = loop;}
    int get_forecast_loop_recruitment () {return i_fcast_loop_stch_recruit;}
    void set_forecast_loop_ctl3 (int floop) {i_fcast_loop_3 = floop;}
    int get_forecast_loop_ctl3 () {return i_fcast_loop_3;}
    void set_forecast_loop_ctl4 (int floop) {i_fcast_loop_4 = floop;}
    int get_forecast_loop_ctl4 () {return i_fcast_loop_4;}
    void set_forecast_loop_ctl5 (int floop) {i_fcast_loop_5 = floop;}
    int get_forecast_loop_ctl5 () {return i_fcast_loop_5;}
    void set_caps_alloc_st_year (int yr) {i_caps_st_year = yr;}
    int get_caps_alloc_st_year () {return i_caps_st_year;}
    void set_log_catch_std_dev (double sd) {f_log_ctch_stdv = (float)sd;}
    float get_log_catch_std_dev () {return f_log_ctch_stdv;}
    void set_do_rebuilder (bool flag) {b_rebuilder = flag;}
    bool get_do_rebuilder () {return b_rebuilder;}
    void set_rebuilder_first_year (int yr) {i_rebuilder_st_yr = yr;}
    int get_rebuilder_first_year () {return i_rebuilder_st_yr;}
    void set_rebuilder_curr_year (int yr) {i_rebuilder_cur_yr = yr;}
    int get_rebuilder_curr_year () {return i_rebuilder_cur_yr;}
    void set_fleet_rel_f (int f) {i_fleet_rel_f = f;}
    void set_combo_box_fleet_relf (int relf);
    int get_fleet_rel_f () {return i_fleet_rel_f;}
    void add_seas_fleet_rel_f (int seas, int flt, float f);
    void set_seas_fleet_rel_f (int seas, int flt, float f);
//    float seas_fleet_rel_f (int seas, int flt) {return f_seas_fleet_rel_f_list.at(seas).at(flt);}
    QStringList getSeasFleetRelF (int seas) {return seasFleetRelF_table->getRowData(seas);}
    void setSeasFleetRelF (int seas, QStringList data) {seasFleetRelF_table->setRowData(seas, data);}
    float get_seas_fleet_rel_f (int seas, int flt) {return seasFleetRelF_table->getRowData(seas).at(flt).toFloat();}
    tablemodel *getSeasFleetRelFTable () {return seasFleetRelF_table;}
    void set_catch_tuning_basis (int basis); // {i_ctch_basis = basis;}
    void set_combo_box_catch_tuning (int basis);
    int get_catch_tuning_basis () {return i_ctch_basis;}
    void set_num_seasons (int seas);
    void set_num_fleets (int flt);
    void set_num_genders (int gen) {i_num_genders = gen;}
    int get_num_seasons () {return i_num_seasons;}

    QStringList getMaxCatchFleets () {return maxCatchFleet->getRowData(0);}
    void setMaxCatchFleets (QStringList data) {maxCatchFleet->setRowData(0, data);}
    float getMaxCatchFleet (int flt) {return maxCatchFleet->getRowData(0).at(flt).toFloat();}
    tablemodel *getMaxCatchFleetTable () {return maxCatchFleet;}
    void set_max_catch_fleet (int flt, float ctch);
    int get_num_fleets () {return maxCatchFleet->columnCount();}
    float max_catch_fleet (int flt) {return maxCatchFleet->getRowData(0).at(flt).toFloat();}

    QStringList getMaxCatchAreas () {return maxCatchArea->getRowData(0);}
    void setMaxCatchAreas (QStringList data) {maxCatchArea->setRowData(0, data);}
    float getMaxCatchArea (int flt) {return maxCatchArea->getRowData(0).at(flt).toFloat();}
    tablemodel *getMaxCatchAreaTable () {return maxCatchArea;}
    void set_max_catch_area (int ar, float ctch);

    void set_num_areas (int ars) ;
    int get_num_areas () {return maxCatchArea->columnCount();}
    float max_catch_area (int ar) {return maxCatchArea->getRowData(0).at(ar).toFloat();}

    QStringList getAllocGrpList () {return allocGrpList->getRowData(0);}
    void setAllocGrpList (QStringList data) {allocGrpList->setRowData(0, data);}
    int getAllocGrp (int flt) {return allocGrpList->getRowData(0).at(flt).toInt();}
    tablemodel *getAllocGrps () {return allocGrpList;}
    void setAllocGrp (int flt, int grp);
    void setAllocGrps ();
    void changeAllocGrps (QModelIndex tl, QModelIndex br);
    void set_alloc_group (int flt, int grp);
    void set_num_alloc_groups (int num);
    int get_num_alloc_groups () {return i_num_alloc_groups;}
    void set_alloc_fractions (int yr, QStringList fractions) {allocGroupFrac->setRowData(yr, fractions);}
    QStringList get_alloc_fractions (int yr) {return allocGroupFrac->getRowData(yr);}
    void setAllocFractions (int index, QStringList data) {allocGroupFrac->setRowData(index, data);}
    QStringList getAllocFractions (int index) {return allocGroupFrac->getRowData(index);}
    tablemodel *getAllocFractModel () {return allocGroupFrac;}

    void set_num_catch_levels (int lvls) {i_num_fcast_ctch_levels = lvls;}
    int num_catch_levels () {return i_num_fcast_ctch_levels;}
    void set_input_catch_basis (int &basis); // {i_input_fcast_ctch_basis = basis;}
    void set_combo_box_catch_input (int basis);
    int get_input_catch_basis () {return i_input_fcast_ctch_basis;}

    void add_fixed_catch_value (QStringList txtlst);

    void setNumFixedFcastCatch (int rows) {fixedFcastCatch->setRowCount(rows);}
    int getNumFixedFcastCatch () {return fixedFcastCatch->rowCount();}
    void addFixedFcastCatch (QStringList data) {setFixedFcastCatch (getNumFixedFcastCatch(), data);}
    void setFixedFcastCatch (int row, QStringList data) {fixedFcastCatch->setRowData(row, data);}
    QStringList getFixedFcastCatch (int row) {return fixedFcastCatch->getRowData(row);}
    tablemodel *getFixedFcastCatchModel () {return fixedFcastCatch;}

    void clear();
    void reset();
    void reset_seas_fleet_relf();

 private:
    int i_num_seasons;
    int i_num_fleets;
    int i_num_genders;
    // Benchmarks: 0=skip; 1=calc F_spr,F_btgt,F_msy
    int   i_bmark;
    // MSY: 1= set to F(SPR); 2=calc F(MSY); 3=set to F(Btgt); 4=set to F(endyr)
    int   i_msy;
    // SPR target (e.g. 0.40)
    float f_spr_tgt;
    // Biomass target (e.g. 0.40)
    float f_bmass_tgt;
    // Bmark_years: beg_bio, end_bio, beg_selex, end_selex, beg_relF, end_relF (enter actual year, or values of 0 or -integer to be rel. endyr)
    int   i_bmark_yrs[10];
    // Bmark_relF_Basis: 1 = use year range; 2 = set relF same as forecast below
    int   i_bmark_rel_f;
    // Forecast: 0=none; 1=F(SPR); 2=F(MSY) 3=F(Btgt); 4=Ave F (uses first-last relF yrs); 5=input annual F scalar
    int   i_method;
    // N forecast years
    int   i_num_fcast_yrs;
    // F scalar (only used for Do_Forecast==5)
    float f_f_scalar;
    // Fcast_years:  beg_selex, end_selex, beg_relF, end_relF  (enter actual year, or values of 0 or -integer to be rel. endyr)
    int   i_fcast_yrs[6];
    // Control rule method (1=catch=f(SSB) west coast; 2=F=f(SSB) )
    int   i_ctl_rule_method;
    // Control rule Biomass level for constant F (as frac of Bzero, e.g. 0.40); (Must be > the no F level below)
    float f_ctl_rule_bmass_const_f;
    // Control rule Biomass level for no F (as frac of Bzero, e.g. 0.10)
    float f_ctl_rule_bmass_no_f;
    // Control rule target as fraction of Flimit (e.g. 0.75)
    float f_ctl_rule_tgt;
    // N forecast loops (1=OFL only; 2=ABC; 3=get F from forecast ABC catch with allocations applied)
    int   i_num_fcast_loops;
    // First forecast loop with stochastic recruitment
    int   i_fcast_loop_stch_recruit;
    // Forecast loop control #3 (reserved for future bells&whistles)
    int   i_fcast_loop_3;
    // Forecast loop control #4 (reserved for future bells&whistles)
    int   i_fcast_loop_4;
    // Forecast loop control #5 (reserved for future bells&whistles)
    int   i_fcast_loop_5;
    // FirstYear for caps and allocations (should be after years with fixed inputs)
    int   i_caps_st_year;
    // stddev of log(realized catch/target catch) in forecast (set value>0.0 to cause active impl_error)
    float f_log_ctch_stdv;
    // Do West Coast gfish rebuilder output (0/1)
    bool  b_rebuilder;
    // Rebuilder:  first year catch could have been set to zero (Ydecl)(-1 to set to 1999)
    int   i_rebuilder_st_yr;
    // Rebuilder:  year for current age structure (Yinit) (-1 to set to endyear+1)
    int   i_rebuilder_cur_yr;
    // fleet relative F:  1=use first-last alloc year; 2=read seas(row) x fleet(col) below
    int   i_fleet_rel_f;
    // Note that fleet allocation is used directly as average F if Do_Forecast=4
    // basis for fcast catch tuning and for fcast catch caps and allocation  (2=deadbio; 3=retainbio; 5=deadnum; 6=retainnum)
    int   i_ctch_basis;
    // Conditional input if relative F choice = 2
    // Fleet relative F:  rows are seasons, columns are fleets
    tablemodel *seasFleetRelF_table;
    // max totalcatch by fleet (-1 to have no max) must enter value for each fleet
    tablemodel *maxCatchFleet;
    // max totalcatch by area (-1 to have no max); must enter value for each fleet
    tablemodel *maxCatchArea;
    // fleet assignment to allocation group (enter group ID# for each fleet, 0 for not included in an alloc group)
    int   i_num_alloc_groups;
    // Conditional on >1 allocation group
    tablemodel *allocGrpList;
    // allocation fraction for each of: 0 allocation groups
    tablemodel *allocGroupFrac;
    // Number of forecast catch levels to input (else calc catch from forecast F)
    int   i_num_fcast_ctch_levels;
    // basis for input Fcast catch: -1=read basis; 2=dead catch; 3=retained catch; 99=input Hrate(F) (units are from fleetunits; note new codes in SSV3.20)
    int   i_input_fcast_ctch_basis;
    // Fixed catch values (Year Seas Fleet Catch(or_F) [Basis])
    tablemodel *fixedFcastCatch;

};

#endif // SS_FORECAST_H
