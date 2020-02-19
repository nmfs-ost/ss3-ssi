#include "dialogparameterview.h"
#include "ui_dialogparameterview.h"

#include <QDesktopWidget>
#include <QScreen>

//#include <cmath>
#include "ss_math.h"

DialogParameterView::DialogParameterView(QWidget *parent, bool showTrans) :
    QDialog(parent),
    ui(new Ui::DialogParameterView)
{
    ui->setupUi(this);
    setShowTrans(showTrans);
    setName("");
    parameters = nullptr;
    numParamsShown = 0;
    numUnParams = 0;
    pLabel.append(nullptr); pLabel.clear();
    pMin.append(nullptr);   pMin.clear();
    pMax.append(nullptr);   pMax.clear();
    pInit.append(nullptr);  pInit.clear();
    pSlider.append(nullptr); pSlider.clear();
    sValue.append(nullptr);  sValue.clear();
    pType.append(nullptr);  pType.clear();
    eInput.append(nullptr); eInput.clear();
    clearAll();

    ui->pushButton_apply->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_reset->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_close->setFocusPolicy(Qt::NoFocus);
    connect (ui->pushButton_apply, SIGNAL(clicked()), SLOT(apply()));
    connect (ui->pushButton_reset, SIGNAL(clicked()), SLOT(reset()));
    connect (ui->pushButton_close, SIGNAL(clicked()), SLOT(close()));
    connectAll();
    hide();
    window = size();
    position = pos();
    int screen = QApplication::desktop()->screenNumber(parent);
    int xdelta = 0;
    if (screen > 0)
        xdelta = QApplication::screens().at(screen)->size().width();
    move(position.x()+xdelta, position.y());
}

DialogParameterView::~DialogParameterView() {
    ui->labelParamMin->releaseKeyboard();
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
    while (unLabel.count() > 0) {
        delete unLabel.takeLast();
        delete unMin.takeLast();
        delete unMax.takeLast();
        delete unSlider.takeLast();
        delete unValue.takeLast();
    }
}


