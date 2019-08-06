#include "dialogparameterview.h"
#include "ui_dialogparameterview.h"

#include <cmath>

DialogParameterView::DialogParameterView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogParameterView)
{
    ui->setupUi(this);
    setTitle("");

}

DialogParameterView::~DialogParameterView() {
    clearAll();
    delete ui;
}

void DialogParameterView::clearAll() {
    if (pLabel.isEmpty())
        return;

    while (pLabel.count() > 0) {
        delete pLabel.takeLast();
        delete pMin.takeLast();
        delete pMax.takeLast();
        delete pInit.takeLast();
        delete pSlider.takeLast();
        delete sValue.takeLast();
        delete pType.takeLast();
        delete eInput.takeLast();
    }
}

void DialogParameterView::setTitle(QString title) {
    QDialog::setWindowTitle(QString("%1 Parameters").arg(title));
}

void DialogParameterView::setParameterTable(tablemodel *params)
{
    parameters = params;
    connect (parameters, SIGNAL(dataChanged()), SLOT(paramsChanged()));
    paramsChanged();
}

int DialogParameterView::getNumParameters() {
    return parameters->rowCount();
}

void DialogParameterView::paramsChanged() {
    int num = parameters->rowCount();
    QStringList paramvalues;
    clearAll();

    for (int i = 0; i < num; i++) {
        int rownum = i + 1;
        pLabel.append(new QLabel(parameters->getRowHeader(i), this));
        pMin.append(new QDoubleSpinBox(this));
        pMax.append(new QDoubleSpinBox(this));
        pInit.append(new QDoubleSpinBox(this));
        pSlider.append(new QSlider(this));
        sValue.append(new QDoubleSpinBox(this));
        pType.append(new QLabel("Value", this));
        eInput.append(new QDoubleSpinBox(this));

        connect (pSlider.at(i), SIGNAL(valueChanged(int)), SLOT(sliderChanged(i,int)));
        connect (sValue.at(i), SIGNAL(valueChanged(double)), SLOT(sValueChanged(i, double)));
        connect (eInput.at(i), SIGNAL(valueChanged(double)), SIGNAL(inputChanged(i, double)));

        paramvalues = parameters->getRowData(i);
        pMin.at(i)->setValue(paramvalues.at(0).toDouble());
        pMax.at(i)->setValue(paramvalues.at(1).toDouble());
        pInit.at(i)->setValue(paramvalues.at(2).toDouble());
        setSliderRange (i, pMin.at(i)->value(), pMax.at(i)->value());
        sValue.at(i)->setValue(pInit.at(i)->value());

        ui->gridLayout->addWidget(pLabel.at(i), rownum, 1);
        ui->gridLayout->addWidget(pMin.at(i), rownum, 2);
        ui->gridLayout->addWidget(pMax.at(i), rownum, 3);
        ui->gridLayout->addWidget(pInit.at(i), rownum, 4);
        ui->gridLayout->addWidget(pSlider.at(i), rownum, 5);
        ui->gridLayout->addWidget(sValue.at(i), rownum, 6);
        ui->gridLayout->addWidget(pType.at(i), rownum, 7);
        ui->gridLayout->addWidget(eInput.at(i), rownum, 8);
    }
}

void DialogParameterView::setSliderRange(int pnum, double min, double max) {
    int intmin = static_cast<int>(min * 1000.0);
    int intmax = static_cast<int>(max * 1000.0);
    pSlider.at(pnum)->setRange(intmin, intmax);
}

void DialogParameterView::setName(int pnum, QString name) {
    if (pLabel.count() > pnum) {
        pLabel.at(pnum)->setText(name);
    }
}

void DialogParameterView::setType(int pnum, QString type) {
    if (pType.count() > pnum) {
        pType.at(pnum)->setText(type);
        convertToInput (pnum);
    }
}

void DialogParameterView::sliderChanged(int pnum, int value) {
    int check = static_cast<int>(sValue.at(pnum)->value() * 1000.0);
    if (value != check)
    {
        double sval = static_cast<double>(value) / 1000.0;
        sValue.at(pnum)->setValue(sval);
    }
}

void DialogParameterView::sValueChanged(int pnum, double value) {
    int check = static_cast<int>(value * 1000.0);
    int svalue = pSlider.at(pnum)->value();

    if (svalue != check)
    {
        pSlider.at(pnum)->setValue(svalue);
        convertToInput(pnum);
    }
}

void DialogParameterView::convertToInput(int pnum) {
    double value = sValue.at(pnum)->value();
    QString txt = pType.at(pnum)->text();
    if (txt.contains("exp", Qt::CaseInsensitive)) {
        eInput.at(pnum)->setValue(exp(value));
    }
    else { //if (txt.contains("value", Qt::CaseInsensitive)) {
        eInput.at(pnum)->setValue(value);
    }

}

double DialogParameterView::getInput(int pnum) {
    double val = -1.0;
    if (eInput.count() > pnum)
        val = eInput.at(pnum)->value();
    return val;
}

void DialogParameterView::cancel() {
    hide();
}

void DialogParameterView::reset() {
    for (int i = 0; i < parameters->rowCount(); i++) {
        QStringList paramvalues = parameters->getRowData(i);
        pMin.at(i)->setValue(paramvalues.at(0).toDouble());
        pMax.at(i)->setValue(paramvalues.at(1).toDouble());
        pInit.at(i)->setValue(paramvalues.at(2).toDouble());
        setSliderRange (i, pMin.at(i)->value(), pMax.at(i)->value());
        sValue.at(i)->setValue(pInit.at(i)->value());
    }
}

void DialogParameterView::apply() {
    for (int i = 0; i < parameters->rowCount(); i++) {
        QStringList paramvalues = parameters->getRowData(i);
        paramvalues[0] = QString::number(pMin.at(i)->value());
        paramvalues[1] = QString::number(pMax.at(i)->value());
        paramvalues[2] = QString::number(pInit.at(i)->value());
        parameters->setRowData(i, paramvalues);
    }
}
