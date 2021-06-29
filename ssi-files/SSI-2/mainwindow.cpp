#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("SS Interface by Files"));

    connect (ui->actionOpenModel, SIGNAL(triggered()), this, SLOT(readModel()));
    connect (ui->actionSaveModel, SIGNAL(triggered()), this, SLOT(saveModel()));

    starter = new starterWidget(this);
    ui->verticalLayout_starter->addWidget(starter);

    data = new dataWidget(this);
    ui->verticalLayout_data->addWidget(data);

    control = new controlWidget(this);
    ui->verticalLayout_control->addWidget(control);

    forecast = new forecastWidget(this);
    ui->verticalLayout_forecast->addWidget(forecast);

    output = new outputWidget(this);
    ui->verticalLayout_output->addWidget(output);

    model = new ssModel(this);
    model->setStarter(starter);
    model->setData(data);
    model->setControl(control);
    model->setForecast(forecast);
    model->setOutput(output);

    connectAll();

    // read Settings
    // defaultModel = settings(something);
    currentDir = QString(QString("%1/default").arg(QDir::currentPath()));
    model->setDirectory(currentDir);
    setSaved();

    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    // save Settings

    delete ui;
}

void MainWindow::setTabWidget(int page, QWidget *widget)
{
    ui->tabWidget->setCurrentIndex(page);
    QVBoxLayout layout(ui->tabWidget->currentWidget());
    layout.addWidget(widget);
}

void MainWindow::setUnsaved(bool flag)
{
    unsavedChanges = flag;
    showUnsaved(unsavedChanges);
}

void MainWindow::setSaved(bool flag)
{
    unsavedChanges = !flag;
    showUnsaved(unsavedChanges);
}

void MainWindow::showUnsaved(bool flag)
{
    QString label(currentDir);
    if (flag)
        label.append(" *");
    ui->label_model->setText(label);
}

void MainWindow::connectAll()
{
    connect (starter, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
    connect (data, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
    connect (control, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
    connect (forecast, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
}

void MainWindow::disconnectAll()
{
    disconnect (starter, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
    disconnect (data, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
    disconnect (control, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
    disconnect (forecast, SIGNAL(dataChanged()), this, SLOT(setUnsaved()));
}

void MainWindow::readSettings()
{

}

void MainWindow::writeSettings()
{

}

void MainWindow::readModel()
{
    QString starterFile(QString("%1/starter.ss").arg(currentDir));
    QString dataFile, controlFile;
    QString forecastFile(QString("%1/forecast.ss").arg(currentDir));
    // read files from current dir
    starter->readFile(currentDir);
    dataFile = starter->getDataFile();
    controlFile = starter->getControlFile();
    data->readFile(dataFile);
    control->readFile(controlFile);
    forecast->readFile(forecastFile);
}

void MainWindow::saveModel()
{
    // write the files in current dir
    starter->writeFile();
    data->writeFile();
    control->writeFile();
    forecast->writeFile();
    // optional files
    // if (starter->get )
}

void MainWindow::saveModelAs()
{
    // get new directory

    // change to new directory

    saveModel();
}

void MainWindow::setDefaultModel(QString dir)
{
    currentDir = dir;
}

void MainWindow::readDefaultModel()
{
    currentDir = defaultDir;
    readModel();
}

void MainWindow::viewModelInfo()
{

}


