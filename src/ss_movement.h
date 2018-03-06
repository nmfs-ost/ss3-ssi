/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SS_MOVEMENT_H
#define SS_MOVEMENT_H

#include "tablemodel.h"
#include "parametermodelTV.h"
#include "setupmodel.h"

class ss_movement
{
public:
    ss_movement(ss_model *parent = 0, int n_fleets = 1);
    ~ss_movement ();

    void reset();

    void setYears (int f_yr, int num);

    int num_fleets;
    int first_year;
    int num_years;

    tablemodel *getMovementDefs() {return movement_defs;}

    void setNumDefs(int value) {movement_defs->setRowCount(value);}
    int getNumDefs() {return movement_defs->rowCount();}

    void addDefinition (QStringList valuelist) {setDefinition(getNumDefs(), valuelist);}
    QStringList getDefinition (int index);
    void setDefinition (int index, QStringList valuelist);

    tablemodel *getMovementParams() {return movement_parms->getParamTable();}
    int getNumParams () {return movement_parms->getNumParams();}
    void setNumParams (int value) {movement_parms->setNumParams(value);}
    QStringList getParameter (int index) {return movement_parms->getParameter(index);}
    void setParameter (int index, QStringList valuelist);
    QString getParamHeader (int index) {return movement_parms->getParamHeader(index);}
    void setParamHeader (int index, QString hdr) {movement_parms->setParamHeader(index, hdr);}

    int getMethod() const;
    void setMethod(int value);

    int getNumAreas() const;
    void setNumAreas(int value);

    float getFirstAge() const;
    void setFirstAge(float value);

private:
    ss_model *parnt;
    int method;
    int numAreas;
    float firstAge;

    QStringList defHeader;

    tablemodel *movement_defs;
    longParameterModel *movement_parms;
//    parameterModelTV *movement_parms;
};

#endif // SS_MOVEMENT_H
