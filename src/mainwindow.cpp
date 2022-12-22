#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "metadata.h"
#include "dialog_about_gui.h"
#include "dialog_about.h"
#include "dialog_about_admb.h"
#include "dialog_about_nft.h"
#include "dialog_readme.h"
#include "dialog_run.h"
#include "documentdialog.h"
#include "message.h"
#include "mainwindowrun.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>

static QString NEWDIR("ssnew");
static QString OUTDIR("sso");


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#ifdef DEBUG
    QMessageBox::information(this, "Information - program flow", "Start main window widget set up.");
#endif
    // set up the ui
    ui->setupUi(this);
    setWindowTitle(tr(app_name));
    setWindowIcon(QIcon(":icons/SSI_icon.ico"));

    readSettings();

    app_dir = qApp->applicationDirPath();
    mainScrn = 0;
    // set up the default model
    modelData = new ss_model(this);
//    modelData->setVisible(false);
#ifdef DEBUG
    QMessageBox::information(this, "Information - program flow", "Model data set up finished.");
#endif
    // furnish the ui with the default model
    current_dir = QString(QString("%1/default/").arg(qApp->applicationDirPath()));
    new_dir = QString();
    output_dir = QString();

    // set up readme viewer
    readme = new Dialog_readme(this, modelData, current_dir);
    readme->hide();

    // run dialog
    dRun = new Dialog_run(this);
    dRun->setExe(ss_exe);
    dRun->setDir(current_dir);
    dRun->setRExe(R_exe);
    dRun->hide();

    // all other widgets
    files = new file_widget (modelData, current_dir, this);
    data = new data_widget(modelData, this);
    forecast = new forecast_widget(modelData, this);
    fleets = new fleet_widget(modelData, this);
    population = new population_widget(modelData, this);
#ifdef DEBUG
    QMessageBox::information(this, "Information - program flow", "Widget set up finished.");
#endif
    // set up information dock widget

    QFont title_font ("Tw Cen MT Condensed", 28, 4);
    title_font.setFamily("Arial");
    title_font.setBold(true);
    title_font.setPointSize(14);
    ui-> label_ss_ver->setFont(title_font);
    ui->label_ss_ver->setText(tr(app_name) + " " + getAppVersion());
    title_font.setPointSize(11);
    ui->label_gui_ver->setFont(title_font);
    ui->label_gui_ver->setText(tr("For use with Stock Synthesis ") + getAppAppliesTo());
    ui->pushButton_about_ftb->setVisible(false);
    ui->pushButton_about_qt->setVisible(false);


//    mainMenu = new QMenuBar(this);
    setupMenus(ui->menuBar);

    connect (ui->pushButton_about_gui, SIGNAL(clicked()), SLOT(helpGUI()));
    connect (ui->pushButton_about_ss, SIGNAL(clicked()), SLOT(helpSS()));
    connect (ui->pushButton_about_ftb, SIGNAL(clicked()), SLOT(helpNFT()));
    connect (ui->pushButton_about_admb, SIGNAL(clicked()), SLOT(helpADMB()));
    connect (ui->pushButton_about_qt, SIGNAL(clicked()), SLOT(helpQt()));
    connect (ui->actionTitle_Window, SIGNAL(toggled(bool)), ui->dockWidget_help, SLOT(setVisible(bool)));
    connect (ui->dockWidget_help, SIGNAL(visibilityChanged(bool)), ui->actionTitle_Window, SLOT(setChecked(bool)));
    connect (ui->actionToolbar, SIGNAL(toggled(bool)), ui->mainToolBar, SLOT(setVisible(bool)));
    connect (ui->mainToolBar, SIGNAL(visibilityChanged(bool)), ui->actionToolbar, SLOT(setChecked(bool)));

    connect (dRun, SIGNAL(runCompleted()), SLOT(runComplete()));

    connect (files, SIGNAL(read_all_files()), SLOT(readFiles()));
    connect (files, SIGNAL(save_data_file()), SLOT(saveDataFile()));
    connect (files, SIGNAL(choose_data_file()),SLOT(openDataFile()));
    connect (files, SIGNAL(save_control_file()), SLOT(saveControlFile()));
    connect (files, SIGNAL(choose_control_file()), SLOT(openControlFile()));
    connect (files, SIGNAL(files_read(bool)), SLOT(refreshAll()));
    connect (files, SIGNAL(directory_changed(QString)), SLOT(changeDirectory(QString)));
    connect (files, SIGNAL(newDirectoryChanged(bool)), SLOT(changeNewDir(bool)));
    connect (files, SIGNAL(outDirectoryChanged(bool)), SLOT(changeOutDir(bool)));

    connect (ui->action_New, SIGNAL(triggered()), SLOT(createNewDirectory()));
    connect (ui->action_Open, SIGNAL(triggered()), SLOT(openDirectory()));
    connect (ui->action_Save_data, SIGNAL(triggered()), SLOT(saveFiles()));
    connect (ui->action_Save_As, SIGNAL(triggered()), SLOT(copyDirectory()));
    connect (ui->actionCreate_Readme, SIGNAL(triggered()), SLOT(createReadme()));
    connect (ui->actionView_Readme, SIGNAL(triggered()), SLOT(viewReadme()));
    connect (ui->action_Exit, SIGNAL(triggered()), SLOT(close()));

    connect (ui->actionAbout_this_model, SIGNAL(triggered()), SLOT(viewReadme()));
    connect (ui->actionAbout_SS_GUI, SIGNAL(triggered()), SLOT(helpGUI()));
    connect (ui->action_About, SIGNAL(triggered()), SLOT(helpSS()));
