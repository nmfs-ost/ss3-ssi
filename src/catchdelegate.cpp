#include "catchdelegate.h"

catchdelegate::catchdelegate(QWidget *parent)
    : QItemDelegate(parent)
{
    numSeasons = 1;
    maxCatch = 19999.0;
    startYear = 1950;
    endYear = 2020;
}

QWidget *catchdelegate::createEditor(QWidget *parent,
           const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *editor;
    Q_UNUSED(option);
    switch (index.column())
    {
    case 0:
        editor = new QSpinBox(parent);
        static_cast<QSpinBox*>(editor)->setRange(startYear, endYear);
        break;
    case 1:
        editor = new QSpinBox(parent);
        static_cast<QSpinBox*>(editor)->setRange(1, numSeasons);
//        static_cast<QSpinBox*>(editor)->setMaximum();
        break;
    case 2:
        editor = new QDoubleSpinBox(parent);
        static_cast<QDoubleSpinBox*>(editor)->setRange(0.0, maxCatch);
        break;
    default:
        editor = new QLineEdit(parent);
    }

    return editor;
}

void catchdelegate::setEditorData(QWidget *editor,
         const QModelIndex &index) const
{
    switch (index.column())
    {
    case 0:
    case 1:
    {
        int val = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *int_ed = static_cast<QSpinBox*>(editor);
        int_ed->setValue(val);
        break;
    }
    case 2:
    {
        double value = index.model()->data(index, Qt::EditRole).toDouble();
        QDoubleSpinBox *doub_ed = static_cast<QDoubleSpinBox*>(editor);
        doub_ed->setValue(value);
        break;
    }
    default:
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *line = static_cast<QLineEdit*>(editor);
        line->setText(text);
    }
    }
}

void catchdelegate::setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
    case 0:
    case 1:
    {
        QSpinBox *int_ed = static_cast<QSpinBox*>(editor);
        int val = int_ed->value();
        model->setData(index, QString::number(val), Qt::EditRole);
        break;
    }
    case 2:
    {
        QDoubleSpinBox *doub_ed = static_cast<QDoubleSpinBox*>(editor);
        double value = doub_ed->value();
        model->setData(index, QString::number(value), Qt::EditRole);
        break;
    }
    default:
    {
        QLineEdit *line = static_cast<QLineEdit*>(editor);
        QString text = line->text();
        model->setData(index, text, Qt::EditRole);
    }
    }
}

void catchdelegate::updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

