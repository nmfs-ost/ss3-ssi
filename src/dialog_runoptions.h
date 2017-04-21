#ifndef DIALOG_RUNOPTIONS_H
#define DIALOG_RUNOPTIONS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Dialog_runoptions;
}

class Dialog_runoptions : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_runoptions(QWidget *parent = 0);
    ~Dialog_runoptions();

public slots:
    void rejectOptions();
    void acceptOptions();
    void buttonPressed(QAbstractButton *btn);

    void noHessChecked(bool flag);
    void sHessChecked(bool flag);
    void estChecked(bool flag);
    void noestChecked(bool flag);

signals:
    void doNoHess(bool);
    void doSparseHess(bool);
    void doMCDiag(bool);
    void doLikeProf(bool);
    void doNoEst(bool);
    void doEstOnly(bool);

private:
    Ui::Dialog_runoptions *ui;

};

#endif // DIALOG_RUNOPTIONS_H
