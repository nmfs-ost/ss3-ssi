#include <QInputDialog>
#include <QFont>
#include <QMessageBox>

#include "fleet_widget.h"
#include "ui_fleet_widget.h"

fleet_widget::fleet_widget(ss_model *m_data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fleet_widget)
{
    ui->setupUi(this);
    titleFont = ui->label_title->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);

    current_fleet = NULL;
    ui->comboBox_fleet_name->setFont(titleFont);

    ui->label_selex_age_discard->setVisible(false);
    ui->spinBox_selex_age_discard->setVisible(false);

    catchview = new tableview();
    catchview->setParent(this);
    catchedit = new catchdelegate(this);
//    catchview->setItemDelegate(catchedit);
    ui->verticalLayout_catch_table->addWidget(catchview);

    abundview = new tableview();
    abundview->setParent(this);
    abundedit = new abundancedelegate(this);
//    abundview->setItemDelegate(abundedit);
    ui->verticalLayout_abund->addWidget(abundview);

    discardview = new tableview();
    discardview->setParent(this);
    discardedit = new abundancedelegate(this);
//    discardview->setItemDelegate(discardedit);
    ui->verticalLayout_discard->addWidget(discardview);

    mbwtView = new tableview();
    mbwtView->setParent(this);
    ui->verticalLayout_mbwt->addWidget(mbwtView);
    lenCompView = new tableview();
    lenCompView->setParent(this);
    ui->verticalLayout_len_obs->addWidget(lenCompView);
    ageCompView = new tableview();
    ageCompView->setParent(this);
    ui->verticalLayout_age_obs->addWidget(ageCompView);
    saaObsView = new tableview();
    saaObsView->setParent(this);
    ui->verticalLayout_saa_obs->addWidget(saaObsView);
    genObsView = new tableview();
    genObsView->setParent(this);
    ui->verticalLayout_gen_obs->addWidget(genObsView);
    morphObsView = new tableview();
    morphObsView->setParent(this);
    ui->verticalLayout_morph_obs->addWidget(morphObsView);
    recapObsView = new tableview();
    recapObsView->setParent(this);
    ui->horizontalLayout_rec_obs->addWidget(recapObsView);

    qSetupView = new tableview();
    qSetupView->setParent(this);
    ui->verticalLayout_q_setup->addWidget(qSetupView);
    qParamsView = new tableview();
    qParamsView->setParent(this);
    ui->verticalLayout_q_parameters->addWidget(qParamsView);
    qTvParamsView =  new tableview();
    qTvParamsView->setParent(this);
    ui->verticalLayout_q_tv_params->addWidget(qTvParamsView);

    sizeSelexParamsView = new tableview();
    sizeSelexParamsView->setParent(this);
    ui->verticalLayout_selex_size_params->addWidget(sizeSelexParamsView);
    sizeSelexRetainView = new tableview();
    sizeSelexRetainView->setParent(this);
    ui->verticalLayout_sel_size_retain->addWidget(sizeSelexRetainView);
    sizeSelexDiscardView = new tableview();
    sizeSelexDiscardView->setParent(this);
    ui->verticalLayout_sel_size_discard->addWidget(sizeSelexDiscardView);
    sizeSelexMaleView = new tableview();
    sizeSelexMaleView->setParent(this);
    ui->verticalLayout_sel_size_male->addWidget(sizeSelexMaleView);
    sizeSelexTimeVaryParamsView = new tableview();
    sizeSelexTimeVaryParamsView->setParent(this);
    ui->verticalLayout_selex_size_timevary_parms->addWidget(sizeSelexTimeVaryParamsView);
    ageSelexParamsView = new tableview();
    ageSelexParamsView->setParent(this);
    ui->verticalLayout_selex_age_params->addWidget(ageSelexParamsView);
    ageSelexMaleView = new tableview();
    ageSelexMaleView->setParent(this);
    ui->verticalLayout_sel_age_male->addWidget(ageSelexMaleView);
    ageSelexTimeVaryParamsView = new tableview();
    ageSelexTimeVaryParamsView->setParent(this);
    ui->verticalLayout_selex_age_timevary_parms->addWidget(ageSelexTimeVaryParamsView);
    selexSizeEqDialog = new equationDialog(this, new QString("Size"));
    selexSizeEqDialog->hide();
    selexSizeInfoDialog = new DialogInfo (this);
    setupSelexSizeInfo();
    selexSizeInfoDialog->hide();
    selexAgeEqDialog = new equationDialog(this, new QString("Age"));
    selexAgeEqDialog->hide();
    selexAgeInfoDialog = new DialogInfo (this);
    setupSelexAgeInfo();
    selexAgeInfoDialog->hide();

    lambdaView = new tableview();
    lambdaView->setParent(this);
    ui->verticalLayout_lambda_changes->addWidget(lambdaView);

    set_model(m_data);
    totalFleets = m_data->get_num_fleets();

    connect (ui->comboBox_fleet_name, SIGNAL(currentIndexChanged(int)), SLOT(set_current_fleet(int)));
    connect (ui->pushButton_edit_name, SIGNAL(clicked()), SLOT(edit_name()));
    connect (ui->checkBox_active, SIGNAL(toggled(bool)), SLOT(setActive(bool)));
