#include <QCloseEvent>
#include "dialoginputerror.h"
#include "ui_dialoginputerror.h"

DialogInputError::DialogInputError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInputError)
{
    ui->setupUi(this);
    connect (ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
                 SLOT(buttonClicked(QAbstractButton*)));
}

DialogInputError::~DialogInputError()
{
    delete ui;
}

void DialogInputError::connectIntCheck()
{
    connect (ui->lineEdit_newValue, SIGNAL(textChanged(QString)),
                 SLOT(checkIntString(QString)));
}

void DialogInputError::connectFloatCheck()
{
    connect (ui->lineEdit_newValue, SIGNAL(textChanged(QString)),
                 SLOT(checkFloatString(QString)));
}

void DialogInputError::disconnectCheck()
{
    disconnect (ui->lineEdit_newValue, SIGNAL(textChanged(QString)),
                 this, SLOT(checkIntString(QString)));
    disconnect (ui->lineEdit_newValue, SIGNAL(textChanged(QString)),
                 this, SLOT(checkFloatString(QString)));
}

void DialogInputError::getNewIntValue(int errValue, QString valDesc,
                       int minValue, int maxValue, int defaultValue)
{
    disconnectCheck();
    connectIntCheck();
    ui->label_error->setText(QString("The following value is out of range:"));
    ui->label_errValue->setText(QString::number(errValue));
    ui->label_description->setText(valDesc);
    ui->label_minValue->setText(QString::number(minValue));
    ui->label_maxValue->setText(QString::number(maxValue));
    ui->lineEdit_newValue->setText(QString::number(defaultValue));
    floatDefault = defaultValue;
    intVal = defaultValue;
    show();
}

void DialogInputError::getNewFloatValue(float errValue, QString valDesc,
                       float minValue, float maxValue, float defaultValue)
{
    disconnectCheck();
    connectFloatCheck();
    ui->label_error->setText(QString("The following value is out of range:"));
    ui->label_errValue->setText(QString::number(errValue));
    ui->label_description->setText(valDesc);
    ui->label_minValue->setText(QString::number(minValue));
    ui->label_maxValue->setText(QString::number(maxValue));
    ui->lineEdit_newValue->setText(QString::number(defaultValue));
    floatDefault = defaultValue;
    floatVal = defaultValue;
    show();
}

void DialogInputError::confirmValue(float errValue, QString valDesc,
                        float minValue, float maxValue, float defaultValue)
{
    ui->label_error->setText(QString("Please confirm the following value:"));
    ui->label_errValue->setText(QString::number(errValue));
    ui->label_description->setText(valDesc);
    ui->label_minValue->setText(QString::number(minValue));
    ui->label_maxValue->setText(QString::number(maxValue));
    ui->lineEdit_newValue->setText(QString::number(defaultValue));
    floatDefault = defaultValue;
    floatVal = defaultValue;
    show();
}

void DialogInputError::setFileName(QString name)
{
    ui->label_filename->setText(name);
}

void DialogInputError::setLineNum(int lineNum)
{
    ui->label_lineno->setText(QString::number(lineNum));
}

void DialogInputError::setError(QString error, float errValue, QString valDesc)
{
    ui->label_error->setText(error);
    ui->label_errValue->setText(QString::number(errValue));
    ui->label_description->setText(valDesc);
}

void DialogInputError::setMinMax(float minValue, float maxValue, float defaultValue)
{
    ui->label_minValue->setText(QString::number(minValue));
    ui->label_maxValue->setText(QString::number(maxValue));
    ui->lineEdit_newValue->setText(QString::number(defaultValue));
    floatDefault = floatVal = defaultValue;
}

void DialogInputError::checkFloatString(QString text)
{
    bool okay = false;
    float check = text.toFloat(&okay);
    if (okay)
        floatVal = check;
    else
        ui->lineEdit_newValue->setText(QString::number(floatVal));
}

void DialogInputError::checkIntString(QString text)
{
    bool okay = false;
    int check = text.toInt(&okay);
    if (okay)
        intVal = check;
    else
        ui->lineEdit_newValue->setText(QString::number(intVal));
}

void DialogInputError::buttonClicked(QAbstractButton *btn)
{
    if (btn->text().contains("Abort"))
    {
        floatVal = intVal = -999999;
        hide();
    }
    if      (btn->text().contains("Restore"))
    {
        ui->lineEdit_newValue->setText(QString::number(floatDefault));
    }
    else
    {
        emit dataChanged();
        hide();
    }
}

float DialogInputError::getFloatValue()
{
    return floatVal;
}

int DialogInputError::getIntValue()
{
    return intVal;
}

void DialogInputError::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}
