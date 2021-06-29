#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>

namespace Ui {
class outputWidget;
}

class outputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit outputWidget(QWidget *parent = nullptr);
    ~outputWidget();

private:
    Ui::outputWidget *ui;
};

#endif // OUTPUTWIDGET_H
