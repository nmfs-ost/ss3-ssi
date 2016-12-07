#include "tableview.h"

#include <QApplication>
#include <QClipboard>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QStringList>
#include <QMessageBox>


tableview::tableview()
{
    createActions();
    createMenu();
}


void tableview::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy))
    {
        copy();
    }
    else if (event->matches(QKeySequence::Paste))
    {
        paste();
    }
    else
    {
        QTableView::keyPressEvent(event);
    }
}

void tableview::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // display context menu
        popup->exec(event->globalPos());
    }
    else
    {
        QTableView::mousePressEvent(event);
    }
}

void tableview::copy()
{
    QStandardItemModel *abmodel = (QStandardItemModel *)model();
    QItemSelectionModel *selmodel = selectionModel();
    QModelIndexList list = selmodel->selectedIndexes();

    qSort(list);

    if(list.size() < 1)
        return;

    QString copy_table;
    QModelIndex last = list.last();
    QModelIndex previous = list.first();

    list.removeFirst();

    for(int i = 0; i < list.size(); i++)
    {
        QVariant data = abmodel->data(previous);
        QString text = data.toString();

        QModelIndex index = list.at(i);
        copy_table.append(text);

        if(index.row() != previous.row())

        {
            copy_table.append('\n');
        }
        else
        {
            copy_table.append('\t');
        }
        previous = index;
    }

    copy_table.append(abmodel->data(last, Qt::EditRole).toString());
    copy_table.append('\n');

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(copy_table);
}

void tableview::paste()
{
    QAbstractItemModel *abmodel = model();
    QItemSelectionModel *selmodel = selectionModel();
    QModelIndexList list = selmodel->selectedIndexes();
    QClipboard *clipboard = QApplication::clipboard();
    QString text (clipboard->text());
    if (text.isEmpty())
        return;
    QStringList rowTextList (text.split('\n', QString::SkipEmptyParts));
    int row, col, curr_row, curr_col;

    if(rowTextList.count() < 1)
        return;

    qSort(list);

    if (list.isEmpty())
        return;

    row = list.first().row();
    col = list.first().column();
    QModelIndex curr_index = list.first();

    for(int i = 0; i < rowTextList.count(); i++)
    {
        curr_row = row + i;
        if (curr_row >= abmodel->rowCount())
             abmodel->insertRow(curr_row);//break;
        QStringList colTextList(rowTextList.at(i).split('\t', QString::SkipEmptyParts));
        for (int j = 0; j < colTextList.count(); j++)
        {
            curr_col = col + j;
            if (col >= abmodel->columnCount())
                break;
            curr_index = abmodel->index(curr_row, curr_col);

            abmodel->setData (curr_index, colTextList[j], Qt::EditRole);
            update(curr_index);
        }
    }
}

void tableview::insertRow()
{
    QStandardItemModel *abmodel = (QStandardItemModel *)model();
    QItemSelectionModel *selmodel = selectionModel();
    QModelIndexList list = selmodel->selectedIndexes();
    QClipboard *clipboard = QApplication::clipboard();
    QString text (clipboard->text());
    if (text.isEmpty())
        return;
    QStringList rowTextList (text.split('\n', QString::SkipEmptyParts));
    int row, col, curr_row, curr_col;

    if(rowTextList.count() < 1)
        return;

    qSort(list);

    if (list.isEmpty())
        return;

    row = list.first().row();
    col = list.first().column();
    QModelIndex curr_index = list.first();

}

void tableview::copyRows()
{
    QStandardItemModel *abmodel = (QStandardItemModel *)model();
    QItemSelectionModel *selmodel = selectionModel();
    QModelIndexList list = selmodel->selectedIndexes();

    qSort(list);

    if(list.size() < 1)
        return;

    QString copy_table;
    QModelIndex last = list.last();
    QModelIndex previous = list.first();

    list.removeFirst();

    for(int i = 0; i < list.size(); i++)
    {
        QVariant data = abmodel->data(previous);
        QString text = data.toString();

        QModelIndex index = list.at(i);
        copy_table.append(text);

        if(index.row() != previous.row())

        {
            copy_table.append('\n');
        }
        else
        {
            copy_table.append('\t');
        }
        previous = index;
    }

    copy_table.append(abmodel->data(last, Qt::EditRole).toString());
    copy_table.append('\n');

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(copy_table);
}

