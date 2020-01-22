/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SD_REPORTING_H
#define SD_REPORTING_H

#include "long_parameter.h"
#include "tablemodel.h"
#include <QStringList>

class sd_reporting
{
public:
    sd_reporting();
    ~sd_reporting();

    void setActive (bool flag);
    void setActive (int value);
    bool getActive () {return reporting;}

/*    void setSpecs (QStringList data);
    QStringList getSpecs ();
    tablemodel *getSpecModel() {return specs;}*/

    void setSelex (QStringList data);
    QStringList getSelex ();
    tablemodel *getSelexModel () {return selex;}
    int getNumSelexBins();
    void setNumSelexBins (int num);
    QStringList getSelexBins ();
    void setSelexBins (QStringList data);
    tablemodel *getSelexBinModel () {return selexBins;}

    void setGrowth (QStringList data);
    QStringList getGrowth ();
    tablemodel *getGrowthModel () {return growth;}
    int getNumGrowthBins();
    void setNumGrowthBins (int num);
    QStringList getGrowthBins ();
    void setGrowthBins (QStringList data);
    tablemodel * getGrowthBinModel () {return growthBins;}

    void setNumAtAge (QStringList data);
    QStringList getNumAtAge ();
    tablemodel *getNumAtAgeModel () {return numAtAge;}
    int getNumNumAtAgeBins();
    void setNumNumAtAgeBins (int num);
    QStringList getNumAtAgeBins ();
    void setNumAtAgeBins (QStringList data);
    tablemodel *getNumAtAgeAgeModel() {return numAtAgeAges;}


private:
    bool reporting;
    tablemodel *selex;
    tablemodel *growth;
    tablemodel *numAtAge;
    tablemodel *selexBins;
    tablemodel *growthBins;
    tablemodel *numAtAgeAges;

    QStringList specsHeader;
    longParameter vector_selex_bins;
    longParameter vector_growth_ages;
    longParameter vector_nat_ages;

};

#endif // SD_REPORTING_H