//    connect (ui->action_About_NFT, SIGNAL(triggered()), SLOT(helpNFT()));
    connect (ui->action_User_Manual, SIGNAL(triggered()), SLOT(showUserManual()));
    connect (ui->action_Using_SS, SIGNAL(triggered()), SLOT(showTechManual()));
    connect (ui->action_About_ADMB, SIGNAL(triggered()), SLOT(helpADMB()));
    connect (ui->action_About_Qt, SIGNAL(triggered()), SLOT(helpQt()));
    connect (ui->actionShow_copyright_info, SIGNAL(triggered()), SLOT(showCopyright()));

    connect (ui->action_Files, SIGNAL(triggered()), SLOT (showFiles()));
    connect (ui->action_Model_Data, SIGNAL(triggered()), SLOT(showModel()));
    connect (ui->action_Forecast, SIGNAL(triggered()), SLOT(showForecast()));
    connect (ui->action_Fleets, SIGNAL(triggered()), SLOT(showFisheries()));
    connect (ui->action_Populations, SIGNAL(triggered()), SLOT(showPopulations()));

    connect (ui->action_IncreaseFont, SIGNAL(triggered()), SLOT(increaseFont()));
    connect (ui->action_DecreaseFont, SIGNAL(triggered()), SLOT(decreaseFont()));

    connect (ui->action_Run_Stock_Synthesis, SIGNAL(triggered()), SLOT(run()));

    connect (ui->action_Locate_SS, SIGNAL(triggered()), SLOT(locateDirectory()));
    connect (ui->actionLocate_SS_executable, SIGNAL(triggered()), SLOT(locateSSExecutable()));
    connect (ui->actionLocate_R_executable, SIGNAL(triggered()), SLOT(locateRExecutable()));
    connect (ui->actionLocate_documents, SIGNAL(triggered()), SLOT(locateDocuments()));
    connect (ui->actionSelect_default_directory, SIGNAL(triggered()), SLOT(locateDirectory()));
    connect (ui->actionSet_default_model, SIGNAL(triggered()), SLOT(setDefaultModel()));
    connect (ui->actionReturn_to_default_model, SIGNAL(triggered()), SLOT(returnToDefault()));

    connect (data, SIGNAL(showLengthObs()), SLOT(showLengthObs()));
    connect (data, SIGNAL(showAgeObs()), SLOT(showAgeObs()));
    connect (data, SIGNAL(showSAAObs()), SLOT(showMeanSAAObs()));
    connect (data, SIGNAL(showGenObs(int)), SLOT(showGenSizeObs(int)));
    connect (data, SIGNAL(showMorphObs()), SLOT(showMorphObs()));
    connect (data, SIGNAL(showRecapObs()), SLOT(showRecapObs()));

    connect (population, SIGNAL(readWtAtAgeSS(int)), SLOT(setReadWtAtAgeSS(int)));

    ui->stackedWidget->setCurrentIndex(FILES);
    ui->stackedWidget->currentWidget()->layout()->addWidget(files);
    ui->stackedWidget->setCurrentIndex(MODEL);
    ui->stackedWidget->currentWidget()->layout()->addWidget(data);
    ui->stackedWidget->setCurrentIndex(FORECAST);
    ui->stackedWidget->currentWidget()->layout()->addWidget(forecast);
    ui->stackedWidget->setCurrentIndex(FLEETS);
    ui->stackedWidget->currentWidget()->layout()->addWidget(fleets);
    ui->stackedWidget->setCurrentIndex(POPULATION);
    ui->stackedWidget->currentWidget()->layout()->addWidget(population);
    connect (ui->stackedWidget, SIGNAL(currentChanged(int)), SLOT(mainTabChanged(int)));
    showFiles();

    main_font = QFont(fontInfo().family(), fontInfo().pointSize());
    setFontSize(9);

    readFiles();
}

MainWindow::~MainWindow()
{
    delete readme;
    delete files;
    delete data;
    delete forecast;
    delete fleets;
    delete population;
    delete modelData;
    delete ui;
}

void MainWindow::reset()
{
    files->reset();
    data->reset();
    forecast->reset();
    fleets->reset();
    population->reset();
}

void MainWindow::refreshAll()
{
//    files->reset();
    data->refresh();
    forecast->refresh();
    fleets->refresh();
    fleets->set_current_fleet();
    population->refresh();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
//    event->accept();
}
void MainWindow::setupMenus (QMenuBar *main)
{
    main->clear();
    QMenu *fileMenu = new QMenu(QString("&File"), main); // fileMenu () (ui->menu_File);
    fileMenu->addAction (ui->actionReturn_to_default_model);
    fileMenu->addAction (ui->actionSet_default_model);
    fileMenu->addAction (ui->actionSelect_default_directory);
    fileMenu->addSeparator();
    fileMenu->addAction (ui->action_New);
    fileMenu->addAction (ui->action_Open);
    fileMenu->addAction (ui->action_Save_data);
    fileMenu->addAction (ui->action_Save_As);
    fileMenu->addSeparator();
    fileMenu->addAction (ui->actionView_Readme);
    fileMenu->addAction (ui->actionCreate_Readme);
    fileMenu->addSeparator();
//    fileMenu.addAction (ui->action_Print);
//    fileMenu.addSeparator();
    fileMenu->addAction (ui->action_Exit);

    QMenu *viewMenu = new QMenu(QString("&View"), main); // viewMenu (ui->menuView);
//    viewMenu->addAction (ui->action_Report_File);
//    viewMenu->addSeparator();
    viewMenu->addAction (ui->action_IncreaseFont);
    viewMenu->addAction (ui->action_DecreaseFont);
    viewMenu->addSeparator();
    viewMenu->addAction (ui->actionTitle_Window);
    viewMenu->addAction (ui->actionToolbar);

    QMenu *run_Menu = new QMenu(QString("&Run"), main); // run_Menu (ui->menu_Run);
    run_Menu->addAction (ui->action_Run_Stock_Synthesis);

    QMenu *optsMenu = new QMenu(QString("&Links"), main); // opt_Menu (ui->menu_Options);
    optsMenu->addAction (ui->actionLocate_SS_executable);
    optsMenu->addAction(ui->actionLocate_R_executable);
    optsMenu->addSeparator();
    optsMenu->addAction (ui->actionLocate_documents);

    QMenu *windMenu = new QMenu(QString("&Windows"), main); // windMenu (ui->menu_Windows);
    windMenu->addAction (ui->action_Files);
    windMenu->addAction (ui->action_Model_Data);
    windMenu->addAction (ui->action_Fleets);
    windMenu->addAction (ui->action_Populations);
    windMenu->addAction (ui->action_Forecast);
    windMenu->addSeparator();
    windMenu->addAction (ui->actionTitle_Window);

    QMenu *helpMenu = new QMenu(QString("&Help"), main); // helpMenu (ui->menu_Help);
    helpMenu->addAction (ui->actionAbout_this_model);
    helpMenu->addSeparator();
    helpMenu->addAction (ui->action_User_Manual);
    helpMenu->addAction (ui->actionAbout_SS_GUI);
    helpMenu->addAction (ui->action_About);
//    helpMenu->addAction (ui->action_About_NFT);
    helpMenu->addSeparator();
    helpMenu->addAction (ui->action_About_ADMB);
    helpMenu->addAction (ui->action_About_Qt);
    helpMenu->addSeparator();
    helpMenu->addAction(ui->actionShow_copyright_info);
//    helpMenu.addAction (ui->action_Using_SS);
    //    mainMenu.clear();
    main->addMenu(fileMenu);
//    main->addMenu(dataMenu);
    main->addMenu(viewMenu);
    main->addMenu(run_Menu);
    main->addMenu(optsMenu);
    main->addMenu(windMenu);
    main->addSeparator();
    main->addMenu(helpMenu);
}