//    connect (ui->pushButton_next, SIGNAL(clicked()), SLOT(nextFleet()));
//    connect (ui->pushButton_prev, SIGNAL(clicked()), SLOT(prevFleet()));
    connect (ui->comboBox_type, SIGNAL(currentIndexChanged(int)), SLOT(set_fleet_type(int)));
    connect (ui->pushButton_delete, SIGNAL(clicked()), SLOT(delete_current_fleet()));
    connect (ui->pushButton_new, SIGNAL(clicked()), SLOT(create_new_fleet()));
    connect (ui->pushButton_duplicate, SIGNAL(clicked()), SLOT(duplicate_current_fleet()));
    connect (ui->spinBox_units, SIGNAL(valueChanged(int)), ui->spinBox_catch_units, SLOT(setValue(int)));
    connect (ui->comboBox_byc_dead, SIGNAL(currentIndexChanged(int)), SLOT(changeBycatchDead(int)));
    connect (ui->comboBox_byc_f, SIGNAL(currentIndexChanged(int)), SLOT(changeBycatchF(int)));
    connect (ui->lineEdit_byc_firstYear, SIGNAL(editingFinished()), SLOT(changeBycatchFirstYr()));
    connect (ui->lineEdit_byc_lastYear, SIGNAL(editingFinished()), SLOT(changeBycatchLastYr()));
    connect (ui->spinBox_q_time_vary_read, SIGNAL(valueChanged(int)), SLOT(setQTimeVaryReadParams(int)));

    connect (ui->spinBox_sel_time_vary_read, SIGNAL(valueChanged(int)), model_data->getFleet(0), SLOT(setSelTimeVaryReadParams(int)));
    connect (ui->spinBox_ar1, SIGNAL(valueChanged(int)), SLOT(setAr1SelexSmoother(int)));
    connect (ui->pushButton_selex_size_pattern_info, SIGNAL(clicked()), selexSizeInfoDialog, SLOT(show()));
    connect (ui->spinBox_selex_size_pattern, SIGNAL(valueChanged(int)), SLOT(changeSelexSizePattern(int)));
    connect (ui->spinBox_selex_size_discard, SIGNAL(valueChanged(int)), SLOT(changeSelexSizeDiscard(int)));
    connect (ui->spinBox_selex_size_male, SIGNAL(valueChanged(int)), SLOT(changeSelexSizeMale(int)));
    connect (ui->spinBox_selex_size_special, SIGNAL(valueChanged(int)), SLOT(changeSelexSizeSpecial(int)));
    connect (ui->pushButton_selex_size_curve, SIGNAL(clicked(bool)), SLOT(showSelexSizeCurve(bool)));
    connect (selexSizeEqDialog, SIGNAL(closed()), SLOT(selexSizeCurveClosed()));

    connect (ui->pushButton_selex_age_pattern_info, SIGNAL(clicked()), selexAgeInfoDialog, SLOT(show()));
    connect (ui->spinBox_selex_age_pattern, SIGNAL(valueChanged(int)), SLOT(changeSelexAgePattern(int)));
    connect (ui->spinBox_selex_age_male, SIGNAL(valueChanged(int)), SLOT(changeSelexAgeMale(int)));
    connect (ui->spinBox_selex_age_special, SIGNAL(valueChanged(int)), SLOT(changeSelexAgeSpecial(int)));
    connect (ui->pushButton_selex_age_curve, SIGNAL(clicked(bool)), SLOT(showSelexAgeCurve(bool)));
    connect (selexAgeEqDialog, SIGNAL(closed()), SLOT(selexAgeCurveClosed()));

    connect (ui->lineEdit_length_comp_tails, SIGNAL(editingFinished()), SLOT(changeLengthMinTailComp()));
    connect (ui->lineEdit_length_constant, SIGNAL(editingFinished()), SLOT(changeLengthAddToData()));
    connect (ui->lineEdit_age_comp_tails, SIGNAL(editingFinished()), SLOT(changeAgeMinTailComp()));
    connect (ui->lineEdit_age_constant, SIGNAL(editingFinished()), SLOT(changeAgeAddToData()));

//    connectFleet();
//    set_current_fleet(0);

//    refresh();
//    ui->comboBox_fleet_name->setCurrentIndex(0);
    ui->tabWidget_fleet->setCurrentIndex(0);
}

void fleet_widget::disconnectFleet()
{
    if (current_fleet)
    {
    disconnect (current_fleet, SIGNAL(newDataRead()), this, SLOT(refresh()));
    disconnect (ui->doubleSpinBox_timing, SIGNAL(valueChanged(double)), current_fleet, SLOT(setSeasTiming(double)));
    disconnect (ui->spinBox_area, SIGNAL(valueChanged(int)), current_fleet, SLOT(setArea(int)));
    disconnect (ui->spinBox_units, SIGNAL(valueChanged(int)), current_fleet, SLOT(setCatchUnits(int)));
//    disconnect (ui->spinBox_need_catch_mult, SIGNAL(valueChanged(int)), current_fleet, SLOT()));
    disconnect (ui->checkBox_need_catch_mult, SIGNAL(toggled(bool)), current_fleet, SLOT(setCatchMultiplier(bool)));
//    disconnect (ui->listWidget_init_catch, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(edit_catch_line(QListWidgetItem*)));
    disconnect (ui->spinBox_abund_units, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAbundUnits(int)));
    disconnect (ui->spinBox_abund_err, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAbundErrType(int)));
    disconnect (ui->spinBox_discard_units, SIGNAL(valueChanged(int)), current_fleet, SLOT(setDiscardUnits(int)));
    disconnect (ui->spinBox_discard_err, SIGNAL(valueChanged(int)), current_fleet, SLOT(setDiscardErrType(int)));
    disconnect (ui->spinBox_mbwt_df, SIGNAL(valueChanged(int)), current_fleet, SLOT(setMbwtDF(int)));
    disconnect (ui->spinBox_num_mbwt, SIGNAL(valueChanged(int)), current_fleet, SLOT(setMbwtNumObs(int)));
    disconnect (ui->spinBox_obs_len_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthNumObs(int)));
    disconnect (ui->spinBox_length_combine, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCombineGen(int)));
    disconnect (ui->spinBox_length_comp_bins, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCompressBins(int)));
    disconnect (ui->spinBox_length_comp_error, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCompError(int)));
    disconnect (ui->spinBox_length_comp_error_parm, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCompErrorParm(int)));
    disconnect (ui->spinBox_obs_age_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeNumObs(int)));
    disconnect (ui->spinBox_age_combine, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCombineGen(int)));
    disconnect (ui->spinBox_age_comp_bins, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCompressBins(int)));
    disconnect (ui->spinBox_age_comp_error, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCompError(int)));
    disconnect (ui->spinBox_age_comp_error_parm, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCompErrorParm(int)));
    disconnect (ui->spinBox_obs_saa_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setSaaNumObs(int)));
    disconnect (ui->spinBox_obs_rec_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setRecapNumEvents(int)));
    disconnect (ui->spinBox_obs_morph_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setMorphNumObs(int)));

    disconnect (ui->groupBox_q_setup, SIGNAL(toggled(bool)), current_fleet, SLOT(setQSetupRead(bool)));
    disconnect (current_fleet->Q()->getSetupTable(), SIGNAL(dataChanged()), this, SLOT(qSetupChanged()));
    disconnect (current_fleet->Q()->getParamTable(), SIGNAL(dataChanged()), this, SLOT(qSetupParamsChanged()));
    disconnect (current_fleet->Q()->getTVParams(), SIGNAL(dataChanged()), this, SLOT(qSetupTVParamsChanged()));

    disconnect (current_fleet->getSizeSelectivity()->getParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(sizeSelexParamsChanged()));
    disconnect (current_fleet->getSizeSelectivity()->getTimeVaryParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(sizeSelexTVParamsChanged()));
    disconnect (ui->spinBox_selex_size_num_params, SIGNAL(valueChanged(int)),
             current_fleet->getSizeSelectivity(), SLOT(setNumParameters(int)));

    disconnect (current_fleet->getAgeSelectivity()->getParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(ageSelexParamsChanged()));
    disconnect (current_fleet->getAgeSelectivity()->getTimeVaryParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(ageSelexTVParamsChanged()));
    disconnect (ui->spinBox_selex_age_num_params, SIGNAL(valueChanged(int)),
             current_fleet->getAgeSelectivity(), SLOT(setNumParameters(int)));
    }
}

