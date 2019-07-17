#include "growth_pattern.h"

growthPattern::growthPattern(ss_model *parent)
    : QObject((QObject *)parent)
{
    parnt = parent;
    morphs.append(new growth_morph());
    num_morphs = 1;

    fracFmParams = new longParameterModel();
    fracFmParams->setNumParams(1);
    fracFmVarParams = new timeVaryParameterModel(parnt);
    fracFmVarParams->setNumParams(1);
    fracFmParams->setParamHeader(0, QString("FracFemale"));
    fracFmVarParams->setTableTitle(0, "FracFemale");

    femGrowthParams = new longParameterModel();
    femGrowthVarParams = new timeVaryParameterModel(parnt);
    maleGrowthParams = new longParameterModel();
    maleGrowthVarParams = new timeVaryParameterModel(parnt);

    femCvParams = new longParameterModel();
    femCvParams->setNumParams(2);
    femCvVarParams = new timeVaryParameterModel(parnt);
    femCvVarParams->setNumParams(2);
    maleCvParams = new longParameterModel();
    maleCvParams->setNumParams(2);
    maleCvVarParams = new timeVaryParameterModel(parnt);
    maleCvVarParams->setNumParams(2);

    natMAges = new tablemodel ();
    natMAges->setRowCount(3);
    natMAges->removeHeader();
    femNatMrtParams = new longParameterModel();
    femNatMrtVarParams = new timeVaryParameterModel(parnt);
    femNatMrtParams->setNumParams(1);
    femNatMrtVarParams->setTotalNumVarTables(1);
    maleNatMrtParams = new longParameterModel();
    maleNatMrtVarParams = new timeVaryParameterModel(parnt);
    maleNatMrtParams->setNumParams(1);
    maleNatMrtVarParams->setTotalNumVarTables(1);

    femWtLenParams = new longParameterModel((QObject *)parnt);
    femWtLenVarParams = new timeVaryParameterModel(parnt);
    femWtLenParams->setNumParams(2);
    femWtLenVarParams->setTotalNumVarTables(2);
    connect (femWtLenParams, SIGNAL(paramChanged(int,QStringList)),
             femWtLenVarParams, SLOT(changeVarParamData(int,QStringList)));

    maleWtLenParams = new longParameterModel((QObject *)parnt);
    maleWtLenVarParams = new timeVaryParameterModel(parnt);
    maleWtLenParams->setNumParams(2);
    maleWtLenVarParams->setTotalNumVarTables(2);
    connect (maleWtLenParams, SIGNAL(paramChanged(int,QStringList)),
             maleWtLenVarParams, SLOT(changeVarParamData(int,QStringList)));

    femMatureParams = new longParameterModel();
    femMatureVarParams = new timeVaryParameterModel(parnt);
    femMatureParams->setNumParams(4);
    femMatureVarParams->setTotalNumVarTables(4);
    connect (femMatureParams, SIGNAL(paramChanged(int,QStringList)),
             femMatureVarParams, SLOT(changeVarParamData(int,QStringList)));

    hermaphParams = new longParameterModel();
    hermaphVarParams = new timeVaryParameterModel(parnt);
    hermaphParams->setNumParams(1);
    hermaphVarParams->setTotalNumVarTables(1);
    connect (hermaphParams, SIGNAL(paramChanged(int,QStringList)),
             hermaphVarParams, SLOT(changeVarParamData(int,QStringList)));

}

