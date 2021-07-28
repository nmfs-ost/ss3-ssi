#ifndef SSMODEL_H
#define SSMODEL_H

#include <ssfile.h>
#include <starterwidget.h>
#include <datawidget.h>
#include <controlwidget.h>
#include <forecastwidget.h>
#include <outputwidget.h>


#include <QObject>
#include <QDir>

class ssModel : public QObject
{
    Q_OBJECT
public:
    explicit ssModel(QObject *parent = nullptr);

public slots:
    void setStarter(starterWidget *start);
    void setData(dataWidget *dat);
    void setControl(controlWidget *ctrl);
    void setForecast(forecastWidget *fcast);
    void setOutput(outputWidget *out);

    void setDirectory(QString dir);
    bool readModel();
    bool saveModel();
    void saveAsModel();


signals:


private:
    ssFile *file;
    starterWidget *starter;
    dataWidget *data;
    controlWidget *control;
    forecastWidget *forecast;
    outputWidget *output;

    QString strtFile;
    QString dataFile;
    QString ctrlFile;
    QString fcstFile;
    QString wtaaFile;

    QDir directory;
};

#endif // SSMODEL_H
