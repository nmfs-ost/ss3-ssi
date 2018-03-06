/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef AREA_H
#define AREA_H

#include <QObject>

class Area : public QObject
{
    Q_OBJECT
public:
    explicit Area(QObject *parent = 0);

    QString *getName() const;
    void setName(QString *value);

signals:

public slots:

private:
    QString *name;

};

#endif // AREA_H
