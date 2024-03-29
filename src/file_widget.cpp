#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

#include <cmath>
using namespace std;


#include "file_info_dialog.h"
#include "fleet.h"
#include "block_pattern.h"
#include "growth.h"
#include "file_widget.h"
//#include "fileIO32.h"
#include "fileIO33.h"
#include "ui_file_widget.h"

//#define DEBUG_FILES

file_widget::file_widget(ss_model *mod, QString dir, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file_widget)
{
    ui->setupUi(this);

    current_dir_name = QString(dir);
//    TODO: This is for debugging only, make sure to set them to false before building release.
    {
    bool check = false;
    ui->label_version->setVisible(check);
    ui->doubleSpinBox_version->setVisible(check);
    }

    current_dir = QDir(dir);

    setSsoutDir(QString());
    setSsnewDir(QString());

    starterFile = new ss_file(QString("%1/%2").arg(dir, STARTER_FILE), this);
    forecastFile = new ss_file(QString("%1/%2").arg(dir, FORECAST_FILE), this);
    dataFile = new ss_file(QString("%1/%2").arg(dir, DATA_FILE), this);
    controlFile = new ss_file(QString("%1/%2").arg(dir, CONTROL_FILE), this);
    runNumberFile = new ss_file(QString("%1/%2").arg(dir, RUN_NUMBER_FILE), this);
    parameterFile = new ss_file(QString("%1/%2").arg(dir, PARAMETER_FILE), this);
    profileFile = new ss_file(QString("%1/%2").arg(dir, PROFILE_VAL_FILE), this);
    userDataFile = nullptr;
    chooseRepDetail = new DialogChoseReport(this);
    connect (chooseRepDetail, SIGNAL(detailChanged(int)), SLOT(changeReportDetail(int)));
    chooseRepDetail->hide();
//    connect (starterFile, SIGNAL(end_reading_file()), SLOT(reset()));
//    connect (forecastFile, SIGNAL(end_reading_file()), SLOT(reset()));
//    connect (dataFile, SIGNAL(end_reading_file()), SLOT(reset()));
//    connect (controlFile, SIGNAL(end_reading_file()), SLOT(reset()));
//    connect (parameterFile, SIGNAL(end_reading_file()), SLOT(reset()));

    model_info = mod;

    setDatafileVersion(3.30, false);  // only version 3.3.0.
    ui->label_note_check_value->setVisible(false);
    ui->label_check_value->setVisible(false);
    ui->label_check_value_info->setVisible(false);

    connect (ui->toolButton_file_read, SIGNAL(clicked()), SIGNAL(read_all_files()));
    connect (ui->toolButton_control_file_new, SIGNAL(clicked()), SIGNAL(choose_control_file()));
    connect (ui->toolButton_control_file_save_as, SIGNAL(clicked()), SIGNAL(save_control_file()));
    connect (ui->toolButton_data_file_new, SIGNAL(clicked()), SIGNAL(choose_data_file()));
    connect (ui->toolButton_data_file_save_as, SIGNAL(clicked()), SIGNAL(save_data_file()));

    connect (ui->pushButton_starter_file, SIGNAL(clicked()), SLOT(show_starter_file_info()));
    connect (ui->pushButton_fcast_file, SIGNAL(clicked()), SLOT(show_forecast_file_info()));
    connect (ui->pushButton_data_file, SIGNAL(clicked()), SLOT(show_data_file_info()));
    connect (ui->pushButton_control_file, SIGNAL(clicked()), SLOT(show_control_file_info()));
    connect (ui->pushButton_viewParFile, SIGNAL(clicked()), SLOT(view_param_file()));
    connect (ui->pushButton_pro_file, SIGNAL(clicked()), SLOT(show_prof_file_info()));
    connect (ui->spinBox_wtatage, SIGNAL(valueChanged(int)), SLOT(changeReadWtAtAge(int)));
    connect (ui->pushButton_wtatage, SIGNAL(clicked()), SLOT(view_wtatage()));

//    connect (ui->checkBox_parm_trace, SIGNAL(toggled(bool)), SLOT(parm_trace_changed(bool)));
    connect (ui->checkBox_parm_trace, SIGNAL(toggled(bool)), ui->comboBox_parm_trace_iter, SLOT(setEnabled(bool)));
    connect (ui->checkBox_parm_trace, SIGNAL(toggled(bool)), ui->comboBox_parm_trace_param, SLOT(setEnabled(bool)));

//    connect (ui->checkBox_cumrpt_fits, SIGNAL(toggled(bool)), SLOT(cumrpt_fits_changed(bool)));
//    connect (ui->checkBox_cumrpt_like, SIGNAL(toggled(bool)), SLOT(cumrpt_like_changed(bool)));

    ui->spinBox_datafiles->setMinimum(0);
    ui->spinBox_datafiles->setMaximum(10);

    connect (ui->checkBox_pro_file, SIGNAL(toggled(bool)), SLOT(changeReadProFile(bool)));
    connect (ui->checkBox_par_file, SIGNAL(toggled(bool)), SLOT(changeReadParFile(bool)));
    connect (ui->pushButton_runnum_file, SIGNAL(clicked()), SLOT(reset_run_num()));

    connect (ui->doubleSpinBox_version, SIGNAL(valueChanged(double)), SLOT(setDatafileVersion(double)));
    ui->label_version->setVisible(false);
    ui->doubleSpinBox_version->setVisible(false);

    connect (ui->checkBox_output_sso, SIGNAL(toggled(bool)), SLOT(changeOutputDirectory(bool)));
    connect (ui->checkBox_output_ssnew, SIGNAL(toggled(bool)), SLOT(changeNewDirectory(bool)));
    connect (ui->comboBox_report_level, SIGNAL(currentIndexChanged(int)), SLOT(reportDetailChanged(int)));
    connect (ui->pushButton_chooseRepDetail, SIGNAL(clicked()), SLOT(chooseReportDetail()));

    setReadParFile(false);
    setReadProFile(false);

    show_input_files();
    if (current_dir_name.isEmpty())
        current_dir = QDir (qApp->applicationDirPath());
    current_dir_name = current_dir.absolutePath();
    data_file_name = QString (DATA_FILE);
    control_file_name = QString (CONTROL_FILE);
    set_default_file_names (current_dir_name, false);
    viewer = new Dialog_fileView(this);
}

