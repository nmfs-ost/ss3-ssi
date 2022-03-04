#include "ss_forecast.h"

ss_forecast::ss_forecast(int fleets, int seasons, QObject *parent) :
    QObject (parent)
{
    QStringList header;

    msyCosts = new tablemodel();
    msyCosts->setColumnCount(4);
    msyCosts->setParent(this);
    header << "Fleet" << "Cost/F" << "Price/F" << "Include in search";
    msyCosts->setHeader(header);

    seasFleetRelF_table = new tablemodel(this);
    seasFleetRelF_table->setColumnCount(0);
    seasFleetRelF_table->setRowCount(1);

    maxCatchFleet = new tablemodel(this);
    maxCatchFleet->setRowCount(1);
    maxCatchFleet->setColumnCount(0);
    maxCatchFleet->setRowHeader(0, QString("Catch"));

    maxCatchArea = new tablemodel(this);
    maxCatchArea->setRowCount(1);
    maxCatchArea->setColumnCount(0);
    maxCatchArea->setRowHeader(0, QString("Catch"));

    allocGrpList = new tablemodel(this);
    allocGrpList->setRowCount(1);
    allocGrpList->setColumnCount(0);
    allocGrpList->setRowHeader(0, QString("Group"));
    connect (allocGrpList, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(changeAllocGrps(QModelIndex, QModelIndex)));\
    connect (allocGrpList, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SIGNAL(allocGrpsChanged()));

    allocGroupFrac = new tablemodel(this);
    allocGroupFrac->setRowCount(1);
    allocGroupFrac->setColumnCount(1);

    fixedFcastCatch = new tablemodel(this);
    fixedFcastCatch->setRowCount(0);
    fixedFcastCatch->setColumnCount(5);
    header.clear();
    header << "Year" << "Season" << "Fleet" << "Catch" << "Basis";
    fixedFcastCatch->setHeader(header);

    ctl_rule_buffer_table = new tablemodel(this);
    ctl_rule_buffer_table->setRowCount(0);
    ctl_rule_buffer_table->setColumnCount(2);
    header.clear();
    header << "Year" << "Value";
    ctl_rule_buffer_table->setHeader(header);

    // set defaults
    i_num_fleets = 0;
    i_num_seasons = 0;
    reset ();
    set_num_seasons(seasons);
    set_num_fleets(fleets);
    set_num_areas(1);
    set_num_forecast_loops(3);
    set_num_forecast_years(10);
    add_seas_fleet_rel_f(seasons, fleets, 0.0);
}

ss_forecast::~ss_forecast()
{
    clear();
    delete allocGroupFrac;
    delete maxCatchFleet;
    delete maxCatchArea;
    delete seasFleetRelF_table;
    delete fixedFcastCatch;
}

void ss_forecast::appendMsyCosts(QStringList data)
{
    int row = msyCosts->rowCount();
    msyCosts->setRowData(row, data);
}

void ss_forecast::set_spr_target(double spr)
{
    float SPR = static_cast<float>(spr);
    if (f_spr_tgt < SPR || f_spr_tgt > SPR)
        f_spr_tgt = SPR;
    emit SPRchanged(SPR);
}

void ss_forecast::set_combo_box_MSY(int msy)
{
    switch (msy)
    {
    case 0:
        set_MSY(1);
        break;
    case 1:
        set_MSY(2);
        break;
    case 2:
        set_MSY(3);
        break;
    case 3:
        set_MSY(4);
        break;
    default:
        set_MSY(1);
    }
}

//void ss_forecast::set_combo_box_relf_basis(int relf)
//{
//    set_benchmark_rel_f(relf);
/*    switch (relf)
    {
    case 0:
        set_benchmark_rel_f(1);
        break;
    case 1:
        set_benchmark_rel_f(2);
        break;
    default:
        set_benchmark_rel_f(1);
    }*/
//}

void ss_forecast::set_combo_box_forecast(int fcast)
{
    set_forecast(fcast-1);
    switch (fcast)
    {
    case 0:
        set_forecast(0);
        break;
    default:
        set_forecast(0);
    }
}

