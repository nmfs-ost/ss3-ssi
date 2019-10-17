#ifndef DIALOGPARAMETERVIEW_H
#define DIALOGPARAMETERVIEW_H

#include <QDialog>
#include <QLabel>
#include <QStringList>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QAbstractButton>

#include "tablemodel.h"
#include "doublelimitspinbox.h"


namespace Ui {
class DialogParameterView;
}

class DialogParameterView : public QDialog
{
    Q_OBJECT

public:
    explicit DialogParameterView(QWidget *parent = nullptr);
    ~DialogParameterView();

    void setNumParamsShown (int num);
    int getNumParameters ();
    double getInput (int pnum);

public slots:
    void setTitle (QString title);
    void setParameterTable(tablemodel *params);
    void setupView(int num);
    void setSliders();
//    void setNumParameters(int num);
//    void setParameter(int pnum, QStringList param);
//    void setParameterLabel(int pnum, QString name);
    void setSliderRange (int pnum, double min, double max);
    void setSlider(int pnum, double value);
    void setName(int pnum, QString name);
    void setType(int pnum, QString type);
    bool setInputValue(int pnum);
//    void changeParameter(int num, QStringList param);
//    void setVisible (bool visible);
    void hide();
    void show();
    void closeEvent(QCloseEvent *evt);
    void cancel();
    void reset();
    void apply();

public slots:
    void clearAll();
    void disconnectAll();
    void connectAll();
    void minChanged (double);
    void maxChanged (double value);
    void sliderChanged(int value);
    void sValueChanged(double value);
    bool convertToInput(int num);
    void paramsChanged();
    doubleLimitSpinBox *valueSpinBox(bool arrows = true, bool read = true);
    QDoubleSpinBox *parameterSpinBox(bool arrows = true, bool read = true);
    QSlider *parameterSlider();

    void buttonClicked(QAbstractButton*btn);

signals:
    void inputChanged();
    void hidden();
//    void parameterChanged (int num, double min, double max, double init);

private:
    QSize window;
    QPoint position;

    tablemodel *parameters;
    int numParamsShown;

    QList<QLabel *> pLabel;
    QList<doubleLimitSpinBox *> pMin;
    QList<doubleLimitSpinBox *> pMax;
    QList<QDoubleSpinBox *> pInit;
    QList<QSlider *> pSlider;
    QList<QDoubleSpinBox *> sValue;
    QList<QLabel *> pType;
    QList<QDoubleSpinBox *> eInput;

private:
    Ui::DialogParameterView *ui;
};

#endif // DIALOGPARAMETERVIEW_H