void file_widget::reset()
{
    setReadParFile(false);
    ui->comboBox_detail_level->setCurrentIndex(1);
    ui->comboBox_report_level->setCurrentIndex(0);
    ui->checkBox_checkup->setChecked(false);
    set_parmtr_write(0);
    ui->comboBox_cumreport->setCurrentIndex(1);
    setReadProFile(false);
    ui->spinBox_datafiles->setValue(0);
//    viewer->viewFile(QString(""));
    setReadWtAtAge(model_info->getReadWtAtAge());
}

file_widget::~file_widget()
{
    delete viewer;
    delete starterFile;
    delete dataFile;
    delete forecastFile;
    delete controlFile;
    delete runNumberFile;
    delete parameterFile;
    delete profileFile;
    delete userDataFile;

    delete ui;
}

QString file_widget::getDatafileVersion()
{
    int major = (int)(datafile_version * 100.0);
    QString ver(QString("%1.0.0").arg(QString::number(major)));
    return ver;
}

void file_widget::setDatafileVersion(double ver, bool flag)
{
    QString msg;
    Q_UNUSED(flag);
    datafile_version = ver + .000001;
    if (datafile_version < 3.30)
    {
        model_info->setReadMonths(false);
    }
    else
    {
        model_info->setReadMonths(true);
    }
    setDatafileVersionString(datafile_version);
}

void file_widget::show_input_files()
{
    ui->tabWidget->setCurrentIndex (0);
}

void file_widget::show_output_files()
{
    ui->tabWidget->setCurrentIndex (1);
}

void file_widget::set_starter_file(QString fname, bool keep)
{
    Q_UNUSED(keep);
    ui->label_starter_file->setText(fname);
    current_dir_name = current_dir.absoluteFilePath(fname);
    current_dir.cd(current_dir_name);
    current_dir_name = current_dir.absolutePath();

    if (starterFile == nullptr)
    {
        starterFile = new ss_file(fname);
    }
    else if (starterFile->fileName().compare(fname, Qt::CaseSensitive))
    {
        starterFile->setFileName(fname);
    }
}


QString file_widget::get_starter_file()
{
    return ui->label_starter_file->text();
}

void file_widget::set_forecast_file(QString fname, bool keep)
{
    Q_UNUSED(keep);
    ui->label_fcast_file->setText(fname);

    if (forecastFile == nullptr)
    {
        forecastFile = new ss_file(fname);
    }
    else if (forecastFile->fileName().compare(fname, Qt::CaseSensitive))
    {
        forecastFile->setFileName(fname);
    }
}

QString file_widget::get_forecast_file ()
{
    return ui->label_fcast_file->text();
}

void file_widget::set_control_file(QString fname, bool keep)
{
    Q_UNUSED(keep);
    if (controlFile == nullptr)
    {
        controlFile = new ss_file(fname);
    }
    else if (controlFile->fileName().compare(fname, Qt::CaseSensitive))
    {
        controlFile->setFileName(fname);
        control_file_name = controlFile->getShortFileName();

        if (fname.contains('/'))
            control_file_name = fname.section('/', -1, -1);
        else
            control_file_name = fname.section('\\', -1, -1);
        ui->label_control_file->setText(fname);
    }
}

QString file_widget::get_control_file()
{
    return ui->label_control_file->text();
}

void file_widget::set_data_file(QString fname, bool keep)
{
    Q_UNUSED(keep);
    if (dataFile == nullptr)
    {
        dataFile = new ss_file(fname);
    }
    else if (dataFile->fileName().compare(fname, Qt::CaseSensitive))
    {
        dataFile->setFileName(fname);
        data_file_name = dataFile->getShortFileName();
//        data_file_name = dataFile->fileName();
//        data_file_name = fname.section('/', -1, -1);
//        data_file_name = data_file_name.section('\\', -1, -1);

        ui->label_data_file->setText(dataFile->fileName());
        ui->label_data_file->repaint();
    }
}

QString file_widget::get_data_file()
{
    return ui->label_data_file->text();
}


QString file_widget::get_run_num_file()
{
    return QString("RunNumber.SS");
}

void file_widget::setReadParFile(bool flag)
{
    ui->checkBox_par_file->setChecked(flag);
    changeReadParFile(flag);
}

void file_widget::changeReadParFile(bool flag)
{
    if (flag)
    {
        QString name(QString("%1/%2").arg(current_dir_name, QString(PARAMETER_FILE)));
        parameterFile->setFileName(name);
        int btn = 0;
        if (!parameterFile->exists()) {
            btn = QMessageBox::information(this,
                 tr("File information"),
                 tr(QString("File %1 does not exist, do still want to set read to true?").arg(name).toUtf8().data()),
                 QMessageBox::Yes, QMessageBox::No);
        }
        if (btn == QMessageBox::No) {
            ui->checkBox_par_file->setChecked(false);
        }
    }
    ui->pushButton_viewParFile->setVisible(ui->checkBox_par_file->isChecked());
}

