#include "ssmodel.h"

ssModel::ssModel(QObject *parent) : QObject(parent)
{
    starter = nullptr;
    data = nullptr;
    control = nullptr;
    forecast = nullptr;
    output = nullptr;
}

void ssModel::setStarter(starterWidget *start)
{
    starter = start;
}

void ssModel::setData(dataWidget *dat)
{
    data = dat;
}

void ssModel::setControl(controlWidget *ctrl)
{
    control = ctrl;
}

void ssModel::setForecast(forecastWidget *fcast)
{
    forecast = fcast;
}

void ssModel::setOutput(outputWidget *out)
{
    output = out;
}

void ssModel::setDirectory(QString dir)
{
    directory = QDir(dir);
}

bool ssModel::readModel()
{
    bool okay = true;
    QString dir = directory.path();
    strtFile = QString("%1/starter.ss").arg(dir);
    okay = starter->readFile(strtFile);
    if (okay) {
    //    if (starter->getReadWtAtAge()) {
            wtaaFile = QString("%1/wtatage.ss").arg(dir);
    //    }
    //    else {
    //        wtaaFile = QString();
    //    }
        dataFile = QString("%1/%2").arg(dir, starter->getDataFile());
        okay = data->readFile(dataFile);
    }
    if (okay) {
        ctrlFile = QString("%1/%2").arg(dir, starter->getControlFile());
        okay = control->readFile(ctrlFile);
    }
    if (okay) {
        fcstFile = QString("%1/forecast.ss").arg(dir);
        okay = forecast->readFile(fcstFile);
    }
    return okay;
}

bool ssModel::saveModel()
{

}

void ssModel::saveAsModel()
{

}