void DialogParameterView::disconnectAll() {
    for (int i = 0; i < pLabel.count(); i++) {
        disconnect (pMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        disconnect (pMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        disconnect (pSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        disconnect (sValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    }
    for (int i = 0; i < unLabel.count(); i++) {
        disconnect (unMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        disconnect (unMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        disconnect (unSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        disconnect (unValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    }
}

void DialogParameterView::connectAll() {
    for (int i = 0; i < numParamsShown; i++) {
        connect (pMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        connect (pMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        connect (pSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        connect (sValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    }
    for (int i = 0; i < unLabel.count(); i++) {
        connect (unMin.at(i), SIGNAL(valueChanged(double)), this, SLOT(minChanged(double)));
        connect (unMax.at(i), SIGNAL(valueChanged(double)), this, SLOT(maxChanged(double)));
        connect (unSlider.at(i), SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
        connect (unValue.at(i), SIGNAL(valueChanged(double)), this, SLOT(sValueChanged(double)));
    }
}

void DialogParameterView::setTitle(QString ttl) {
    title = ttl;
    if (name.isEmpty())
        QDialog::setWindowTitle(title);
    else {
        QDialog::setWindowTitle(QString("%1 %2").arg(name, title));
    }
}

void DialogParameterView::setName(QString nm) {
    name = nm;
    if (title.isEmpty()) {
        QDialog::setWindowTitle(QString("%1 Parameters").arg(name));
    }
    else {
        QDialog::setWindowTitle(QString("%1 %2").arg(name, title));
    }
}

void DialogParameterView::setParameterTable(tablemodel *params)
{
    if (params != nullptr) {
        parameters = params;
//        paramsChanged();
    }
}

void DialogParameterView::setupView(int num, int unsaved)
{
    int rownum = 0;
    int baseht = 174; int rowht = 35;
    if (parameters->rowCount() < num)
        parameters->setRowCount(num);
    position = pos();
    window = size();
    clearAll();
    for (int i = 0; i < num; i++) {
        rownum = i + 1;
        pLabel.append(new QLabel(parameters->getRowHeader(i), this));
        pMin.append(valueSpinBox());
        pMax.append(valueSpinBox());
        pInit.append(parameterSpinBox(false, false));
        pSlider.append(parameterSlider());
        sValue.append(parameterSpinBox());
        pType.append(new QLabel("Value", this));
        eInput.append(parameterSpinBox(false, false));

        ui->gridLayoutParam->addWidget(pLabel.at(i), rownum, 0);
        ui->gridLayoutParam->addWidget(pMin.at(i), rownum, 1);
        ui->gridLayoutParam->addWidget(pMax.at(i), rownum, 2);
        ui->gridLayoutParam->addWidget(pInit.at(i), rownum, 3);
        ui->gridLayoutParam->addWidget(pSlider.at(i), rownum, 4);
        ui->gridLayoutParam->addWidget(sValue.at(i), rownum, 5);
        if (transVisible) {
            ui->gridLayoutParam->addWidget(pType.at(i), rownum, 6);
            ui->gridLayoutParam->addWidget(eInput.at(i), rownum, 7);
        }
    }
    for (int i = 0; i < unsaved; i++) {
        unLabel.append(new QLabel("", this));
        unMin.append(valueSpinBox());
        unMax.append(valueSpinBox());
        unSlider.append(parameterSlider());
        unValue.append(parameterSpinBox(false, false));

        rownum = i + 1;
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unLabel.at(i), rownum, 0);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unMin.at(i), rownum, 1);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unMax.at(i), rownum, 2);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unSlider.at(i), rownum, 3);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unValue.at(i), rownum, 4);
    }
    ui->groupBoxUnsaved->setVisible(numUnParams > 0);

    int ht = window.height();
    int numshow = num > 1? (num > 10? 10: num-1): 0;
    int newht = baseht + (numshow * rowht);
    int ypos = position.ry() + ((ht - newht)/2);
    int xpos = position.rx();
    if (numUnParams > 0)
        newht += baseht + ((numUnParams - 2) * rowht);
    QPoint parpos = static_cast<QWidget *>(parent())->pos();
    window.setHeight(newht);
    position.setX((xpos == 0)? parpos.rx(): xpos);
    position.setY(ypos);
    resize(window);
    move(position);
}

/*void DialogParameterView::setupUnSavedView(int unsaved) {
    int rownum = 0;
    disconnectAll();
    for (int i = 0; i < unsaved; i++) {
        unLabel.append(new QLabel("", this));
        unMin.append(valueSpinBox());
        unMax.append(valueSpinBox());
        unSlider.append(parameterSlider());
        unValue.append(parameterSpinBox(false, false));

        rownum = i + 1;
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unLabel.at(i), rownum, 0);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unMin.at(i), rownum, 1);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unMax.at(i), rownum, 2);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unSlider.at(i), rownum, 3);
        static_cast<QGridLayout *>(ui->groupBoxUnsaved->layout())->addWidget(unValue.at(i), rownum, 4);
    }
    ui->groupBoxUnsaved->setVisible(numUnParams > 0);
    connectAll();
}*/

void DialogParameterView::setSliders()
{
    QStringList paramvalues;
    double min, max, init;
    if (numParamsShown <= parameters->rowCount()) {
        for (int i = 0; i < numParamsShown; i++) {
            paramvalues = parameters->getRowData(i);
            min = paramvalues.at(0).toDouble();
            max = paramvalues.at(1).toDouble();
            init = paramvalues.at(2).toDouble();
            pInit.at(i)->setRange(min, max);
            pInit.at(i)->setValue(init);
            setSliderRange (i, min, max);
            setSliderValue(i, init);
            setInput(i);
        }
    }
}

bool DialogParameterView::setInput(int pnum) {
    bool changed = false;
    if (pLabel.count() > pnum) {
        double value = sValue.at(pnum)->value();
        double oldValue = eInput.at(pnum)->value(), newValue = 0;
        QString txt = pType.at(pnum)->text();

        if (txt.contains("exp", Qt::CaseInsensitive)) {
            newValue = exp(value);
        }
        else if (txt.contains("logist", Qt::CaseInsensitive)) {
            newValue = logist(value);
        }
        else if (txt.contains("log", Qt::CaseInsensitive)) {
            newValue = log(value);
        }
        else if (txt.contains("int", Qt::CaseInsensitive)) {
            newValue = static_cast<int>(value + .5);
        }
        else { //if (txt.contains("value", Qt::CaseInsensitive)) {
            newValue = value;
        }

        if (newValue < oldValue || newValue > oldValue) {
            changed = true;
            setInputValue(pnum, newValue);
        }
    }
//    if (changed)
//        emit inputChanged();
    return changed;
}

void DialogParameterView::setInputValue(int pnum, double value) {
    eInput.at(pnum)->setRange((value-5.0), (value+5.0));
    eInput.at(pnum)->setValue(value);
}

void DialogParameterView::setNumParamsShown(int num, int unsaved) {
    disconnectAll();
    numUnParams = unsaved;
    if (parameters != nullptr) {
        if (numParamsShown != num) {
            numParamsShown = num;
            if (parameters->rowCount() < numParamsShown)
                numParamsShown = parameters->rowCount();
            setupView(num, unsaved);
            setSliders();
        }
    }
    else {
        numParamsShown = 0;
        setupView(num, unsaved);
    }
    connectAll();
}

int DialogParameterView::getNumParameters() {
    return numParamsShown;
}

void DialogParameterView::setNumUnParameters(int num) {
    if (numUnParams != num) {
        numUnParams = num;
    }
}

void DialogParameterView::paramsChanged() {
    if (parameters != nullptr)
    {
        disconnectAll();
        setSliders();
        sliderChanged(1);
        connectAll();
    }
}

void DialogParameterView::setSliderRange(int pnum, double min, double max) {
    int intmin = static_cast<int>(min * 1000.0 + .5);
    int intmax = static_cast<int>(max * 1000.0 + .5);
    int unnum = pnum - numParamsShown;
    if (unnum < 0) {
        pSlider.at(pnum)->setRange(intmin, intmax);
        sValue.at(pnum)->setRange(min, max);
        pMax.at(pnum)->setRange(max);
        pMax.at(pnum)->setValue(max);
        pMin.at(pnum)->setRange(min);
        pMin.at(pnum)->setValue(min);
    }
    else {
        if (unnum < numUnParams) {
            unSlider.at(unnum)->setRange(intmin, intmax);
            unValue.at(unnum)->setRange(min, max);
            unMax.at(unnum)->setRange(max);
            unMax.at(unnum)->setValue(max);
            unMin.at(unnum)->setRange(min);
            unMin.at(unnum)->setValue(min);
        }
    }
}

void DialogParameterView::setSliderValue(int pnum, double value) {
    int intval = static_cast<int>(value * 1000.0 + .5);
    int unnum = pnum - numParamsShown;
    if (unnum < 0) {
        pSlider.at(pnum)->setValue(intval);
        sValue.at(pnum)->setValue(value);
    }
    else {
        if (unnum < numUnParams)
            unSlider.at(unnum)->setValue(intval);
            unValue.at(unnum)->setValue(value);
    }
}

void DialogParameterView::setName(int pnum, QString name) {
    int unnum = pnum - numParamsShown;
    if (unnum < 0) {
        pLabel.at(pnum)->setText(name);
        parameters->setRowHeader(pnum, name);
    }
    else {
        if (unnum < numUnParams)
            unLabel.at(unnum)->setText(name);
    }
}

void DialogParameterView::setType(int pnum, QString type) {
    int unnum = pnum - numParamsShown;
    if (unnum < 0) {
        pType.at(pnum)->setText(type);
        setInput(pnum);
    }
}

void DialogParameterView::minChanged(double value) {
    double dMin = 0, dMax = 1;

    for (int i = 0; i < numParamsShown; i++) {
        dMin = pMin.at(i)->value();
        dMax = pMax.at(i)->value();
        if (checkMinMax(&dMin, &dMax)) {
            pMin.at(i)->setValue(dMin);
            pMax.at(i)->setValue(dMax);
        }
        setSliderRange(i, dMin, dMax);
    }
    for (int i = 0; i < numUnParams; i++) {
        dMin = unMin.at(i)->value();
        dMax = unMax.at(i)->value();
        if (checkMinMax(&dMin, &dMax)) {
            unMin.at(i)->setValue(dMin);
            unMax.at(i)->setValue(dMax);
        }
        setSliderRange((numParamsShown + i), dMin, dMax);
    }
}

void DialogParameterView::maxChanged(double value) {
    double dMin = 0, dMax = 1;

    for (int i = 0; i < numParamsShown; i++) {
        dMin = pMin.at(i)->value();
        dMax = pMax.at(i)->value();
        if (checkMinMax(&dMin, &dMax)) {
            pMin.at(i)->setValue(dMin);
            pMax.at(i)->setValue(dMax);
        }
        setSliderRange(i, dMin, dMax);
    }
    for (int i = 0; i < numUnParams; i++) {
        dMin = unMin.at(i)->value();
        dMax = unMax.at(i)->value();
        if (checkMinMax(&dMin, &dMax)) {
            unMin.at(i)->setValue(dMin);
            unMax.at(i)->setValue(dMax);
        }
        setSliderRange((numParamsShown + i), dMin, dMax);
    }
}

void DialogParameterView::sliderChanged(int value) {
    Q_UNUSED(value);
    bool changed = false;
    int i = 0;
    int val = 0, iMin = 0, iMax = 1;
    int check = 0;
    double sval = 0;

    for (i = 0; i < pSlider.count(); i++) {
        val = pSlider.at(i)->value();
        iMin = static_cast<int>(pMin.at(i)->value() * 1000 + .5);
        iMax = static_cast<int>(pMax.at(i)->value() * 1000 + .5);
        checkMinMax(&iMin, &iMax);
        check = static_cast<int>(sValue.at(i)->value() * 1000.0 + .5);
        if (val > iMax) {
            pSlider.at(i)->setRange(iMin, iMax);
        }
        else if (val < iMin) {
            pSlider.at(i)->setRange(iMin, iMax);
        }
        else if (val != check)
        {
            sval = static_cast<double>(val) / 1000.0;
            sValue.at(i)->setValue(sval);
            changed = true;
        }
    }
    for (i = 0; i < numUnParams; i++) {
        val = unSlider.at(i)->value();
        iMin = static_cast<int>(unMin.at(i)->value() * 1000 + .5);
        iMax = static_cast<int>(unMax.at(i)->value() * 1000 + .5);
        checkMinMax(&iMin, &iMax);
        check = static_cast<int>(unValue.at(i)->value() * 1000.0 + .5);
        if (val > iMax) {
            unSlider.at(i)->setRange(iMin, iMax);
        }
        else if (val < iMin) {
            unSlider.at(i)->setRange(iMin, iMax);
        }
        else if (val != check)
        {
            sval = static_cast<double>(val) / 1000.0;
            unValue.at(i)->setValue(sval);
            changed = true;
        }
    }
    if (changed)
        emit inputChanged();
}


void DialogParameterView::sValueChanged(double value) {
    Q_UNUSED(value);
    bool changed = false;
    int i = 0, check = 0, sliderVal = 0;
    double sval = 0;
    double dMin = 0;
    double dMax = 0;
    int num = pLabel.count();
    for (i = 0; i < num; i++) {
        sval = sValue.at(i)->value();
        dMin = pMin.at(i)->value();
        dMax = pMax.at(i)->value();
        if (checkMinMax (&dMin, &dMax)) {
            pMin.at(i)->setValue(dMin);
            pMax.at(i)->setValue(dMax);
        }
        check = static_cast<int>(sval * 1000.0 + .5);
        sliderVal = pSlider.at(i)->value();
        if (sval > dMax) {
            sValue.at(i)->setRange(dMin, dMax);
        }
        else if (sval < dMin) {
            sValue.at(i)->setRange(dMin, dMax);
        }
        else {
            check = static_cast<int>(sValue.at(i)->value() * 1000 + .5);
            if (sliderVal != check){
                pSlider.at(i)->setValue(check);
                changed = true;
            }
        }
        changed = convertToInput(i)? true: changed;
    }

    for (i = 0; i < numUnParams; i++) {
        sval = unValue.at(i)->value();
        dMin = unMin.at(i)->value();
        dMax = unMax.at(i)->value();
        if (checkMinMax (&dMin, &dMax)) {
            unMin.at(i)->setValue(dMin);
            unMax.at(i)->setValue(dMax);
        }
        check = static_cast<int>(sval * 1000.0 + .5);
        sliderVal = unSlider.at(i)->value();
        if (sval > dMax) {
            unValue.at(i)->setRange(dMin, dMax);
        }
        else if (sval < dMin) {
            unValue.at(i)->setRange(dMin, dMax);
        }
        else {
            check = static_cast<int>(unValue.at(i)->value() * 1000 + .5);
            if (sliderVal != check){
                unSlider.at(i)->setValue(check);
                changed = true;
            }
        }
    }
    if (changed)
        emit inputChanged();
}


bool DialogParameterView::convertToInput(int pnum) {
    return setInput(pnum);
/*    if (!pLabel.isEmpty()) {
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
            emit dataChanged();
        }
    }*/
}

double DialogParameterView::getValue(int pnum) {
    double val = -1.0;
    int unnum = pnum - numParamsShown;
    if (unnum < 0) {
        val = sValue.at(pnum)->value();
    }
    else {
        if (unnum < numUnParams)
            val = unValue.at(unnum)->value();
    }
    return val;
}

double DialogParameterView::getInput(int pnum) {
    double val = -1.0;
    int unnum = pnum - numParamsShown;
    if (unnum < 0) {
        val = eInput.at(pnum)->value();
    }
    else {
        val = unValue.at(unnum)->value();
    }
    return val;
}

void DialogParameterView::setVisible(bool visible) {
    if (visible) {
        resize(window);
        if (position.rx() != 0)
            move(position);
//        if (parameters != nullptr)
//            connect (parameters, SIGNAL(dataChanged()), SLOT(paramsChanged()));
        reset();
    }
    else {
        position = pos();
        window = size();
//        if (parameters != nullptr)
//            disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(paramsChanged()));
    }
    QDialog::setVisible(visible);
}

void DialogParameterView::show() {
    setVisible(true);
}

void DialogParameterView::hide() {
    setVisible(false);
}

void DialogParameterView::closeEvent(QCloseEvent *evt) {
    Q_UNUSED(evt);
    QDialog::hide();
    emit hidden();
}

void DialogParameterView::cancel() {
    close();
}

void DialogParameterView::reset() {
    disconnectAll();
    setSliders();
//    sValueChanged(0);
    connectAll();
}

void DialogParameterView::apply() {
    QStringList paramvalues;
    double val, min, max;
    QString label;

    disconnectAll();
//    disconnect (parameters, SIGNAL(dataChanged()), this, SLOT(paramsChanged()));

    for (int i = 0; i < numParamsShown; i++) {
        paramvalues = parameters->getRowData(i);
        label = pLabel.at(i)->text();
        min = pMin.at(i)->value();
        max = pMax.at(i)->value();
        if (pType.at(i)->text().contains("Int"))
            val = eInput.at(i)->value();
        else
            val = sValue.at(i)->value();
        paramvalues[0] = QString::number(min);
        paramvalues[1] = QString::number(max);
        paramvalues[2] = QString::number(val);
        parameters->setRowData(i, paramvalues);
        parameters->setRowHeader(i, label);
        pInit.at(i)->setValue(val);
    }
    connectAll();
//    connect (parameters, SIGNAL(dataChanged()), this, SLOT(paramsChanged()));
}

doubleLimitSpinBox *DialogParameterView::valueSpinBox(bool arrows, bool read) {
    doubleLimitSpinBox *vsb = new doubleLimitSpinBox(this);
    vsb->setFocusPolicy(Qt::StrongFocus);
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
    psb->setFocusPolicy(Qt::StrongFocus);
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
    ps->setFocusPolicy(Qt::StrongFocus);
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

void DialogParameterView::resizeEvent(QResizeEvent *event)
{
    window = size();
    QDialog::resizeEvent(event);
}
void DialogParameterView::moveEvent(QMoveEvent *event)
{
    position = pos();
    QDialog::moveEvent(event);
}
void DialogParameterView::moveDelta(QPoint delta)
{
    QRect thisRect;
    bool doMove = false;
    int x = position.x() + delta.x();
    int y = position.y() + delta.y();
    move (x, y);
    thisRect = QApplication::desktop()->screenGeometry(this);
    if (window.width() > thisRect.width()) {
        x = thisRect.width() - window.width();
        doMove = true;
    }
    if (window.height() > thisRect.height()) {
        y = thisRect.height() - window.height();
        doMove = true;
    }
    if (doMove)
       move (x, y);
}

void DialogParameterView::setScreen(int scrNum)
{
    int thisScrNum = QApplication::desktop()->screenNumber(position);
    int deltaWidth = 0;
    for (int i = thisScrNum; i < scrNum; i++)
        deltaWidth += QApplication::desktop()->screenGeometry(i).width();
    for (int i = scrNum; i < thisScrNum; i++)
        deltaWidth -= QApplication::desktop()->screenGeometry(i).width();
    moveDelta(QPoint(deltaWidth, 0));
}

bool DialogParameterView::getShowTrans() const
{
    return transVisible;
}

void DialogParameterView::setShowTrans(bool value)
{
    transVisible = value;
    ui->labelParamType->setVisible(transVisible);
    ui->labelEquationInput->setVisible(transVisible);
}


template<typename T>
bool DialogParameterView::checkMinMax(T *min, T *max)
{
    bool changed = false;
    if (*min > *max) {
        T temp = *min;
        *min = *max;
        *max = temp;
        changed = true;
    }
    return changed;
}

