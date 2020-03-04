#include "sd_reporting.h"

sd_reporting::sd_reporting()
{
    QString header;
    QStringList headers;
    selex = new tablemodel();
    selex->setColumnCount(4);
    selex->setRowCount(1);
    header = QString("Selectivity");
    selex->setRowHeader(0, header);
    header.clear();
    headers << "fleet" << "len/age" << "year" << "num bins";
    selex->setHeader(headers);
    headers.clear();
    selexBins = new tablemodel();
    selexBins->setColumnCount(1);
    selexBins->setRowCount(1);
    header = QString("bins");
    selexBins->setRowHeader(0, header);
    header.clear();

    growth = new tablemodel();
    growth->setColumnCount(2);
    growth->setRowCount(1);
    header = QString("Growth");
    growth->setRowHeader(0, header);
    header.clear();
    headers << "pattern" << "num ages";
    growth->setHeader(headers);
    headers.clear();
    growthBins = new tablemodel();
    growthBins->setColumnCount(1);
    growthBins->setRowCount(1);
    header = QString("ages");
    growthBins->setRowHeader(0, header);
    header.clear();

    numAtAge = new tablemodel();
    numAtAge->setColumnCount(3);
    numAtAge->setRowCount(1);
    header = QString("Numbers at Age");
    numAtAge->setRowHeader(0, header);
    header.clear();
    headers << "area" << "year" << "num ages";
    numAtAge->setHeader(headers);
    headers.clear();
    numAtAgeAges = new tablemodel();
    numAtAgeAges->setColumnCount(1);
    numAtAgeAges->setRowCount(1);
    header = QString("ages");
    numAtAgeAges->setRowHeader(0, header);
    header.clear();

    natMort = new tablemodel();
    natMort->setColumnCount(2);
    natMort->setRowCount(1);
    header = QString("Natural Mort");
    natMort->setRowHeader(0, header);
    header.clear();
    headers << "pattern" << "num ages";
    natMort->setHeader(headers);
    headers.clear();
    natMortAges = new tablemodel();
    natMortAges->setColumnCount(1);
    natMortAges->setRowCount(1);
    header = QString("ages");
    natMortAges->setRowHeader(0, header);
    header.clear();

/*    specsHeader << "sel type" << "len/age" << "year" << "N sel bins";
    specsHeader << "Gr Pat" << "N Gr ages";
    specsHeader << "NatAge_area" << "NatAge_yr" << "N Natages";
    specs->setHeader(specsHeader);

    bins = new tablemodel();
    bins->setRowCount(3);
    bins->setRowHeader(0, QString("Selex std bins"));
    bins->setRowHeader(1, QString("Growth std bins"));
    bins->setRowHeader(2, QString("NatAge std bins"));*/
}

sd_reporting::~sd_reporting()
{
    delete selex;
    delete growth;
    delete numAtAge;
    delete natMort;
    delete selexBins;
    delete growthBins;
    delete numAtAgeAges;
    delete natMortAges;
}

void sd_reporting::setActive(bool flag)
{
    if (flag) {
        reporting = 1;
    }
    else {
        reporting = 0;
    }
//    reporting = flag;
/*    if (reporting)
    {
        specs->setRowCount(1);
        setNumBins(3);
    }
    else
    {
        specs->setRowCount(0);
        setNumBins(0);
    }
*/
}

void sd_reporting::setActive(int value)
{
    reporting = value;
}
/*
void sd_reporting::setSpecs (QStringList data)
{
    specs->setRowData(0, data);
    int binscount = getNumSelexBins();
    int temp = getNumGrowthBins();
    if (binscount < temp)
        binscount = temp;
    temp = getNumNumAtAgeBins();
    if (binscount < temp)
        binscount = temp;
    setNumBins(binscount);
}

QStringList sd_reporting::getSpecs ()
{
    return specs->getRowData(0);
}

void sd_reporting::setNumBins (int num)
{
    bins->setColumnCount(num);
}*/

// selex reporting
void sd_reporting::setSelex (QStringList data)
{
    int num = data.last().toInt();
    selex->setRowData(0, data);
    setNumSelexBins(num);
}

QStringList sd_reporting::getSelex ()
{
    return selex->getRowData(0);
}

int sd_reporting::getNumSelexBins()
{
//    QStringList spc = specs->getRowData(0);
    int num = selexBins->columnCount();//spc.at(3).toInt();
    return num;
}

void sd_reporting::setNumSelexBins (int num)
{
    selexBins->setColumnCount(num);
}

QStringList sd_reporting::getSelexBins ()
{
    return selexBins->getRowData(0);
}

void sd_reporting::setSelexBins (QStringList data)
{
    selexBins->setRowData(0, data);
}

// growth reporting
void sd_reporting::setGrowth (QStringList data)
{
    int num = data.last().toInt();
    growth->setRowData(0, data);
    setNumGrowthBins(num);
}

QStringList sd_reporting::getGrowth ()
{
    return growth->getRowData(0);
}

int sd_reporting::getNumGrowthBins()
{
//    QStringList spc = specs->getRowData(0);
    int num = growthBins->columnCount();
    return num;
}

void sd_reporting::setNumGrowthBins(int num)
{
    growthBins->setColumnCount(num);
}

QStringList sd_reporting::getGrowthBins ()
{
    return growthBins->getRowData(0);
}

void sd_reporting::setGrowthBins (QStringList data)
{
    growthBins->setRowData(0, data);
}

// num at Age reporting
void sd_reporting::setNumAtAge (QStringList data)
{
    int num = data.last().toInt();
    numAtAge->setRowData(0, data);
    setNumNumAtAgeBins(num);
}

QStringList sd_reporting::getNumAtAge ()
{
    return numAtAge->getRowData(0);
}

int sd_reporting::getNumNumAtAgeBins()
{
    int num = numAtAgeAges->columnCount();
    return num;
}

void sd_reporting::setNumNumAtAgeBins(int num)
{
    numAtAgeAges->setColumnCount(num);
}

QStringList sd_reporting::getNumAtAgeBins ()
{
    return numAtAgeAges->getRowData(0);
}

void sd_reporting::setNumAtAgeBins (QStringList data)
{
    numAtAgeAges->setRowData(0, data);
}

// natMort reporting
void sd_reporting::setNatMort (QStringList data)
{
    int num = data.last().toInt();
    natMort->setRowData(0, data);
    setNumNatMortBins(num);
}

QStringList sd_reporting::getNatMort ()
{
    return natMort->getRowData(0);
}

int sd_reporting::getNumNatMortBins()
{
    int num = natMortAges->columnCount();
    return num;
}

void sd_reporting::setNumNatMortBins(int num)
{
    natMortAges->setColumnCount(num);
}

QStringList sd_reporting::getNatMortBins ()
{
    return natMortAges->getRowData(0);
}

void sd_reporting::setNatMortBins (QStringList data)
{
    natMortAges->setRowData(0, data);
}

