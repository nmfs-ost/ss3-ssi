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

class population : public QObject
{
    Q_OBJECT
public:
    population(QWidget *parent = 0);
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

    void readSeasonalEffects(ss_file *input);
    QString writeSeasonalEffects();

    float get_frac_female() const;
    void set_frac_female(float value);

    spawn_recruit *SR() {return pop_recruitment;}
    ss_movement *Move() {return pop_movement;}
    ss_fecundity *Fec() {return pop_fecundity;}
    ss_growth * Grow() {return pop_growth;}
    ss_mortality *M () {return pop_mortality;}

    QStringList getFractionFemale();
    void setFractionFemale (QStringList data);

    bool getFemwtlen1() const;
    void setFemwtlen1(bool value);
    void changeFemWtLn1 (bool flag);
    bool getFemwtlen2() const;
    void setFemwtlen2(bool value);
    void changeFemWtLn2 (bool flag);
    bool getMat1() const;
    void setMat1(bool value);
    void changeMaturity1 (bool flag);
    bool getMat2() const;
    void setMat2(bool value);
    void changeMaturity2 (bool flag);
    bool getFec1() const;
    void setFec1(bool value);
    void changeFecundity1(bool flag);
    bool getFec2() const;
    void setFec2(bool value);
    void changeFecundity2(bool flag);
    bool getMalewtlen1() const;
    void setMalewtlen1(bool value);
    void changeMaleWtLn1(bool flag);
    bool getMalewtlen2() const;
    void setMalewtlen2(bool value);
    void changeMaleWtLn2(bool flag);
    bool getL1() const;
    void setL1(bool value);
    void changeL1(bool flag);
    bool getK() const;
    void setK(bool value);
    void changeK(bool flag);
    void setNumSeas (int seas);
    int getNumSeas () {return iNumSeas;}
    void setNumSeasParams ();
    int getNumSeasParams () {return seasparamtable->rowCount();}
    void addSeasParam (QStringList data) {setSeasParam(seasparamtable->rowCount(), data);}
    void setSeasParam (int index, QStringList data);
    QStringList getSeasParam (int index);
    parametermodel *getSeasonalParams() {return seasparamtable;}
    void insertSeasParams (int num);
    void removeSeasParams (int num);

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

    parametermodel *seasparamtable;

};

#endif // POPULATION_H