bool file_widget::getReadParFile()
{
    return ui->checkBox_par_file->isChecked();
}

QString file_widget::get_param_file()
{
    return QString(PARAMETER_FILE);
}

/*void file_widget::set_par_file(QString fname, bool keep)
{
    if (fname.isEmpty())
    {
        set_par_file(false);
    }
    else
    {
        if (parameterFile == nullptr)
        {
            parameterFile = new ss_file(fname);
        }
        else if (fname != parameterFile->fileName())
        {
            parameterFile->setFileName(fname);
        }
        set_par_file(true);
    }
}*/

QString file_widget::get_profile_file()
{
    return ui->label_profile_file->text();
}

void file_widget::setReadProFile(bool flag)
{
    ui->checkBox_pro_file->setChecked(flag);
    changeReadProFile(flag);
}

bool file_widget::getReadProFile()
{
    return ui->checkBox_pro_file->isChecked();
}

void file_widget::changeReadProFile(bool flag)
{
    ui->label_profile_file->setVisible(flag);
    ui->pushButton_pro_file->setVisible(flag);
}

void file_widget::setReadProFile(QString fname, bool keep)
{
    QStringList commnts;
    Q_UNUSED(keep);
    if (fname.isEmpty())
    {
        setReadProFile(false);
    }
    else
    {
        if (profileFile == nullptr)
        {
            profileFile = new ss_file(fname);
        }
        else if (fname != profileFile->fileName())
        {
            profileFile->setFileName(fname);
        }
        ui->label_profile_file->setText(fname);
        setReadProFile(true);
    }
}

void file_widget::setReadWtAtAge(int flag)
{
    ui->label_wtatage_file->setVisible(flag);
    ui->label_wtatage_info->setVisible(flag);
    ui->pushButton_wtatage->setVisible(flag);
    ui->spinBox_wtatage->setValue(flag);
}

void file_widget::changeReadWtAtAge(int flag)
{
    ui->label_wtatage_file->setVisible(flag);
    ui->label_wtatage_info->setVisible(flag);
    ui->pushButton_wtatage->setVisible(flag);
    model_info->setReadWtAtAge(flag);
}

float file_widget::get_version_number(QString token)
{
    bool okay;
    float ver = 0.00;
    ver = token.toFloat(&okay);
    if (!okay)
    {
        QStringList nums = token.split('.', QString::SkipEmptyParts);
        ver = nums.at(0).toFloat(&okay);
        if (okay)
            ver += nums.at(1).toFloat()/10.0;
        else
            ver = 2.4;
    }
    return ver + .0000005;
}

void file_widget::parm_trace_changed(bool flag)
{
    ui->comboBox_parm_trace_iter->setEnabled(flag);
    ui->comboBox_parm_trace_param->setEnabled(flag);
/*    if (flag)
    {
        ui->comboBox_parm_trace_iter->show();
        ui->comboBox_parm_trace_param->show();
    }
    else
    {
        ui->comboBox_parm_trace_iter->hide();
        ui->comboBox_parm_trace_param->hide();
    }*/
}

/*void file_widget::cumrpt_fits_changed(bool flag)
{
    if (flag)
        ui->checkBox_cumrpt_like->setChecked(flag);
}

void file_widget::cumrpt_like_changed(bool flag)
{
    if (!flag)
        ui->checkBox_cumrpt_fits->setChecked(flag);
}*/

void file_widget::increase_font ()
{
    QString f_fam = font().family();
    int f_size = font().pointSize();
    if (f_size < 24)
    {
        f_size += 2;
        setFont(QFont(f_fam, f_size));
    }
}

void file_widget::decrease_font()
{
    QString f_fam = font().family();
    int f_size = font().pointSize();
    if (f_size > 12)
    {
        f_size -= 2;
        setFont(QFont(f_fam, f_size));
    }
}

void file_widget::updateOutputFiles(QString curdir)
{
    ui->label_comp_report_file->setText(QString("%1%2/%3").arg(curdir, ssoutDir, QString(COMP_REPORT_FILE)));
    ui->label_cum_report_file->setText(QString("%1%2/%3").arg(curdir, ssoutDir, QString(CUM_REPORT_FILE)));
    ui->label_fcast_report_file->setText(QString("%1%2/%3").arg(curdir, ssoutDir, QString(FCAST_REPORT_FILE)));
    ui->label_report_file->setText(QString("%1%2/%3").arg(curdir, ssoutDir, QString(REPORT_FILE)));
}

void file_widget::set_default_file_names(QString dir, bool keep)
{
    // standard input file names
    set_starter_file(QString("%1/%2").arg(dir, QString(STARTER_FILE)), keep);
    set_forecast_file(QString("%1/%2").arg(dir, QString(FORECAST_FILE)), keep);
    set_data_file(QString("%1/%2").arg(dir, QString(data_file_name)), keep);
    set_control_file(QString("%1/%2").arg(dir, QString(control_file_name)), keep);
//    set_par_file(QString("%1/%2").arg(dir, QString(PARAMETER_FILE)));
    setReadProFile(QString("%1/%2").arg(dir, QString(PROFILE_VAL_FILE)));

    updateOutputFiles(dir);

    ui->label_wtatage_file->setText(QString("%1/%2").arg(dir, QString(WTATAGE_FILE)));
    QFileInfo qfi(ui->label_wtatage_file->text());
    if (qfi.exists() && qfi.size() > 0)
    {
        ui->pushButton_wtatage->show();
        ui->label_wtatage_file->show();
    }
    else
    {
        ui->pushButton_wtatage->hide();
        ui->label_wtatage_file->hide();
    }
    // reset defaults
    setReadParFile(false);
    setReadProFile(false);

//    parm_trace_changed(false);
    show_input_files();
    // our plucky error file
#ifdef DEBUG
    error->close();
    error->setFileName(QString("%1/%2").arg(dir, QString(ERROR_FILE)));
    error->open(QIODevice::WriteOnly);
#endif
}