void tableview::pasteRows()
{
    QStandardItemModel *abmodel = (QStandardItemModel *)model();
    QItemSelectionModel *selmodel = selectionModel();
    QModelIndexList list = selmodel->selectedIndexes();
    QClipboard *clipboard = QApplication::clipboard();
    QString text (clipboard->text());
    if (text.isEmpty())
        return;
    QStringList rowTextList (text.split('\n', QString::SkipEmptyParts));
    int row, col, curr_row, curr_col;

    if(rowTextList.count() < 1)
        return;

    qSort(list);

    if (list.isEmpty())
        return;

    row = list.first().row();
    col = list.first().column();
    QModelIndex curr_index = list.first();

}

void tableview::insertCopiedRows()
{
    QStandardItemModel *abmodel = (QStandardItemModel *)model();
    QItemSelectionModel *selmodel = selectionModel();
    QModelIndexList list = selmodel->selectedIndexes();
    QClipboard *clipboard = QApplication::clipboard();
    QString text (clipboard->text());
    if (text.isEmpty())
        return;
    QStringList rowTextList (text.split('\n', QString::SkipEmptyParts));
    int row, col, curr_row, curr_col;

    if(rowTextList.count() < 1)
        return;

    qSort(list);

    if (list.isEmpty())
        return;

    row = list.first().row();
    col = list.first().column();
    QModelIndex curr_index = list.first();

}

void tableview::appendRow()
{
    QStandardItemModel *abmodel = (QStandardItemModel *)model();
    QList<QStandardItem *> data;
    for (int i = 0; i < abmodel->columnCount(); i++)
        data.append(new QStandardItem(QString("")));
    abmodel->appendRow(data);
}

void tableview::deleteRows()
{
    QStandardItemModel *abmodel = (QStandardItemModel *)model();
    QItemSelectionModel *selmodel = selectionModel();
    QModelIndexList list = selmodel->selectedIndexes();

    qSort(list);

    int first, last;
    first = list.first().row();
    last = list.last().row();

    for (int i = first; i <= last; i++)
        abmodel->takeRow(i);
}

void tableview::contextMenu(QMouseEvent *event)
{
    popup->exec(event->globalPos());
}

void tableview::setHeight(tablemodel *tm, int rd)
{
    int rows = tm->rowCount() - rd;
    setHeight(rows);
}

void tableview::setHeight(int rows)
{
    if (rows == 0)
        setFixedHeight(0);
    else if (rows < 15)
        setFixedHeight(55 + rows * 36);
    else
        setFixedHeight(55 + 15 * 36);
}

void tableview::createActions()
{
    actionCopy = new QAction(tr("&Copy Values"), this);
    actionCopy->setShortcuts(QKeySequence::Copy);
    actionCopy->setStatusTip(tr("Copy selected values"));
    connect (actionCopy, &QAction::triggered, this, &tableview::copy);
    actionPaste = new QAction(tr("Paste Values"), this);
    actionPaste->setStatusTip(tr("Paste copied values at cursor position"));
    actionPaste->setShortcuts(QKeySequence::Paste);
    connect (actionPaste, &QAction::triggered, this, &tableview::paste);

    actionCopyRows = new QAction(tr("Copy Rows"), this);
    actionCopyRows->setStatusTip(tr("Copy the selected rows to clipboard"));
    connect (actionCopyRows, &QAction::triggered, this &tableview::copyRows);
    actionInsertCopied = new QAction(tr("Insert Copied Rows"), this);
    actionInsertCopied->setStatusTip(tr("Insert rows from clipboard after selection"));
    connect (actionInsertCopied, &QAction::triggered, this, &tableview::insertCopiedRows);
    actionInsert = new QAction(tr("Insert Row"), this);
    actionInsert->setStatusTip(tr("Insert a row before selection"));
    connect (actionInsert, &QAction::triggered, this, &tableview::insertRow);
    actionAppend = new QAction(tr("Append Row"));
    actionAppend->setStatusTip(tr("Add a row to the bottom of the table"));
    connect (actionAppend, &QAction::triggered, this, &tableview::appendRow);
    actionDelete = new QAction(tr("Delete Rows"), this);
    actionDelete->setStatusTip(tr("Delete selected rows"));
    connect (actionDelete, &QAction::triggered, this, &tableview::deleteRows);
}

void tableview::createMenu()
{
    popup = new QMenu(this);
    popup->addAction(actionCopy);
    popup->addAction(actionPaste);
    popup->addSeparator();
    popup->addAction(actionCopyRows);
    popup->addAction(actionInsertCopied);
    popup->addAction(actionInsert);
    popup->addAction(actionAppend);
    popup->addAction(actionDelete);
}
