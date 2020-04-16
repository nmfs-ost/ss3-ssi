#ifndef DIALOGINPUTERROR_H
#define DIALOGINPUTERROR_H

#include <QWidget>
#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class DialogInputError;
}

class DialogInputError : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInputError(QWidget *parent = 0);
    ~DialogInputError();

public slots:
    void getNewFloatValue(float errValue, QString valDesc,
                      float minValue, float maxValue, float defaultValue);
    void getNewIntValue(int errValue, QString valDesc,
                      int minValue, int maxValue, int defaultValue);
    void confirmValue(float errValue, QString valDesc,
                      float minValue, float maxValue, float defaultValue);
    void setFileName (QString name);
    void setLineNum (int lineNum);
    void setError (QString error, float errValue, QString valDesc);
    void setMinMax (float minValue, float maxValue, float defaultValue);
    float getFloatValue();
    int getIntValue();

signals:
    void dataChanged();
    void error(int code);

private slots:
    void connectIntCheck();
    void connectFloatCheck();
    void disconnectCheck();
    void checkFloatString(QString text);
    void checkIntString(QString text);
    void buttonClicked(QAbstractButton *btn);
    void buttonOKClicked ();
    void buttonAbortFileClicked();
    void buttonAbortSSIClicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::DialogInputError *ui;

    float floatDefault;
    float floatVal;
    int intVal;
//    int intDefault;

};

#endif // DIALOGINPUTERROR_H