void file_widget::new_directory(QString dir, bool keep)
{
    current_dir.cd(dir);
    current_dir_name = current_dir.absolutePath();
    ui->label_current_dir->setText(dir);
    set_default_file_names (dir, keep);
}

void file_widget::set_parmtr_write(int flag)
{
    ui->comboBox_parm_trace_iter->setEnabled(true);
    ui->comboBox_parm_trace_param->setEnabled(true);
    switch (flag)
    {
    case 0:
        ui->checkBox_parm_trace->setChecked(false);
        ui->comboBox_parm_trace_iter->setEnabled(false);
        ui->comboBox_parm_trace_param->setEnabled(false);
        break;
    case 1:
        ui->checkBox_parm_trace->setChecked(true);
        ui->comboBox_parm_trace_iter->setCurrentIndex(0);
        ui->comboBox_parm_trace_param->setCurrentIndex(0);
        break;
    case 2:
        ui->checkBox_parm_trace->setChecked(true);
        ui->comboBox_parm_trace_iter->setCurrentIndex(0);
        ui->comboBox_parm_trace_param->setCurrentIndex(1);
        break;
    case 3:
        ui->checkBox_parm_trace->setChecked(true);
        ui->comboBox_parm_trace_iter->setCurrentIndex(1);
        ui->comboBox_parm_trace_param->setCurrentIndex(1);
        break;
    case 4:
        ui->checkBox_parm_trace->setChecked(true);
        ui->comboBox_parm_trace_iter->setCurrentIndex(1);
        ui->comboBox_parm_trace_param->setCurrentIndex(0);
        break;
    }
}

int file_widget::get_parmtr_write()
{
    int flag, ret;

    flag = ui->checkBox_parm_trace->isChecked()? 1: 0;
    flag += ui->comboBox_parm_trace_iter->currentIndex() * 2;
    flag += ui->comboBox_parm_trace_param->currentIndex() * 4;

    switch (flag)
    {
    case 1:
        ret = 1;
        break;
    case 3:
        ret = 4;
        break;
    case 5:
        ret = 2;
        break;
    case 7:
        ret = 3;
        break;
    case 0:
    default:
        ret = 0;
    }
    return ret;
}
/*
void file_widget::set_cumrpt_write(int flag)
{
    switch (flag)
    {
    case 0:
        ui->checkBox_cumrpt_like->setChecked(false);
        ui->checkBox_cumrpt_fits->setChecked(false);
        break;
    case 1:
        ui->checkBox_cumrpt_like->setChecked(true);
        ui->checkBox_cumrpt_fits->setChecked(false);
        break;
    case 2:
        ui->checkBox_cumrpt_like->setChecked(true);
        ui->checkBox_cumrpt_fits->setChecked(true);
        break;
    }
}

int file_widget::get_cumrpt_write()
{
    int ret = 0;
    if (ui->checkBox_cumrpt_like->isChecked())
        ret = 1;
    if (ui->checkBox_cumrpt_fits->isChecked())
        ret = 2;
    return ret;
}
*/
bool file_widget::read_files()
{
    if (model_info == nullptr)
        model_info = new ss_model((QWidget*)this->parent());
    else
        model_info->reset();
    return read_files(model_info);
}

bool file_widget::read_files(ss_model *model_inf)
{
    bool okay = true;
#ifdef DEBUG
    QMessageBox::information(this, "Information - program flow", "Begin reading files.");
#endif
    if (model_info == nullptr)
        model_info = model_inf;
    if (model_info == nullptr)
        okay = false;
    if (okay)
    {
        if (!starterFile->exists()) {
            okay = error_no_file(starterFile);
            if (starterFile->exists()) {
                new_directory(starterFile->getDirectory());
                okay = true;
            }
        }
        if (okay && !starterFile->open(QIODevice::ReadOnly)) {
            okay = false;
            error_unreadable(starterFile->fileName());
        }
        else if (okay) {
            starterFile->close();
            okay = read_starter_file();
        }
    }

    if (okay)
    {
#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - program flow", "Starter file read okay.");
#endif
        setDatafileVersion (datafile_version);
        if (datafile_version < 3.30)
        {
            okay = false;
        }
        else if (datafile_version < 3.40)
        {
            okay = read33_dataFile(dataFile, model_info);
            if (okay) {
#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - program flow", "Data file read okay.");
#endif
                okay = read33_forecastFile(forecastFile, model_info);
            }
            if (okay) {
#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - program flow", "Forecast file read okay.");
#endif
                okay = read33_controlFile(controlFile, model_info);
            }
            if (okay && ui->checkBox_par_file->isChecked())
            {
#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - program flow", "Control file read okay.");
#endif
                okay = read33_parameterFile(parameterFile, model_info);
            }
            if (okay && ui->checkBox_pro_file->isChecked())
            {
                okay = read33_profileFile(profileFile, model_info);
            }
        }
        if (okay)
        {
            read_run_num_file(QString ("%1/%2").arg
                           (current_dir_name, QString(RUN_NUMBER_FILE)));
        }
    }

    return okay;
}


void file_widget::write_files()
{
    // only write ver 3.30 files
    setDatafileVersion(3.30, false);

    write_starter_file(ui->label_starter_file->text());

    if (datafile_version < 3.40)
    {
        write33_dataFile(dataFile, model_info);
        write33_forecastFile(forecastFile, model_info);
        write33_controlFile(controlFile, model_info);
    /*    if (ui->checkBox_par_file->isChecked())
        {
            write33_parameterFile(parameterFile, model_info);
        }*/
        if (ui->checkBox_pro_file->isChecked())
        {
            reset_run_num();
            write33_profileFile(profileFile, model_info);
        }
    }

    write_run_num_file();
}

