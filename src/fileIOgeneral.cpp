#include "fileIOgeneral.h"

int writeSSVersion(ss_file *file)
{
    return file->writeline(QString("#V%1").arg(getAppAppliesTo()));
}

int writeVersionComment(ss_file *file)
{
    int chars = 0;
    QString line;
    line = QString ("# File written by GUI version %1 for SS version %2").arg
            (getAppVersion(), getAppAppliesTo());
    chars = file->writeline (line);
    return chars;
}


QStringList readParameter(ss_file *file)
{
    QStringList datalist;
    for (int i = 0; i < 14; i++)
        datalist << file->get_next_value();
    return datalist;
}


QStringList readShortParameter(ss_file *file)
{
    QStringList datalist;
    for (int i = 0; i < 7; i++)
        datalist << file->get_next_value();
    return datalist;
}

bool useBlks(QStringList param)
{
    int val = 0;
    if (param.count() > 13)
        val = QString(param.at(12)).toInt();
    return (val != 0);
}

bool useDev(QStringList param)
{
    int val = 0;
    if (param.count() > 13)
        val = QString(param.at(8)).toInt();
    return (val != 0);
}

bool useEnv(QStringList param)
{
    int val = 0;
    if (param.count() > 13)
        val = QString(param.at(7)).toInt();
    return (val != 0);
}
