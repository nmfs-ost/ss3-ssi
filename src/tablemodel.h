/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QStandardItemModel>
#include <QHeaderView>
#include <QVector>
#include <QStringList>

class tablemodel : public QStandardItemModel
{
    Q_OBJECT

public:
    tablemodel(QObject *parent = nullptr);
    ~tablemodel();

    void reset();

public slots:
    void setRowCount(int rows);
    void setRowData(int &row, QString rowstring);
    void setRowData(int row, QStringList &rowstringlist);
    void setRowData(int row, QVector<double> rowdata);
    void addRowData(QStringList &rowstringlist);

    QStringList getRowData (int row);
    QString getRowText (int row);

    void removeHeader();
    void setHeader (QStringList &titles);
    void setColumnHeader (int column, QString title);
    QString getColumnHeader(int column);
    void setRowHeader (int row, QString title);
    QString getRowHeader (int row);

    QString &getTitle ();
    void setTitle (QString strtitle);

signals:
    void dataChanged();

protected:
    QString title;
};


#endif // TABLEMODEL_H
