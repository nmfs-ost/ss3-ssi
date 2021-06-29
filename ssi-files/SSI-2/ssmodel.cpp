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

void ssModel::readModel()
{

}

void ssModel::saveModel()
{

}

void ssModel::saveAsModel()
{

}