void MainWindow::writeSettings()
{
    QSettings settings(app_copyright_org, app_name);

    settings.beginGroup("MainWindow");
    settings.setValue("screenNum", QApplication::desktop()->screenNumber(this));
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("defaultModel", default_model);
    settings.setValue("defaultDir", default_dir);
    settings.setValue("ss_exe", ss_exe);
    settings.setValue("r_exe", R_exe);
    settings.endGroup();
    settings.beginGroup ("HelpWindow");
    settings.setValue("size", ui->dockWidget_help->size());
    settings.setValue("pos", ui->dockWidget_help->pos());
    settings.setValue("visible", ui->dockWidget_help->isVisible());
    settings.endGroup();
    settings.beginGroup("ToolBar");
    settings.setValue("rect", ui->mainToolBar->geometry());
    settings.setValue("visible", ui->mainToolBar->isVisible());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QDesktopWidget *desk = QApplication::desktop();
    int numscrns = desk->numScreens();
    QSize qsize;
    QPoint qpos;
    int xdelta = 0;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect scrRect = screen->availableGeometry();
//    QRect deskRect = desk->screenGeometry();
    QSettings settings(app_copyright_org, app_name);
    QString model (app_dir + "/default");
    QString exe (app_dir + "/ss.exe");
    QString blank_exe ("");

    settings.beginGroup("MainWindow");
    mainScrn = settings.value("screenNum", 0).toInt();
    qsize = settings.value("size", QSize(700, 600)).toSize();
    qpos = settings.value("pos", QPoint(200, 200)).toPoint();
    if (mainScrn < numscrns) {
        scrRect = desk->screenGeometry(mainScrn);
    }
    else {
        mainScrn = 0;
        scrRect = desk->screenGeometry(mainScrn);
        for (int i = 0; i < mainScrn; i++)
            xdelta += desk->screenGeometry(i).width();

        if (qsize.height() > scrRect.height())
            qsize.setHeight(scrRect.height() - 100);
        if (qsize.width() > scrRect.width())
            qsize.setWidth(scrRect.width() - 100);

        if (qpos.x() < xdelta)
            qpos.setX(qpos.x() + xdelta);
        if (qpos.x() > scrRect.topRight().x())
            qpos.setX(scrRect.topLeft().x() + 20);
        if (qpos.x() + qsize.width() < scrRect.topLeft().x())
            qpos.setX(scrRect.topLeft().x() + 20);
        if (qpos.y() > scrRect.bottomLeft().y())
            qpos.setY(scrRect.topLeft().y() + 20);
        if (qpos.y() < scrRect.topLeft().y())
            qpos.setY(scrRect.topLeft().y() + 20);
    }
    resize(qsize);
    move(qpos);

    default_model = settings.value("defaultModel", model).toString();
    default_dir = settings.value("defaultDir", app_dir).toString();
    current_dir = default_model;
    ss_exe = settings.value("ss_exe", exe).toString();
    R_exe = settings.value("r_exe", blank_exe).toString();
    settings.endGroup();

    settings.beginGroup("HelpWindow");
    ui->dockWidget_help->resize(settings.value("size").toSize());
    ui->dockWidget_help->move(settings.value("pos", QPoint(300,300)).toPoint());
    ui->dockWidget_help->setVisible(settings.value("visible", true).toBool());
    settings.endGroup();

    settings.beginGroup("ToolBar");
    ui->mainToolBar->setGeometry(settings.value("rect", QRect(350,330, 500, 35)).toRect());
    ui->mainToolBar->setVisible(settings.value("visible", true).toBool());
    settings.endGroup();
}

void MainWindow::openNewDirectory()
{
//    QString old_dir(default_dir);
    QString new_dir(QFileDialog::getExistingDirectory(this, tr("Select New Directory"),
                                default_dir, QFileDialog::ShowDirsOnly));
    if (!new_dir.isEmpty() && new_dir != default_dir)
    {
/*        copyFile(old_dir + QString("/wtatage.ss"), new_dir);
        copyFile(old_dir + QString("/ss.par"), new_dir);
        copyFile(old_dir + QString("/ss.bar"), new_dir);
        QString waa_str ("/wtatage.ss");
        QFile waa (old_dir + waa_str);
        if (waa.exists())
            waa.copy (new_dir + waa_str);*/

        current_dir = new_dir;
        changeNewDir(checkNewDir(current_dir));
        changeOutDir(checkOutDir(current_dir));
        files->new_directory(current_dir, true);
        dRun->setDir(current_dir);
    }
}

