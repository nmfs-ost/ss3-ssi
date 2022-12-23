/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef METADATA_H
#define METADATA_H

#include <QString>
#include "dialog_copyright.h"

#define app_name          ("Stock Synthesis Interface")
#define app_version_major  3
#define app_version_minor  30
#define app_version_sub    20
#define app_version_bugfix 02
#define app_release_level  " "  // a - alpha, b - beta, blank - release candidate
#define app_version_apply ("3.30.20")
#define app_build_date    __DATE__
#define app_copyright_org ("NOAA")

#define app_manual        ("SS330_User_Manual.pdf")
#define app_technical     ("") //("SS_technical_description_2012.pdf")
#define app_gui_manual    ("")


QString getAppVersion ();
QString getAppAppliesTo ();
QString getAppName ();
QString getAppBuildDate ();
QString getAppCopyright ();
QString getAppOrg ();
QString getAppUserManual ();
QString getAppTechDescription ();
QString getAppGuiManual ();

Dialog_copyright *copyrightNotice();

QString getDatafileVersionString ();
void setDatafileVersionString (float ver);

#endif // METADATA_H