void fleet_widget::connectFleet()
{
    connect (current_fleet, SIGNAL(newDataRead()), SLOT(refresh()));
    connect (ui->doubleSpinBox_timing, SIGNAL(valueChanged(double)), current_fleet, SLOT(setSeasTiming(double)));
    connect (ui->spinBox_area, SIGNAL(valueChanged(int)), current_fleet, SLOT(setArea(int)));
//    connect (ui->spinBox_need_catch_mult, SIGNAL(valueChanged(int)), current_fleet, SLOT()));
    connect (ui->spinBox_units, SIGNAL(valueChanged(int)), current_fleet, SLOT(setCatchUnits(int)));
    connect (ui->checkBox_need_catch_mult, SIGNAL(toggled(bool)), current_fleet, SLOT(setCatchMultiplier(bool)));
//    connect (ui->listWidget_init_catch, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(edit_catch_line(QListWidgetItem*)));
    connect (ui->spinBox_abund_units, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAbundUnits(int)));
    connect (ui->spinBox_abund_err, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAbundErrType(int)));
    connect (ui->spinBox_discard_units, SIGNAL(valueChanged(int)), current_fleet, SLOT(setDiscardUnits(int)));
    connect (ui->spinBox_discard_err, SIGNAL(valueChanged(int)), current_fleet, SLOT(setDiscardErrType(int)));

    connect (ui->spinBox_mbwt_df, SIGNAL(valueChanged(int)), current_fleet, SLOT(setMbwtDF(int)));
    connect (ui->spinBox_num_mbwt, SIGNAL(valueChanged(int)), current_fleet, SLOT(setMbwtNumObs(int)));

    connect (ui->spinBox_obs_len_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthNumObs(int)));
    connect (ui->spinBox_length_combine, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCombineGen(int)));
    connect (ui->spinBox_length_comp_bins, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCompressBins(int)));
    connect (ui->spinBox_length_comp_error, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCompError(int)));
    connect (ui->spinBox_length_comp_error_parm, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthCompErrorParm(int)));
    connect (ui->spinBox_length_min_sample, SIGNAL(valueChanged(int)), current_fleet, SLOT(setLengthMinSampleSize(int)));

    connect (ui->spinBox_obs_age_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeNumObs(int)));
    connect (ui->spinBox_age_combine, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCombineGen(int)));
    connect (ui->spinBox_age_comp_bins, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCompressBins(int)));
    connect (ui->spinBox_age_comp_error, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCompError(int)));
    connect (ui->spinBox_age_comp_error_parm, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCompErrorParm(int)));
    connect (ui->spinBox_age_min_sample, SIGNAL(valueChanged(int)), current_fleet, SLOT(setAgeCompMinSampleSize(int)));

    connect (ui->spinBox_obs_saa_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setSaaNumObs(int)));
    connect (ui->spinBox_obs_gen_num, SIGNAL(valueChanged(int)), SLOT(changeGenMethodNum(int)));
    connect (ui->spinBox_obs_gen_numObs, SIGNAL(valueChanged(int)), SLOT(changeGenNumObs(int)));
    connect (ui->spinBox_obs_rec_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setRecapNumEvents(int)));
    connect (ui->spinBox_obs_morph_numObs, SIGNAL(valueChanged(int)), current_fleet, SLOT(setMorphNumObs(int)));

    connect (ui->groupBox_q_setup, SIGNAL(toggled(bool)), current_fleet, SLOT(setQSetupRead(bool)));
    connect (current_fleet->Q()->getSetupTable(), SIGNAL(dataChanged()),
             this, SLOT(qSetupChanged()));
    connect (current_fleet->Q()->getParamTable(), SIGNAL(dataChanged()),
             this, SLOT(qSetupParamsChanged()));
    connect (current_fleet->Q()->getTVParams(), SIGNAL(dataChanged()),
             this, SLOT(qSetupTVParamsChanged()));

    connect (current_fleet->getSizeSelectivity()->getParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(sizeSelexParamsChanged()));
    connect (current_fleet->getSizeSelectivity()->getTimeVaryParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(sizeSelexTVParamsChanged()));
    connect (ui->spinBox_selex_size_num_params, SIGNAL(valueChanged(int)),
             current_fleet->getSizeSelectivity(), SLOT(setNumParameters(int)));

    connect (current_fleet->getAgeSelectivity()->getParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(ageSelexParamsChanged()));
    connect (current_fleet->getAgeSelectivity()->getTimeVaryParameterModel(),
             SIGNAL(dataChanged()), this, SLOT(ageSelexTVParamsChanged()));
    connect (ui->spinBox_selex_age_num_params, SIGNAL(valueChanged(int)),
             current_fleet->getAgeSelectivity(), SLOT(setNumParameters(int)));

}

fleet_widget::~fleet_widget()
{
    delete ui;
}

void fleet_widget::edit_name()
{
    int in = ui->comboBox_fleet_name->currentIndex();
    bool ok;
    QString name (ui->comboBox_fleet_name->currentText());
    QString text = QInputDialog::getText(this, tr("Edit Fleet Name"),
                tr("Fleet name:"), QLineEdit::Normal, name, &ok);
    if (ok && !text.isEmpty())
    {
        // set name in fleet in model data
        current_fleet->setName(text);
        // clear list item and set it to new
        ui->comboBox_fleet_name->removeItem(in);
        ui->comboBox_fleet_name->insertItem(in, text);
        ui->comboBox_fleet_name->setCurrentIndex(in);
        ui->label_name->setText(text);
    }
}

void fleet_widget::setActive(bool flag)
{
    ui->label_number->setVisible(flag);
    ui->spinBox_number->setVisible(flag);
    if (current_fleet->getActive() != flag)
    {
        current_fleet->setActive(flag);
        model_data->assignFleetNumbers();
        refresh();
    }
}

void fleet_widget::readingNewModel()
{
    disconnectFleet();
    current_fleet = NULL;
}

void fleet_widget::reset ()
{
    totalFleets = model_data->get_num_fleets();
    refresh();
}

void fleet_widget::refreshFleetNames()
{
    disconnect (ui->comboBox_fleet_name, SIGNAL(currentIndexChanged(int)), this, SLOT(set_current_fleet(int)));
    while (ui->comboBox_fleet_name->count() > 0)
        ui->comboBox_fleet_name->removeItem(0);

    for (int i = 0; i < totalFleets; i++)
    {
        QString str = QString(model_data->getFleet(i)->getName());
        ui->comboBox_fleet_name->addItem(str);
    }
    connect (ui->comboBox_fleet_name, SIGNAL(currentIndexChanged(int)), SLOT(set_current_fleet(int)));
}