void MainWindow::createNewDirectory()
{
//    QString oldDir (default_dir);
    int btn =
    QMessageBox::question(this, tr("Create New Model"),
             tr("This will reset data to the default values and write the files to the selected directory.\nDo you wish to continue?"),
                          QMessageBox::Yes, QMessageBox::No);
    if (btn == QMessageBox::Yes)
    {
        returnToDefault();
        openNewDirectory();
//        copyFiles(oldDir, current_dir);
        refreshAll();
    }
    saveFiles();
}

void MainWindow::changeDirectory(QString dirname)
{
    if (!dirname.isEmpty())
    {
        fleets->readingNewModel();
        current_dir = dirname;
        QDir::setCurrent(current_dir);
        dRun->setDir(current_dir);
        readme->setDirectory(current_dir);
        setNewDir(current_dir);
        setOutDir(current_dir);
    }
}

void MainWindow::openDirectory(QString fname)
{
    QString title;
    QFileInfo start_file;
    bool repeat = true;
    title = QString ("Select Starter File");
    do
    {
        if (fname.isEmpty())
            fname = QFileDialog::getOpenFileName (this, tr(title.toUtf8()),
                               default_dir, tr("Starter files (starter.ss)"));

        if (fname.isEmpty())
            break;
        files->set_starter_file(fname);

        start_file = QFileInfo(files->get_starter_file());
        if (start_file.isReadable())
        {
            repeat = false;
        }
        else
        {
            int btn;
            repeat = true;
            fname.clear();
            title = QString("Invalid Starter File - Select File");
            btn = QMessageBox::question (this, tr("Unreadable file"),
                 tr("starter.ss is unreadable. Do you wish to select another file?"),
                  QMessageBox::Yes, QMessageBox::No);
            if (btn == QMessageBox::No)
                repeat = false;
        }
     }while (repeat);

    if (!fname.isEmpty())
    {
        fleets->readingNewModel();
        current_dir = start_file.absolutePath();
        QDir::setCurrent(current_dir);
        files->new_directory(current_dir);
        dRun->setDir(current_dir);
        readme->setDirectory(current_dir);
        readFiles();
        files->setReadWtAtAge(modelData->getReadWtAtAge());
    }

}

void MainWindow::copyDirectory()
{
    QString old_dir (current_dir);
    QString new_dir;

    int btn =
    QMessageBox::question(this, tr("Copy Current Model"),
             tr("This will copy the current model files and over-write any duplicate files in the selected directory.\nDo you wish to continue?"),
                          QMessageBox::Yes, QMessageBox::No);
    if (btn == QMessageBox::Yes)
    {
        openNewDirectory();
    }
    new_dir = QString (current_dir);
    if (new_dir.compare(old_dir) != 0)
    {
        copyFiles(old_dir, new_dir);
    }
}

void MainWindow::openControlFile()
{
    QString fname (QFileDialog::getOpenFileName (this, tr("Select Control File"),
        current_dir, tr("Control Files (*.ctl; *.ss);;All Files (*.*)")));
    if (!fname.isEmpty())
    {
        files->set_control_file(fname);
        files->write_starter_file();
        readFiles();
    }
}

void MainWindow::saveControlFile()
{
    QString fname = files->get_control_file();
    fname = QFileDialog::getSaveFileName(this, tr("Select Control File"),
            current_dir, tr("Control Files (*.ctl; *.ss);;All Files (*.*)"));
    if (!fname.isEmpty())
    {
        files->set_control_file(fname);
        saveFiles();
    }
}

void MainWindow::openDataFile()
{
    QString fname (QFileDialog::getOpenFileName (this, tr("Select Data File"),
        current_dir, tr("Data Files (*.ss; *.dat);;All Files (*.*)")));
    if (!fname.isEmpty())
    {
        files->set_data_file(fname);
        files->write_starter_file();
        readFiles();
    }
}

void MainWindow::saveDataFile()
{
    QString fname = files->get_data_file();
    fname = QFileDialog::getSaveFileName(this, tr("Select Data File"),
            current_dir, tr("Data Files (*.ss; *.dat);;All Files (*.*)"));
    if (!fname.isEmpty())
    {
        files->set_data_file(fname);
        saveFiles();
    }
}

void MainWindow::readFiles()
{
    bool worked = true;
    int choice = 1;
    fleets->selexAgeCurveClosed();
    fleets->selexSizeCurveClosed();
    fleets->set_current_fleet(0);
    population->setSRequationDialogVisible(false);
    modelData->reset();
#ifdef DEBUG
    QMessageBox::information(this, "Information - program flow", "Ready to read files.");
#endif
    worked = files->read_files(modelData);
    if (worked)
    {
        data->refresh();
        fleets->refresh();
        population->refresh();
        forecast->refresh();
    }
    else
    {
        float ver = files->get_version_number(files->getDatafileVersion());
        QMessageBox msgbx (this);
        msgbx.setIcon(QMessageBox::Question);
        msgbx.setWindowTitle("Datafile error");
        QString normal, detail;
        if (ver < 330)
        {
            msgbx.setWindowTitle("Datafile version mis-match");
            normal = QString("This program does not support datafiles earlier than 3.30\nWhat do you wish to do?");
            detail = QString("You may proceed in one of two ways:\n 1) Choose another starter.ss (a different model) or\n 2) Quit the program.");
        }
        else
        {
            normal = QString("There was a problem reading the data files.\nWhat do you wish to do?");
            detail = QString();
        }
        msgbx.setText(normal);
        msgbx.setDetailedText(detail);
        msgbx.addButton("Choose Another", QMessageBox::RejectRole);
        msgbx.addButton("Exit GUI", QMessageBox::NoRole);
        choice = msgbx.exec();
             if (choice == 0)
        {
            openDirectory();
        }
        else if (choice == 1)
        {
            if (!close())
                exit(1);
        }
    }
}

