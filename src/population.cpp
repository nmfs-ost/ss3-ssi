#include "population.h"

population::population(ss_model *parent) :
    QObject((QObject *)parent)
{
    QStringList header;
    pop_growth = new ss_growth(parent);
    pop_fecundity = new ss_fecundity(parent);
    pop_recruitment = new spawn_recruit(parent);
    pop_mortality = new ss_mortality(parent);
    pop_movement = new ss_movement(parent);
    meanBwtModel = new tablemodel(this);
    meanBwtModel->setColumnCount(20);
    header << "Year" << "Seas" << "Type" << "Part" << "Value" << "CV";
    meanBwtModel->setHeader(header);
    seasparamtable = new seasonalEffectsModel();// shortParameterModel(this);
    header.clear();
//    header << "FWtLn1" << "FWtLn2" << "Mat1" << "Mat2" << "Fec1" << "Fec2" << "MWtLn1" << "MWtLn2" << "L1" << "K";
//    seasparamtable->setSetupHeader(header);
//    header.clear();
//    header << "st_age" << "st_age bias" << "max_age bias" << "bias coeff" << "st_age sd" << "max_age sd" << "sd coeff";
//    seasparamtable->setColumnCount(7);
//    seasparamtable->setParamModelHeader(header);//->setHeader(header);
    reset();
}

population::~population()
{
    delete meanBwtModel;
    delete pop_growth;
    delete pop_fecundity;
    delete pop_recruitment;
    delete pop_mortality;
    delete pop_movement;
    delete seasparamtable;
}

void population::reset()
{
    Fec()->reset();
    Grow()->reset();
    SR()->reset();
    Move()->reset();
    M()->setMethod(1);
    M()->reset();

    set_frac_female(0.5);

    i_gender = 1;
    i_mean_bwt_deg_freedom = 1;
    i_mean_bwt_count = 1;

    meanBwtModel->setRowCount(0);

    femwtlen1 = false;
    femwtlen2 = false;
    mat1 = false;
    mat2 = false;
    fec1 = false;
    fec2 = false;

    malewtlen1 = false;
    malewtlen2 = false;
    L1 = false;
    K = false;
    setNumSeas(1);
//    setNumSeasParams();
}

void population::setNumSeas(int seas)
{
    // default value since insertSeasParams needs it to be > 0
    iNumSeas = 1;
    seasparamtable->setNumSeasons(seas);
//    seasparamtable->setMultiplier(seas);
/*
    if (seas >= 1 && seas <= 12)
    {
        if (seas > iNumSeas)
            insertSeasParams (seas - iNumSeas);
        else if (seas < iNumSeas)
            removeSeasParams (iNumSeas - seas);
        iNumSeas = seas;
    }*/
}

void population::setStartYear(int yr)
{
    iStartYear = yr;
    M()->setStartYear(yr);
}

void population::setTotalYears(int yrs)
{
    iNumYears = yrs;
    M()->setNumYears(yrs);
}

void population::setNumAges(int ages)
{
    iNumAges = ages;
    Grow()->setNumAges(ages);
}

float population::get_frac_female() const
{
    return f_frac_female;
}

void population::set_frac_female(float value)
{
    f_frac_female = value;
}

void population::setFractionFemale(QStringList data)
{
    if (!fractionFem.isEmpty())
        fractionFem.clear();
    for (int i = 0; i < data.count(); i++)
        fractionFem.append(data.at(i));
}

QStringList population::getFractionFemale()
{
    return fractionFem;
}

void population::setSeasParamSetup(QStringList values)
{
    seasparamtable->setSetup(values);
}

QStringList population::getSeasParamSetup()
{
    return seasparamtable->getSetup();
}

bool population::getK() const
{
    return seasparamtable->getK();
}

void population::setK(bool value)
{
    seasparamtable->setK(value);
/*    if (seasparamtable->getK() != value)
    {
        seasparamtable->setK(value);
    }
    malewtlen1 = value;*/
}

bool population::getL1() const
{
    return seasparamtable->getL1();
}

void population::setL1(bool value)
{
    seasparamtable->setL1(value);
/*    if (seasparamtable->getL1() != value)
    {
        seasparamtable->setL1(value);
    }
    malewtlen1 = value;*/
}

bool population::getMaleWtLen2() const
{
    return seasparamtable->getMalewtlen2();
}