void fleet_widget::refresh()
{
    int curr = ui->comboBox_fleet_name->currentIndex();
    catchedit->setYearRange(model_data->get_start_year(), model_data->get_end_year());
    catchedit->setNumSeasons(model_data->get_num_seasons());
    catchedit->setMaxCatch(1999999.0);
    abundedit->setYearRange(model_data->get_start_year(), model_data->get_end_year());
    abundedit->setValueRange(0, 1999999.0);
    discardedit->setYearRange(model_data->get_start_year(), model_data->get_end_year());
    discardedit->setValueRange(0, 1999999.0);

    totalFleets = model_data->get_num_fleets();
    ui->spinBox_selex_size_discard->setMinimum(-totalFleets);
//    ui->spinBox_total->setValue(totalFleets);
    ui->spinBox_area->setMaximum(model_data->get_num_areas());
    refreshFleetNames();

    ui->spinBox_q_time_vary_read->setValue(model_data->getFleet(0)->getQTimeVaryReadParams());
    setQTimeVaryReadParams(model_data->getFleet(0)->getQTimeVaryReadParams());
    ui->spinBox_sel_time_vary_read->setValue(model_data->getFleet(0)->getSelTimeVaryReadParams());

    if (curr >= totalFleets)
        curr == totalFleets - 1;
    if (curr < 0)
        curr = 0;
    ui->comboBox_fleet_name->setCurrentIndex(curr);
    set_current_fleet(curr);

    ui->tabWidget_fleet->setCurrentIndex(0);
    ui->tabWidget_obs->setCurrentIndex(0);
    ui->tabWidget_comp->setCurrentIndex(0);
    ui->tabWidget_selex->setCurrentIndex(0);
}

void fleet_widget::set_model (ss_model *model)
{
    model_data = model;
    refresh();
}

void fleet_widget::setAgeLengthBins()
{
    int ages = model_data->get_num_ages();
    if (ages > 0)
    {
        QStringList agebinlist (model_data->get_age_composition()->getBins());
        int firstage = QString(agebinlist.at(0)).toInt();
        int age = firstage;
        int interval = QString(agebinlist.at(2)).toInt() - QString(agebinlist.at(1)).toInt();
        QStringList agelist;
        agelist.append(QString::number(age));
        for (int i = 1; i < ages; i++)
        {
            age += interval;
            agelist.append(QString::number(age));
        }
        selexAgeEqDialog->setXvals(agelist);
    }
    selexSizeEqDialog->setXvals(model_data->get_length_composition()->getBins());
}

void fleet_widget::set_current_fleet(int index)
{
    int tabset = ui->tabWidget_fleet->currentIndex();
    if (index >= totalFleets)
        ui->comboBox_fleet_name->setCurrentIndex(totalFleets - 1);
    else if (index < 0)
        ui->comboBox_fleet_name->setCurrentIndex(0);
    else
    {
    Fleet *temp_flt = model_data->getFleet(index);
 //   if (temp_flt != current_fleet)
    {
        disconnectFleet ();
        current_fleet = temp_flt;

        ui->label_name->setText(current_fleet->getName());
        ui->checkBox_active->setChecked(current_fleet->isActive());
        ui->spinBox_number->setValue(current_fleet->getNumber());
        set_type_fleet(current_fleet->getType());
        ui->doubleSpinBox_timing->setValue(current_fleet->getSeasTiming());

        ui->spinBox_area->setValue(current_fleet->getArea());
        ui->spinBox_units->setValue(current_fleet->getCatchUnits());
    //    ui->spinBox_need_catch_mult->setValue(current_fleet->catch_mult());
        ui->spinBox_catch_units->setValue(current_fleet->getCatchUnits());
        ui->checkBox_need_catch_mult->setChecked (current_fleet->getCatchMultiplier() > 0);
        catchview->setModel(current_fleet->getCatchModel());

        if (ui->groupBox_bycatch->isVisible())
        {
            ui->comboBox_byc_dead->setCurrentIndex(current_fleet->getBycatchDead() - 1);
            ui->comboBox_byc_f->setCurrentIndex(current_fleet->getBycatchF() - 1);
            ui->lineEdit_byc_firstYear->setText(current_fleet->getBycFirstYr());
            ui->lineEdit_byc_lastYear->setText(current_fleet->getBycLastYr());
        }
        ui->spinBox_abund_units->setValue(current_fleet->getAbundUnits());
        ui->spinBox_abund_err->setValue(current_fleet->getAbundErrType());
        abundview->setModel(current_fleet->getAbundanceModel());

        ui->spinBox_discard_units->setValue(current_fleet->getDiscardUnits());
        ui->spinBox_discard_err->setValue(current_fleet->getDiscardErrType());
        discardview->setModel(current_fleet->getDiscardModel());

        ui->spinBox_mbwt_df->setValue(current_fleet->getMbwtDF());
        ui->spinBox_num_mbwt->setValue(current_fleet->getMbwtNumObs());
        mbwtView->setModel(current_fleet->getMbwtModel());
        mbwtView->resizeColumnsToContents();

        lenCompView->setModel(current_fleet->getLengthModel());
        lenCompView->resizeColumnsToContents();
        ui->spinBox_obs_len_numObs->setValue(current_fleet->getLengthNumObs());
        ui->lineEdit_length_comp_tails->setText(current_fleet->getLengthMinTailComp());
        ui->lineEdit_length_constant->setText(current_fleet->getLengthAddToData());
        ui->spinBox_length_combine->setValue(current_fleet->getLengthCombineGen());
        ui->spinBox_length_comp_bins->setValue(current_fleet->getLengthCompressBins());
        ui->spinBox_length_comp_error->setValue(current_fleet->getLengthCompError());
        ui->spinBox_length_comp_error_parm->setValue(current_fleet->getLengthCompErrorParm());
        ui->spinBox_length_min_sample->setValue(current_fleet->getLengthMinSampleSize());

        ageCompView->setModel(current_fleet->getAgeModel());
        ageCompView->resizeColumnsToContents();
        ui->spinBox_obs_age_numObs->setValue(current_fleet->getAgeNumObs());
        ui->lineEdit_age_comp_tails->setText(current_fleet->getAgeMinTailComp());
        ui->lineEdit_age_constant->setText(current_fleet->getAgeAddToData());
        ui->spinBox_age_combine->setValue(current_fleet->getAgeCombineGen());
        ui->spinBox_age_comp_bins->setValue(current_fleet->getAgeCompressBins());
        ui->spinBox_age_comp_error->setValue(current_fleet->getAgeCompError());
        ui->spinBox_age_comp_error_parm->setValue(current_fleet->getAgeCompErrorParm());
        ui->spinBox_age_min_sample->setValue(current_fleet->getAgeCompMinSampleSize());

        saaObsView->setModel(current_fleet->getSaaModel());
        saaObsView->resizeColumnsToContents();
        ui->spinBox_obs_saa_numObs->setValue(current_fleet->getSaaNumObs());

        int genmod = ui->spinBox_obs_gen_num->value();
        if (genmod > 0)
            genObsView->setModel(current_fleet->getGenModel(genmod-1));

        morphObsView->setModel(current_fleet->getMorphModel());
        morphObsView->resizeColumnsToContents();
        ui->spinBox_obs_morph_numObs->setValue(current_fleet->getMorphNumObs());
        recapObsView->setModel(current_fleet->getRecapModel());
        recapObsView->resizeColumnsToContents();
        ui->spinBox_obs_rec_numObs->setValue(current_fleet->getRecapNumEvents());

        ui->groupBox_q_setup->setChecked(current_fleet->getQSetupRead());
        qSetupView->setModel(current_fleet->Q()->getSetupTable());
        qSetupView->setHeight(current_fleet->Q()->getSetupTable());
        current_fleet->Q()->setParamHdrs();
        qSetupView->resizeColumnsToContents();
        qParamsView->setModel(current_fleet->Q()->getParamTable());
        qParamsView->setHeight(current_fleet->Q()->getNumParams());
        qParamsView->resizeColumnsToContents();
        qTvParamsView->setModel(current_fleet->Q()->getTVParams());
        qTvParamsView->setHeight(current_fleet->Q()->getNumTVParams());
        qTvParamsView->resizeColumnsToContents();
        current_fleet->Q()->getTVParamModel()->updateVarParams();

        ui->spinBox_selex_size_pattern->setValue(current_fleet->getSizeSelectivity()->getPattern());
        ui->spinBox_selex_size_discard->setValue(current_fleet->getSizeSelectivity()->getDiscard());
        ui->spinBox_selex_size_male->setValue(current_fleet->getSizeSelectivity()->getMale());
        ui->spinBox_selex_size_special->setValue(current_fleet->getSizeSelectivity()->getSpecial());
        sizeSelexParamsView->setModel(current_fleet->getSizeSelectivity()->getParameterModel());
        sizeSelexParamsView->setHeight(current_fleet->getSizeSelectivity()->getParameterModel());
        sizeSelexParamsView->resizeColumnsToContents();
        sizeSelexRetainView->setModel(current_fleet->getSizeSelectivity()->getRetainParameterModel()->getParamTable());
        sizeSelexRetainView->setHeight(current_fleet->getSizeSelectivity()->getRetainParameterModel()->getNumParams());
        sizeSelexRetainView->resizeColumnsToContents();
        sizeSelexDiscardView->setModel(current_fleet->getSizeSelectivity()->getDiscardParameterModel()->getParamTable());
        sizeSelexDiscardView->setHeight(current_fleet->getSizeSelectivity()->getDiscardParameterModel()->getNumParams());
        sizeSelexDiscardView->resizeColumnsToContents();
        sizeSelexMaleView->setModel(current_fleet->getSizeSelectivity()->getMaleParameterModel()->getParamTable());
        sizeSelexMaleView->setHeight(current_fleet->getSizeSelectivity()->getMaleParameterModel()->getNumParams());
        sizeSelexMaleView->resizeColumnsToContents();
        sizeSelexTimeVaryParamsView->setModel(current_fleet->getSizeSelectivity()->getTimeVaryParameterModel());
        sizeSelexTimeVaryParamsView->setHeight(current_fleet->getSizeSelectivity()->getNumTimeVaryParameters());
        sizeSelexTimeVaryParamsView->resizeColumnsToContents();

        ui->spinBox_selex_age_pattern->setValue(current_fleet->getAgeSelectivity()->getPattern());
        ui->spinBox_selex_age_discard->setValue(current_fleet->getAgeSelectivity()->getDiscard());
        ui->spinBox_selex_age_male->setValue(current_fleet->getAgeSelectivity()->getMale());
        ui->spinBox_selex_age_special->setValue(current_fleet->getAgeSelectivity()->getSpecial());
        ageSelexParamsView->setModel(current_fleet->getAgeSelectivity()->getParameterModel());
        ageSelexParamsView->setHeight(current_fleet->getAgeSelectivity()->getParameterModel());
        ageSelexParamsView->resizeColumnsToContents();
        ageSelexMaleView->setModel(current_fleet->getAgeSelectivity()->getMaleParameterModel()->getParamTable());
        ageSelexMaleView->setHeight(current_fleet->getAgeSelectivity()->getMaleParameterModel()->getNumParams());
        ageSelexMaleView->resizeColumnsToContents();
        ageSelexTimeVaryParamsView->setModel(current_fleet->getAgeSelectivity()->getTimeVaryParameterModel());
        ageSelexTimeVaryParamsView->setHeight(current_fleet->getAgeSelectivity()->getNumTimeVaryParameters());
        ageSelexTimeVaryParamsView->resizeColumnsToContents();
        ui->spinBox_ar1->setValue(current_fleet->getAr1SelSmoother());

        setAgeLengthBins();
        selexSizeEqDialog->setFleet(current_fleet);
        selexAgeEqDialog->setFleet(current_fleet);
        selexSizeEqDialog->setMidBin(current_fleet->getSeasTiming());
        selexAgeEqDialog->setMidBin(0);
        selexSizeEqDialog->setSelex(current_fleet->getSizeSelectivity());
        selexAgeEqDialog->setSelex(current_fleet->getAgeSelectivity());

        ui->spinBox_lambda_max_phase->setValue(model_data->getLambdaMaxPhase());
        ui->spinBox_lambda_sd_offset->setValue(model_data->getLambdaSdOffset());
        ui->spinBox_lambda_num_changes->setValue(current_fleet->getNumLambdas());
        lambdaView->setModel(current_fleet->getLambdaModel());
        lambdaView->resizeColumnsToContents();

        connectFleet ();

    }
    ui->tabWidget_fleet->setCurrentIndex(tabset);
    }
}

