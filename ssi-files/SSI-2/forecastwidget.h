#ifndef FORECASTWIDGET_H
#define FORECASTWIDGET_H

#include "ssfile.h"

#include <QWidget>

namespace Ui {
class forecastWidget;
}

class forecastWidget : public QWidget
{
    Q_OBJECT

public:
    explicit forecastWidget(QWidget *parent = nullptr);
    ~forecastWidget();

public slots:
    bool readFile(QString file);
    bool writeFile();

    int getBenchmarks();
    void setBenchmarks(int bmark);

signals:
    void dataChanged();

private:
    Ui::forecastWidget *ui;

    ssFile *forecastFile;

    int benchmarks;
};

#endif // FORECASTWIDGET_H
