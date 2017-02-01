#include "ss_recruitment.h"

spawn_recruit::spawn_recruit(ss_model *parent)
{
    parnt = parent;
//    parameters = new parametermodel();
//    parameters->setRowCount(0);
//    parameters->setColumnCount(7);
    full_parameters = new setupParamVarModel(parnt);// parameterModelTV(parnt);
    full_parameters->setNumParams(5);

    assignments = new tablemodel();
    assignments->setColumnCount(4);
    assignments->setRowCount(0);
    header << "GP" << "seas" << "area" << "Settle_age";
    assignments->setHeader(header);

    assignmentParams = new setupParamVarModel(parnt);// parameterModelTV(parnt);
    interactParams = new setupParamVarModel(parnt);// parameterModelTV(parnt);
    recruitDistParams = new setupParamVarModel(parnt);// parameterModelTV(parnt);
    recruitDistParams->setNumParams(2);

    tvParameters = full_parameters->getParamVarsModel();//getTimeVaryParams();//new shortParameterModel((QObject *)parnt);
//    tvParameters->setColumnCount(7);

    reset();
/*    method = 3;
    env_link = 0;
    env_target = 0;
    rec_dev = 1;
    rec_dev_start_yr = 1980;
    rec_dev_end_yr = 2009;
    rec_dev_phase = -3;
    advanced_opts = true;
    rec_dev_early_start = 0;
    rec_dev_early_phase = -4;
    fcast_rec_phase = 0;
    fcast_lambda = 1;
    nobias_last_early_yr = 971;
    fullbias_first_yr = 1931;
    fullbias_last_yr = 2010;
    nobias_first_recent_yr = 2011;
    max_bias_adjust = 0;
    rec_cycles = 0;
    rec_dev_min = -5;
    rec_dev_max = 5;
    num_rec_dev = 0;*/
    recruitDeviations = new recruitDevs();
}

spawn_recruit::~spawn_recruit()
{
//    delete parameters;
    delete full_parameters;
    delete assignments;
    delete assignmentParams;
    delete recruitDeviations;
    delete interactParams;
    delete recruitDistParams;
}

void spawn_recruit::reset()
{
    distrib_method = 1;
    distrib_area = 1;
    method = 3;
    env_link = 0;
    env_target = 0;
    rec_dev = 1;
    rec_dev_start_yr = 1980;
    rec_dev_end_yr = 2009;
    rec_dev_phase = -3;
    advanced_opts = true;
    rec_dev_early_start = 0;
    rec_dev_early_phase = -4;
    fcast_rec_phase = 0;
    fcast_lambda = 1;
    nobias_last_early_yr = 971;
    fullbias_first_yr = 1931;
    fullbias_last_yr = 2010;
    nobias_first_recent_yr = 2011;
    max_bias_adjust = 0;
    rec_cycles = 0;
    rec_dev_min = -5;
    rec_dev_max = 5;
    num_rec_dev = 0;
    use_steepness = 0;
}

int spawn_recruit::getNumAssignments()
{
    return assignments->rowCount();
}

void spawn_recruit::setNumAssignments(int rows)
{
    assignments->setRowCount(rows);
}

tablemodel *spawn_recruit::getAssignments() const
{
    return assignments;
}

QStringList spawn_recruit::getAssignment(int row)
{
    return assignments->getRowData(row);
}

void spawn_recruit::setAssignment(int row, QStringList data)
{
    if (row >= assignments->rowCount())
        assignments->setRowCount(row + 1);
    assignments->setRowData(row, data);
}

bool spawn_recruit::getDoRecruitInteract() const
{
    return doRecruitInteract;
}

void spawn_recruit::setDoRecruitInteract(bool value)
{
    doRecruitInteract = value;
}

void spawn_recruit::setDoRecruitInteract(int value)
{
    if (value != 0)
        doRecruitInteract = true;
    else
        doRecruitInteract = false;
}

void spawn_recruit::setInteractParam(int index, QStringList data)
{
    if (index >= interactParams->getNumParams())
        interactParams->setNumParams(index + 1);
    interactParams->setParamData(index, data);
}

void spawn_recruit::setRecruitDistParam(int index, QStringList data)
{
    if (index >= recruitDistParams->getNumParams())
        recruitDistParams->setNumParams(index + 1);
    recruitDistParams->setParamData(index, data);
}

void spawn_recruit::setAssignmentParam(int index, QStringList data)
{
    if (index >= assignmentParams->getNumParams())
        assignmentParams->setNumParams(index + 1);
    assignmentParams->setParamData(index, data);
}

void spawn_recruit::setTVParameter(int index, QStringList values)
{
    if (index >= tvParameters->rowCount())
        tvParameters->setRowCount(index + 1);
    tvParameters->setRowData(index, values);
}

