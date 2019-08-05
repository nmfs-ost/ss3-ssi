#ifndef PARAMETERVIEW_H
#define PARAMETERVIEW_H

#include <QWidget>
#include <QLabel>
#include <QStringList>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include "tablemodel.h"

namespace Ui {
class parameterView;
}

class parameterView : public QWidget
{
    Q_OBJECT

public:
    explicit parameterView(QWidget *parent = nullptr);
    ~parameterView();

    double getInput (int pnum);


public slots:
    void setParameterTable(tablemodel *params);
//    void setNumParameters(int num);
//    void setParameter(int pnum, QStringList param);
//    void setParameterLabel(int pnum, QString name);
    void setSliderRange (int pnum, double min, double max);
    void setType(int pnum, QString type);
//    void changeParameter(int num, QStringList param);
    void cancel();

private slots:
    void sliderChanged(int num, int value);
    void sValueChanged(int num, double value);
    void convertToInput(int num);
    void paramsChanged();
    void reset();
    void apply();

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
    Ui::parameterView *ui;
};

#endif // PARAMETERVIEW_H
