#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include <QDialog>

namespace Ui {
class DialogInfo;
}

class DialogInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInfo(QWidget *parent = 0);
    ~DialogInfo();

public slots:
    void setTitle(QString title);
    void setText (QString text);

signals:

private:
    Ui::DialogInfo *ui;

private slots:

};

#endif // DIALOGINFO_H
