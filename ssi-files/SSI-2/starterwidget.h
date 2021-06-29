#ifndef STARTERWIDGET_H
#define STARTERWIDGET_H

#include "ssfile.h"

#include <QWidget>

namespace Ui {
class starterWidget;
}

class starterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit starterWidget(QWidget *parent = nullptr);
    ~starterWidget();

public slots:
    bool readFile(QString file);
    bool writeFile();

    QString getDataFile();
    QString getControlFile();

signals:
    void dataChanged();

private:
    Ui::starterWidget *ui;

    ssFile *starterFile;
};

#endif // STARTERWIDGET_H