void spawn_recruit::fromFile(ss_file *file)
{
    QString token('#'), temp_str;
    QStringList datalist;
    int i;
    
    method = file->get_next_value().toInt();
    i = 0;
    {
        datalist = readShortParameter(file);
        for (int j = 0; j < 7; j++)
            datalist.append("0"); // to fill out long parameter line
        full_parameters->setParamData(i, datalist);
        full_parameters->setParamHeader(i++, "SR_LN(R0)");
    }
    {
        datalist = readShortParameter(file);
        for (int j = 0; j < 7; j++)
            datalist.append("0"); // to fill out long parameter line
        full_parameters->setParamData(i, datalist);
        full_parameters->setParamHeader(i++, "SR_BH_flat_steep");
    }
    if (method == 5 ||
            method == 7 ||
            method == 8)
    {
        datalist = readShortParameter(file);
        for (int j = 0; j < 7; j++)
            datalist.append("0");
        full_parameters->setParamData(i, datalist);
        full_parameters->setParamHeader(i++, "SR_3rd_PARM");
    }
    {
        datalist = readShortParameter(file);
        for (int j = 0; j < 7; j++)
            datalist.append("0"); // to fill out long parameter line
        full_parameters->setParamData(i, datalist);
        full_parameters->setParamHeader(i++, "SR_sigmaR");
    }
    {
        datalist = readShortParameter(file);
        for (int j = 0; j < 7; j++)
            datalist.append("0"); // to fill out long parameter line
        full_parameters->setParamData(i, datalist);
        full_parameters->setParamHeader(i++, "SR_nullparm");
    }
    {
        datalist = readShortParameter(file);
        for (int j = 0; j < 7; j++)
            datalist.append("0"); // to fill out long parameter line
        full_parameters->setParamData(i, datalist);
        full_parameters->setParamHeader(i++, "SR_R1_offset");
    }
    {
        datalist = readShortParameter(file);
        for (int j = 0; j < 7; j++)
            datalist.append("0"); // to fill out long parameter line
        full_parameters->setParamData(i, datalist);
        full_parameters->setParamHeader(i++, "SR_autocorr");
    }

    env_link = file->get_next_value().toFloat();
    env_target = file->get_next_value().toInt();
    rec_dev = file->get_next_value().toInt();
    rec_dev_start_yr = file->get_next_value().toInt();
    rec_dev_end_yr = file->get_next_value().toInt();
    rec_dev_phase = file->get_next_value().toInt();
    advanced_opts = (file->get_next_value().compare("0") != 0);

    if (advanced_opts)
    {
        rec_dev_early_start = file->get_next_value().toInt();
        rec_dev_early_phase = file->get_next_value().toInt();
        fcast_rec_phase = file->get_next_value().toInt();
        fcast_lambda = file->get_next_value().toFloat();
        nobias_last_early_yr = file->get_next_value().toInt();
        fullbias_first_yr = file->get_next_value().toInt();
        fullbias_last_yr = file->get_next_value().toInt();
        nobias_first_recent_yr = file->get_next_value().toInt();
        max_bias_adjust = file->get_next_value().toFloat();
        rec_cycles = file->get_next_value().toInt();
        rec_dev_min = file->get_next_value().toInt();
        rec_dev_max = file->get_next_value().toInt();
        num_rec_dev = file->get_next_value().toInt();

        assignmentParams->setNumParams(rec_cycles);
        for (i = 0; i < rec_cycles; i++)
        {
            datalist = readParameter(file);
            assignmentParams->setParamData(i, datalist);
        }

        getRecruitDevs()->setNumRecruitDevs(num_rec_dev);
        for (i = 0; i < num_rec_dev; i++)
        {
            datalist.clear();
            datalist.append(file->get_next_value());
            datalist.append(file->get_next_value());
            getRecruitDevs()->setRecruitDev(i, datalist);
/*            temp_int = file->next_value().toInt();
            temp_float = file->next_value().toFloat();
            yearly_devs[temp_int] = temp_float;*/
        }
    }
}

