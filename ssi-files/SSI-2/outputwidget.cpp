#include "outputwidget.h"
#include "ui_outputwidget.h"

outputWidget::outputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputWidget)
{
    ui->setupUi(this);
}

outputWidget::~outputWidget()
{
    delete ui;
}
