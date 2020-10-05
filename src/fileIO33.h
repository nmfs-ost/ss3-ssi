/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef FILEIO33_H
#define FILEIO33_H

#include <QStringList>

#include "input_file.h"
#include "model.h"
#include "growth_pattern.h"

bool read33_dataFile (ss_file *d_file, ss_model *data);
int write33_dataFile (ss_file *d_file, ss_model *data);

bool read33_controlFile (ss_file *c_file, ss_model *data);
int write33_controlFile (ss_file *c_file, ss_model *data);

void readTimeVaryParams (ss_file *infile, ss_model *data, tablemodel *paramTable, int varRead, tablemodel *varParamTable);
int readTimeVaryBlockParams(ss_file *infile, ss_model *data, int value, int fnx, QString hdr, int row, tablemodel *varParamTable);
int readTimeVaryDevParams (ss_file *infile, ss_model *data, int value, QString hdr, int row, tablemodel *varParamTable);
int readTimeVaryEnvParams(ss_file *infile, ss_model *data, int value, QString hdr, int row, tablemodel *varParamTable);

int writeTimeVaryParams (ss_file *outfile, ss_model *data, tablemodel *table, QStringList parmlist);

bool read33_forecastFile (ss_file *f_file, ss_model *data);
int write33_forecastFile (ss_file *f_file, ss_model *data);

bool read33_parameterFile (ss_file *pr_file, ss_model *data);
int write33_parameterFile (ss_file *pr_file, ss_model *data);

bool read33_profileFile(ss_file *pf_file, ss_model *data);
int write33_profileFile(ss_file *pf_file, ss_model *data);

bool negateParameterPhase(QStringList &datalist);

#endif // FILEIO33_H

