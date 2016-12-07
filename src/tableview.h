#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QTableView>
#include <QKeyEvent>
#include <QMouseEvent>

#include "lineeditdelegate.h"
#include "tablemodel.h"

class tableview : public QTableView
{
public:
    tableview();

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void setHeight (tablemodel *tm, int rd = 0);
    void setHeight (int rows);

private:
    QMenu *popup;
    QAction *actionCopy;
    QAction *actionPaste;

    QAction *actionCopyRows;
    QAction *actionInsertCopied;
    QAction *actionInsert;
    QAction *actionAppend;
    QAction *actionDelete;
    void createActions();
    void createMenu();

    void contextMenu(QMouseEvent *event);

    void copy();
    void paste();
    void copyRows();
    void pasteRows();
    void insertCopiedRows();
    void insertRow();
    void appendRow();
    void deleteRows();

};

#endif // TABLEVIEW_H
