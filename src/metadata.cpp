
#include "metadata.h"

#include <QDate>
#include <QString>


#include <QArrayData>

static QString datafile_version_str;

QString getAppVersion ()
{
    QString str(QString("%1.%2.%3.%4%5").
                    arg(QString::number(app_version_major)).
                    arg(QString::number(app_version_minor)).
                    arg(QString::number(app_version_sub)).
                    arg(QString::number(app_version_bugfix)).
                    arg(QString(app_release_level)));
    return str;
}

QString getAppAppliesTo ()
{
    QString str(app_version_apply);
    return str;
}

QString getAppName ()
{
    QString str(app_name);
    return str;
}

QString getAppBuildDate ()
{
    QString str(app_build_date);
    return str;
}

QString getAppCopyright ()
{
    QString str(app_build_date);
    return str;
}

QString getAppOrg ()
{
    QString str(app_copyright_org);
    return str;
}

QString getAppUserManual ()
{
    QString str(app_manual);
    return str;
}

QString getAppTechDescription ()
{
    QString str(app_technical);
    return str;
}

QString getAppGuiManual ()
{
    QString str(app_gui_manual);
    return str;
}

QString getDatafileVersionString()
{
    return datafile_version_str;
}

void setDatafileVersionString(float ver)
{
    int major = static_cast<int>(ver * 100.0);
    datafile_version_str = QString(QString("%1.03.0").arg(QString::number(major)));
}
