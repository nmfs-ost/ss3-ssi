#include "dialogtable.h"
#include "ui_dialogtable.h"

DialogTable::DialogTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTable)
{
    ui->setupUi(this);
    setWindowTitle("Table Dialog");

    table = nullptr;
    view = new tableview();
    view->setParent(this);
    ui->verticalLayout_table->addWidget(view);
    connect (ui->pushButton_done, SIGNAL(released()), SLOT(close()));
    resize(500, 900);
}

DialogTable::~DialogTable()
{
    delete ui;
    delete view;
}

void DialogTable::setTitle(QString title)
{
    ui->label_title->setText(title);
}

tablemodel *DialogTable::getTable() const
{
    return table;
}

void DialogTable::setTable(tablemodel *value)
{
    table = value;
    view->setModel(table);
    view->setHeight(table->rowCount() + 1);
}

void DialogTable::closeTable()
{
    setVisible(false);
    emit tableClosed();
}

void DialogTable::setHeaders(QStringList header)
{
    table->setHeader(header);
}

void DialogTable::setRowHeader(int row, QString header)
{
    table->setRowHeader(row, header);
}

void DialogTable::setRowData (int row, QStringList data)
{
    table->setRowData(row, data);
}


void DialogTable::resizeEvent(QResizeEvent *event) {
    currRect = QDialog::geometry();
    QDialog::resizeEvent(event);
}

void DialogTable::closeEvent(QCloseEvent *event) {
    closeTable();
//    QDialog::closeEvent(event);
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

