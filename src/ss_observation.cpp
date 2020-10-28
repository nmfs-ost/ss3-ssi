
#include "ss_observation.h"

#include <QStringList>

ssObservation::ssObservation()
{
    observations = new tablemodel();
    observations->setRowCount(0);
    obsHeader << "Year" << "Month" << "Sex" << "Part" << "NSamp" ;
    observations->setHeader(obsHeader);
}

ssObservation::~ssObservation()
{
    delete observations;
}

void ssObservation::setNumBins(int bins, int numGenders)
{
    numBins = bins;
    QStringList header(obsHeader);

    for (int i = 0; i < bins; i++)
        header.append(QString("F%1").arg(i+1));
    if (numGenders == 2)
    for (int i = 0; i < bins; i++)
        header.append(QString("M%1").arg(i+1));
    observations->setColumnCount(header.count());
    observations->setHeader(header);
}

int ssObservation::getNumBins()
{
//    int obsCount = observations->columnCount() - obsHeader.count();
    return numBins; //obsCount / 2;
}

void ssObservation::setNumObs(int obs)
{
    numObs = obs;
    observations->setRowCount(obs);
}

int ssObservation::getNumObs()
{
//    int obs = observations->rowCount();
    return numObs = observations->rowCount();
}

void ssObservation::addObservation(QStringList txtlst)
{
    int numRows = getNumObs();
    setObservation(numRows, txtlst);
}

void ssObservation::setObservation(int index, QStringList txtlst)
{
    if (getNumObs() <= index)
        setNumObs(index + 1);
    if (observations->columnCount() < txtlst.count())
        observations->setColumnCount(txtlst.count());
    observations->setRowData(index, txtlst);
}

QStringList ssObservation::getObservation(int index)
{
    return observations->getRowData(index);
}

void ssObservation::fromText(QString line)
{
    QStringList txtlst (line.split(' ', QString::SkipEmptyParts));
    addObservation(txtlst);
}

QString ssObservation::toText()
{
    int rows = getNumObs();
    int cols = observations->columnCount();
    QString txt("");
    QStringList txtlst;
    for (int i = 0; i < rows; i++)
    {
        txtlst = getObservation(i);
        for (int j = 0; j < cols; j++)
            txt.append(QString(" %1").arg(txtlst.at(j)));
        txt.append('\n');
    }
    return txt;
}


meanBwtObservation::meanBwtObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "Year" << "Month" << "Fleet" << "Part."  << "Type" << "Obs." << "SE";
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumObs(0);
}

lengthObservation::lengthObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "Year" << "Month" << "Fleet" << "Sex" << "Part" << "NSamp" ;
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumObs(0);
}


ageObservation::ageObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader  << "Year" << "Month" << "Fleet" << "Sex" << "Part" << "AgeErr" << "Lbin_lo" << "Lbin_hi" << "NSamp";
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumObs(0);
}


saaObservation::saaObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader  << "Year" << "Month" << "Fleet" << "Sex" << "Part" << "AgeErr" << "NSamp";
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumObs(0);
}

void saaObservation::setNumBins(int num, int numGenders)
{
    QStringList saaHeader(obsHeader);
    numBins = num;

    for (int i = 0; i < num; i++)
        saaHeader.append(QString("F%1").arg(i+1));
    if (numGenders == 2)
    for (int i = 0; i < num; i++)
        saaHeader.append(QString("M%1").arg(i+1));
    for (int i = 0; i < num; i++)
        saaHeader.append(QString("FS%1").arg(i+1));
    if (numGenders == 2)
    for (int i = 0; i < num; i++)
        saaHeader.append(QString("MS%1").arg(i+1));

    observations->setColumnCount(saaHeader.count());
    observations->setHeader(saaHeader);
}


environmentalVars::environmentalVars()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "Year" << "Variable" << "Value" ;
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumObs(0);
    setNumVars(0);
}



tagObservation::tagObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "Tag" << "area" << "year" << "season" << "<tfill>" << "sex" << "age" << "numRel";
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumTagGroups(0);
    setNumTagGroups(0);
    setLatency(2);
    setMaxPeriods(10);
}

recapObservation::recapObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "Tag" << "year" << "season" << "fleet" << "numRecap";
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumRecapEvnts(0);
}

morphObservation::morphObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "year" << "month" << "fleet" << "part" << "Nsamp";
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumObs(0);
}

void morphObservation::setNumMorphs(int num)
{
    QStringList header(obsHeader);
    for (int i = 0; i < num; i++)
        header.append(QString("Mph%1").arg(i+1));
    observations->setColumnCount(header.count());
    observations->setHeader(header);

}

int morphObservation::getNumMorphs()
{
    int num = observations->columnCount() - obsHeader.count();
    return num;
}


generalObservation::generalObservation()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "Method" << "Year" << "Month" << "Fleet" << "Sex" << "Part" << "NSamp" ;
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumObs(0);
}

recruitDevs::recruitDevs()
    : ssObservation()
{
    obsHeader.clear();
    obsHeader << "Year" << "Dev";
    observations->setColumnCount(obsHeader.count());
    observations->setHeader(obsHeader);
    setNumRecruitDevs(0);
}