void population::setMaleWtLen2(bool value)
{
    seasparamtable->setMalewtlen2(value);
/*    if (seasparamtable->getMalewtlen2() != value)
    {
        seasparamtable->setMalewtlen2(value);
    }
    malewtlen1 = value;*/
}

bool population::getMaleWtLen1() const
{
    return seasparamtable->getMalewtlen1();
}

void population::setMaleWtLen1(bool value)
{
    seasparamtable->setMalewtlen1(value);
/*    if (seasparamtable->getMalewtlen1() != value)
    {
        seasparamtable->setMalewtlen1(value);
    }
    malewtlen1 = value;*/
}

bool population::getFecund2() const
{
    return seasparamtable->getFec2();
}

void population::setFecund2(bool value)
{
    seasparamtable->setFec2(value);
/*    if (seasparamtable->getFec2() != value)
    {
        seasparamtable->setFec2(value);
    }*/
}

bool population::getFecund1() const
{
    return seasparamtable->getFec1();
}

void population::setFecund1(bool value)
{
    seasparamtable->setFec1(value);
/*    if (seasparamtable->getFec1() != value)
    {
        seasparamtable->setFec1(value);
    }*/
}

bool population::getMaturity2() const
{
    return seasparamtable->getMat2();
}

void population::setMaturity2(bool value)
{
    seasparamtable->setMat2(value);
/*    if (seasparamtable->getMat2() != value)
    {
        seasparamtable->setMat2(value);
    }*/
}

bool population::getMaturity1() const
{
    return seasparamtable->getMat1();
}

void population::setMaturity1(bool value)
{
    seasparamtable->setMat1(value);
/*    if (seasparamtable->getMat1() != value)
    {
        seasparamtable->setMat1(value);
    }*/
}

bool population::getFemWtLen2() const
{
    return seasparamtable->getFemwtlen2();
}

void population::setFemWtLen2(bool value)
{
    seasparamtable->setFemwtlen2(value);
/*    if (seasparamtable->getFemwtlen2() != value)
    {
        seasparamtable->setFemwtlen2(value);
    }*/
}

bool population::getFemWtLen1() const
{
    return seasparamtable->getFemwtlen1();
}

void population::setFemWtLen1(bool value)
{  
    seasparamtable->setFemwtlen1(value);
/*    if (seasparamtable->getFemwtlen1() != value)
    {
        seasparamtable->setFemwtlen1(value);
    }*/
}
/*
void population::setNumSeasParams()
{
    int num = 0;
    int row, i;
    if (femwtlen1)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 FemWtLen1").arg(QString::number(i)));
    }
    if (femwtlen2)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 FemWtLen2").arg(QString::number(i)));
    }
    if (mat1)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 Maturity1").arg(QString::number(i)));
    }
    if (mat2)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 Maturity2").arg(QString::number(i)));
    }
    if (fec1)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 Fecundity1").arg(QString::number(i)));
    }
    if (fec2)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 Fecundity2").arg(QString::number(i)));
    }
    if (malewtlen1)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 MaleWtLen1").arg(QString::number(i)));
    }
    if (malewtlen2)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 MaleWtLen2").arg(QString::number(i)));
    }
    if (L1)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 L1").arg(QString::number(i)));
    }
    if (K)
    {
        num++;
        seasparamtable->setParamCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setParamHeader(row, QString("Seas%1 K").arg(QString::number(i)));
    }
}*/
/*
void population::changeFemWtLn1(bool flag)
{
    int row = 0;// = 10 * iNumSeas - iNumSeas;
    if (flag)
    {
        for (int i = 1; i <= iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 FemWtLen1").arg(QString::number(i)));
        }
        seasparamtable->setParamCount(row);
    }*/
