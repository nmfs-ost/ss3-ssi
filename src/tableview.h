#ifndef TABLEVIEW_H
#define TABLEVIEW_H

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
    void copy();
    void paste();


};

#endif // TABLEVIEW_H
