#ifndef METADATA_H
#define METADATA_H

#include <QString>

QString version();
QString copyright();
QString maintainer();

int versionTop();
int versionMajor();
int versionMinor();
int versionBugFx();
QString versionRelease();

#endif // METADATA_H