/*    else
    {
        for (int i = iNumSeas; i > 0; i--)
        {
            seasparamtable->removeRow(i-1);
        }
    }*//*
    femwtlen1 = flag;

}*//*
void population::changeFemWtLn2(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 FemWtLen2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    femwtlen2 = flag;
}
void population::changeMaturity1(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;
    if (femwtlen2) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Maturity1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    mat1 = flag;
}
void population::changeMaturity2(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;
    if (femwtlen2) row++;
    if (mat1) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Maturity2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    mat2 = flag;
}
void population::changeFecundity1(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;
    if (femwtlen2) row++;
    if (mat1) row++;
    if (mat2) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Fecundity1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    fec1 = flag;
}
void population::changeFecundity2(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;
    if (femwtlen2) row++;
    if (mat1) row++;
    if (mat2) row++;
    if (fec1) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Fecundity2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    fec2 = flag;
}
void population::changeMaleWtLn1(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;
    if (femwtlen2) row++;
    if (mat1) row++;
    if (mat2) row++;
    if (fec1) row++;
    if (fec2) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 MaleWtLen1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    malewtlen1 = flag;
}
void population::changeMaleWtLn2(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;
    if (femwtlen2) row++;
    if (mat1) row++;
    if (mat2) row++;
    if (fec1) row++;
    if (fec2) row++;
    if (malewtlen1) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 MaleWtLen2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    malewtlen2 = flag;
}
void population::changeL1(bool flag)
{
    int row = 0;
    if (femwtlen1) row = row + 1;
    if (femwtlen2) row++;
    if (mat1) row++;
    if (mat2) row++;
    if (fec1) row++;
    if (fec2) row++;
    if (malewtlen1) row++;
    if (malewtlen2) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 L1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    L1 = flag;
}
void population::changeK(bool flag)
{
    int row = 0;
    if (femwtlen1) row = row + 1;
    if (femwtlen2) row = row + 1;
    if (mat1) row = row + 1;
    if (mat2) row = row + 1;
    if (fec1) row = row + 1;
    if (fec2) row = row + 1;
    if (malewtlen1) row = row + 1;
    if (malewtlen2) row = row + 1;
    if (L1) row = row + 1;

    if (flag)
    {
        row = row * iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 K").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->setParamCount(row - 1);
        }
    }
    K = flag;
}


void population::insertSeasParams(int num)
{
    int numchcks = seasparamtable->getParamCount() / iNumSeas;
    int row = 0;
    if (numchcks > 0 && K)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 K").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && L1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row - 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 L1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 MaleWtLen2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 MaleWtLen1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && fec2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Fecundity2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && fec1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Fecundity1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && mat2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Maturity2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && mat1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 Maturity1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 FemWtLen2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->setParamCount(row + 1);
            seasparamtable->setParamHeader(row, QString("Seas%1 FemWtLen1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
}

void population::removeSeasParams(int num)
{
    int numchcks = seasparamtable->getParamCount() / iNumSeas;
    int row = 0;
    if (numchcks > 0 && K)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && L1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && fec2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && fec1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && mat2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && mat1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->setParamCount(row - i);
        }
        numchcks--;
    }
}*/

void population::setSeasonalParam(int index, QStringList data)
{
//    if (index >= seasparamtable->getParamCount())
//        seasparamtable->setParamCount(index + 1);
    seasparamtable->setParameter(index, data);
}

QStringList population::getSeasonalParam(int index)
{
    return seasparamtable->getParameter(index);
}

/*void population::readSeasonalEffects(ss_file *input)
{
    femwtlen1 = input->get_next_value().toInt() == 1? true:false;
    femwtlen2 = input->get_next_value().toInt() == 1? true:false;
    mat1 = input->get_next_value().toInt() == 1? true:false;
    mat2 = input->get_next_value().toInt() == 1? true:false;
    fec1 = input->get_next_value().toInt() == 1? true:false;
    fec2 = input->get_next_value().toInt() == 1? true:false;
    malewtlen1 = input->get_next_value().toInt() == 1? true:false;
    malewtlen2 = input->get_next_value().toInt() == 1? true:false;
    L1 = input->get_next_value().toInt() == 1? true:false;
    K = input->get_next_value().toInt() == 1? true:false;
}*/

/*QString population::writeSeasonalEffects()
{
    QString line("");
    line.append(QString ("%1 %2 %3 %4 %5 %6 ").arg(
                    QString::number(femwtlen1? 1: 0),
                    QString::number(femwtlen2? 1: 0),
                    QString::number(mat1? 1: 0),
                    QString::number(mat2? 1: 0),
                    QString::number(fec1? 1: 0),
                    QString::number(fec2? 1: 0)));
    line.append(QString ("%1 %2 %3 %4").arg(
                    QString::number(malewtlen1? 1: 0),
                    QString::number(malewtlen2? 1: 0),
                    QString::number(L1? 1: 0),
                    QString::number(K? 1: 0)));
    return line;
}*/
