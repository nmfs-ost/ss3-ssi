#ifndef EQUATIONDIALOG_H
#define EQUATIONDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "selectivity.h"
#include "fleet.h"
#include "model.h"

namespace Ui {
class equationDialog;
}

class equationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit equationDialog(QWidget *parent = 0);
    ~equationDialog();

public slots:
    void setFleet (Fleet *flt);
    void setDataModel (ss_model *data);

    void slider1Changed(int value);
    void value1Changed (double value);

    void slider2Changed(int value);
    void value2Changed (double value);
    void min2Changed (double value);
    void max2Changed (double value);

    void slider3Changed(int value);
    void value3Changed (double value);
    void min3Changed (double value);
    void max3Changed (double value);

    void slider4Changed(int value);
    void value4Changed (double value);
    void min4Changed (double value);
    void max4Changed (double value);

    void slider5Changed(int value);
    void value5Changed (double value);
    void min5Changed (double value);
    void max5Changed (double value);

    void slider6Changed(int value);
    void value6Changed (double value);
    void min6Changed (double value);
    void max6Changed (double value);

    void slider7Changed(int value);
    void value7Changed (double value);
    void min7Changed (double value);
    void max7Changed (double value);

    void slider8Changed(int value);
    void value8Changed (double value);
    void min8Changed (double value);
    void max8Changed (double value);

    void binMinChanged (int value);
    void binMaxChanged (int value);
    void binWidthChanged (int value);
    void binMidChanged (double value);

    void join1Changed (int value);
    void join2Changed (int value);
    void join3Changed (int value);

    void apply();
    void reset();
    void restore();
    void close();

    void buttonClicked (QAbstractButton *btn);
    void closeEvent (QCloseEvent *event);

    void parametersChanged();
    void setupChanged();

signals:
    void closed ();

private:
    Ui::equationDialog *ui;

    Fleet * fleet;
    ss_model * dataModel;

    QList<float> ageList;
    QList<float> lenList;

    double min1, max1;
    double min2, max2;
    double min3, max3;
    double min4, max4;
    double min5, max5;
    double min6, max6;
    double min7, max7;
    double min8, max8;

    int binMin, binMax, binWidth;
    double binMidWidth;

    int join1, join2, join3;

};

#endif // EQUATIONDIALOG_H