void MainWindow::saveFiles()
{
    files->write_files ();
}

void MainWindow::copyFiles(QString oldDir, QString newDir)
{
    QFileInfo qfi;
    // save standard files in current directory
    saveFiles();
    // copy readme.txt
    qfi = QFileInfo (oldDir + QString("/readme.txt"));
    if (qfi.exists())
        copyFile (qfi.absolutePath(), newDir);

    // copy ss.par
    qfi = QFileInfo (oldDir + QString("/ss.par"));
    if (qfi.exists())
        copyFile (qfi.absoluteFilePath(), newDir);

    // copy ss.bar
    qfi = QFileInfo (oldDir + QString("/ss.bar"));
    if (qfi.exists())
        copyFile (oldDir + QString("/ss.bar"), newDir);

    // copy wtatage.ss
    if (modelData->getReadWtAtAge())
    {
        copy_file (oldDir + QString("/wtatage.ss_new"), newDir + QString("/wtatage.ss"));
    }

    // check for output and new directories
    if (checkNewDir(oldDir))
    {
        new_dir = NEWDIR;
    }
    else
    {
        new_dir = QString();
    }
    files->setSsnewDir(new_dir);
    if (checkOutDir(oldDir))
    {
        output_dir = OUTDIR;
    }
    else
    {
        output_dir = QString();
    }
    files->setSsoutDir(output_dir);
}

void MainWindow::copyFile(QString fname, QString newDir)
{
    QString sname (fname.section('/', -1));
    QFile qfile (fname);
    if (qfile.exists())
        qfile.copy (QString("%1/%2").arg(newDir, sname));
}

void MainWindow::printFiles()
{
    files->print_files ();
}

QString MainWindow::getDataFile()
{
    return files->getDataFileName();
}
QString MainWindow::getControlFile()
{
    return files->getControlFileName();
}

void MainWindow::setReadWtAtAgeSS(int flag)
{
    modelData->setReadWtAtAge(flag);
    files->setReadWtAtAge(flag);
}

void MainWindow::createReadme()
{
    readme->setDirectory(current_dir);
    readme->createReadme();
}

void MainWindow::viewReadme()
{
    readme->setDirectory(current_dir);
    readme->viewReadme();
}

void MainWindow::mainTabChanged(int tab)
{
    switch(tab)
    {
    case MODEL:
        data->refresh();
        break;
    case FORECAST:
        forecast->refresh();
        break;
    case FLEETS:
        fleets->refresh();
        break;
    case POPULATION:
        population->refresh();
        break;
    }
}

void MainWindow::showFiles()
{
    set_checks_false();
    ui->stackedWidget->setCurrentIndex(FILES);
    ui->action_Files->setChecked(true);
    files->show_input_files();
}

void MainWindow::showModel()
{
    set_checks_false();
    ui->stackedWidget->setCurrentIndex(MODEL);
    ui->action_Model_Data->setChecked(true);

}

void MainWindow::showForecast()
{
    set_checks_false();
    ui->stackedWidget->setCurrentIndex(FORECAST);
    ui->action_Forecast->setChecked(true);
}

void MainWindow::showParameters()
{
    set_checks_false();
    ui->stackedWidget->setCurrentIndex(PARAMETERS);
    ui->action_View_parameter_list->setChecked(true);
}

void MainWindow::showPopulations()
{
    set_checks_false();
    ui->stackedWidget->setCurrentIndex(POPULATION);
    ui->action_Populations->setChecked(true);
}

void MainWindow::showSurveys()
{
    set_checks_false();
    ui->action_Surveys->setChecked(true);
}

void MainWindow::showFisheries()
{
    set_checks_false();
    ui->stackedWidget->setCurrentIndex(FLEETS);
    ui->action_Fleets->setChecked(true);
}

void MainWindow::helpGUI()
{
    Dialog_about_gui *about = new Dialog_about_gui (this);
    QString title(QString("%1 %2").arg(
                      tr("About"), getAppName()));
    about->setWindowTitle(title);
    connect (about, SIGNAL(showGuide()), SLOT(showGUIGuide()));
    connect (about, SIGNAL(showAboutQt()), SLOT(helpQt()));
    connect (about, SIGNAL(showCopyright()), this, SLOT(showCopyright()));

    about->exec();
    delete about;
}

void MainWindow::helpSS()
{
    Dialog_about *about = new Dialog_about (this);
    QString title(QString("%1 %2").arg(
                      tr("About"), tr("Stock Synthesis")));
    about->setWindowTitle(title);
    connect (about, SIGNAL(show_manual()), SLOT(showUserManual()));
    connect (about, SIGNAL(show_webpage(QString)), SLOT(showWebpage(QString)));
    connect (about, SIGNAL(show_copyright()), this, SLOT(showCopyright()));

    about->exec();
    delete about;
}

void MainWindow::helpNFT()
{
    Dialog_About_NFT *about_NFT = new Dialog_About_NFT (this);
    QString title(tr("About NOAA Fisheries Toolbox"));
    about_NFT->setWindowTitle(title);
    connect (about_NFT, SIGNAL(show_webpage(QString)), SLOT(showWebpage(QString)));

    about_NFT->exec();
    delete about_NFT;
}

void MainWindow::helpADMB()
{
    Dialog_About_ADMB *about_admb = new Dialog_About_ADMB (this);
    QString title (tr("About ADMB"));
    about_admb->setWindowTitle(title);
    connect (about_admb, SIGNAL(show_webpage(QString)), SLOT(showWebpage(QString)));

    about_admb->exec();
    delete about_admb;
}

