#ifndef FILEIOGENERAL_H
#define FILEIOGENERAL_H

#include <QStringList>

#include "input_file.h"
#include "metadata.h"

int writeSSVersion (ss_file *file);
int writeVersionComment(ss_file *file);

QStringList readParameter (ss_file *file);
QStringList readShortParameter (ss_file *file);


#endif // FILEIOGENERAL_H