growthPattern::~growthPattern()
{
    clear();
    delete fracFmParams;
    delete fracFmVarParams;
    delete femGrowthParams;
    delete femGrowthVarParams;
    delete maleGrowthParams;
    delete maleGrowthVarParams;
    delete femCvParams;
    delete femCvVarParams;
    delete maleCvParams;
    delete maleCvVarParams;
    delete femNatMrtParams;
    delete femNatMrtVarParams;
    delete maleNatMrtParams;
    delete maleNatMrtVarParams;
    delete natMAges;
    delete femWtLenParams;
    delete femWtLenVarParams;
    delete maleWtLenParams;
    delete maleWtLenVarParams;
    delete femMatureParams;
    delete femMatureVarParams;
    delete hermaphParams;
    delete hermaphVarParams;
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

void growthPattern::setNumGenders(int num)
{
    if (num == 1)
    {
        maleGrowthParams->setNumParams(0);
        maleCvParams->setNumParams(0);
        maleWtLenParams->setNumParams(0);
        maleNatMrtParams->setNumParams(0);
    }
    else {
        maleGrowthParams->setNumParams(femGrowthParams->getNumParams());
        maleCvParams->setNumParams(2);
        maleWtLenParams->setNumParams(femWtLenParams->getNumParams());
        maleNatMrtParams->setNumParams(femNatMrtParams->getNumParams());
    }
}

void growthPattern::setTimeVaryReadParams(int value)
{
    int i;
    femNatMrtVarParams->setAutoGenerate(value);
    maleNatMrtVarParams->setAutoGenerate(value);
    femGrowthVarParams->setAutoGenerate(value);
    maleGrowthVarParams->setAutoGenerate(value);
    femCvVarParams->setAutoGenerate(value);
    maleCvVarParams->setAutoGenerate(value);
    femWtLenVarParams->setAutoGenerate(value);
    maleWtLenVarParams->setAutoGenerate(value);
    fracFmVarParams->setAutoGenerate(value);
    femMatureVarParams->setAutoGenerate(value);
    hermaphVarParams->setAutoGenerate(value);
    for (i = 0; i < getNumNatMParams(); i++) {
        femNatMrtVarParams->changeVarParamData(i, femNatMrtParams->getParameter(i));
        maleNatMrtVarParams->changeVarParamData(i, maleNatMrtParams->getParameter(i));
    }
    for (i = 0; i < getNumGrowthParams(); i++) {
        femGrowthVarParams->changeVarParamData(i, femGrowthParams->getParameter(i));
        maleGrowthVarParams->changeVarParamData(i, maleGrowthParams->getParameter(i));
    }
    for (i = 0; i < 2; i++) {
        femCvVarParams->changeVarParamData(i, femCvParams->getParameter(i));
        maleCvVarParams->changeVarParamData(i, maleCvParams->getParameter(i));
    }
    for (i = 0; i < 2; i++) {
        femWtLenVarParams->changeVarParamData(i, femWtLenParams->getParameter(i));
        maleWtLenVarParams->changeVarParamData(i, maleWtLenParams->getParameter(i));
    }
    for (i = 0; i < getNumFemMatrParams(); i++) {
        femMatureVarParams->changeVarParamData(i, femMatureParams->getParameter(i));
    }
    for (i = 0; i< getNumHermaphParams(); i++) {
        hermaphVarParams->changeVarParamData(i, hermaphParams->getParameter(i));
    }
    fracFmVarParams->changeVarParamData(0, fracFmParams->getParameter(0));
}

void growthPattern::setNumNatMParams(int num)
{
    femNatMrtParams->setNumParams(num);
    femNatMrtVarParams->setTotalNumVarTables(num);
    maleNatMrtParams->setNumParams(num);
    maleNatMrtVarParams->setTotalNumVarTables(num);
}

void growthPattern::clear()
{
    setNum_morphs(1);
}

void growthPattern::setFemNatMParam(int index, QStringList data)
{
    if (index >= femNatMrtParams->getNumParams())
        setNumNatMParams(index + 1);
    femNatMrtParams->setParameter(index, data);
    femNatMrtVarParams->setParameter(index, data);
}

void growthPattern::setFemNatMParamHeader(int index, QString hdr)
{
    femNatMrtParams->setParamHeader(index, hdr);
    femNatMrtVarParams->setTableTitle(index, hdr);
}

void growthPattern::setFemNatMTVParam(int index, QStringList data)
{
    femNatMrtVarParams->setVarParameter(index, data);
}

void growthPattern::setMaleNatMParam(int index, QStringList data)
{
    if (index >= maleNatMrtParams->getNumParams())
        setNumNatMParams(index + 1);
    maleNatMrtParams->setParameter(index, data);
    maleNatMrtVarParams->setParameter(index, data);
}

void growthPattern::setMaleNatMParamHeader(int index, QString hdr)
{
    maleNatMrtParams->setParamHeader(index, hdr);
    maleNatMrtVarParams->setTableTitle(index, hdr);
}

void growthPattern::setMaleNatMTVParam(int index, QStringList data)
{
    maleNatMrtVarParams->setVarParameter(index, data);
}

void growthPattern::setFractionFemaleParam(QStringList data)
{
    fracFmParams->setNumParams(1);
    fracFmParams->setParameter(0, data);
    fracFmVarParams->setParameter(0, data);
}

void growthPattern::setFractionFemParamHeader(QString hdr)
{
    fracFmParams->setParamHeader(0, hdr);
    fracFmVarParams->setTableTitle(0, hdr);
}

void growthPattern::setFracFmTVParam(int index, QStringList data)
{
    fracFmVarParams->setVarParameter(index, data);
}

void growthPattern::setNumGrowthParams(int num)
{
    femGrowthParams->setNumParams(num);
    femGrowthVarParams->setTotalNumVarTables(num);
    maleGrowthParams->setNumParams(num);
    maleGrowthVarParams->setTotalNumVarTables(num);
}

void growthPattern::setFemGrowthParam(int index, QStringList data)
{
    if (index >= femGrowthParams->getNumParams())
        setNumGrowthParams(index + 1);
    femGrowthParams->setParameter(index, data);
    femGrowthVarParams->setParameter(index, data);
}

void growthPattern::setFemGrowthParamHeader(int index, QString hdr)
{
    femGrowthParams->setParamHeader(index, hdr);
    femGrowthVarParams->setTableTitle(index, hdr);
}

void growthPattern::setFemGrowthTVParam(int index, QStringList data)
{
    femGrowthVarParams->setVarParameter(index, data);
}

void growthPattern::setMaleGrowthParam(int index, QStringList data)
{
    if (index >= femGrowthParams->getNumParams())
        setNumGrowthParams(index + 1);
    maleGrowthParams->setParameter(index, data);
    maleGrowthVarParams->setParameter(index, data);
}

void growthPattern::setMaleGrowthParamHeader(int index, QString hdr)
{
    maleGrowthParams->setParamHeader(index, hdr);
    maleGrowthVarParams->setTableTitle(index, hdr);
}

void growthPattern::setMaleGrowthTVParam(int index, QStringList data)
{
    maleGrowthVarParams->setVarParameter(index, data);
}

void growthPattern::setNumCVParams(int num)
{
    femCvParams->setNumParams(num);
    femCvVarParams->setTotalNumVarTables(num);
    maleCvParams->setNumParams(num);
    maleCvVarParams->setTotalNumVarTables(num);
}

void growthPattern::setFemCVParam(int index, QStringList data)
{
    if (index >= femCvParams->getNumParams())
        femCvParams->setNumParams(index + 1);
    femCvParams->setParameter(index, data);
    femCvVarParams->setParameter(index, data);
}

void growthPattern::setFemCVParamHeader(int index, QString hdr)
{
    femCvParams->setParamHeader(index, hdr);
    femCvVarParams->setTableTitle(index, hdr);
}

void growthPattern::setFemCVTVParam(int index, QStringList data)
{
    femCvVarParams->setVarParameter(index, data);
}

void growthPattern::setMaleCVParam(int index, QStringList data)
{
    if (index >= maleCvParams->getNumParams())
        maleCvParams->setNumParams(index + 1);
    maleCvParams->setParameter(index, data);
    maleCvVarParams->setParameter(index, data);
}

void growthPattern::setMaleCVParamHeader(int index, QString hdr)
{
    maleCvParams->setParamHeader(index, hdr);
    maleCvVarParams->setTableTitle(index, hdr);
}

void growthPattern::setMaleCVTVParam(int index, QStringList data)
{
    maleCvVarParams->setVarParameter(index, data);
}

void growthPattern::setNumWtLenParams(int num)
{
    femWtLenParams->setNumParams(num);
    femWtLenVarParams->setTotalNumVarTables(num);
    maleWtLenParams->setNumParams(num);
    maleWtLenVarParams->setTotalNumVarTables(num);
}

void growthPattern::setFemWtLenParam(int index, QStringList data)
{
    if (index >= femWtLenParams->getNumParams())
        femWtLenParams->setNumParams(index + 1);
    femWtLenParams->setParameter(index, data);
    femWtLenVarParams->setParameter(index, data);
}

void growthPattern::setFemWtLenParamHeader(int index, QString hdr)
{
    femWtLenParams->setParamHeader(index, hdr);
    femWtLenVarParams->setTableTitle(index, hdr);
}

void growthPattern::setFemWtLenTVParam(int index, QStringList data)
{
    femWtLenVarParams->setParameter(index, data);
}

void growthPattern::setMaleWtLenParam(int index, QStringList data)
{
    if (index >= maleWtLenParams->getNumParams())
        maleWtLenParams->setNumParams(index + 1);
    maleWtLenParams->setParameter(index, data);
    maleWtLenVarParams->setParameter(index, data);
}

void growthPattern::setMaleWtLenParamHeader(int index, QString hdr)
{
    maleWtLenParams->setParamHeader(index, hdr);
    maleWtLenVarParams->setTableTitle(index, hdr);
}

void growthPattern::setMaleWtLenTVParam(int index, QStringList data)
{
    maleWtLenVarParams->setParameter(index, data);
}

void growthPattern::setNumFemMatrParams(int num)
{
    femMatureParams->setNumParams(num);
    femMatureVarParams->setTotalNumVarTables(num);
}

void growthPattern::setFemMatureParam(int index, QStringList data)
{
    if (index >= femMatureParams->getNumParams())
        femMatureParams->setNumParams(index + 1);
    femMatureParams->setParameter(index, data);
    femMatureVarParams->setParameter(index, data);
}

void growthPattern::setFemMatureParamHeader(int index, QString hdr)
{
    femMatureParams->setParamHeader(index, hdr);
    femMatureVarParams->setTableTitle(index, hdr);
}

void growthPattern::setFemMatureTVParam(int index, QStringList data)
{
    femMatureVarParams->setVarParameter(index, data);
}

void growthPattern::setNumHermaphParams(int num)
{
    hermaphParams->setNumParams(num);
    hermaphVarParams->setTotalNumVarTables(num);
}

void growthPattern::setHermaphParam(int index, QStringList data)
{
    if (index >= hermaphParams->getNumParams())
        hermaphParams->setNumParams(index + 1);
    hermaphParams->setParameter(index, data);
    hermaphVarParams->setParameter(index, data);
}

void growthPattern::setHermaphParamHeader(int index, QString hdr)
{
    hermaphParams->setParamHeader(index, hdr);
    hermaphVarParams->setTableTitle(index, hdr);
}

void growthPattern::setHermaphTVParam(int index, QStringList data)
{
    hermaphVarParams->setVarParameter(index, data);
}



