#include "sscomposition.h"

ssComposition::ssComposition(ss_model *parent)
{
    ssModel = static_cast<ss_model *>(parent);

    binsModel = new tablemodel(this);
    binsModel->setColumnCount(0);
    binsModel->setRowCount(1);
    binsModel->setRowHeader(0, QString(""));

    altBinsModel = new tablemodel(this);

    dirichletParams = new longParameterTable(this);

    reset();
}

ssComposition::~ssComposition()
{
    delete binsModel;
    delete altBinsModel;
    delete dirichletParams;
}

void ssComposition::reset()
{
    altBinsModel->setRowCount(0);
    altBinsModel->setColumnCount(0);
    dirichletParams->setParamCount(0);

    i_method = 0;
    f_bin_width = 0;
    i_bin_max = 0;
    i_bin_min = 0;

    i_num_obs = 0;
}

void ssComposition::setAltBinMethod (int method)
{
    i_method = method;
    switch (method)
    {
    case 1:
        altBinsModel->setRowCount(0);
        altBinsModel->setColumnCount(0);
        break;
    case 2:
    case 3:
        altBinsModel->setRowCount(1);
        altBinsModel->setColumnCount(0);
        binsModel->setRowCount(1);
        binsModel->setRowHeader(0, QString(""));
        break;
    }
}

int ssComposition::generateAltBins ()
{
    QStringList bins;
    int num = 0, bin;
    if (i_method == 2)
    {
        num = (i_bin_max - i_bin_min) / f_bin_width + 1;
        for (bin = i_bin_min; bin <= i_bin_max; bin += f_bin_width)
        {
            bins.append(QString::number(bin));
        }
        altBinsModel->setColumnCount(bins.count());
        setAltBins(bins);
    }
    return num;
}


compositionLength::compositionLength(ss_model *parent)
    : ssComposition(parent)
{
    binsModel->setColumnCount(0);
    binsModel->setRowCount(1);
    binsModel->setRowHeader(0, "");
}

void compositionLength::setNumberBins(int num)
{
    binsModel->setColumnCount(num);
}

compositionAge::compositionAge(ss_model *parent)
    : ssComposition(parent)
{
    useAgeKeyZero = -1;
    i_num_error_defs = 0;
    i_num_saa_obs = 0;
    binsModel->setColumnCount(0);
    binsModel->setRowCount(1);
    binsModel->setRowHeader(0, "");

    saaModel = new tablemodel(this);

    errorModel = new tablemodel(this);
    errorModel->setColumnCount(10);
    errorModel->setRowCount(0);
    errorModel->setRowHeader(0,"");

    useParameters = false;
    connect (errorModel, SIGNAL(dataChanged()), SLOT(checkUseParams()));
    errorParams = new longParameterModel(ssModel);
    errorParams->setNumParams(7);
    errorTVParams = new timeVaryParameterModel(ssModel);
    connect (errorParams, SIGNAL(paramsChanged()), SLOT(checkParams()));
}

compositionAge::~compositionAge()
{
    reset();

    delete saaModel;
    delete errorModel;
    delete errorParams;
    delete errorTVParams;
}

void compositionAge::reset()
{
    ssComposition::reset();

    saaModel->reset();
    errorModel->reset();

    while (error_defs.count() > 0)
    {
        error_vector *evct = error_defs.takeFirst();
        delete evct;
    }
    useAgeKeyZero = -1;
    i_num_error_defs = 0;

/*    while (o_saa_obs_list.count() > 0)
    {
        ssObservation *obs = o_saa_obs_list.takeFirst();
        delete obs;
    }*/
    i_num_saa_obs = 0;

    useParameters = false;
    errorParams->getParamTable()->reset();
    errorTVParams->getVarParamTable()->reset();
}