void ss_forecast::set_num_forecast_years (int yrs)
{
    if (yrs < 1)
        yrs = 1;
    i_num_fcast_yrs = yrs;
    allocGroupFrac->setRowCount(yrs);
    for (int i = 0; i < yrs; i++)
        allocGroupFrac->setRowHeader(i, QString("Year_%1").arg(QString::number(i+1)));
}

void ss_forecast::append_cr_buffer_list (QStringList rowdata)
{
    int num = ctl_rule_buffer_table->rowCount();
    ctl_rule_buffer_table->setRowData(num, rowdata);
}
int ss_forecast::get_num_cr_buffer_rows ()
{
    return ctl_rule_buffer_table->rowCount();
}
QStringList ss_forecast::get_cr_buffer_row (int row)
{
    return ctl_rule_buffer_table->getRowData(row);
}


void ss_forecast::set_combo_box_cr_method(int ctl)
{
    set_cr_method(ctl);
/*    switch (ctl)
    {
    case 0:
        set_cr_method(1);
        break;
    case 1:
        set_cr_method(2);
        break;
    default:
        set_cr_method(1);
    }*/
}

void ss_forecast::set_combo_box_fleet_relf(int relf)
{
    switch (relf)
    {
    case 0:
        set_fleet_rel_f(1);
        break;
    case 1:
        set_fleet_rel_f(2);
        break;
    default:
        set_fleet_rel_f(1);
    }
}

void ss_forecast::set_combo_box_catch_tuning(int basis)
{
    switch (basis)
    {
    case 0:
        set_catch_tuning_basis(2);
        break;
    case 1:
        set_catch_tuning_basis(3);
        break;
    case 2:
        set_catch_tuning_basis(5);
        break;
    case 3:
        set_catch_tuning_basis(6);
        break;
    default:
        set_catch_tuning_basis(2);
    }
}

void ss_forecast::set_num_fleets(int flt)
{
    QStringList header;
    seasFleetRelF_table->setColumnCount(flt);
    seasFleetRelF_table->setRowHeader(0, QString("Season_1"));
    maxCatchFleet->setColumnCount(flt);
    maxCatchFleet->setRowHeader(0, QString("Catch"));
    allocGrpList->setColumnCount(flt);
    allocGrpList->setRowHeader(0, QString("Group"));
    if (i_num_fleets < flt)
    {
        for (int j = 0; j < flt; j++)
        {
           header.append(QString("Fleet_%1").arg(j+1));
        }
        seasFleetRelF_table->setHeader(header);
        maxCatchFleet->setHeader(header);
        allocGrpList->setHeader(header);
        for (int j = i_num_fleets; j < flt; j++)
        {
            for (int i = 0; i < i_num_seasons; i++)
            {
                    set_seas_fleet_rel_f(i, j, 0.0);
            }
            set_max_catch_fleet(j, -1);
            setAllocGrp(j, 0);
        }
    }

    i_num_fleets = flt;
}

void ss_forecast::set_num_seasons(int seas)
{
    seasFleetRelF_table->setRowCount(seas);
    if (i_num_seasons < seas)
    {
        for (int i = i_num_seasons; i < seas; i++)
        {
            seasFleetRelF_table->setRowHeader(i, QString("Season_%1").arg(QString::number(i+1)));
            for (int j = 0; j < i_num_fleets; j++)
                set_seas_fleet_rel_f(i, j, 0.0);
        }
    }
    i_num_seasons = seas;
}

void ss_forecast::set_max_catch_fleet(int flt, float ctch)
{
    QStringList data;
    if (maxCatchFleet->columnCount() < flt+1)
        set_num_fleets(flt+1);
    data = maxCatchFleet->getRowData(0);
    data[flt] = QString::number(ctch);
    maxCatchFleet->setRowData(0, data);
}

void ss_forecast::set_num_areas(int ars)
{
    QStringList header;
    int num_areas = maxCatchArea->columnCount();
    int i;
    maxCatchArea->setColumnCount(ars);
    maxCatchArea->setRowHeader(0, QString("Catch"));
    if (num_areas < ars)
    {
        for (i = 0; i < num_areas; i++)
        {
            header.append(QString("Area_%1").arg(QString::number(i+1)));
        }
        for (; i < ars; i++)
        {
            header.append(QString("Area_%1").arg(QString::number(i+1)));
            set_max_catch_area(i, -1);
        }
        maxCatchArea->setHeader(header);
    }
}

