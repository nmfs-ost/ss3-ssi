/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class spinBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    spinBoxDelegate(QObject *parent = 0);

    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;
    void setMinimum (int value) {minimum = value;}
    void setMaximum (int value) {maximum = value;}
    void setRange (int lowvalue, int highvalue) {minimum = lowvalue;maximum = highvalue;}
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

protected:
    int minimum;
    int maximum;
};

#endif // SPINBOXDELEGATE_H