void file_widget::print_files()
{

}

bool file_widget::read_starter_file (QString filename)
{
    bool okay = true;
    QString token;

#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - Checking Starter file name.");
#endif
    if (filename.isEmpty())
    {
        filename = ui->label_starter_file->text();
    }
    if (starterFile != nullptr) {
        if (starterFile->fileName() != filename) {
            delete starterFile;
            starterFile = nullptr;
        }
    }
    if (starterFile == nullptr)
        starterFile = new ss_file (filename, this);

    if (!starterFile->exists()) {
        okay = error_no_file(starterFile);
    }
    if (okay) {
        current_dir_name = starterFile->getDirectory();
        current_dir = QDir(current_dir_name);
        emit directory_changed(current_dir_name);
        read_starter_file(starterFile);
    }
    return okay;
}

bool file_widget::read_starter_file(ss_file *starter)
{
    bool okay = true;
    bool end = false;
    QString token;
    float temp_float;
    int temp_int = 0;

    if (starterFile == nullptr) {
        starterFile = starter;
    }
    else if (starterFile != starter) {
        delete starterFile;
        starterFile = starter;
    }
    okay = starterFile->open(QIODevice::ReadOnly);

#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - Reading Starter", "Start.");
#endif

    if(okay)
    {
        starterFile->setOkay(true);
        starterFile->seek(0);
        starterFile->resetLineNum();
        starterFile->read_comments();

        if (starterFile->getOkay() && !starterFile->getStop()) {
        token = starterFile->get_next_value("data file");
        data_file_name = token;
        set_data_file(QString("%1/%2").arg(current_dir_name, token));
        token = starterFile->get_next_value("control file");
        control_file_name = token;
        set_control_file(QString("%1/%2").arg(current_dir_name, token));

        temp_int = starterFile->getIntValue(QString("Read ss.par choice"), 0, 1, 0);
        ui->checkBox_par_file->setChecked(temp_int != 0);
        }
#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - Read Starter", "File names read.");
#endif
        if (starterFile->getOkay() && !starterFile->getStop()) {
        temp_int = starterFile->getIntValue(QString("run display detail"), 0, 2, 1);
        ui->comboBox_detail_level->setCurrentIndex(temp_int);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        temp_int = starterFile->getIntValue(QString("detailed age-structured reports in REPORT.SSO"), 0, 3, 1);
        ui->comboBox_report_level->setCurrentIndex(temp_int);
        if (temp_int == 3) {
            // read custom reports
            temp_int = starterFile->get_next_value().toInt();
            while (temp_int != -999) {
                starterFile->checkIntValue(temp_int, QString("Custom report value"), -102, 60, 1);
                chooseRepDetail->setCustomReport(temp_int);
                temp_int = starterFile->get_next_value().toInt();
            }
        }
        chooseRepDetail->hide();
        temp_int = starterFile->getIntValue(QString("write check up data to  echoInput.sso"), 0, 1, 1);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        ui->checkBox_checkup->setChecked(temp_int);
        temp_int = starterFile->getIntValue(QString("what to write to ParmTrace.sso"), 0, 4, 1);
        set_parmtr_write(temp_int);
        }


        if (starterFile->getOkay() && !starterFile->getStop()) {
        temp_int = starterFile->getIntValue(QString("what to write to CumReport.sso"), 0, 2, 1);
        ui->comboBox_cumreport->setCurrentIndex(temp_int);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
#ifdef DEBUG_FILES
    QMessageBox::information(this, "Information - Read Starter", "Reports set.");
#endif
        temp_int = starterFile->getIntValue(QString("calculate all prior likelihoods"), 0, 1, 1);
        model_info->set_prior_likelihood (temp_int);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        temp_int = starterFile->getIntValue(QString("use soft bounds"), 0, 1, 1);
        model_info->set_use_softbounds(temp_int);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        temp_int = starterFile->getIntValue(QString("number of datafiles"), 0, 100, 0);
        ui->spinBox_datafiles->setValue(temp_int);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        temp_int = starterFile->getIntValue(QString("last estimation phase"), -1, 100, 8);
        model_info->set_last_estim_phase(temp_int);
        temp_int = starterFile->get_next_value("MCMC burn interval").toInt(); //getIntValue(QString("MC burn interval"), 0, 10000, 10);
        temp_int =  (temp_int < 0)? 0: temp_int;
        model_info->set_mc_burn(temp_int);
        temp_int = starterFile->get_next_value("MCMC thin interval").toInt(); //getIntValue(QString("MC thin interval"), 1, 1000, 2);
        temp_int = temp_int < 1? 1: temp_int;
        model_info->set_mc_thin(temp_int);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        temp_float = starterFile->getFloatValue(QString("jitter value"), 0.0, 11.0, 0.0);
        model_info->set_jitter_param(temp_float);
        token = starterFile->get_next_value("std dev report begin year");
        temp_int = token.toInt();
        model_info->set_bio_sd_min_year(temp_int);
        token = starterFile->get_next_value("std dev report end year");
        temp_int = token.toInt();
        model_info->set_bio_sd_max_year(temp_int);
        token = starterFile->get_next_value("extra sd report years");
        temp_int = token.toInt();
        model_info->set_num_std_years(temp_int);
        for (int i = 0; i < model_info->get_num_std_years(); i++) // vector of year values
        {
            token = starterFile->get_next_value("sd year");
            model_info->set_std_year(i, token);
        }
        starterFile->skip_line();

        }
        temp_float = starterFile->getFloatValue(QString("final convergence criteria"), 0.0, 0.1, 0.0001);
        if (starterFile->getOkay() && !starterFile->getStop()) {
        model_info->set_convergence_criteria(temp_float);
        token = starterFile->get_next_value("retrospective year");
        temp_int = token.toInt();
        model_info->set_retrospect_year(temp_int);
        token = starterFile->get_next_value("summary biomass min age");
        temp_int = token.toInt();
        model_info->set_biomass_min_age(temp_int);
        temp_int = starterFile->getIntValue(QString("depletion basis"), 0, 105, 1);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        model_info->set_depletion_basis(temp_int);
        token = starterFile->get_next_value("depletion denominator");
        model_info->set_depletion_denom(token.toDouble());
        temp_int = starterFile->getIntValue(QString("SPR report basis"), 0, 4, 1);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        model_info->set_spr_basis(temp_int);
        temp_int = starterFile->getIntValue(QString("F std report value"), 0, 5, 1);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        model_info->set_f_units(temp_int);
        // min and max age over which average F will be calculated with F_reporting=4 or 5
        if (model_info->get_f_units() >= 4)
        {
            token = starterFile->get_next_value("F min age");
            temp_int = token.toInt();
            model_info->set_f_min_age(temp_int);
            token = starterFile->get_next_value("F max age");
            temp_int = token.toInt();
            model_info->set_f_max_age(temp_int);
        }
        temp_int = starterFile->getIntValue(QString("F std report basis"), 0, 4, 1);
        }
        if (starterFile->getOkay() && !starterFile->getStop()) {
        model_info->set_f_basis(temp_int);

        token = starterFile->get_next_value("check value for end of file or something else");
        temp_int = token.toInt();
        temp_float = token.toFloat();
        }
        if (temp_int == END_OF_DATA)
        {
            datafile_version = 3.2;
            ui->comboBox_MCMC_output->setCurrentIndex(0);
            ui->lineEdit_mcmc_bump_val->setText(QString("0.01"));
            model_info->setALKTol(0.0);
            end = true;
        }
        else
        {
            if (starterFile->getOkay() && !starterFile->getStop()) {
            // MCMC output format
            starterFile->checkIntValue(temp_int, QString("MCMC output detail"), 0, 3, 0);
            ui->comboBox_MCMC_output->setCurrentIndex(temp_int);
            // MCMC Bump value
            temp_float -= temp_int;
            ui->lineEdit_mcmc_bump_val->setText(QString::number(temp_float));

            // ALK tolerance
            token = starterFile->get_next_value("ALK Tolerance");
            temp_float = token.toFloat();
            if (token.compare("3.30"))
            {
                if (temp_float < 0.0)
                    temp_float = 0.0;
                if (temp_float > 0.1)
                    temp_float = 0.1;
                model_info->setALKTol (temp_float);
            }
            else {
                model_info->setALKTol(0.0);
                end = true;
            }

            // check for random number seed
            token = starterFile->get_next_value();
            temp_int = token.toInt();
            if (!end && token.compare("3.30"))
            {
                model_info->setRandSeed(temp_int);
                token = starterFile->get_next_value();  // for version number
            }
            else
            {
                model_info->setRandSeed(0);
                end = true;
            }
            // version number
            temp_float = get_version_number(token);
            datafile_version = temp_float;
            ui->doubleSpinBox_version->setValue(datafile_version);
            datafile_version = ui->doubleSpinBox_version->value();
            }
        }

        starterFile->close();
        return starterFile->getOkay();
    }
    else
    {
        error_unreadable(get_starter_file());
        okay = false;
        starterFile->setOkay(false);
    }
    return okay;
}