void ss_forecast::set_max_catch_area(int ar, float ctch)
{
    QStringList data;
    if (maxCatchArea->columnCount() < ar+1)
        set_num_areas(ar+1);
    data = maxCatchArea->getRowData(0);
    data[ar] = QString::number(ctch);
//    data.takeAt(ar);
//    data.insert(ar, QString::number(ctch));
    maxCatchArea->setRowData(0, data);
}

void ss_forecast::set_num_alloc_groups(int num)
{
    QStringList header;
    header << "Year";
    if (num != i_num_alloc_groups)
    {
        i_num_alloc_groups = num;
        allocGroupFrac->setColumnCount(num+1);

        for (int i = 0; i < num; i++)
            header << QString("Group_%1").arg(i+1);
        allocGroupFrac->setHeader(header);
    }
}

void ss_forecast::setAllocGrp (int flt, int grp)
{
    if (grp > i_num_alloc_groups)
        set_num_alloc_groups(grp);
    QStringList data = getAllocGrpList();
    data[flt] = QString::number(grp);
//    data.takeAt(flt);
//    data.insert(flt, QString::number(grp));
    setAllocGrpList(data);
}

void ss_forecast::setAllocGrps()
{
    QStringList data = getAllocGrpList();

    for (int i = 0; i < i_num_fleets; i++)
    {
        setAllocGrp(i, 0);
    }
}

void ss_forecast::changeAllocGrps (QModelIndex tl, QModelIndex br)
{
    QStringList data = getAllocGrpList();
    QString group;
    Q_UNUSED(tl); Q_UNUSED(br);
    int col, cols = allocGrpList->columnCount();

    int grp = 0, grps = 0;
    for (col = 0; col < cols; col++)
    {
        group = data.at(col);
        if (group.isEmpty())
        {
            data[col] = QString("0");
//            data.insert(col, QString("0"));
            grp = 0;
        }
        else
        {
            grp = group.toInt();
        }
        if (grp > grps)
            grps = grp;
    }
    set_num_alloc_groups(grps);
}

void ss_forecast::set_alloc_group(int flt, int grp)
{
    if (i_num_fleets < flt+1)
        set_num_fleets(flt+1);
    else
    {
        setAllocGrp(flt, grp);
    }
}

void ss_forecast::add_seas_fleet_rel_f (int seas, int flt, float f)
{
    QStringList header;
    QStringList data;

    int old_seas = seasFleetRelF_table->rowCount();
    int old_flt = seasFleetRelF_table->columnCount();
    int row, col;

    set_num_seasons(seas);
    set_num_fleets(flt);
    for (row = 0; row < old_seas; row++)
    {
        for (col = old_flt; col < flt; col++)
        {
            set_seas_fleet_rel_f(row, col, f);
        }
    }
    for (; row < seas; row++)
    {
        for (col = 0; col < flt; col++)
            set_seas_fleet_rel_f(row, col, f);
    }
}

void ss_forecast::set_seas_fleet_rel_f(int seas, int flt, float f)
{
    int old_seas = seasFleetRelF_table->rowCount();
    int old_flt = seasFleetRelF_table->columnCount();
    QStringList data;
    if (seas >= old_seas || flt >= old_flt)
        add_seas_fleet_rel_f(seas, flt, 0.0);

    data = seasFleetRelF_table->getRowData(seas);
    data[flt] = QString::number(f);
    seasFleetRelF_table->setRowData(seas, data);
/*    {
        f_seas_fleet_rel_f_list[seas][flt] = f;
    }*/
}

float ss_forecast::getSeasFleetRelF(int seas, int fleet)
{
    QStringList relflist (getSeasFleetRelF(seas));
    float value = QString(relflist.at(fleet-1)).toFloat();
    return value;
}