void fleet_widget::set_fleet_type(int type)
{
    ui->groupBox_bycatch->setVisible(false);
    switch(type)
    {
    case 0:
        current_fleet->setType(Fleet::Fishing);
        break;
    case 1:
        current_fleet->setType(Fleet::Bycatch);
        ui->groupBox_bycatch->setVisible(true);
        ui->comboBox_byc_dead->setCurrentIndex(current_fleet->getBycatchDead() - 1);
        ui->comboBox_byc_f->setCurrentIndex(current_fleet->getBycatchF() - 1);
        ui->lineEdit_byc_firstYear->setText(current_fleet->getBycFirstYr());
        ui->lineEdit_byc_lastYear->setText(current_fleet->getBycLastYr());
        break;
    case 2:
        current_fleet->setType(Fleet::Survey);
        break;
    case 3:
    default:
        current_fleet->setType(Fleet::Predator);
    }
    model_data->assignFleetNumbers();
    refresh();
//    current_fleet->setType(Fleet::FleetType(type));
}

void fleet_widget::set_type_fleet(Fleet::FleetType ft)
{
    ui->groupBox_bycatch->setVisible(false);
    switch (ft)
    {
    case Fleet::Fishing:
        ui->comboBox_type->setCurrentIndex(0);
        break;
    case Fleet::Bycatch:
        ui->comboBox_type->setCurrentIndex(1);
        ui->groupBox_bycatch->setVisible(true);
        break;
    case Fleet::Survey:
        ui->comboBox_type->setCurrentIndex(2);
        break;
    case Fleet::Predator:
    default:
        ui->comboBox_type->setCurrentIndex(3);
        break;
    }
}

void fleet_widget::nextFleet()
{
/*    int index = ui->spinBox_num->value();
    if (index < ui->spinBox_total->value())
        ui->comboBox_fleet_name->setCurrentIndex(--index + 1);*/
}

void fleet_widget::prevFleet()
{
/*    int index = ui->spinBox_num->value();
    if (index > 0)
        ui->comboBox_fleet_name->setCurrentIndex(--index - 1);*/
}

