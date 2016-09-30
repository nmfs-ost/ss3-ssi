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
