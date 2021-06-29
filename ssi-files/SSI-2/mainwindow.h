#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

#include <ssfile.h>
#include <ssmodel.h>
#include <starterwidget.h>
#include <datawidget.h>
#include <controlwidget.h>
#include <forecastwidget.h>
#include <outputwidget.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setUnsaved(bool flag = true);
    void setSaved(bool flag = true);
    void showUnsaved(bool flag = true);

    void readSettings();
    void writeSettings();

    void readModel();
    void saveModel();
    void saveModelAs();

    void setDefaultModel(QString dir = QString());
    void readDefaultModel();
    void viewModelInfo();

private slots:
    void setTabWidget(int page, QWidget *widget);
    void connectAll();
    void disconnectAll();

private:
    Ui::MainWindow *ui;

    ssModel *model;
    starterWidget *starter;
    dataWidget *data;
    controlWidget *control;
    forecastWidget *forecast;
    outputWidget *output;

//    ssFileViewer *fileViewer;

    QString user;
    QString defaultDir;
    QString currentDir;
    bool unsavedChanges;
};
#endif // MAINWINDOW_H
