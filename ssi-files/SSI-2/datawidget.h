#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include "ssfile.h"

#include <QWidget>

namespace Ui {
class dataWidget;
}

class dataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dataWidget(QWidget *parent = nullptr);
    ~dataWidget();

public slots:
    bool readFile(QString file);
    bool writeFile();

signals:
    void dataChanged();

private:
    Ui::dataWidget *ui;

    ssFile *dataFile;
};

#endif // DATAWIDGET_H