void MainWindow::helpQt()
{
    QMessageBox::aboutQt (this, tr("About the Qt toolkit"));
}

void MainWindow::showCopyright()
{
    Dialog_copyright *copyright = new Dialog_copyright(this);
    QString title(tr("Copyright Notice"));
    copyright->setWindowTitle(title);
    copyright->exec();
    delete copyright;
}

void MainWindow::showLengthObs()
{
    showFisheries();
    fleets->showLengthObs();
}

void MainWindow::showAgeObs()
{
    showFisheries();
    fleets->showAgeObs();
}

void MainWindow::showMeanSAAObs()
{
    showFisheries();
    fleets->showMeanSAAObs();
}

void MainWindow::showGenSizeObs(int index)
{
    showFisheries();
    fleets->showGenSizeObs(index);
}

void MainWindow::showMorphObs()
{
    showFisheries();
    fleets->showMorphObs();
}

void MainWindow::showRecapObs()
{
    showFisheries();
    fleets->showRecapObs();
}

void MainWindow::showDocument(QString setting)
{
    QDir appdir (qApp->applicationDirPath());
    QString start ("");
    QStringList filter(QString("*.*"));
    QStringList files;
    QProcess *process = new QProcess (this);
    QString filename, fullname;
    QString command;
    QFileInfo qf;
    QSettings settings (app_copyright_org, app_name);
    settings.beginGroup("Documents");
    fullname = settings.value(setting, QString("")).toString();

    if (setting.compare("manual") == 0) {
        start = QString("SS_User_Manual");
        filter.clear();
        filter.append(QString("*.pdf"));
    }
    else if (setting.compare("technical") == 0) {
        start = QString("SS_Technical");
        filter.clear();
        filter.append(QString("*.pdf"));
    }
    else if (setting.compare("guiguide") == 0) {
        start = QString("Stock Synthesis");
        filter.clear();
        filter.append(QString("*.doc, *.docx"));
    }

    if (fullname.isEmpty())
    {
        // search application dir
        files.append(appdir.entryList(filter, QDir::Files, QDir::Name));
        for (int i = 0; i < files.count(); i++)
            if (files.at(i).startsWith(start))
                filename = files[i];
        fullname = QString(QString("%1/%2").arg(appdir.absolutePath(), filename));
//        filename.prepend(QString("%1/").arg(appdir.absolutePath()));
    }
    else {
        filename = fullname.section('/', -2, -1);
    }
    qf = QFileInfo(fullname);

    if (!qf.exists() || qf.isDir())
    {
        // dialog asking to find
        QString msg (tr(QString("The file %1 was not found.\n").arg(filename).toUtf8()));
        msg.append  (tr("Would you like to search for it manually?"));
        QMessageBox::StandardButton btn = QMessageBox::information(this,
            tr("File Not Found"), msg, QMessageBox::Yes | QMessageBox::No);
        // file open dialog
        if (btn == QMessageBox::Yes)
        {
            fullname = (QFileDialog::getOpenFileName (this, tr("Select File"),
                   qApp->applicationDirPath(), tr(QString("documentation files (%1)").arg(filter.at(0)).toUtf8())));
        }
        qf = QFileInfo(fullname);
    }

    // if okay file, show it in default application
    if (qf.exists())
    {
        settings.setValue(setting, fullname);
        command = QString("cmd /k \"%1\"").arg(fullname);
        process->start(command);
    }
    settings.endGroup();
}

void MainWindow::showUserManual()
{
    showDocument("manual");
/*    QDir appdir (qApp->applicationDirPath());
    QStringList filter(QString("*.pdf"));
    QStringList files;
    QProcess *process = new QProcess (this);
    QString filename;
    QString command;
    QFileInfo qf;
    QSettings settings (app_copyright_org, app_name);
    settings.beginGroup("Documents");
    filename = settings.value("manual", QString("")).toString();

    if (filename.isEmpty())
    {
        // search application dir
        files.append(appdir.entryList(filter, QDir::Files, QDir::Name));
        for (int i = 0; i < files.count(); i++)
            if (files.at(i).startsWith("SS_User_Manual"))
                filename = files[i];
        filename.prepend(QString("%1/").arg(appdir.absolutePath()));
    }
    qf = QFileInfo(filename);

    if (!qf.exists())
    {
        // dialog asking to find
        QString msg (tr("The User Manual document was not found.\n"));
        msg.append  (tr("Would you like to find the document?"));
        QMessageBox::StandardButton btn = QMessageBox::information(this,
            tr("File Not Found"), msg, QMessageBox::Yes | QMessageBox::No);
        // file open dialog
        if (btn == QMessageBox::Yes)
        {
            filename = (QFileDialog::getOpenFileName (this, tr("Select File"),
                   qApp->applicationDirPath(), tr("documentation files (*.pdf)")));
        }
        qf = QFileInfo(filename);
    }

    // if okay file, show it in a pdf reader
    if (qf.exists())
    {
        settings.setValue("manual", filename);
        command = QString("cmd /k %1").arg(filename);
        process->start(command);
    }
    settings.endGroup();*/
}