void ss_forecast::setSeasFleetRelF(int seas, int fleet, float relf)
{
    QStringList relflist (getSeasFleetRelF(seas));
    relflist[fleet - 1] = relf;
    setSeasFleetRelF(seas, relflist);
}

void ss_forecast::set_catch_tuning_basis(int basis)
{
    if (basis < 2)
        basis = 2;
    if (basis > 6)
        basis = 6;
    if (basis == 4)
        basis = 3;
    i_ctch_basis = basis;
}

void ss_forecast::set_input_catch_basis(int &basis)
{
    i_input_fcast_ctch_basis = basis;
    switch (basis)
    {
    case -1:
        fixedFcastCatch->setColumnCount(5);
        fixedFcastCatch->setColumnHeader(3, "Catch or F");
        fixedFcastCatch->setColumnHeader(4, "Basis");
        break;
    default:
        i_input_fcast_ctch_basis = basis = 2;
        [[clang::fallthrough]];
    case 2:
        fixedFcastCatch->setColumnCount(4);
        fixedFcastCatch->setColumnHeader(3, QString("Dead Catch"));
        break;
    case 3:
        fixedFcastCatch->setColumnCount(4);
        fixedFcastCatch->setColumnHeader(3, QString("Ret. Catch"));
        break;
    case 99:
        fixedFcastCatch->setColumnCount(4);
        fixedFcastCatch->setColumnHeader(3, QString("Hrate"));
        break;
    }
}

void ss_forecast::set_combo_box_catch_input(int basis)
{
    int val = 2;
    switch (basis)
    {
    case 0:
        val = -1;
//        set_input_catch_basis(-1);
        break;
    case 1:
        val = 2;
//        set_input_catch_basis(2);
        break;
    case 2:
        val = 3;
//        set_input_catch_basis(3);
        break;
    case 3:
        val = 99;
//        set_input_catch_basis(99);
        break;
    }
    set_input_catch_basis(val);
}

void ss_forecast::add_fixed_catch_value(QStringList txtlst)
{
    fixedFcastCatch->setRowData(fixedFcastCatch->rowCount(), txtlst);
}

