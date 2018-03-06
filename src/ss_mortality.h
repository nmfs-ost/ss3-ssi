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
    ss_mortality(ss_model *parent = 0, int n_fisheries = 1, int n_surveys = 0);

    void reset();
    void setYears (int f_year, int num); // num may be number of years or last year
    void fromFile (ss_file *file, int num);
    QString toText();

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

    int getNumInputs() const {return parameterTable->getParamCount();}
    void setNumInputs(int value) {parameterTable->setParamCount(value);}
    void setInputLine (int index, QStringList data) {parameterTable->setParameter(index, data);}
    QStringList getInputLine (int index) {return parameterTable->getParameter(index);}
    tablemodel *getInputModel () {return parameterTable->getParameters();}

    int getNumInitialParams () {return initialParams->getParamCount();}
    void setNumInitialParams (int num) {initialParams->setParamCount(num);}
    void setInitialParam (int index, QStringList data) {initialParams->setParameter(index, data);}
    QStringList getInitialParam (int index) {return initialParams->getParameter(index);}
    tablemodel *getInitialParams () {return initialParams->getParameters();}

private:
    ss_model *parnt;
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

    shortParameterTable *initialParams;
    shortParameterTable *parameterTable;

    QString m_text;
};

#endif // SS_MORTALITY_H
