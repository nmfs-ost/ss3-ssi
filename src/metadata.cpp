
#include "metadata.h"

QString datafile_version_str;


QString getAppVersion ()
{
    QString str(QString("%1.%2.%3%4").arg(
                    QString::number(app_version_major),
                    QString::number(app_version_minor),
                    QString::number(app_version_bugfix),
                    QString(app_release_level)));
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

QString getAppCopyright ()
{
    QString str(app_copyright_date);
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

QString getDatafileVersionString()
{
    return datafile_version_str;
}

void setDatafileVersionString(float ver)
{
    int major = (int)(ver * 100.0);
    datafile_version_str = QString(QString("%1.0.0").arg(QString::number(major)));
}
