/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef COMPOSITION_WIDGET_H
#define COMPOSITION_WIDGET_H

#include <QWidget>

namespace Ui {
class composition_widget;
}

class composition_widget : public QWidget
{
    Q_OBJECT

public:
    explicit composition_widget(QWidget *parent = 0);
    ~composition_widget();

private:
    Ui::composition_widget *ui;
};

class length_composition_widget : public QWidget
{
    Q_OBJECT

public:
    explicit length_composition_widget(QWidget *parent = 0);
    ~length_composition_widget();

private:
    Ui::composition_widget *ui;
};

class age_composition_widget : public QWidget
{
    Q_OBJECT

public:
    explicit age_composition_widget(QWidget *parent = 0);
    ~age_composition_widget();

private:
    Ui::composition_widget *ui;
};

class morph_composition_widget : public QWidget
{
    Q_OBJECT

public:
    explicit morph_composition_widget(QWidget *parent = 0);
    ~morph_composition_widget();

private:
    Ui::composition_widget *ui;
};

#endif // COMPOSITION_WIDGET_H
