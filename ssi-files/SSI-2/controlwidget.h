#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include "ssfile.h"

#include <QWidget>

namespace Ui {
class controlWidget;
}

class controlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit controlWidget(QWidget *parent = nullptr);
    ~controlWidget();

public slots:
    bool readFile(QString file);
    bool writeFile();

signals:
    void dataChanged();

private:
    Ui::controlWidget *ui;

    ssFile *controlFile;
};

#endif // CONTROLWIDGET_H
