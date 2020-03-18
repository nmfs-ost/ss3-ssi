#include "dialogtable.h"
#include "ui_dialogtable.h"

DialogTable::DialogTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTable)
{
    ui->setupUi(this);

    table = nullptr;
    view = new tableview();
    view->setParent(this);
    connect (ui->buttonBox, SIGNAL(done()), SLOT(close()));
}

DialogTable::~DialogTable()
{
    delete ui;
    delete view;
}

tablemodel *DialogTable::getTable() const
{
    return table;
}

void DialogTable::setTable(tablemodel *value)
{
    table = value;
}
