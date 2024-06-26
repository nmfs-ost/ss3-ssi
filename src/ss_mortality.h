/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SS_MORTALITY_H
#define SS_MORTALITY_H

#include "input_file.h"
#include "short_parameter.h"
#include "parametermodelTV.h"
#include <QString>

class ss_mortality
{
public:
    ss_mortality(QObject *parent = nullptr, int n_fisheries = 1, int n_surveys = 0);

    void reset();
    void setYears (int f_year, int num); // num may be number of years or last year

    void setStartYear(int value);
    void setNumYears(int value);

    void setNumFisheries(int value) {numFisheries = value;}
    void setNumSurveys(int value);

    float getBparkF() const;
    void setBparkF(float value);

    int getBparkYr() const;
    void setBparkYr(int value);

    int getMethod() const;
    void setMethod(int value);

    float getMaxF() const;
    void setMaxF(float value);

    float getStartF() const;
    void setStartF(float value);

    int getPhase() const;
    void setPhase(int value);

    int getNumTuningIters() const;
    void setNumTuningIters(int value);

    int getNumInputs() const {return fleetYearValues->rowCount();}
    void setNumInputs(int value) {fleetYearValues->setRowCount(value);}
    void setInputLine (int index, QStringList data) {fleetYearValues->setRowData(index, data);}
    QStringList getInputLine (int index) {return fleetYearValues->getRowData(index);}
    tablemodel *getInputModel () {return fleetYearValues;}

    int getNumInitialParams () {return initialParams->getParamCount();}
    void setNumInitialParams (int num) {initialParams->setParamCount(num);}
    void setInitialParam (int index, QStringList data) {initialParams->setParameter(index, data);}
    QStringList getInitialParam (int index) {return initialParams->getParameter(index);}
    tablemodel *getInitialParams () {return initialParams->getParameters();}

    tablemodel *getFleetF() {return fleetF;}
    int getNumFleetSpecF() {return fleetF->rowCount();}
    QStringList getFleetSpecF(int fleet) {return fleetF->getRowData(fleet);}
    void setFleetF(int fleet, float startF, int phaseF);

private:
    ss_model *ssModel;
    int firstYear;
    int numYears;
    int numFisheries;
    int numSurveys;
    float bparkF;
    int   bparkYr;
    int   method;
    float maxF;
    float startF;
    int   phase;
    int   numInputs;
    int   numTuningIters;

    tablemodel *fleetYearValues; // Method 2
    shortParameterTable *initialParams; // Method 3
    tablemodel *fleetF; // Method 4

    QString m_text;
};

#endif // SS_MORTALITY_H