void file_widget::write_starter_file (QString filename)
{
    QString line("");
    int temp_int = 0, chars = 0;

    if (filename.isEmpty())
        filename = ui->label_starter_file->text();

    if (!starterFile)
        starterFile = new ss_file (filename, this);
    else if (starterFile->fileName().compare(filename, Qt::CaseSensitive))
    {
        starterFile->setFileName(filename);
    }

    if(starterFile->open(QIODevice::WriteOnly))
    {
        chars += writeVersionComment(starterFile);
        starterFile->write_comments();

        chars += starterFile->write_val(data_file_name, 24,
                    QString("data file name"));
        chars += starterFile->write_val(control_file_name, 24,
                    QString("control file name"));

        temp_int = getReadParFile()? 1: 0;
        chars += starterFile->write_val(temp_int, 1,
                    QString("0=use init values in control file; 1=use ss.par"));

        chars += starterFile->write_val(ui->comboBox_detail_level->currentIndex(), 1,
                    QString("run display detail (0,1,2)"));

        temp_int = ui->comboBox_report_level->currentIndex();
        chars += starterFile->write_val(temp_int, 1,
                    QString("detailed output (0=minimal for data-limited, 1=high (w/ wtatage.ss_new), 2=brief, 3=custom) "));
        chars += starterFile->writeline("# custom report options: -100 to start with minimal; -101 to start with all; -number to remove, +number to add, -999 to end");
        if (temp_int == 3) {
            QList<bool> list = chooseRepDetail->getReports();
            for (int i = 1; i < list.count(); i++) {
                if (list.at(i)) {
                    chars += starterFile->write_val(i, 1, QString("Report_%1").arg(QString::number(i)));
                }
            }
            starterFile->write_val(-999, 1, QString("terminator"));
        }

        chars += starterFile->write_val(QString(ui->checkBox_checkup->isChecked()?"1":"0"), 1,
                    QString("write 1st iteration details to echoinput.sso file (0,1) "));

        chars += starterFile->write_val(get_parmtr_write(), 1,
                    QString("write parm values to ParmTrace.sso (0=no,1=good,active; 2=good,all; 3=every_iter,all_parms; 4=every,active)"));

        chars += starterFile->write_val(ui->comboBox_cumreport->currentIndex(), 1,
                    QString("write to cumreport.sso (0=no,1=like&timeseries; 2=add survey fits)"));

        chars += starterFile->write_val(QString(model_info->get_prior_likelihood()?"1":"0"), 1,
                     QString("Include prior_like for non-estimated parameters (0,1) "));

        chars += starterFile->write_val(QString(model_info->get_use_softbounds()?"1":"0"), 1,
                     QString("Use Soft Boundaries to aid convergence (0,1) (recommended)"));
        chars += starterFile->writeline("#");

        chars += starterFile->write_val(ui->spinBox_datafiles->value(), 1,
                     QString("Number of datafiles to produce: 0 turns off all *.ss_new; 1st is data_echo.ss_new, 2nd is data_expval.ss, 3rd and higher are data_boot_**N.ss,"));

        chars += starterFile->write_val(model_info->get_last_estim_phase(), 1,
                     QString("Turn off estimation for parameters entering after this phase"));
        chars += starterFile->writeline("#");

        chars += starterFile->write_val(model_info->mc_burn(), 1,
                     QString("MCeval burn interval"));

        chars += starterFile->write_val(model_info->mc_thin(), 1,
                    QString("MCeval thin interval"));

        chars += starterFile->write_val(model_info->jitter_param(), 1,
                    QString("jitter initial parm value by this fraction"));

        chars += starterFile->write_val(model_info->bio_sd_min_year(), 1,
                    QString("min yr for sdreport outputs (-1 for styr)"));

        chars += starterFile->write_val(model_info->bio_sd_max_year(), 1,
                    QString("max yr for sdreport outputs (-1 for endyr+1; -2 for endyr+Nforecastyrs)"));

        chars += starterFile->write_val(model_info->get_num_std_years(), 1,
                    QString("N individual STD years "));

        chars += starterFile->write_val(QString("#vector of year values "));
        chars += starterFile->write_vector(model_info->get_std_years(), 6);

        chars += starterFile->write_val(model_info->get_convergence_criteria(), 1,
                    QString("final convergence criteria (e.g. 1.0e-04) "));

        chars += starterFile->write_val(model_info->get_retrospect_year(), 1,
                    QString("retrospective year relative to end year (e.g. -4)"));

        chars += starterFile->write_val(model_info->get_biomass_min_age(), 1,
                    QString("min age for calc of summary biomass"));

        chars += starterFile->write_val(model_info->get_depletion_basis(), 1,
                    QString("Depletion basis: denom is: 0=skip; 1=X*SPBvirgin; 2=X*SPBmsy; 3=X*SPB_styr; 4=X*SPB_endyr; 5=X*dyn_Bzero; values>=11 invoke N multiyr (up to 9!) with 10's digit; >100 invokes log(ratio)"));

        chars += starterFile->write_val(model_info->get_depletion_denom(), 1,
                    QString("Fraction (X) for Depletion denominator (e.g. 0.4)"));

        chars += starterFile->write_val(model_info->get_spr_basis(), 1,
                    QString("SPR_report_basis:  0=skip; 1=(1-SPR)/(1-SPR_tgt); 2=(1-SPR)/(1-SPR_MSY); 3=(1-SPR)/(1-SPR_Btarget); 4=rawSPR"));

        chars += starterFile->write_val(model_info->get_f_units(), 1,
                    QString("F_reporting_units: 0=skip; 1=exploitation(Bio); 2=exploitation(Num); 3=sum(Apical_F's); 4=true F for range of ages; 5=unweighted avg. F for range of ages"));

        line.clear();
        line.append(QString(" %1 %2 #_min and max age over which average F will be calculated with F_reporting=4 or 5" ).arg
                   (QString::number(model_info->get_f_min_age()),
                    QString::number(model_info->get_f_max_age())));
        if (model_info->get_f_units() < 4)
        {
            line.prepend(QString ("#COND"));
            line.append(QString(" with F_reporting>3"));
        }
        chars += starterFile->writeline (line);
        chars += starterFile->write_val (model_info->get_f_basis(), 1,
                    QString("F_std_basis: 0=raw_annual_F; 1=F/Fspr; 2=F/Fmsy; 3=F/Fbtgt; where F means annual_F; values >=11 invoke N multiyr (up to 9!) with 10's digit; >100 invokes log(ratio)"));

        if (datafile_version < 3.30)
        {
            chars += starterFile->write_val(END_OF_DATA, 1,
                    QString("check value for end of file and for version control"));
        }
        else
        {
            float temp_float  = ui->lineEdit_mcmc_bump_val->text().toFloat();
            temp_int = ui->comboBox_MCMC_output->currentIndex();
            temp_float += temp_int;
            chars += starterFile->write_val(temp_float, 1,
                    QString("MCMC output detail: integer part (0=default; 1=adds obj func components; 2= +write_report_for_each_mceval); and decimal part (added to SR_LN(R0) on first call to mcmc)"));
            chars += starterFile->write_val(model_info->getALKTol(), 1,
                    QString ("ALK tolerance ***disabled in code*** (example 0.0001)"));
            if (model_info->getRandSeed() != 0)
                chars += starterFile->write_val(model_info->getRandSeed(), 1,
                        QString ("random number seed for bootstrap data (-1 to use long(time) as seed)"));
            chars += starterFile->write_val(QString::number(datafile_version, 'f', 2), 1,
                    QString("check value for end of file and for version control"));
        }

        starterFile->close();
    }
}