void fleet_widget::create_new_fleet()
{
    new_fleet("New_Fleet");
    refresh();
}

void fleet_widget::duplicate_current_fleet()
{
    int index = ui->comboBox_fleet_name->currentIndex();
    duplicate_fleet(index);
    refresh();
}

void fleet_widget::delete_current_fleet()
{
    int index = ui->comboBox_fleet_name->currentIndex();
    delete_fleet(index);
    refresh();
}

void fleet_widget::new_fleet (QString name)
{
    model_data->addFleet(model_data->newFleet(name));
//    totalFleets++;
    totalFleets = model_data->get_num_fleets();
    set_current_fleet(totalFleets);
}

void fleet_widget::duplicate_fleet (int index)
{
    ui->comboBox_fleet_name->setCurrentIndex(index);
    Fleet *newfl, *oldfl = model_data->getFleet(index);
    QString newname(oldfl->getName());
    newname.prepend("Copy_of_");
    newfl = model_data->duplicateFleet(oldfl);
    newfl->setName(newname);
    totalFleets++;
    set_current_fleet(totalFleets);
}

void fleet_widget::delete_fleet(int index)
{
    if (totalFleets == 1)
    {
        model_data->getFleet(0)->reset();
        model_data->getFleet(0)->setName("newFleet");
    }
    else
    {
        if (index >= 0 &&
            index < totalFleets)
        {
            disconnectFleet();
            current_fleet = NULL;
            model_data->deleteFleet(index);
            totalFleets--;
        }
    }
    set_current_fleet(index - 1);
}

void fleet_widget::changeBycatchDead (int value)
{
    current_fleet->setBycatchDead(value + 1);
}

void fleet_widget::changeBycatchF (int value)
{
    current_fleet->setBycatchF(value + 1);
}

void fleet_widget::changeBycatchFirstYr ()
{
    QString year(ui->lineEdit_byc_firstYear->text());
    current_fleet->setBycFirstYr(year);
}

void fleet_widget::changeBycatchLastYr ()
{
    QString year(ui->lineEdit_byc_lastYear->text());
    current_fleet->setBycLastYr(year);
}


void fleet_widget::showLengthObs()
{
    ui->tabWidget_fleet->setCurrentIndex(2);
    ui->tabWidget_comp->setCurrentIndex(0);
}

void fleet_widget::changeLengthMinTailComp()
{
    current_fleet->setLengthMinTailComp(ui->lineEdit_length_comp_tails->text());
}

void fleet_widget::changeLengthAddToData()
{
    current_fleet->setLengthAddToData(ui->lineEdit_length_constant->text());
}

void fleet_widget::showAgeObs()
{
    ui->tabWidget_fleet->setCurrentIndex(2);
    ui->tabWidget_comp->setCurrentIndex(1);
}

void fleet_widget::changeAgeMinTailComp()
{
    current_fleet->setAgeMinTailComp(ui->lineEdit_age_comp_tails->text());
}

void fleet_widget::changeAgeAddToData()
{
    current_fleet->setAgeAddToData(ui->lineEdit_age_constant->text());
}

void fleet_widget::showMeanSAAObs()
{
    ui->tabWidget_fleet->setCurrentIndex(1);
    ui->tabWidget_obs->setCurrentIndex(4);
}

void fleet_widget::showGenSizeObs(int index)
{
    ui->tabWidget_fleet->setCurrentIndex(2);
    ui->tabWidget_comp->setCurrentIndex(2);
    ui->spinBox_obs_gen_num->setValue(index);
}

void fleet_widget::showRecapObs()
{
    ui->tabWidget_fleet->setCurrentIndex(1);
    ui->tabWidget_obs->setCurrentIndex(5);
}

void fleet_widget::showMorphObs()
{
    ui->tabWidget_fleet->setCurrentIndex(2);
    ui->tabWidget_comp->setCurrentIndex(3);
}

void fleet_widget::qSetupChanged()
{
    current_fleet->qSetupChanged();
    qSetupParamsChanged();
}

void fleet_widget::qSetupParamsChanged()
{
    int ht;
    ht = current_fleet->Q()->getNumParams();
    qParamsView->setHeight(ht);
    qParamsView->resizeColumnsToContents();
    qSetupTVParamsChanged();
}

void fleet_widget::qSetupTVParamsChanged()
{
    int ht;
    ht = current_fleet->Q()->getNumTVParams();
    qTvParamsView->setHeight(ht);
    qTvParamsView->resizeColumnsToContents();
    qTvParamsView->setVisible(ht);
    ui->label_q_tv_parameters->setVisible(ht);
}

void fleet_widget::setQTimeVaryReadParams(int flag)
{
    for (int i = 0; i < model_data->get_num_fleets(); i++)
        model_data->getFleet(i)->setQTimeVaryReadParams(flag);
    if (flag == 0)
    {
        qTvParamsView->setEnabled(false);
    }
    else
    {
        qTvParamsView->setEnabled(true);
    }
}

void fleet_widget::setSelTimeVaryReadParams(int flag)
{
    for (int i = 0; i < model_data->get_num_fleets(); i++)
        model_data->getFleet(i)->setSelTimeVaryReadParams(flag);
    if (flag == 0)
    {
        sizeSelexTimeVaryParamsView->setEnabled(false);
        ageSelexTimeVaryParamsView->setEnabled(false);
    }
    else
    {
        sizeSelexTimeVaryParamsView->setEnabled(true);
        ageSelexTimeVaryParamsView->setEnabled(true);
    }
}