void ss_forecast::reset()
{
    i_bmark = 1; // Benchmarks: 0=skip; 1=calc F_spr,F_btgt,F_msy
    i_msy = 2;   // MSY: 1= set to F(SPR); 2=calc F(MSY); 3=set to F(Btgt); 4=set to F(endyr)
    i_msyUnits = 1; //
    msyCosts->clear();
    f_spr_tgt = 0.45;  // SPR target (e.g. 0.40)
    f_bmass_tgt = 0.40;// Biomass target (e.g. 0.40)
    for (int i = 0; i < 6; i++)
        i_bmark_yrs[i] = 0;// Bmark_years: beg_bio, end_bio, beg_selex, end_selex, beg_relF, end_relF (enter actual year, or values of 0 or -integer to be rel. endyr)
    i_bmark_rel_f = 1; // Bmark_relF_Basis: 1 = use year range; 2 = set relF same as forecast below
    i_method = 1;      // Forecast: 0=none; 1=F(SPR); 2=F(MSY) 3=F(Btgt); 4=Ave F (uses first-last relF yrs); 5=input annual F scalar
    i_num_fcast_yrs = 1;// N forecast years
    f_f_scalar = 1.0;  // F scalar (only used for Do_Forecast==5)
    for (int i = 0; i < 4; i++)
        i_fcast_yrs[i] = 0;// Fcast_years:  beg_selex, end_selex, beg_relF, end_relF  (enter actual year, or values of 0 or -integer to be rel. endyr)
    i_ctl_rule_method = 1;// Control rule method (1=catch=f(SSB) west coast; 2=F=f(SSB) )
    f_ctl_rule_bmass_const_f = 0.40;// Control rule Biomass level for constant F (as frac of Bzero, e.g. 0.40); (Must be > the no F level below)
    f_ctl_rule_bmass_no_f = 0.10;// Control rule Biomass level for no F (as frac of Bzero, e.g. 0.10)
    f_ctl_rule_buff = 0.75;// Control rule target as fraction of Flimit (e.g. 0.75)
    ctl_rule_buffer_table->clear();
    i_num_fcast_loops = 3;// N forecast loops (1=OFL only; 2=ABC; 3=get F from forecast ABC catch with allocations applied)
    i_fcast_loop_first = 3;// First forecast loop with stochastic recruitment
    i_fcast_recr_adj = 0;// Forecast loop control #3 (reserved for future bells&whistles)
    f_fcast_recr_adj_val = 0;// Forecast loop control #4 (reserved for future bells&whistles)
    i_fcast_loop_5 = 0;// Forecast loop control #5 (reserved for future bells&whistles)
    i_caps_st_year = 2015;// FirstYear for caps and allocations (should be after years with fixed inputs)
    f_log_ctch_stdv = 0.0;// stddev of log(realized catch/target catch) in forecast (set value>0.0 to cause active impl_error)
    b_rebuilder = false;// Do West Coast gfish rebuilder output (0/1)
    i_rebuilder_st_yr = 2004;// Rebuilder:  first year catch could have been set to zero (Ydecl)(-1 to set to 1999)
    i_rebuilder_cur_yr = -1;// Rebuilder:  year for current age structure (Yinit) (-1 to set to endyear+1)
    i_fleet_rel_f = 1;// fleet relative F:  1=use first-last alloc year; 2=read seas(row) x fleet(col) below
    //# Note that fleet allocation is used directly as average F if Do_Forecast=4
    i_ctch_basis = 2;// basis for fcast catch tuning and for fcast catch caps and allocation  (2=deadbio; 3=retainbio; 5=deadnum; 6=retainnum)
    //# Fleet relative F:  rows are seasons, columns are fleets
    reset_seas_fleet_relf();
    //# max totalcatch by fleet (-1 to have no max) must enter value for each fleet
    maxCatchFleet->setRowCount(0);
    //# max totalcatch by area (-1 to have no max); must enter value for each fleet
    maxCatchArea->setRowCount(0);
    //# fleet assignment to allocation group (enter group ID# for each fleet, 0 for not included in an alloc group)
    set_num_alloc_groups(0);//
    //# Input fixed catch values
    set_input_catch_basis(i_ctch_basis);
    setNumFixedFcastCatch(0);
}

void ss_forecast::clear()
{
    i_bmark = 0;
    i_msy = 0;
    i_msyUnits = 0;
    msyCosts->clear();
    f_spr_tgt = 0.0;
    f_bmass_tgt = 0.0;
    for (int i = 0; i < 6; i++)
        i_bmark_yrs[i] = 0;
    i_bmark_rel_f = 0;
    i_method = 0;
    i_num_fcast_yrs = 0;
    f_f_scalar = 0.0;
    for (int i = 0; i < 4; i++)
        i_fcast_yrs[i] = 0;
    i_ctl_rule_method = 1;
    f_ctl_rule_bmass_const_f = 0.0;
    f_ctl_rule_bmass_no_f = 0.0;
    f_ctl_rule_buff = 0.0;
    ctl_rule_buffer_table->clear();
    i_num_fcast_loops = 1;
    i_fcast_loop_first = 1;
    i_fcast_recr_adj = 0;
    f_fcast_recr_adj_val = 0;
    i_fcast_loop_5 = 0;
    i_caps_st_year = 0;
    f_log_ctch_stdv = 0.0;
    b_rebuilder = 0;
    i_rebuilder_st_yr = 0;
    i_rebuilder_cur_yr = 0;
    i_fleet_rel_f = 0;
    i_ctch_basis = 0;
    i_num_alloc_groups = 0;
    i_num_fcast_ctch_levels = 0;
    i_input_fcast_ctch_basis = 0;
    allocGroupFrac->setRowCount(1);
    set_num_alloc_groups(0);
}

void ss_forecast::reset_seas_fleet_relf()
{
    QStringList data;
    set_num_seasons(i_num_seasons);

    for (int i = 0; i < i_num_seasons; i++)
    {
        for (int j = 0; j < i_num_fleets; j++)
        {
            data.append("1.0");
        }
        seasFleetRelF_table->setRowData(i, data);
        data.clear();
    }
}

