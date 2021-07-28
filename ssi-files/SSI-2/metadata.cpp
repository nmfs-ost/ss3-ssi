#include "metadata.h"

#include <QString>
#include <QDate>

static int version_top = 3;
static int version_major = 30;
static int version_minor = 17;
static int version_bugfix = 5;
static QString version_rel_level(" "); // "a" - alpha, "b" - beta, " " - release

static QString ver;
static QString cpyright;
static QString create;

QString version()
{
    if (ver.isEmpty())
    {
        ver = QString("%1.%2.%3.%4%5")
                .arg(version_top)
                .arg(version_major)
                .arg(version_minor)
                .arg(version_bugfix)
                .arg(version_rel_level);
    }
    return ver;
}

QString copyright()
{
    if (cpyright.isEmpty())
    {
        cpyright.append("#Stock Synthesis (SS) is a work of the U.S. Government and is not subject to copyright protection in the United States.\n");
        cpyright.append("#Foreign copyrights may apply. See copyright.txt for more information.\n");
        cpyright.append("#_user_support_available_at:NMFS.Stock.Synthesis@noaa.gov\n");
        cpyright.append("#_user_info_available_at:https://vlab.noaa.gov/group/stock-synthesis\n");
    }
    return cpyright;
}

QString maintainer()
{
    if (create.isEmpty())
    {
        QString date(QString("%1").arg(QDate::currentDate().toString()));
        create = QString("#SSI-V%1;date;SSI_by_Neal_Schindler_using_Qt5\n");
    }
    return create;
}

int versionTop()
{
    return version_top;
}
int versionMajor()
{
    return version_major;
}
int versionMinor()
{
    return version_minor;
}
int versionBugFx()
{
    return version_bugfix;
}
QString versionRelease()
{
    return version_rel_level;
}
