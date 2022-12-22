/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef FILE_WIDGET_H
#define FILE_WIDGET_H

#include <QWidget>
#include <QDir>

#include "input_file.h"
#include "model.h"
#include "sscomposition.h"
#include "tableview.h"
#include "dialog_fileview.h"
#include "dialogchosereport.h"

// standard input file names
#define STARTER_FILE      "starter.ss"
#define FORECAST_FILE     "forecast.ss"
#define PARAMETER_FILE    "ss.par"
#define PROFILE_VAL_FILE  "ProfileValues.ss"
#define WTATAGE_FILE      "wtatage.ss"
#define RUN_NUMBER_FILE   "runnumber.ss"
// default input file names
#define DATA_FILE         "data.ss"
#define CONTROL_FILE      "control.ss"

// standard output file names
#define CHECKUP_FILE      "checkup.sso"
#define COMP_REPORT_FILE  "CompReport.sso"
#define COVAR_FILE        "covar.sso"
#define CUM_REPORT_FILE   "CumReport.sso"
#define DERIVED_POST_FILE "derived_posteriors.sso"
#define ECHOED_INPUT_FILE "echoinput.sso"
#define FCAST_REPORT_FILE "ForecastReport.sso"
#define PARAM_TRACE_FILE  "ParmTrace.sso"
#define POST_VECTOR_FILE  "posterior_vectors.sso"
#define POSTERIORS_FILE   "posteriors.sso"
#define REBUILD_FILE      "rebuild.sso"
#define REPORT_FILE       "Report.sso"
#define SIS_OUTPUT_FILE   "SIS_table.sso"
#define SS_WARNING_FILE   "warning.sso"

#define ERROR_FILE        "/ss_error.txt"

namespace Ui {
class file_widget;
}

class file_widget : public QWidget
{
    Q_OBJECT

public:
    explicit file_widget (ss_model *mod = 0, QString dir = QString(""), QWidget *parent = 0);
    ~file_widget();

    const QString &getSsnewDir() const;
    void setSsnewDir(const QString &newSsnewDir);

    const QString &getSsoutDir() const;
    void setSsoutDir(const QString &newSsoutDir);

private:
    Ui::file_widget *ui;

    ss_model *model_info;
    QDir current_dir;
    QString current_dir_name;

    ss_file *starterFile;
    ss_file *dataFile;
    ss_file *forecastFile;
    ss_file *controlFile;
    ss_file *runNumberFile;
    ss_file *parameterFile;
    ss_file *profileFile;
    ss_file *userDataFile;

    QString control_file_name;
    QString data_file_name;

    float datafile_version;
#ifdef DEBUG
    QFile *error;
#endif

    Fleet *getActiveFleet (int index, Fleet::FleetType type);

    Dialog_fileView *viewer;

public slots:
    void reset();

    void set_starter_file (QString fname, bool keep = false);
    QString get_starter_file();
    void set_forecast_file (QString fname, bool keep = false);
    QString get_forecast_file ();
    void set_control_file (QString fname, bool keep = false);
    QString get_control_file ();
    void set_data_file (QString fname, bool keep = false);
    QString get_data_file ();
    QString get_param_file ();
    QString get_profile_file ();
    QString get_run_num_file ();
    QString getDataFileName () {return data_file_name;}
    QString getControlFileName () {return control_file_name;}
    ss_model *this_model() {return model_info;}
    void setReadWtAtAge (int flag);
    void changeReadWtAtAge (int flag);

    float get_version_number(QString token);
    QString getDatafileVersion ();
    void setDatafileVersion(double ver, bool flag = true);

    void increase_font();
    void decrease_font();

    void set_default_file_names (QString dir, bool keep = false);
    void updateOutputFiles(QString dir);
    void new_directory (QString dir, bool keep = false);
    bool read_files();
    bool read_files(ss_model *model_inf);

    bool read_starter_file(QString filename = QString(""));
    bool read_starter_file(ss_file *starter);
    int read_run_num_file (QString filename = QString(""));


    void write_files ();

    void write_starter_file (QString filename = QString(""));
    void write_run_num_file (QString filename = QString(""));

    void print_files ();

    void setReadParFile (bool flag);
    void changeReadParFile (bool flag);
    bool getReadParFile ();
//    void set_par_file (QString fname, bool keep = false);
    void setReadProFile (bool flag);
    void changeReadProFile (bool flag);
    bool getReadProFile ();
    void setReadProFile (QString fname, bool keep = false);
    void set_parmtr_write(int flag = 0);
    int get_parmtr_write ();
//    void set_cumrpt_write(int flag = 0);
//    int get_cumrpt_write ();

    void show_input_files ();
    void show_output_files ();
    void view_wtatage();

    void reportDetailChanged(int value);
    void chooseReportDetail();
    void changeReportDetail(int value);

signals:
    void directory_changed (QString dirname);
    void starter_file_changed (QString fname);
    void forecast_file_changed (QString fname);
    void control_file_changed (QString fname);
    void data_file_changed (QString fname);
    void choose_control_file();
    void save_control_file();
    void choose_data_file();
    void save_data_file();
    void read_all_files();
    void files_read(bool okay);
    void newDirectoryChanged(bool);
    void outDirectoryChanged(bool);

private slots:

    void parm_trace_changed(bool flag);
//    void cumrpt_fits_changed(bool flag);
//    void cumrpt_like_changed(bool flag);

    void show_file_info (ss_file *file);
    void show_starter_file_info () {show_file_info (starterFile);}
    void show_forecast_file_info () {show_file_info (forecastFile);}
    void show_data_file_info () {show_file_info (dataFile);}
    void show_control_file_info () {show_file_info (controlFile);}
//    void show_param_file_info () {show_file_info (parameterFile);}
    void view_param_file ();
    void show_prof_file_info () {show_file_info (profileFile);}

    void reset_run_num ();

    void changeOutputDirectory(bool flag);
    void changeNewDirectory(bool flag);

    bool error_no_file (ss_file *file, int type = 1);
    void error_unreadable (QString fname);
    void error_problem (ss_file *file);

private:
    DialogChoseReport *chooseRepDetail;

    QString ssnewDir;
    QString ssoutDir;

};

#endif // FILE_WIDGET_H