void MainWindow::showTechManual()
{
    showDocument("technical");
/*    QDir appdir (qApp->applicationDirPath());
    QStringList filter(QString("*.pdf"));
    QStringList files;
    QProcess *process = new QProcess (this);
    QString filename;
    QString command;
    QFileInfo qf;
    QSettings settings (app_copyright_org, app_name);
    settings.beginGroup("Documents");
    filename = settings.value("technical", QString("")).toString();

    if (filename.isEmpty())
    {
        // search application dir
        files.append(appdir.entryList(filter, QDir::Files, QDir::Name));
        for (int i = 0; i < files.count(); i++)
            if (files.at(i).startsWith("SS_technical"))
                filename = files[i];
        filename.prepend(QString("%1/").arg(appdir.absolutePath()));
    }
    qf = QFileInfo(filename);

    if (!qf.exists())
    {
        // dialog asking to find
        QString msg (tr("The Technical Description document was not found.\n"));
        msg.append  (tr("Would you like to find the document?"));
        QMessageBox::StandardButton btn = QMessageBox::information(this,
            tr("File Not Found"), msg, QMessageBox::Yes | QMessageBox::No);
        // file open dialog
        if (btn == QMessageBox::Yes)
        {
            filename = (QFileDialog::getOpenFileName (this, tr("Select File"),
                appdir.absolutePath(), tr("documentation files (*.pdf)")));
        }
        qf = QFileInfo(filename);
    }

    // if okay file, show it in a pdf reader
    if (qf.exists())
    {
        settings.setValue("technical", filename);
        command = QString("cmd /k %1").arg(filename);
        process->start(command);
    }
    settings.endGroup();*/
}

void MainWindow::showGUIGuide()
{
    showDocument("guiguide");
}

void MainWindow::showWebpage(QString pg)
{
    QDesktopServices web;
    web.openUrl(QUrl(pg));
}

void MainWindow::run()
{
    saveFiles();
//    Dialog_run dRun(this);
    dRun->setDir(current_dir);
    while (!QFileInfo(ss_exe).exists())
        locateSSExecutable();
    dRun->setExe(ss_exe);
    dRun->setRExe(R_exe);
    dRun->show();
}

void MainWindow::runComplete()
{
    files->read_run_num_file();
}

void MainWindow::runConversion()
{
//    int lastEstPhase = modelData->get_last_estim_phase();
    QString notice ("If the model has complex features, it may not convert completely.\n");
    notice.append("Also note that reading the parameter file is set to false.\n");
    QMessageBox::information(this, "NOTICE", notice);
    Dialog_run drun(this);
    // edit starter.ss to set max phase 0 and turn off reading parameter file
    modelData->set_last_estim_phase(0);
    files->setReadParFile(false);
    files->write_starter_file();
    // use run dialog to run ss_trans
/*    drun.setDir(current_dir);
    if (ss_trans_exe.isEmpty())
        locateSSConverter();
    else
    {
        if (!QFileInfo(ss_trans_exe).exists())
            locateSSConverter();
    }
    if (QFileInfo(ss_trans_exe).exists())
    {
        drun.setExe(ss_trans_exe);
        drun.setOptions("-nohess");
        drun.exec();
        // reset last estimation phase
        modelData->set_last_estim_phase(lastEstPhase);
        files->write_starter_file();
        // ask user for new directory and copy files there
        // save old directory
        QString old_dir (current_dir);
        // open new directory and move there
        openNewDirectory();
        if (old_dir == current_dir)
        {
            int btn =
            QMessageBox::question(this, tr("Same Directory"), tr("You may not select the same directory.\nDo you wish to try again?"), QMessageBox::Yes, QMessageBox::No);
            if (btn == QMessageBox::Yes)
                openNewDirectory();
        }
        if (old_dir != current_dir)
        {
            // copy files
            copy_file (old_dir + QString("/starter.ss_new"), current_dir + QString("/starter.ss"));
            files->read_starter_file("starter.ss");
            copy_file (old_dir + QString("/forecast.ss_new"), current_dir + QString("/forecast.ss"));
            copy_file (old_dir + QString("/data.ss_new"), current_dir + QString("/") + files->getDataFileName());
            copy_file (old_dir + QString("/control.ss_new"), current_dir + QString("/") + files->getControlFileName());
            copy_file (old_dir + QString("/ss.par"), current_dir + QString("/ss.par"));
            copy_file (old_dir + QString("/ss.bar"), current_dir + QString("/ss.bar"));
            if (QFile(old_dir + QString("/wtatage.ss")).exists())
            {
                copy_file (old_dir + QString("/wtatage.ss_new"), current_dir + QString("/wtatage.ss"));
            }
            // read data into GUI
            readFiles();
            modelData->set_last_estim_phase(lastEstPhase);
            files->write_starter_file();
        }
    }*/
}

void MainWindow::setDefaultModel()
{
    default_model = QString(current_dir);
}

void MainWindow::returnToDefault()
{
    openDirectory(default_model + "/starter.ss");
//    QDir cDir(current_dir);
//    cDir.cd(default_model);
//    current_dir = default_model;
//    readFiles();
}

void MainWindow::locateDirectory()
{
    QSettings settings (app_copyright_org, app_name);
    QString newdir(QFileDialog::getExistingDirectory(this, tr("Select New Directory"),
                                current_dir, QFileDialog::ShowDirsOnly));
    if (!newdir.isEmpty())
        default_dir = newdir;
    settings.beginGroup("MainWindow");
    settings.setValue ("defaultDir", default_dir);
    settings.endGroup();
}

void MainWindow::locateDocuments()
{
    QSettings settings (app_copyright_org, app_name);
    documentDialog dd (this);
    dd.exec();
}

void MainWindow::locateExecutable(QString &savename, QString type, QString hint)
{
    QString filename ("");
    QString dir = savename.section('/', 0, -2);
    QString filter(QString("applications (%1*.exe)").arg(hint));
    filename = (findFile (dir, type, filter));
    if (!filename.isEmpty())
        savename = filename;
}

void MainWindow::locateSSExecutable()
{
    // locate ss.exe
//    QSettings settings (app_copyright_org, app_name);
    locateExecutable(ss_exe, "SS Executable", "ss");
    changeExecutable(QString("ss_exe"), ss_exe);
    dRun->setExe(ss_exe);
/*    settings.beginGroup("MainWindow");
    settings.setValue("executable", ss_exe);
    settings.endGroup();*/
}
void MainWindow::locateSSConverter()
{
    // locate ss_trans.exe
//    locateExecutable(ss_trans_exe, "SS Converter", "ss_tr");
//    changeExecutable(QString("trans_exe"), ss_trans_exe);
}

