#ifndef DIALOG_COPYRIGHT_H
#define DIALOG_COPYRIGHT_H

#include <QDialog>

namespace Ui {
class Dialog_copyright;
}

class Dialog_copyright : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_copyright(QWidget *parent = nullptr);
    ~Dialog_copyright();

private:
    Ui::Dialog_copyright *ui;
};

#endif // DIALOG_COPYRIGHT_H
