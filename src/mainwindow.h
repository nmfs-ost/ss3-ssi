/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef NULL
#undef NULL
#endif // NULL
#define NULL ((void *)0)

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QDir>

#include "file_widget.h"
#include "data_widget.h"
#include "forecast_widget.h"
#include "fleet_widget.h"

#include "population_widget.h"
#include "model.h"

#include "dialog_run.h"
#include "dialog_readme.h"

//#define DEBUG 1

//#undef DEBUG

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void reset();

    void closeEvent(QCloseEvent *event);
    void setupMenus(QMenuBar *menu);
    void readSettings();
    void writeSettings();

 /*   void read_starter_file(ss_file *ifile, ss_model *modl);
    void read_forecast_file();
    void read_control_file();
    void read_data_file();
    void read_param_file();
    void read_profile_file();

    void writeForecastFile();
    void writeStarterFile();
    void writeControlFile();
    void writeDataFile();
    void writeParamFile();
    void writeProfileFile();*/


public slots:
    void refreshAll();
    void openNewDirectory();
    void createNewDirectory();
    void changeDirectory(QString dirname);
    void openDirectory(QString fname = QString(""));
    void copyDirectory();
    void openControlFile();
    void saveControlFile();
    void openDataFile();
    void saveDataFile();
    void readFiles ();
    void saveFiles ();
    void copyFiles (QString oldDir, QString newDir);
    void copyFile (QString fname, QString newDir);
    void printFiles();
    QString getDataFile();
    QString getControlFile();
    void setReadWtAtAgeSS (bool flag);

    void createReadme();
    void viewReadme();

    void helpGUI();
    void helpSS();
    void helpNFT();
    void helpADMB();
    void helpQt();
    void showCopyright();
    void run();
    void runComplete();
    void runConversion();

    void changeDataFile (QString fname);
    void changeControlFile (QString fname);

    void showLengthObs();
    void showAgeObs();
    void showMeanSAAObs();
    void showGenSizeObs(int index);
    void showMorphObs();
    void showRecapObs();

    void showDocument (QString setting);
    void showUserManual ();
    void showTechManual ();
    void showGUIGuide();
    void showWebpage (QString pg);
    void setDefaultModel();
    void returnToDefault();
    void locateDirectory ();
    void locateDocuments ();
    void locateExecutable(QString &savename, QString type, QString hint = QString(""));
    void locateSSExecutable();
    void locateSSConverter ();
    void locateRExecutable ();
    void changeExecutable(QString key, QString filename);
    QString findFile (QString dir, QString title, QString filters);

    void moveEvent(QMoveEvent *event);

signals:

private:
    Ui::MainWindow *ui;
    QMenuBar *mainMenu;
    int mainScrn;

    ss_model *modelData;
    file_widget *files;
    data_widget *data;
    forecast_widget *forecast;
    fleet_widget *fleets;
    population_widget *population;

    Dialog_run *dRun;
    Dialog_readme *readme;

    void set_checks_false();

    enum {
        FILES = 0,
        MODEL,
        FORECAST,
        PARAMETERS,
        POPULATION,
        FLEETS
    };

//    QDir current_dir;
    QFont main_font;

//    starter_widget *starter_info;
//    forecast_widget *forecast_info;
//    control_widget *control_info;
//    data_widget *data_info;

    QString app_dir;
    QString current_dir;
    QString starter_file;
    QString control_file;
    QString forecast_file;
    QString data_file;
    QString parameter_file;
    QString profile_file;
    QString run_num_file;

    QString ss_exe;
    QString ss_trans_exe;
    QString R_exe;
    QString default_dir;
    QString default_model;

private slots:
    void mainTabChanged(int tab);
    void showFiles();
    void showModel();
    void showForecast();
    void showParameters();
    void showPopulations();
    void showFisheries();
    void showSurveys();

    void copy_file(QString old_file, QString new_file);
    int askMissingFile(QString name = QString("starter.ss"));
    void increaseFont();
    void decreaseFont();
    void setFontSize (int fsize);

};

#endif // MAINWINDOW_H
