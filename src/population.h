/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef POPULATION_H
#define POPULATION_H

#include <QWidget>

//#include "area.h"
#include "parametermodel.h"
#include "ss_observation.h"
#include "composition.h"
#include "growth.h"
#include "ss_recruitment.h"
#include "ss_mortality.h"
#include "ss_movement.h"
#include "ss_fecundity.h"
#include "seasonaleffectsmodel.h"

class population : public QObject
{
    Q_OBJECT
public:
    population(ss_model *parent = 0);
    ~population ();

    void reset();

    tablemodel *meanBwtModel;


public slots:
    void setStartYear (int yr);
    void setTotalYears (int yrs);
    void setNumAges (int ages);
    void set_gender (int gender) {i_gender = gender;}
    int gender () {return i_gender;}
    void set_mean_bwt_df (int deg_free) {i_mean_bwt_deg_freedom = deg_free;}
    int mean_bwt_df () {return i_mean_bwt_deg_freedom;}
    void set_mean_body_wt_count (int count) {i_mean_bwt_count = count; meanBwtModel->setRowCount(count);}
    int mean_bwt_count () {return meanBwtModel->rowCount();}
    void setMeanBwt (int index, QStringList values) {meanBwtModel->setRowData(index, values);}
    QStringList getMeanBwt (int index) {return meanBwtModel->getRowData(index);}

//    void readSeasonalEffects(ss_file *input);
//    QString writeSeasonalEffects();

    float get_frac_female() const;
    void set_frac_female(float value);

    spawn_recruit *SR() {return pop_recruitment;}
    ss_movement *Move() {return pop_movement;}
    ss_fecundity *Fec() {return pop_fecundity;}
    ss_growth * Grow() {return pop_growth;}
    ss_mortality *M () {return pop_mortality;}

    QStringList getFractionFemale();
    void setFractionFemale (QStringList data);

    QStringList getSeasParamSetup();
    void setSeasParamSetup (QStringList values);

    bool getFemWtLen1() const;
    void setFemWtLen1(bool value);
    bool getFemWtLen2() const;
    void setFemWtLen2(bool value);
    bool getMaturity1() const;
    void setMaturity1(bool value);
    bool getMaturity2() const;
    void setMaturity2(bool value);
    bool getFecund1() const;
    void setFecund1(bool value);
    bool getFecund2() const;
    void setFecund2(bool value);
    bool getMaleWtLen1() const;
    void setMaleWtLen1(bool value);
    bool getMaleWtLen2() const;
    void setMaleWtLen2(bool value);
    bool getL1() const;
    void setL1(bool value);
    bool getK() const;
    void setK(bool value);
    void setNumSeas (int seas);
    int getNumSeas () {return iNumSeas;}
//    void setNumSeasParams ();
    int getNumSeasParams () {return seasparamtable->getNumParameters();}// getParamCount();}
//    void addSeasonalParam (QStringList data) {setSeasonalParam(seasparamtable->getNumParams(), data);}
    void setSeasonalParam (int index, QStringList data);
    QStringList getSeasonalParam (int index);
    tablemodel *getSeasonalParams() {return seasparamtable->getParamTable();}
//    void insertSeasParams (int num);
//    void removeSeasParams (int num);

signals:

private:
    spawn_recruit *pop_recruitment;
    ss_movement *pop_movement;
    ss_fecundity *pop_fecundity;
    ss_growth *pop_growth;
    ss_mortality *pop_mortality;

    float f_frac_female;
    QStringList fractionFem;

    int i_gender;
    int i_mean_bwt_deg_freedom;
    int i_mean_bwt_count;

    bool femwtlen1, femwtlen2, mat1, mat2, fec1, fec2;
    bool malewtlen1, malewtlen2, L1, K;

    int iNumYears;
    int iNumSeas;
    int iStartYear;
    int iNumAges;

    seasonalEffectsModel *seasparamtable;
//    setupShortParamModel *seasparamtable;
//    shortParameterModel *seasparamtable;

};

#endif // POPULATION_H
