#ifndef METADATA_H
#define METADATA_H

#include <QString>

#define app_name          ("Stock Synthesis Interface")
#define app_version_major  3
#define app_version_minor  3
#define app_version_sub    10
#define app_version_bugfix 0
#define app_release_level  " "  // a - alpha, b - beta, blank - release candidate
#define app_version_apply ("3.30.10")
#define app_copyright_date  __DATE__
#define app_copyright_org ("NOAA")

#define app_manual        ("SS330_User_Manual.pdf")
#define app_technical     ("SS_technical_description_2012.pdf")


QString getAppVersion ();
QString getAppAppliesTo ();
QString getAppName ();
QString getAppCopyright ();
QString getAppOrg ();
QString getAppUserManual ();
QString getAppTechDescription ();

QString getDatafileVersionString ();
void setDatafileVersionString (float ver);

#endif // METADATA_H
