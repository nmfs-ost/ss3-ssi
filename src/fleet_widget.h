#ifndef FLEET_WIDGET_H
#define FLEET_WIDGET_H

#include <QWidget>
#include <QFont>

#include "model.h"
#include "fleet.h"
#include "tableview.h"
#include "catchdelegate.h"
#include "abundancedelegate.h"
#include "equationdialog.h"

namespace Ui {
class fleet_widget;
}

class fleet_widget : public QWidget
{
    Q_OBJECT

public:
    explicit fleet_widget(ss_model *m_data, QWidget *parent = 0);
    ~fleet_widget();

public slots:
    void edit_name ();
    void setActive (bool flag);
    void readingNewModel ();
    void reset ();
    void refresh();
    void set_model (ss_model *model);
    void setAgeLengthBins ();
    void set_current_fleet (int index);
    void set_fleet_type (int type);
    void set_type_fleet (Fleet::FleetType ft);
    void nextFleet();
    void prevFleet();
    void create_new_fleet();
    void duplicate_current_fleet();
    void delete_current_fleet();
    void new_fleet (QString name = QString(""));
    void duplicate_fleet (int index);
    void delete_fleet (int index);
    void changeBycatchDead (int value);
    void changeBycatchF (int value);
    void changeBycatchFirstYr ();
    void changeBycatchLastYr ();

    void showLengthObs();
    void changeLengthMinTailComp();
    void changeLengthAddToData();
    void showAgeObs();
    void changeAgeMinTailComp();
    void changeAgeAddToData();
    void showMeanSAAObs();
    void showGenSizeObs(int index);
    void setGenMethodTotal(int num);
    void changeGenMethodTotal(int num);
    void setGenMethodNum(int num);
    void changeGenMethodNum(int num);
    void setGenNumObs(int num);
    void changeGenNumObs(int num);
    void showMorphObs();
    void showRecapObs();

    void qSetupChanged();
    void qSetupParamsChanged();
    void qSetupTVParamsChanged();
    void setQTimeVaryReadParams(int flag);

    void setAr1SelexSmoother(int val);

    void changeSelexSizePattern(int pat);
    void showSelexSizeInfo();
    void changeSelexSizeDiscard(int disc);
    void changeSelexSizeMale (int mal);
    void changeSelexSizeSpecial (int spc);
    void sizeSelexParamsChanged();
    void sizeSelexTVParamsChanged();
    void setSelTimeVaryReadParams(int flag);
    void showSelexSizeCurve(bool flag);
    void selexSizeCurveClosed ();

    void changeSelexAgePattern(int pat);
    void changeSelexAgeMale (int mal);
    void changeSelexAgeSpecial (int spc);
    void showSelexAgeInfo();
    void ageSelexParamsChanged();
    void ageSelexTVParamsChanged();
    void showSelexAgeCurve(bool flag);
    void selexAgeCurveClosed ();

private:

    Ui::fleet_widget *ui;
    QFont titleFont;

    ss_model *model_data;
    Fleet * current_fleet;
    int totalFleets;

    tableview *catchview;
    catchdelegate *catchedit;

    tableview *abundview;
    abundancedelegate *abundedit;

    tableview *discardview;
    abundancedelegate *discardedit;

    tableview *mbwtView;
    tableview *lenCompView;
    tableview *ageCompView;
    tableview *saaObsView;
    int cur_gen_obs;
    tableview *genObsView;
    tableview *morphObsView;
    tableview *recapObsView;

    tableview *qSetupView;
    tableview *qParamsView;
    tableview *qTvParamsView;

    tableview *sizeSelexParamsView;
    tableview *sizeSelexRetainView;
    tableview *sizeSelexDiscardView;
    tableview *sizeSelexMaleView;
    tableview *sizeSelexTimeVaryParamsView;
    tableview *ageSelexParamsView;
    tableview *ageSelexMaleView;
    tableview *ageSelexTimeVaryParamsView;
    equationDialog *selexSizeEqDialog;
    equationDialog *selexAgeEqDialog;

    tableview *lambdaView;


    void refreshFleetNames();
    void connectFleet ();
    void disconnectFleet ();
};

#endif // FLEET_WIDGET_H

