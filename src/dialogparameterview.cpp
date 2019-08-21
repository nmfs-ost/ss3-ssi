#include "dialogparameterview.h"
#include "ui_dialogparameterview.h"

#include <cmath>

DialogParameterView::DialogParameterView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogParameterView)
{
    ui->setupUi(this);
    setTitle("");
    parameters = new tablemodel();
    numParamsShown = -1;
    pLabel.append(nullptr); pLabel.clear();
    pMin.append(nullptr);   pMin.clear();
    pMax.append(nullptr);   pMax.clear();
    pInit.append(nullptr);  pInit.clear();
    pSlider.append(nullptr); pSlider.clear();
    sValue.append(nullptr);  sValue.clear();
    pType.append(nullptr);  pType.clear();
    eInput.append(nullptr); eInput.clear();
    clearAll();
    hide();

    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
}

DialogParameterView::~DialogParameterView() {
    clearAll();
    delete ui;
}

void DialogParameterView::clearAll() {
    if (pLabel.isEmpty())
        return;

    disconnectAll();
/*    for (int i = 0; i < pLabel.count(); i++) {
        disconnect (pMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        disconnect (pMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        disconnect (pSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        disconnect (sValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    }
*/
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

void DialogParameterView::disconnectAll() {
    if (pMin.count() < numParamsShown)
        numParamsShown = pMin.count();
    for (int i = 0; i < numParamsShown; i++) {
        disconnect (pMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        disconnect (pMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        disconnect (pSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        disconnect (sValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    }
}

void DialogParameterView::connectAll() {
    for (int i = 0; i < numParamsShown; i++) {
        connect (pMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        connect (pMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        connect (pSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        connect (sValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    }
}

void DialogParameterView::setTitle(QString title) {
    QDialog::setWindowTitle(QString("%1 Parameters").arg(title));
}

void DialogParameterView::setParameterTable(tablemodel *params)
{
    if (params->rowCount() > 0) {
        disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(paramsChanged()));
        parameters = params;
        paramsChanged();
        connect (parameters, SIGNAL(dataChanged()), SLOT(paramsChanged()));
    }
}

void DialogParameterView::setNumParamsShown(int num) {
    numParamsShown = num;
    paramsChanged();
}

int DialogParameterView::getNumParameters() {
    return parameters->rowCount();
}

void DialogParameterView::paramsChanged() {
    int num = parameters->rowCount();
    if (numParamsShown < 0)
        numParamsShown = num;
    QStringList paramvalues;
    double min, max, init;

    clearAll();

    for (int i = 0; i < numParamsShown; i++) {
        int rownum = i + 1;
        pLabel.append(new QLabel(parameters->getRowHeader(i), this));
        pMin.append(valueSpinBox());
        pMax.append(valueSpinBox());
        pInit.append(parameterSpinBox(false, false));
        pSlider.append(parameterSlider());
        sValue.append(parameterSpinBox());
        pType.append(new QLabel("Value", this));
        eInput.append(parameterSpinBox(false, false));

        ui->gridLayout->addWidget(pLabel.at(i), rownum, 0);
        ui->gridLayout->addWidget(pMin.at(i), rownum, 1);
        ui->gridLayout->addWidget(pMax.at(i), rownum, 2);
        ui->gridLayout->addWidget(pInit.at(i), rownum, 3);
        ui->gridLayout->addWidget(pSlider.at(i), rownum, 4);
        ui->gridLayout->addWidget(sValue.at(i), rownum, 5);
        ui->gridLayout->addWidget(pType.at(i), rownum, 6);
        ui->gridLayout->addWidget(eInput.at(i), rownum, 7);

        paramvalues = parameters->getRowData(i);
        min = paramvalues.at(0).toDouble();
        max = paramvalues.at(1).toDouble();
        init = paramvalues.at(2).toDouble();
        pMin.at(i)->setValue(min);
        pMax.at(i)->setValue(max);
        pInit.at(i)->setRange(min, max);
        pInit.at(i)->setValue(init);
        setSliderRange (i, min, max);
        setSlider(i, init);
        sValue.at(i)->setRange(min, max);
        sValue.at(i)->setValue(init);

/*        connect (pMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        connect (pMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        connect (pSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        connect (sValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    */}
    minChanged(1.0);
    maxChanged(1.0);
    connectAll();
}

void DialogParameterView::setSliderRange(int pnum, double min, double max) {
    int intmin = static_cast<int>(min * 1000.0);
    int intmax = static_cast<int>(max * 1000.0);
    pSlider.at(pnum)->setRange(intmin, intmax);
}

void DialogParameterView::setSlider(int pnum, double value) {
    int intval = static_cast<int>(value * 1000.0);
    pSlider.at(pnum)->setValue(intval);
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

void DialogParameterView::minChanged(double value) {
    Q_UNUSED(value);
    for (int i = 0; i < pLabel.count(); i++) {
        double dMin = pMin.at(i)->value();
        double dMax = pMax.at(i)->value();

        sValue.at(i)->setRange(dMin, dMax);
        setSliderRange(i, dMin, dMax);
    }
}

void DialogParameterView::maxChanged(double value) {
    Q_UNUSED(value);
    for (int i = 0; i < pLabel.count(); i++) {
        double dMin = pMin.at(i)->value();
        double dMax = pMax.at(i)->value();

        sValue.at(i)->setRange(dMin, dMax);
        setSliderRange(i, dMin, dMax);
    }
}

void DialogParameterView::sliderChanged(int value) {
    Q_UNUSED(value);
    int num = pLabel.count();
    for (int i = 0; i < num; i++) {
        int val = pSlider.at(i)->value();
        int iMin = static_cast<int>(pMin.at(i)->value() * 1000);
        int iMax = static_cast<int>(pMax.at(i)->value() * 1000);
        int check = static_cast<int>(sValue.at(i)->value() * 1000.0);
        if (val > iMax) {
            pSlider.at(i)->setValue(iMax);
        }
        else if (val < iMin) {
            pSlider.at(i)->setValue(iMin);
        }
        else if (val != check)
        {
            double sval = static_cast<double>(value) / 1000.0;
            sValue.at(i)->setValue(sval);
        }
    }
}


void DialogParameterView::sValueChanged(double value) {
    Q_UNUSED(value);
    int num = pLabel.count();
    for (int i = 0; i < num; i++) {
        double sval = sValue.at(i)->value();
        double dMin = pMin.at(i)->value();
        double dMax = pMax.at(i)->value();
        int check = static_cast<int>(sval * 1000.0);
        int sliderval = pSlider.at(i)->value();
        if (sval > dMax) {
            sValue.at(i)->setValue(dMax);
        }
        else if (sval < dMin) {
            sValue.at(i)->setValue(dMin);
        }
        else
        {
            if (sliderval != check)
                pSlider.at(i)->setValue(check);
        }
        convertToInput(i);
    }
    emit dataChanged();
}


void DialogParameterView::convertToInput(int pnum) {
    double value = sValue.at(pnum)->value();
    double oldValue = eInput.at(pnum)->value(), newValue = 0;
    QString txt = pType.at(pnum)->text();
    if (txt.contains("exp", Qt::CaseInsensitive)) {
        newValue = exp(value);
    }
    else { //if (txt.contains("value", Qt::CaseInsensitive)) {
        newValue = value;
    }
    if (newValue < oldValue || newValue > oldValue) {
        if (eInput.at(pnum)->minimum() > newValue)
            eInput.at(pnum)->setMinimum(newValue - 1.0);
        if (eInput.at(pnum)->maximum() < newValue)
            eInput.at(pnum)->setMaximum(newValue + 1.0);
        eInput.at(pnum)->setValue(newValue);
    }
}

double DialogParameterView::getInput(int pnum) {
    double val = -1.0;
    if (eInput.count() > pnum) {
        val = eInput.at(pnum)->value();
    }
    return val;
}
void DialogParameterView::closeEvent(QCloseEvent *evt) {
    Q_UNUSED(evt);
    hide();
    emit closed();
}

void DialogParameterView::cancel() {
    close();
}

void DialogParameterView::reset() {
    disconnectAll();
    for (int i = 0; i < numParamsShown; i++) {
        QStringList paramvalues = parameters->getRowData(i);
        pMin.at(i)->setValue(paramvalues.at(0).toDouble());
        pMax.at(i)->setValue(paramvalues.at(1).toDouble());
        pInit.at(i)->setValue(paramvalues.at(2).toDouble());
        setSliderRange (i, pMin.at(i)->value(), pMax.at(i)->value());
        sValue.at(i)->setValue(pInit.at(i)->value());
    }
    connectAll();
    sValueChanged(1.0);
}

void DialogParameterView::apply() {
    disconnectAll();
    for (int i = 0; i < numParamsShown; i++) {
        QStringList paramvalues = parameters->getRowData(i);
        paramvalues[0] = QString::number(pMin.at(i)->value());
        paramvalues[1] = QString::number(pMax.at(i)->value());
        paramvalues[2] = QString::number(pInit.at(i)->value());
        parameters->setRowData(i, paramvalues);
    }
    connectAll();
}

doubleLimitSpinBox *DialogParameterView::valueSpinBox(bool arrows, bool read) {
    doubleLimitSpinBox *vsb = new doubleLimitSpinBox(this);
    vsb->setValue(0);
    vsb->setDecimals(3);
    vsb->setSingleStep(.001);
    if (!arrows)
        vsb->setButtonSymbols(QAbstractSpinBox::NoButtons);
    if (!read)
        vsb->setReadOnly(true);
    return vsb;
}

QDoubleSpinBox *DialogParameterView::parameterSpinBox(bool arrows, bool read) {
    QDoubleSpinBox *psb = new QDoubleSpinBox(this);
    psb->setRange(-200, 200);
    psb->setDecimals(3);
    psb->setSingleStep(.001);
    if (!arrows)
        psb->setButtonSymbols(QAbstractSpinBox::NoButtons);
    if (!read)
        psb->setReadOnly(true);
    return psb;
}

QSlider *DialogParameterView::parameterSlider() {
    QSlider *ps = new QSlider(Qt::Horizontal, this);
    ps->setRange(-20000, 20000);
    return ps;
}

void DialogParameterView::buttonClicked(QAbstractButton* btn) {
    QString txt = btn->text();

    if      (txt.contains("Apply")) {
        apply();
    }
    else if (txt.contains("Reset")) {
        reset();
    }
    else {// if (btn->text().contains("Close")) {
        close();
    }
}