void fleet_widget::changeSelexSizePattern(int pat)
{
    int oldNumParams = current_fleet->getSizeSelectivity()->getNumParameters();
    int newNumParams = 0;
    switch (pat)
    {
    case 0:
        ui->label_selex_size_pattern_info->setText(tr("Selex = 1.0"));
        break;
    case 1:
        ui->label_selex_size_pattern_info->setText(tr("Logistic"));
        break;
    case 5:
        ui->label_selex_size_pattern_info->setText(tr("Mirror another selex (Special) with limits"));
        break;
    case 6:
        ui->label_selex_size_pattern_info->setText(tr("Linear segments exp(y)"));
        break;
    case 8:
        ui->label_selex_size_pattern_info->setText(tr("Double logistic with smooth joins"));
        break;
    case 9:
        ui->label_selex_size_pattern_info->setText(tr("Simple double logistic"));
        break;
    case 15:
        ui->label_selex_size_pattern_info->setText(tr("Mirror another selex (Special)"));
        break;
    case 22:
        ui->label_selex_size_pattern_info->setText(tr("Double normal, similar to CASAL"));
        break;
    case 23:
        ui->label_selex_size_pattern_info->setText(tr("Similar to #24"));
        break;
    case 24:
        ui->label_selex_size_pattern_info->setText(tr("Double normal with start and end values"));
        break;
    case 25:
        ui->label_selex_size_pattern_info->setText(tr("Exponential - Logistic"));
        break;
    case 27:
        ui->label_selex_size_pattern_info->setText(tr("Cubic spline"));
        break;
    case 30:
        ui->label_selex_size_pattern_info->setText(tr("Set in Survey units: abundance is spawning biomass"));
        break;
    case 31:
        ui->label_selex_size_pattern_info->setText(tr("Set in Survey units: abundance is exp(recr dev)"));
        break;
    case 32:
        ui->label_selex_size_pattern_info->setText(tr("Set in Survey units: abundance is exp(recr dev) * spawn Biomass"));
        break;
    case 33:
        ui->label_selex_size_pattern_info->setText(tr("Set in Survey units: abundance is age 0 recr"));
        break;
    case 34:
        ui->label_selex_size_pattern_info->setText(tr("Set in Survey units: spawning biomass depletion"));
        break;
    case 2:
    case 7:
        ui->label_selex_size_pattern_info->setText(tr("Discontinued, use pattern #8."));
        return;
    case 3:
        ui->label_selex_size_pattern_info->setText(tr("Discontinued, select another pattern."));
        return;
    case 4:
        ui->label_selex_size_pattern_info->setText(tr("Discontinued, use pattern #30."));
        return;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 26:
    case 41:
        ui->label_selex_size_pattern_info->setText(tr("Used for age selectivity only."));
        return;
    case 21:
    case 28:
    case 29:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
        ui->label_selex_size_pattern_info->setText(tr("Not used, select another pattern."));
        return;
    case 42:
        ui->label_selex_size_pattern_info->setText(tr("Cubic spline with scaling"));
        break;
    case 43:
        ui->label_selex_size_pattern_info->setText(tr("Linear segments exp(y) with scaling"));
        break;
    default:
        ui->label_selex_size_pattern_info->setText(" ");
    }

    current_fleet->getSizeSelectivity()->setPattern(pat);
    newNumParams = current_fleet->getSizeSelectivity()->getNumParameters();
    for (int i = oldNumParams; i < newNumParams; i++)
    {
        QStringList prm;
        prm << "-5" << "5" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
        current_fleet->getSizeSelectivity()->setParameter(i, prm);
    }

    ui->spinBox_selex_size_pattern->setValue (pat);
    ui->spinBox_selex_size_num_params->setValue (newNumParams);
    sizeSelexParamsView->setHeight (newNumParams);
}

void fleet_widget::setAr1SelexSmoother(int val)
{
    for (int i = 0; i < model_data->get_num_fleets(); i++)
        model_data->getFleet(i)->setAr1SelSmoother(val);
}

void fleet_widget::setupSelexSizeInfo()
{
    QString title("Size Selectivity Pattern Numbers");
    QString msg ("Pattern\tNumParams\tDescription\n");
    msg.append("0\t0\tSelectivity = 1.0 for all sizes.\n");
    msg.append("1\t2\tLogistic.\n");
    msg.append("2\t8\tDiscontinued, use pattern #8.\n");
    msg.append("3\t0\tDiscontinued.\n");
    msg.append("4\t0\tDiscontinued, use special pattern #30.\n");
    msg.append("5\t2\tMirror another selectivity, special is source fleet.\n");
    msg.append("6\t2+\tLinear, special value is num of segments.\n");
    msg.append("7\t8\tDiscontinued, use pattern #8.\n");
    msg.append("8\t8\tDouble logistic with defined peak, smooth joiners.\n");
    msg.append("9\t6\tSimple double logistic, no defined peak.\n");
    msg.append("15\t0\tMirror another selex, special is source fleet.\n");
    msg.append("22\t4\tDouble normal, similar to CASAL.\n");
    msg.append("23\t6\tSimilar to #24.\n");
    msg.append("24\t6\tDouble normal with defined initial and final selex.\n");
    msg.append("25\t3\tExponential - logistic.\n");
    msg.append("27\t3+Specl\tCubic spline, 3 + Number of nodes parameters.\n");
    msg.append("30\t0\tExpected survey abundance equals spawning biomass.\n");
    msg.append("31\t0\tExpected survey abundance equals exp(recr dev).\n");
    msg.append("32\t0\tExpected survey abundance equals exp(recr dev) * SpawnBiomass.\n");
    msg.append("33\t0\tExpected survey abundance equals age 0 recruitment.\n");
    msg.append("34\t0\tSpawning biomass depletion (B/B0).\n");
    msg.append("42\t0\tCubic spline with user defined scaling.\n");
    msg.append("43\t0\tLinear segements with user defined scaling.\n");
    msg.append("");
    selexSizeInfoDialog->setWindowTitle(tr("Information"));
    selexSizeInfoDialog->setTitle(title);
    selexSizeInfoDialog->setText(msg);
//    /*int button = */QMessageBox::information(this, title, msg, QMessageBox::Ok);
}

void fleet_widget::changeSelexSizeDiscard(int disc)
{
    current_fleet->getSizeSelectivity()->setDiscard(disc);
    sizeSelexRetainView->setHeight(current_fleet->getSizeSelectivity()->getRetainParameterModel()->getNumParams());
    sizeSelexDiscardView->setHeight(current_fleet->getSizeSelectivity()->getDiscardParameterModel()->getNumParams());
}

void fleet_widget::changeSelexSizeMale(int mal)
{
    current_fleet->getSizeSelectivity()->setMale(mal);
    sizeSelexMaleView->setHeight(current_fleet->getSizeSelectivity()->getMaleParameterModel()->getNumParams());
}

void fleet_widget::changeSelexSizeSpecial(int spc)
{
    current_fleet->getSizeSelectivity()->setSpecial(spc);
}

void fleet_widget::sizeSelexParamsChanged()
{
    int ht;
    ht = current_fleet->getSizeSelectivity()->getNumParameters();
    sizeSelexParamsView->setHeight(ht);
    sizeSelexParamsView->resizeColumnsToContents();
}

void fleet_widget::sizeSelexTVParamsChanged()
{
    int ht;
    ht = current_fleet->getSizeSelectivity()->getNumTimeVaryParameters();
    sizeSelexTimeVaryParamsView->setHeight(ht);
    sizeSelexTimeVaryParamsView->resizeColumnsToContents();
    sizeSelexTimeVaryParamsView->setVisible(ht);
    ui->label_selex_size_timevary_parms->setVisible(ht);
}

void fleet_widget::showSelexSizeCurve(bool flag)
{
    selexSizeEqDialog->setVisible(flag);
    if (flag)
        selexSizeEqDialog->restoreAll();
}

void fleet_widget::selexSizeCurveClosed()
{
    ui->pushButton_selex_size_curve->setChecked(false);
}

void fleet_widget::ageSelexParamsChanged()
{
    ageSelexParamsView->setHeight (current_fleet->getAgeSelexModel());
    ageSelexParamsView->resizeColumnsToContents();
}

void fleet_widget::ageSelexTVParamsChanged()
{
    ageSelexTimeVaryParamsView->setHeight (
            current_fleet->getAgeSelectivity()->getTimeVaryParameterModel());
    ageSelexTimeVaryParamsView->resizeColumnsToContents();
}