int file_widget::read_run_num_file (QString filename)
{
//    QString filename (ui->lineEdit_runnum_file->text());
    QString token;
//    float temp_float;
    int temp_int = 0;

    if (filename.isEmpty())
        filename = QString(QString("%1/%2").arg(current_dir_name, RUN_NUMBER_FILE));

    if (runNumberFile != nullptr)
        delete runNumberFile;
    runNumberFile = new ss_file (filename, this);

    if(runNumberFile->open(QIODevice::ReadOnly))
    {
        runNumberFile->seek(0);
        token = runNumberFile->get_next_value();
        temp_int = token.toInt();
        model_info->set_run_number (temp_int);
        ui->label_run_num_val->setText(QString(" %1 ").arg(token));
    }
    runNumberFile->close();
    return 1;
}

void file_widget::reset_run_num()
{
    model_info->set_run_number(0);
    write_run_num_file();
    ui->label_run_num_val->setText(QString::number(0));
}

void file_widget::write_run_num_file (QString filename)
{
    int temp_int;
    QString line;
    if (filename.isEmpty())
        filename = QString("%1/%2").arg(current_dir_name, RUN_NUMBER_FILE);

    if (runNumberFile == nullptr)
        runNumberFile = new ss_file (filename, this);
    else
        runNumberFile->setFileName(filename);

    if(runNumberFile->open(QIODevice::WriteOnly))
    {
        temp_int = model_info->get_run_number();
        line.append(QString("%1" ).arg(QString::number(temp_int)));
        runNumberFile->writeline(line);
    }
    runNumberFile->close();
}


