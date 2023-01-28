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
    line.append(QString(";File_written_by_SSI_version_%1").arg(getAppVersion()));
    chars = file->writeline (line);
    line = QString("#_Stock_Synthesis_is_a_work_of_the_U.S._Government_and_is_not_subject_to_copyright_protection_in_the_United_States.");
    chars += file->writeline(line);
    line = QString("#_Foreign_copyrights_may_apply._See_copyright.txt_for_more_information.");
    chars += file->writeline(line);
    line = QString("##_User_support_available_at:NMFS.Stock.Synthesis@noaa.gov");
    chars += file->writeline(line);
    line = QString("#_User_info_available_at:https://vlab.noaa.gov/group/stock-synthesis");
    chars += file->writeline(line);
    line = QString("#_Source_code_at:_https://github.com/nmfs-stock-synthesis/stock-synthesis");
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
