/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef FILEIOGENERAL_H
#define FILEIOGENERAL_H

#include <QStringList>

#include "input_file.h"
#include "metadata.h"

int writeSSVersion (ss_file *file);
int writeVersionComment(ss_file *file);

QStringList readParameter (ss_file *file);
QStringList readShortParameter (ss_file *file);

bool useBlks (QStringList param);
bool useDev (QStringList param);
bool useEnv (QStringList param);

#endif // FILEIOGENERAL_H