void file_widget::show_file_info(ss_file *file)
{
    if (file != nullptr)
    {
        file_info_dialog(file).exec();
    }
}

void file_widget::view_param_file()
{
    viewer->viewFile(PARAMETER_FILE);
    viewer->show();
}

void file_widget::view_wtatage()
{
    viewer->viewFile(ui->label_wtatage_file->text());
    viewer->show();
}

void file_widget::changeOutputDirectory(bool flag)
{
    emit outDirectoryChanged(flag);
    ui->label_output_sso_info->setVisible(flag);
}

void file_widget::changeNewDirectory(bool flag)
{
    emit newDirectoryChanged(flag);
    ui->label_output_ssnew_info->setVisible(flag);
}


bool file_widget::error_no_file(ss_file *file, int type)
{
    bool okay = true;
    QString fname;
    QString msg(QString("File %1 does not exist.\n").arg(file->fileName()));
    msg = tr(msg.toUtf8());
    QString filetype;
    if (type == 1)
        filetype = QString(tr("Stock Synthesis starter files (starter.ss);;all files (*.*)"));
    else if (type == 4)
        filetype = QString(tr("Stock Synthesis forecast files (forecast.ss);;all files (*.*)"));
    else
        filetype = QString(tr("Stock Synthesis files (*.ss);;all files (*.*)"));
#ifdef DEBUG
    error->write(msg.toUtf8()) ;
#endif
    msg.append(tr(" Do you want to select a new file?"));
    int btn = QMessageBox::warning((QWidget*)parent(), tr("File Read Error"), msg, QMessageBox::Cancel | QMessageBox::Ok);
    if (btn == QMessageBox::Ok)
    {
        fname = QFileDialog::getOpenFileName (this, tr("Select File"),
                           current_dir_name, filetype);
        file->setFileName(fname);
        if (file->exists())
            okay = true;
        else
            okay = false;
    }
    else
    {
        okay = false;
    }
    return okay;
}

void file_widget::error_unreadable(QString fname)
{
    QString msg (QString("File %1 is unreadable.\n").arg(fname));
    msg = tr(msg.toUtf8());
#ifdef DEBUG
    error->write (msg.toUtf8()) ;
#endif
    QMessageBox::warning((QWidget*)parent(), tr("File Read Error"), msg, QMessageBox::Cancel | QMessageBox::Ok);
}

void file_widget::error_problem(ss_file *file)
{
    QString msg (QString("Problem in reading file %1, line %2.\n").arg(file->fileName(), QString::number(file->getLineNum())));
    msg = tr(msg.toUtf8());
#ifdef DEBUG
    error->write (msg.toUtf8()) ;
#endif
    QMessageBox::warning((QWidget*)parent(), tr("File Read Error"), msg, QMessageBox::Cancel | QMessageBox::Ok);
}

const QString &file_widget::getSsoutDir() const
{
    return ssoutDir;
}

void file_widget::setSsoutDir(const QString &newSsoutDir)
{
    ssoutDir = newSsoutDir;
    bool used = !ssoutDir.isEmpty();
    ui->label_output_sso_info->setVisible(used);
    ui->checkBox_output_sso->setChecked(used);
    updateOutputFiles(current_dir_name);
}

const QString &file_widget::getSsnewDir() const
{
    return ssnewDir;
}

void file_widget::setSsnewDir(const QString &newSsnewDir)
{
    ssnewDir = newSsnewDir;
    bool used = !ssnewDir.isEmpty();
    ui->label_output_ssnew_info->setVisible(used);
    ui->checkBox_output_ssnew->setChecked(used);
}

void file_widget::reportDetailChanged(int value)
{
    if (chooseRepDetail->getRepDetail() != value) {
        chooseRepDetail->setRepDetail(value);
        if (value == 3)
            chooseReportDetail();
    }
}

void file_widget::chooseReportDetail()
{
    chooseRepDetail->show();
}

void file_widget::changeReportDetail(int value)
{
    ui->comboBox_report_level->setCurrentIndex(value);
}