void fleet_widget::changeSelexAgePattern(int pat)
{
    switch (pat)
    {
    case 0:
        ui->label_selex_age_pattern_info->setText(tr("Selex = 1, ages 1-n"));
        break;
    case 10:
        ui->label_selex_age_pattern_info->setText(tr("Selex = 1, ages 2-n"));
        break;
    case 11:
        ui->label_selex_age_pattern_info->setText(tr("Selex = 1 between ages"));
        break;
    case 12:
        ui->label_selex_age_pattern_info->setText(tr("Logistic"));
        break;
    case 13:
        ui->label_selex_age_pattern_info->setText(tr("Double Logistic with IF joins"));
        break;
    case 14:
        ui->label_selex_age_pattern_info->setText(tr("Selex = 1/(1+exp(-param))"));
        break;
    case 15:
        ui->label_selex_age_pattern_info->setText(tr("Mirror another selex (Special)"));
        break;
    case 16:
        ui->label_selex_age_pattern_info->setText(tr("Coleraine single Gaussian"));
        break;
    case 17:
        ui->label_selex_age_pattern_info->setText(tr("Random Walk"));
        break;
    case 18:
        ui->label_selex_age_pattern_info->setText(tr("Double Logistic with smooth joins"));
        break;
    case 19:
        ui->label_selex_age_pattern_info->setText(tr("Simple Double Logistic"));
        break;
    case 20:
        ui->label_selex_age_pattern_info->setText(tr("Double Normal with start and end values"));
        break;
    case 26:
        ui->label_selex_age_pattern_info->setText(tr("Exponential - Logistic"));
        break;
    case 27:
        ui->label_selex_size_pattern_info->setText(tr("Cubic spline"));
        break;
    case 21:
    case 28:
    case 29:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
        ui->label_selex_age_pattern_info->setText(tr("Not used, select another pattern."));
        return;
    case 22:
    case 23:
    case 24:
    case 25:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 43:
        ui->label_selex_age_pattern_info->setText(tr("Used for size selectivity only."));
        return;
    case 41:
        ui->label_selex_age_pattern_info->setText(tr("Random Walk with scaling"));
        break;
    case 42:
        ui->label_selex_size_pattern_info->setText(tr("Cubic spline with scaling"));
        break;
    default:
        ui->label_selex_age_pattern_info->setText(tr(" "));
    }
    current_fleet->getAgeSelectivity()->setPattern(pat);
    ui->spinBox_selex_age_pattern->setValue
            (current_fleet->getAgeSelectivity()->getPattern());
    ui->spinBox_selex_age_num_params->setValue
            (current_fleet->getAgeSelectivity()->getNumParameters());
    ageSelexParamsView->setHeight(ui->spinBox_selex_age_num_params->value());
}

void fleet_widget::showSelexAgeCurve(bool flag)
{
    selexAgeEqDialog->setVisible(flag);
    if (flag)
        selexAgeEqDialog->restoreAll();
}

void fleet_widget::selexAgeCurveClosed()
{
    ui->pushButton_selex_age_curve->setChecked(false);
}

void fleet_widget::changeSelexAgeMale(int mal)
{
    int ht;
    current_fleet->getAgeSelectivity()->setMale(mal);
    ht = current_fleet->getAgeSelectivity()->getMaleParameterModel()->getNumParams();
    ageSelexMaleView->setHeight(ht);
    ageSelexMaleView->resizeColumnsToContents();
}

void fleet_widget::changeSelexAgeSpecial(int spc)
{
    current_fleet->getAgeSelectivity()->setSpecial(spc);
}

void fleet_widget::setupSelexAgeInfo()
{
    QString title("Age Selectivity Pattern Numbers");
    QString msg ("Pattern\tNumParams\tDescription\n");
    msg.append("10\t0\tSelectivity = 1.0 for all ages, age 1+.\n");
    msg.append("11\t2\tChoose minimum and maximum ages.\n");
    msg.append("12\t2\tLogistic.\n");
    msg.append("13\t8\tDouble logistic, IF joiners, Discouraged, use pattern #18.\n");
    msg.append("14\tNages+1\tEach age, value at age is 1/(1+exp(-x)).\n");
    msg.append("15\t0\tMirror another selectivity, special is source fleet.\n");
    msg.append("16\t2\tColeraine single Gaussian.\n");
    msg.append("17\tNages+1\tEach age as random walk.\n");
    msg.append("18\t8\tDouble logistic with defined peak, smooth joiners.\n");
    msg.append("19\t6\tSimple double logistic, no defined peak.\n");
    msg.append("20\t6\tDouble normal with defined init and final level.\n");
    msg.append("26\t3\tExponential - logistic.\n");
    msg.append("41\t2+Nages+1\tRandom walk with user defined scaling.\n");
    msg.append("42\t2+3+Specl\tCubic spline with user defined scaling.\n");
    msg.append("");
    selexAgeInfoDialog->setWindowTitle(tr("Information"));
    selexAgeInfoDialog->setTitle(title);
    selexAgeInfoDialog->setText(msg);
//    QMessageBox::information(this, title, msg);
}

void fleet_widget::setGenMethodTotal(int num)
{
    ui->spinBox_obs_gen_num_total->setValue(num);
}

void fleet_widget::changeGenMethodTotal(int num)
{

    if (num > 0)
    {
        setGenMethodNum(0);
        ui->spinBox_obs_gen_numObs->setVisible(true);
        genObsView->setVisible(true);
    }
    else
    {
        ui->spinBox_obs_gen_numObs->setVisible(true);
        genObsView->setVisible(true);
    }

}

void fleet_widget::setGenMethodNum(int num)
{
    if (num < 0)
        num = 0;
    if (num >= ui->spinBox_obs_gen_num_total->value())
        num = ui->spinBox_obs_gen_num_total->value() - 1;
    changeGenMethodNum(num + 1);
}

void fleet_widget::changeGenMethodNum(int num)
{
    int totnum = ui->spinBox_obs_gen_num_total->value();
    cur_gen_obs = num - 1;
    if (totnum < 1)
        ui->spinBox_obs_gen_num->setValue(0);
    else
    {
        if (num < 1)
            ui->spinBox_obs_gen_num->setValue(1);
        else if (num > totnum)
            ui->spinBox_obs_gen_num->setValue(totnum);
        else
        {
            ui->spinBox_obs_gen_numObs->setValue(current_fleet->getGenNumObs(cur_gen_obs));
            genObsView->setModel(current_fleet->getGenModel(cur_gen_obs));
        }
    }
}

void fleet_widget::setGenNumObs(int num)
{
    if (num < 0) num = 0;
    ui->spinBox_obs_gen_numObs->setValue(num);
}

void fleet_widget::changeGenNumObs(int num)
{
    if (ui->spinBox_obs_gen_num_total->value() < 1)
    {
        ui->spinBox_obs_gen_numObs->setValue(0);
    }
    else
    {
        current_fleet->setGenNumObs(cur_gen_obs, num);
    }
}


