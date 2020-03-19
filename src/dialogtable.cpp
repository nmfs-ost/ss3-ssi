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
    ui->verticalLayout_table->addWidget(view);
    connect (ui->buttonBox, SIGNAL(done()), SLOT(closeTable()));
    resize(500, 900);
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
    view->setModel(table);
}

void DialogTable::closeTable()
{
    setVisible(false);
    emit close();
}

void DialogTable::resizeEvent(QResizeEvent *event) {
    currRect = QDialog::geometry();
    QDialog::resizeEvent(event);
}

void DialogTable::closeEvent(QCloseEvent *event) {
    closeTable();
    QDialog::closeEvent(event);
}

void DialogTable::moveEvent(QMoveEvent *event) {
    currRect = QDialog::geometry();
    QDialog::moveEvent(event);
}

void DialogTable::setVisible(bool flag) {
    if (flag)
        setGeometry(currRect);
    QDialog::setVisible(flag);
}

