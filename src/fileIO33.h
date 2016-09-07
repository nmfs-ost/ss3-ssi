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

void readGrowthTimeVaryParams (ss_file *infile, ss_model *data, int gpat, QStringList parmlist, QString header);
void readSelexTimeVaryParams (ss_file *infile, ss_model *data, tablemodel *table, QStringList parmlist, QString header);

bool read33_forecastFile (ss_file *f_file, ss_model *data);
int write33_forecastFile (ss_file *f_file, ss_model *data);

bool read33_parameterFile (ss_file *pr_file, ss_model *data);
int write33_parameterFile (ss_file *pr_file, ss_model *data);

bool read33_profileFile(ss_file *pf_file, ss_model *data);
int write33_profileFile(ss_file *pf_file, ss_model *data);

#endif // FILEIO33_H

