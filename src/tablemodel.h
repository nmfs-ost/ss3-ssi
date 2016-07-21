#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QStandardItemModel>
#include <QHeaderView>
#include <QVector>
#include <QStringList>

class tablemodel : public QStandardItemModel
{
public:
    tablemodel(QObject *parent = 0);
    ~tablemodel();

    void reset() {setRowCount(0);}

public slots:
    void setRowData(int &row, QString rowstring);
    void setRowData(int row, QStringList &rowstringlist);
    void setRowData(int row, QVector<double> rowdata);

    QStringList getRowData (int row);
    QString getRowText (int row);

    void setHeader (QStringList titles);
    void setColumnHeader (int column, QString title);
    QString getColumnHeader (int column);
    void setRowHeader (int row, QString title);
    QString getRowHeader (int row);

};


#endif // TABLEMODEL_H
