/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SEASONALEFFECTSMODEL_H
#define SEASONALEFFECTSMODEL_H

#include <QObject>
#include "setupmodel.h"

class seasonalEffectsModel : public QObject
{
    Q_OBJECT
public:
    explicit seasonalEffectsModel(QObject *parent = nullptr);
    ~seasonalEffectsModel();

signals:
    void parametersChanged();

public slots:
    void setParamHeaders ();
    void setNumSeasons (int seas);

    void setSetupHeader (QStringList hdr) {setup->setHeader(hdr);}
    QStringList getSetup () {return setup->getData();}
    void setSetup (QStringList values) {setup->setData(values);}

    void changeParameters(QList<int> setupvals);
    void setParamModelHeader (QStringList hdr) {params->setParamModelHeader(hdr);}
    int getNumParameters () {return params->getNumParams();}
    QStringList getParameter (int num) {return params->getParameter(num);}
    void setParameter (int num, QStringList data) {params->setParameter(num, data);}

    tablemodel *getParamTable () {return params->getParamTable();}

    bool getFemwtlen1() const;
    void setFemwtlen1(bool value);
    bool getFemwtlen2() const;
    void setFemwtlen2(bool value);
    bool getMat1() const;
    void setMat1(bool value);
    bool getMat2() const;
    void setMat2(bool value);
    bool getFec1() const;
    void setFec1(bool value);
    bool getFec2() const;
    void setFec2(bool value);
    bool getMalewtlen1() const;
    void setMalewtlen1(bool value);
    bool getMalewtlen2() const;
    void setMalewtlen2(bool value);
    bool getL1() const;
    void setL1(bool value);
    bool getK() const;
    void setK(bool value);

private:
    setupModel *setup;
    shortParamMultModel *params;
};

#endif // SEASONALEFFECTSMODEL_H
