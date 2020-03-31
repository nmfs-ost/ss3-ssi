#include "dar1.h"

DAR1::DAR1()
{
    active = false;

    QStringList header;
    QStringList data;
    header << "Ymin" << "Ymax" << "Amin" << "Amax" << "SigMax" << "UseRho";
    header << "Len:Age" << "Phase" << "Pre Range" << "PostRange";
    data << "1981" << "1989" << "16" << "31" << "16" << "1" << "1" << "5" << "1" << "3";
    specification = new tablemodel();
    specification->setColumnCount(10);
    specification->setHeader(header);
    specification->setRowCount(1);
    specification->setRowHeader(0, QString("Spec"));
    specification->setRowData(0, data);

    header.clear();
    header << "Lo" << "Hi" << "Init" << "Prior" << "Prior SD" << "Prior Type" << "Phase";
    parameters = new tablemodel();
    parameters->setColumnCount(7);
    parameters->setHeader(header);
    parameters->setRowCount(3);
    data.clear();
    parameters->setRowHeader(0, QString("Sigma selex"));
    data << "0.01" << "5" << "0.6" << "1" << "99" << "0" << "-5";
    parameters->setRowData(0, data);
    data.clear();
    parameters->setRowHeader(1, QString("Rho year"));
    data << "-0.8" << "0.8" << "0" << "0" << "99" << "0" << "-3";
    parameters->setRowData(1, data);
    data.clear();
    parameters->setRowHeader(2, QString("Rho age"));
    data << "-0.8" << "0.8" << "0.05" << "0" << "99" << "0" << "-3";
    parameters->setRowData(2, data);

}

DAR1::~DAR1()
{
    delete specification;
    delete parameters;
}

void DAR1::reset()
{
    active = false;
}

bool DAR1::getUse()
{
    return active;
}
void DAR1::setUse(bool use)
{
    active = use;
}

void DAR1::setSpec(QStringList data)
{
    active = true;
    specification->setRowData(0, data);
}
QStringList DAR1::getSpec()
{
    return specification->getRowData(0);
}
tablemodel *DAR1::getSpecTable()
{
    return specification;
}

void DAR1::setParam(int row, QStringList data)
{
    parameters->setRowData(row, data);
}
QStringList DAR1::getParam (int row)
{
    return parameters->getRowData(row);
}
tablemodel *DAR1::getParamTable()
{
    return parameters;
}

