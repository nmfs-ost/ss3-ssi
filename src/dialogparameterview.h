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
    explicit DialogParameterView(QWidget *parent = nullptr, bool showTrans = true);
    ~DialogParameterView();

    void setNumParamsShown (int num, int unsaved = 0);
    int getNumParameters ();
    double getInput (int pnum);
    double getValue (int pnum);

    bool getShowTrans() const;
    void setShowTrans(bool value);

    int getNumUnParameters () {return numUnParams;}
    void setNumUnParameters (int num);

public slots:
    void setTitle (QString title);
    void setName (QString nm);
    void setParameterTable(tablemodel *params);
    void setupView(int num, int unsaved = 0);
//    void setupUnSavedView(int unsaved);
    void setSliders();
//    void setNumParameters(int num);
//    void setParameter(int pnum, QStringList param);
//    void setParameterLabel(int pnum, QString name);
    void setSliderRange (int pnum, double min, double max);
    void setSliderValue(int pnum, double value);
    void setName(int pnum, QString name);
    void setType(int pnum, QString type);
    bool setInput(int pnum);
    void setInputValue(int pnum, double value);
//    void changeParameter(int num, QStringList param);
    void setVisible (bool visible);
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

protected:
    template<typename T>
    bool checkMinMax (T *min, T *max);

signals:
    void inputChanged();
    void hidden();
//    void parameterChanged (int num, double min, double max, double init);

private:
    QString title;
    QString name;
    QSize window;
    QPoint position;
    bool transVisible;

    tablemodel *parameters;
    int numParamsShown;

    int numUnParams;

    // parameter sliders
    QList<QLabel *> pLabel;
    QList<doubleLimitSpinBox *> pMin;
    QList<doubleLimitSpinBox *> pMax;
    QList<QDoubleSpinBox *> pInit;
    QList<QSlider *> pSlider;
    QList<QDoubleSpinBox *> sValue;
    QList<QLabel *> pType;
    QList<QDoubleSpinBox *> eInput;

    // unsaved values
    QList<QLabel *> unLabel;
    QList<doubleLimitSpinBox *> unMin;
    QList<doubleLimitSpinBox *> unMax;
    QList<QSlider *> unSlider;
    QList<QDoubleSpinBox *> unValue;

private:
    Ui::DialogParameterView *ui;
};

#endif // DIALOGPARAMETERVIEW_H
