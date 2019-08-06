#ifndef DIALOGPARAMETERVIEW_H
#define DIALOGPARAMETERVIEW_H

#include <QDialog>
#include <QLabel>
#include <QStringList>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include "tablemodel.h"

namespace Ui {
class DialogParameterView;
}

class DialogParameterView : public QDialog
{
    Q_OBJECT

public:
    explicit DialogParameterView(QWidget *parent = nullptr);
    ~DialogParameterView();

    int getNumParameters ();
    double getInput (int pnum);

public slots:
    void setTitle (QString title);
    void setParameterTable(tablemodel *params);
//    void setNumParameters(int num);
//    void setParameter(int pnum, QStringList param);
//    void setParameterLabel(int pnum, QString name);
    void setSliderRange (int pnum, double min, double max);
    void setName(int pnum, QString name);
    void setType(int pnum, QString type);
//    void changeParameter(int num, QStringList param);
    void cancel();
    void reset();
    void apply();

private slots:
    void clearAll();
    void sliderChanged(int num, int value);
    void sValueChanged(int num, double value);
    void convertToInput(int num);
    void paramsChanged();

signals:
    void inputChanged(int num, double value);
//    void parameterChanged (int num, double min, double max, double init);

private:

    tablemodel *parameters;

    QList<QLabel *> pLabel;
    QList<QDoubleSpinBox *> pMin;
    QList<QDoubleSpinBox *> pMax;
    QList<QDoubleSpinBox *> pInit;
    QList<QSlider *> pSlider;
    QList<QDoubleSpinBox *> sValue;
    QList<QLabel *> pType;
    QList<QDoubleSpinBox *> eInput;

private:
    Ui::DialogParameterView *ui;
};

#endif // DIALOGPARAMETERVIEW_H
