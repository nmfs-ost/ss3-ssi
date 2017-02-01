#ifndef SEASONALEFFECTSMODEL_H
#define SEASONALEFFECTSMODEL_H

#include <QObject>
#include "setupmodel.h"

class seasonalEffectsModel : public setupShortParamModel
{
    Q_OBJECT
public:
    explicit seasonalEffectsModel(ss_model *parent = 0);

signals:

public slots:
    void setMultiplier (int mlt);
    void setParamHeaders ();
    void setNumSeasons (int seas);
    void changeParamData ();

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
};

#endif // SEASONALEFFECTSMODEL_H