void MainWindow::changeExecutable(QString key, QString filename) {
    QSettings settings (app_copyright_org, app_name);

    settings.beginGroup("MainWindow");
    settings.setValue(key, filename);
    settings.endGroup();
}

void MainWindow::locateRExecutable()
{
//    QSettings settings (app_copyright_org, app_name);
    locateExecutable(R_exe, "R Executable", "R");
    changeExecutable(QString("r_exe"), R_exe);
    dRun->setRExe(R_exe);
/*    settings.beginGroup("MainWindow");
    settings.setValue("Rexecutable", R_exe);
    settings.endGroup();*/
}

QString MainWindow::findFile(QString dir, QString title, QString filters)
{
    QString filename ("");
    QString str (QString ("Select %1 File").arg(title));

    filename = (QFileDialog::getOpenFileName (this, tr(str.toUtf8()),
        dir, tr(filters.toUtf8())));

    return filename;
}

void MainWindow::moveEvent(QMoveEvent *event) {
    int x = 0, y = 0;
    bool doMove = false;
    int width = 0, height = 0;
    int newScrn = QApplication::desktop()->screenNumber(this);
    QRect thisRect = QApplication::desktop()->screenGeometry(this);
    width = size().width();
    height = size().height();
    x = pos().x();
    y = pos().y();
    if (mainScrn != newScrn) {
        if (width > thisRect.width()) {
            width = thisRect.width();
            x = 0;
            doMove = true;
        }
        if (height > thisRect.height()) {
            height = thisRect.height();
            y = 0;
            doMove = true;
        }
    }
    if (doMove) {
        resize(width, height);
        move (x, y);
    }
    else {
        QMainWindow::moveEvent(event);
    }
}


void MainWindow::changeControlFile(QString fname)
{
    if (control_file.compare(fname) != 0)
    {
        control_file = fname;
    }
}

void MainWindow::changeDataFile(QString fname)
{
    if (data_file.compare(fname) != 0)
    {
        data_file = fname;
    }
}



int MainWindow::askMissingFile(QString name)
{
    QMessageBox mbox (this);
    int btn;
    QString query(QString("%1 file is not found in this directory or is not readable. \n").arg(name));
    query.append("Do you wish to create a new one?");
    mbox.setWindowTitle("Missing File");
    mbox.setText(query);
    mbox.setStandardButtons(QMessageBox::Yes |
                            QMessageBox::No |
                            QMessageBox::Cancel);
    btn = mbox.exec();
    return btn;
}

void MainWindow::increaseFont ()
{
    int f_size = main_font.pointSize();
    if (f_size < 18)
    {
        f_size += 1;
        setFontSize(f_size);
    }
}

void MainWindow::decreaseFont()
{
    int f_size = main_font.pointSize();
    if (f_size > 8)
    {
        f_size -= 1;
        setFontSize(f_size);
    }
}

void MainWindow::setFontSize(int fsize)
{
    main_font.setPointSize(fsize);
    setFont(main_font);
    readme->setFontSize(fsize);
}


void MainWindow::set_checks_false()
{
    ui->action_Files->setChecked(false);
    ui->action_Model_Data->setChecked(false);
    ui->action_Observed_data->setChecked(false);
    ui->action_Forecast->setChecked(false);
    ui->action_Fleets->setChecked(false);
    ui->action_Surveys->setChecked(false);
    ui->action_Populations->setChecked(false);
}

// copies a file, first deleting the target file if it exists
// (new_file is either in current_dir or is fully qualified)
void MainWindow::copy_file(QString old_file, QString new_file)
{
    QDir cur(current_dir);
    QFile old(old_file);
    if (cur.exists(new_file))
        cur.remove(new_file);
    old.copy(new_file);
}

void MainWindow::setNewDir(QString dir)
{
    if (checkNewDir(dir))
        files->setSsnewDir(NEWDIR);
    else
        files->setSsnewDir(QString());
}
void MainWindow::setOutDir(QString dir)
{
    if (checkOutDir(dir))
        files->setSsoutDir(OUTDIR);
    else
        files->setSsoutDir(QString());
}

bool MainWindow::checkNewDir(QString dir)
{
    QDir check(dir + "/" + NEWDIR);
    return check.exists();
}

bool MainWindow::checkOutDir(QString dir)
{
    QDir check(dir + '/' + OUTDIR);
    return check.exists();
}

void MainWindow::changeNewDir(bool flag)
{
    bool newDirExists = checkNewDir(current_dir);
    if (flag)
    {
        new_dir = QString("/" + NEWDIR);
        if (!newDirExists)
        {
            QDir::current().mkdir(current_dir + new_dir);
        }
    }
    else
    {
        if (newDirExists)
        {
            QDir nwDir(current_dir + new_dir);
            QStringList files(nwDir.entryList());
            for (int i = 0; i < files.count(); i++)
                nwDir.remove(files.at(i));
            QDir::current().rmdir(nwDir.path());
        }
        new_dir = QString();
    }
    files->setSsnewDir(new_dir);
}
void MainWindow::changeOutDir(bool flag)
{
    bool outDirExists = checkOutDir(current_dir);
    if (flag)
    {
        output_dir = QString("/" + OUTDIR);
        if (!outDirExists)
        {
            QDir::current().mkdir(current_dir + output_dir);
        }
    }
    else
    {
        if (outDirExists)
        {
            QDir outDir(current_dir + output_dir);
            QStringList files(outDir.entryList());
            for (int i = 0; i < files.count(); i++)
                outDir.remove(files.at(i));
            QDir::current().rmdir(outDir.path());
        }
        output_dir = QString();
    }
    files->setSsoutDir(output_dir);
}