QString spawn_recruit::toText()
{
    int i, temp_ind;
    float temp_val;
    QString txt;
    QStringList datalist;

    sr_text.clear();
    sr_text.append(QString("#_Spawner-Recruitment" ));
    sr_text.append(QString("%1 #_SR_function").arg(
                       QString::number(method)));
    sr_text.append(QString(": 2=Ricker; 3=std_B-H; 4=SCAA; 5=Hockey; 6=B-H_flattop; 7=survival_3Parm; 8=Shepard_3Parm" ));
    sr_text.append(QString("#_LO HI INIT PRIOR PR_TYPE SD PHASE" ));
/*    for (i = 0; i < 6; i++)
    {
        datalist = parameters->getRowData(i);
        for (int j = 0; j < 7; j++)
            sr_text.append(QString(" %1").arg(datalist.at(j)));

        sr_text.append(QString (" # " ));
    }*/
    sr_text.append(QString ("%1 # SR_LN(R0)" ).arg(full_parameters->getParamText(0)));
    sr_text.append(QString ("%1 # SR_BH_steep" ).arg(full_parameters->getParamText(1)));
    sr_text.append(QString ("%1 # SR_sigmaR" ).arg(full_parameters->getParamText(2)));
    sr_text.append(QString ("%1 # SR_envlink" ).arg(full_parameters->getParamText(3)));
    sr_text.append(QString ("%1 # SR_R1_offset" ).arg(full_parameters->getParamText(4)));
    sr_text.append(QString ("%1 # SR_autocorr" ).arg(full_parameters->getParamText(5)));

    sr_text.append(QString ("%1 #_do_rec_dev:  0=none; 1=devvector; 2=simple deviations" ).arg(
                       QString::number(rec_dev)));
    sr_text.append(QString ("%1 #_first_year_of_main_rec_devs; early devs can preceed this era" ).arg(
                       QString::number(rec_dev_start_yr)));
    sr_text.append(QString ("%1 #_last_year_of_main_rec_devs; forecast devs start in following year" ).arg(
                       QString::number(rec_dev_end_yr)));
    sr_text.append(QString ("%1 #_rec-dev phase" ).arg(
                       QString::number(rec_dev_phase)));
    sr_text.append(QString ("%1 # (0/1) to read 13 advanced options" ).arg(
                       advanced_opts? "1":"0"));

    if (advanced_opts)
    {
        sr_text.append(QString (" %1 #_rec-dev early start (0=none; neg value makes relative to recdev_start)" ).arg(
                           QString::number(rec_dev_early_start)));
        sr_text.append(QString (" %1 #_rec-dev early phase" ).arg(
                           QString::number(rec_dev_early_phase)));
        sr_text.append(QString (" %1 #_forecast recruitment phase (incl. late recr) (0 value resets to maxphase+1)" ).arg(
                           QString::number(fcast_rec_phase)));
        sr_text.append(QString (" %1 #_lambda for forecast_recr_like occurring before endyr+1" ).arg(
                           QString::number(fcast_lambda)));
        sr_text.append(QString (" %1 #_last_early_yr_nobias_adj_in_MPD" ).arg(
                           QString::number(nobias_last_early_yr)));
        sr_text.append(QString (" %1 #_first_yr_fullbias_adj_in_MPD" ).arg(
                           QString::number(fullbias_first_yr)));
        sr_text.append(QString (" %1 #_last_yr_fullbias_adj_in_MPD" ).arg(
                           QString::number(fullbias_last_yr)));
        sr_text.append(QString (" %1 #_first_recent_yr_nobias_adj_in_MPD" ).arg(
                           QString::number(nobias_first_recent_yr)));
        sr_text.append(QString (" %1 #_max_bias_adj_in_MPD (-1 to override ramp and set biasadj=1.0 for all estimated recdevs)" ).arg(
                           QString::number(max_bias_adjust)));
        sr_text.append(QString (" %1 #_period of cycles in recruitment (N parms read below)" ).arg(
                           QString::number(rec_cycles)));
        sr_text.append(QString (" %1 #_min rec_dev" ).arg(
                           QString::number(rec_dev_min)));
        sr_text.append(QString (" %1 #_max rec_dev" ).arg(
                           QString::number(rec_dev_max)));
        sr_text.append(QString (" %1 #_read rec_devs" ).arg(
                           QString::number(num_rec_dev)));
        sr_text.append(QString ("#_end of advanced SR options" ));
    }
    sr_text.append("#" );

    if (rec_cycles == 0)
    {
        sr_text.append("#_placeholder for full parameter lines for recruitment cycles" );
    }
    else
    {
        for (i = 0; i < rec_cycles; i++)
        {
//            txt.clear();
            datalist = full_parameters->getParamData(i);
            for (int j = 0; j < 14; j++)
                sr_text.append(QString(" %1").arg(datalist.at(j)));
            sr_text.append(QString (" # " ));
        }
    }

    sr_text.append(QString("# read %1 specified recr devs\n#_Yr Input_value" ).arg(
                       QString::number(num_rec_dev)));
    for (std::map<int,float>::iterator itr = yearly_devs.begin(); itr != yearly_devs.end(); itr++)
    {
        temp_ind = itr->first;
        temp_val = itr->second;
        sr_text.append(QString("%1 %2" ).arg(
                           QString::number(temp_ind),
                           QString::number(temp_val)));
    }
    sr_text.append("#" );

    return sr_text;
}
