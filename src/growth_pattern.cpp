#include "growth_pattern.h"

growthPattern::growthPattern()
{
    morphs.append(new growth_morph());
    num_morphs = 1;

    fractionFemParams = new parametermodel();
    fractionFemParams->setRowCount(1);
    fractionFemParams->setRowHeader(0, QString("FracFemale"));

    growthParams = new parametermodel();
    growthTVParams = new parametermodel();
    growthTVParams->setColumnCount(7);

    cvParams = new parametermodel();
    cvParams->setRowCount(2);
    cvTVParams = new parametermodel();
    cvTVParams->setColumnCount(7);

    natMParams = new parametermodel();
    natMParams->setRowCount(1);
    natMTVParams = new parametermodel();
    natMTVParams->setColumnCount(7);
    natMAges = new tablemodel ();
    natMAges->setRowCount(2);

    devParams = new parametermodel();
    devParams->setColumnCount(7);

    timeVaryParams = new parametermodel();
    timeVaryParams->setColumnCount(7);
}

growthPattern::~growthPattern()
{
    clear();
    delete fractionFemParams;
    delete growthParams;
    delete growthTVParams;
    delete cvParams;
    delete cvTVParams;
    delete natMParams;
    delete natMTVParams;
    delete natMAges;
    delete devParams;
    delete timeVaryParams;
}

growthPattern::growthPattern (const growthPattern &rhs)
{
    copy (rhs);
}

growthPattern& growthPattern::operator = (const growthPattern &rhs)
{
    return copy (rhs);
}

growthPattern& growthPattern::copy(const growthPattern &rhs)
{
    clear();
    num_morphs = rhs.getNum_morphs();

    for (int i = 0; i < num_morphs; i++)
        morphs.append(new growth_morph(*rhs.getMorph(i)));
    return *this;
}

int growthPattern::getNum_morphs() const
{
    return num_morphs;
}

void growthPattern::setNum_morphs(int value)
{
    growth_morph *gmp;
    num_morphs = morphs.count();
    while (num_morphs < value)
    {
        gmp = new growth_morph();
        morphs.append(gmp);
    }
    while (value < num_morphs)
    {
        gmp = morphs.takeLast();
        delete gmp;
    }
    num_morphs = value;
}
growth_morph * growthPattern::getMorph(int index) const
{
    return morphs[index];
}

void growthPattern::setMorph(int index, growth_morph * &value)
{
    growth_morph * gm = morphs.takeAt(index);
    morphs.insert(index, value);
    delete gm;
}

void growthPattern::clear()
{
    setNum_morphs(1);
}

void growthPattern::setNatMParam(int index, QStringList data)
{
    if (index >= natMParams->rowCount())
        natMParams->setRowCount(index + 1);
    natMParams->setRowData(index, data);
}

void growthPattern::setNatMTVParam(int index, QStringList data)
{
    if (index >= natMTVParams->rowCount())
        natMTVParams->setRowCount(index + 1);
    natMTVParams->setRowData(index, data);
}

void growthPattern::setGrowthParam(int index, QStringList data)
{
    if (index >= growthParams->rowCount())
        growthParams->setRowCount(index + 1);
    growthParams->setRowData(index, data);
}

void growthPattern::setGrowthTVParam(int index, QStringList data)
{
    if (index >= growthTVParams->rowCount())
        growthTVParams->setRowCount(index + 1);
    growthTVParams->setRowData(index, data);
}

void growthPattern::setTimeVaryParam(int index, QStringList data)
{
    if (index >= timeVaryParams->rowCount())
        timeVaryParams->setRowCount(index + 1);
    timeVaryParams->setRowData(index, data);
}

void growthPattern::setTimeVaryParamHeader(int index, QString header)
{
    if (index >= timeVaryParams->rowCount())
        timeVaryParams->setRowCount(index + 1);
    timeVaryParams->setRowHeader(index, header);
}

void growthPattern::setCVParam(int index, QStringList data)
{
    if (index >= cvParams->rowCount())
        cvParams->setRowCount(index + 1);
    cvParams->setRowData(index, data);
}

void growthPattern::setCVTVParam(int index, QStringList data)
{
    if (index >= cvTVParams->rowCount())
        cvTVParams->setRowCount(index + 1);
    cvTVParams->setRowData(index, data);
}

void growthPattern::setDevParam(int index, QStringList data)
{
    if (index >= devParams->rowCount())
        devParams->setRowCount(index + 1);
    devParams->setRowData(index, data);
}


