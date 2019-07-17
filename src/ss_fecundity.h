/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SS_FECUNDITY_H
#define SS_FECUNDITY_H

#include <QStringList>

#include "parametermodel.h"
#include "setupmodel.h"

class ss_model;

class ss_fecundity : public QObject
{
    Q_OBJECT

public:
    ss_fecundity(ss_model *parent = 0);
    ~ss_fecundity();

    void reset();

    int getMethod() const;
    void setMethod(int value);
//    void setAutogenerate(int value);

    int getHermIncludeMales() const;
    void setHermIncludeMales(int value);

    int getHermSeason() const;
    void setHermSeason(int value);

    bool getHermaphroditism() const;
    void setHermaphroditism(int value);

private:
    ss_model *parnt;
    int method;
    int hermaphroditism;
    int hermSeason;
    int hermIncludeMales;

    QString pHeader;
};

#endif // SS_FECUNDITY_H
