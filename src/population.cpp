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
    seasparamtable = new shortParameterModel(this);
    header.clear();
//    header << "st_age" << "st_age bias" << "max_age bias" << "bias coeff" << "st_age sd" << "max_age sd" << "sd coeff";
//    seasparamtable->setColumnCount(7);
    seasparamtable->setHeader(header);
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
    setNumSeasParams();
}

void population::setNumSeas(int seas)
{
    // default value since insertSeasParams needs it to be > 0
    iNumSeas = 1;

    if (seas >= 1 && seas <= 12)
    {
        if (seas > iNumSeas)
            insertSeasParams (seas - iNumSeas);
        else if (seas < iNumSeas)
            removeSeasParams (iNumSeas - seas);
        iNumSeas = seas;
    }
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

bool population::getK() const
{
    return K;
}

void population::setK(bool value)
{
    K = value;
}

bool population::getL1() const
{
    return L1;
}

void population::setL1(bool value)
{
    L1 = value;
}

bool population::getMalewtlen2() const
{
    return malewtlen2;
}

void population::setMalewtlen2(bool value)
{
    malewtlen2 = value;
}

bool population::getMalewtlen1() const
{
    return malewtlen1;
}

void population::setMalewtlen1(bool value)
{
    malewtlen1 = value;
}

bool population::getFec2() const
{
    return fec2;
}

void population::setFec2(bool value)
{
    fec2 = value;
}

bool population::getFec1() const
{
    return fec1;
}

void population::setFec1(bool value)
{
    fec1 = value;
}

bool population::getMat2() const
{
    return mat2;
}

void population::setMat2(bool value)
{
    mat2 = value;
}

bool population::getMat1() const
{
    return mat1;
}

void population::setMat1(bool value)
{
    mat1 = value;
}

bool population::getFemwtlen2() const
{
    return femwtlen2;
}

void population::setFemwtlen2(bool value)
{
    femwtlen2 = value;
}

bool population::getFemwtlen1() const
{
    return femwtlen1;
}

void population::setFemwtlen1(bool value)
{  
    if (femwtlen1 != value)
    {
        femwtlen1 = value;
    }
}

void population::setNumSeasParams()
{
    int num = 0;
    int row, i;
    if (femwtlen1)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 FemWtLen1").arg(QString::number(i)));
    }
    if (femwtlen2)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 FemWtLen2").arg(QString::number(i)));
    }
    if (mat1)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 Maturity1").arg(QString::number(i)));
    }
    if (mat2)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 Maturity2").arg(QString::number(i)));
    }
    if (fec1)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 Fecundity1").arg(QString::number(i)));
    }
    if (fec2)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 Fecundity2").arg(QString::number(i)));
    }
    if (malewtlen1)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 MaleWtLen1").arg(QString::number(i)));
    }
    if (malewtlen2)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 MaleWtLen2").arg(QString::number(i)));
    }
    if (L1)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 L1").arg(QString::number(i)));
    }
    if (K)
    {
        num++;
        seasparamtable->setRowCount(num * iNumSeas);
        for (i = 1, row = iNumSeas * num - iNumSeas; i <= iNumSeas; i++, row++)
            seasparamtable->setRowHeader(row, QString("Seas%1 K").arg(QString::number(i)));
    }
}

void population::changeFemWtLn1(bool flag)
{
    int row = 0;// = 10 * iNumSeas - iNumSeas;
    if (flag)
    {
        for (int i = 1; i <= iNumSeas; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 FemWtLen1").arg(QString::number(i)));
        }
    }
    else
    {
        for (int i = iNumSeas; i > 0; i--)
        {
            seasparamtable->removeRow(i-1);
        }
    }
    femwtlen1 = flag;

}
void population::changeFemWtLn2(bool flag)
{
    int row = 0;
    if (femwtlen1) row++;

    if (flag)
    {
        row *= iNumSeas;
        for (int i = 0; i < iNumSeas; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 FemWtLen2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Maturity1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Maturity2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Fecundity1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Fecundity2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 MaleWtLen1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 MaleWtLen2").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 L1").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
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
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 K").arg(QString::number(i + 1)));
        }
    }
    else
    {
        row = (row + 1) * iNumSeas - 1;
        for (int i = iNumSeas; i > 0; i--, row--)
        {
            seasparamtable->removeRow(row);
        }
    }
    K = flag;
}


void population::insertSeasParams(int num)
{
    int numchcks = seasparamtable->rowCount() / iNumSeas;
    int row = 0;
    if (numchcks > 0 && K)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 K").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && L1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 L1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 MaleWtLen2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 MaleWtLen1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && fec2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Fecundity2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && fec1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Fecundity1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && mat2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Maturity2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && mat1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 Maturity1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 FemWtLen2").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++, row++)
        {
            seasparamtable->insertRow(row);
            seasparamtable->setRowHeader(row, QString("Seas%1 FemWtLen1").arg(QString::number(iNumSeas + i)));
        }
        numchcks--;
    }
}

void population::removeSeasParams(int num)
{
    int numchcks = seasparamtable->rowCount() / iNumSeas;
    int row = 0;
    if (numchcks > 0 && K)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && L1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && malewtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && fec2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && fec1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && mat2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && mat1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen2)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
    if (numchcks > 0 && femwtlen1)
    {
        row = numchcks * iNumSeas;
        for (int i = 0; i < num; i++)
        {
            seasparamtable->removeRow(row - i);
        }
        numchcks--;
    }
}

void population::setSeasParam(int index, QStringList data)
{
    if (index >= seasparamtable->rowCount())
        seasparamtable->setRowCount(index + 1);
    seasparamtable->setRowData(index, data);
}

QStringList population::getSeasParam(int index)
{
    return seasparamtable->getRowData(index);
}

void population::readSeasonalEffects(ss_file *input)
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
}

QString population::writeSeasonalEffects()
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
}