void compositionAge::setNumberBins(int num)
{
//    int saaCount;
    QStringList saaHeader;
    binsModel->setColumnCount(num);

    saaHeader << "Year" << "Month" << "Fleet" << "Sex" << "Part" << "AgeErr" << "Ignore";
    for (int i = 0; i < num; i++)
        saaHeader.append(QString("F%1").arg(i+1));
    for (int i = 0; i < num; i++)
        saaHeader.append(QString("M%1").arg(i+1));
    for (int i = 0; i < num; i++)
        saaHeader.append(QString("FS%1").arg(i+1));
    for (int i = 0; i < num; i++)
        saaHeader.append(QString("MS%1").arg(i+1));
//    saaCount = 7 + (4 * num);
    saaModel->setColumnCount(saaHeader.count());
    saaModel->setHeader(saaHeader);
}

void compositionAge::set_number_ages(int num)
{
    int actAges (num + 1);
    QStringList errheader;
    errorModel->setColumnCount(actAges);
    for (int i = 0; i < actAges; i++)
    {
        errheader.append(QString("Age%1").arg(i));
    }
    errorModel->setHeader(errheader);
}

void compositionAge::set_num_error_defs(int num)
{
    i_num_error_defs = num;
    errorModel->setRowCount(num * 2);
    for (int i = 0; i < errorModel->rowCount(); i++)
    {
        int def = (i / 2) + 1;
        errorModel->setRowHeader(i, QString("mean %1").arg(QString::number(def)));
        i++;
        errorModel->setRowHeader(i, QString("stdv %1").arg(QString::number(def)));
    }
}

void compositionAge::set_error_def_ages (int index, QStringList ages)
{
    errorModel->setRowData(index * 2, ages);
}

void compositionAge::set_error_def (int index, QStringList errs)
{
    errorModel->setRowData(index * 2 + 1, errs);
}

QStringList compositionAge::get_error_ages (int index)
{
    return errorModel->getRowData(index * 2);
}

QStringList compositionAge::get_error_def(int index)
{
    return errorModel->getRowData(index * 2 + 1);
}

bool compositionAge::getUseParameters()
{
    return useParameters;
}

void compositionAge::setErrorParam (int index, QStringList data)// {errorParam->setParameter(index, data);}
{
    errorParams->setParamData(index, data);
    errorTVParams->setParameter(index, data);
}

void compositionAge::setErrorTVParam (int index, QStringList data)
{
    errorTVParams->setVarParameter(index, data);
}

void compositionAge::checkUseParams()
{
    useParameters = false;
    for (int i = 0; i < i_num_error_defs; i++)
    {
        int index = i * 2 + 1;
        float val = errorModel->getRowData(index).at(0).toFloat();
        if(val < 0)
            useParameters = true;
    }
    emit dataChanged();
}

void compositionAge::checkParams()
{
    QStringList ql;
    for (int i = 0; i < 7; i++)
    {
        errorTVParams->setParameter(i, errorParams->getParameter(i));
    }
    emit dataChanged();
}

int compositionAge::getUseAgeKeyZero() const
{
    return useAgeKeyZero;
}

void compositionAge::setUseAgeKeyZero(int newUseAgeKeyZero)
{
    useAgeKeyZero = newUseAgeKeyZero;
}


compositionMorph::compositionMorph(ss_model *parent)
 : ssComposition(parent)
{
}

void compositionMorph::setNumberMorphs(int num)
{
    binsModel->setColumnCount(num);
}


compositionGeneral::compositionGeneral(ss_model *parent)
  : ssComposition(parent)
{
    reset();
}

compositionGeneral::~compositionGeneral()
{
    reset();
}

void compositionGeneral::reset()
{
    ssComposition::reset();
    i_scale = 0;
    i_units = 0;
    min_comp = 0.000000001;
}

void compositionGeneral::setNumberBins(int num)
{
    binsModel->setColumnCount(num);
}

float compositionGeneral::getMinComp() const
{
    return min_comp;
}

void compositionGeneral::setMinComp(float value)
{
    min_comp = value;
}

int compositionGeneral::getCompErrorIndex() const
{
    return compErrorIndex;
}

void compositionGeneral::setCompErrorIndex(int newCompErrorIndex)
{
    compErrorIndex = newCompErrorIndex;
}

int compositionGeneral::getCompErrorType() const
{
    return compErrorType;
}

void compositionGeneral::setCompErrorType(int newCompErrorType)
{
    compErrorType = newCompErrorType;
}
