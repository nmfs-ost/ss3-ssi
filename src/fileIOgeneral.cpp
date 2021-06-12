#include "fileIOgeneral.h"

int writeSSVersion(ss_file *file)
{
    return file->writeline(QString("#V%1").arg(getAppAppliesTo()));
}

int writeVersionComment(ss_file *file)
{
    int chars = 0;
    QString line;
    line = QString ("#V%1;_Stock_Synthesis_by_Richard_Methot_(NOAA)").arg
            (getAppAppliesTo());
    chars = file->writeline (line);
    line = QString ("#_File written by GUI version %1").arg(getAppVersion());
    chars += file->writeline (line);
    line = QString("#Stock Synthesis (SS) is a work of the U.S. Government and is not subject to copyright protection in the United States.");
    chars += file->writeline(line);
    line = QString("#Foreign copyrights may apply. See copyright.txt for more information.");
    chars += file->writeline(line);
    line = QString("#_user_support_available_at:NMFS.Stock.Synthesis@noaa.gov");
    chars += file->writeline(line);
    line = QString("#_user_info_available_at:https://vlab.noaa.gov/group/stock-synthesis");
    chars += file->writeline(line);

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
