#include "growth_pattern.h"

growthPattern::growthPattern(ss_model *parent)
{
    parnt = parent;
    morphs.append(new growth_morph());
    num_morphs = 1;

    fracFmParams = new longParameterModel((QObject *)parnt);
    fracFmParams->setNumParams(1);
    fracFmParams->setParamHeader(0, QString("FracFemale"));

    growthParams = new longParameterModel();
    growthVarParams = new timeVaryParameterModel(parnt);

    cvParams = new longParameterModel();
    cvParams->setNumParams(2);

    natMAges = new tablemodel ();
    natMAges->setRowCount(2);
    natMrtParams = new longParameterModel();
    natMrtParams->setNumParams(1);
    natMrtVarParams = new timeVaryParameterModel(parnt);

}

growthPattern::~growthPattern()
{
    clear();
    delete fracFmParams;
    delete growthParams;
    delete growthVarParams;
    delete cvParams;
    delete natMrtParams;
    delete natMrtVarParams;
    delete natMAges;
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
    if (index >= natMrtParams->getNumParams())
        natMrtParams->setNumParams(index + 1);
    natMrtParams->setParameter(index, data);
}

void growthPattern::setNatMTVParam(int index, QStringList data)
{
    natMrtParams->setParameter(index, data);
}

void growthPattern::setGrowthParam(int index, QStringList data)
{
    if (index >= growthParams->getNumParams())
        growthParams->setNumParams(index + 1);
    growthParams->setParameter(index, data);
}

void growthPattern::setGrowthTVParam(int index, QStringList data)
{
    growthParams->setParameter(index, data);
}

void growthPattern::setCVParam(int index, QStringList data)
{
    if (index >= cvParams->getNumParams())
        cvParams->setNumParams(index + 1);
    cvParams->setParameter(index, data);
}



