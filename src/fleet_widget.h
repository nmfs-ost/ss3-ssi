/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

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
#include "dialoginfo.h"

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
    void set_current_fleet (int index = -1);
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

    void setSelTimeVaryReadParams(int flag);
    void setSelexSizePattern (int pat);
    void changeSelexSizePattern(int value);
    void selexSizeParamsChanged();
    void selexSizeTVParamsChanged();
    void setupSelexSizeInfo();
    void selexSizeDataChanged();
    void changeSelexSizeDiscard(int disc);
    void selexSizeDiscParamsChanged();
    void selexSizeDiscTVParamsChanged();
    void changeSelexSizeMale (int mal);
    void selexSizeMaleParamsChanged();
    void selexSizeMaleTVParamsChanged();
    void changeSelexSizeSpecial (int spc);
    void showSelexSizeCurve(bool flag);
    void selexSizeCurveClosed ();

    void setSelexAgePattern (int pat);
    void changeSelexAgePattern(int value);
    void selexAgeParamsChanged();
    void selexAgeTVParamsChanged();
    void changeSelexAgeMale (int mal);
    void selexAgeMaleParamsChanged();
    void selexAgeMaleTVParamsChanged();
    void changeSelexAgeSpecial (int spc);
    void setupSelexAgeInfo();
    void selexAgeDataChanged();
    void showSelexAgeCurve(bool flag);
    void selexAgeCurveClosed ();

    void lambdaPhaseChanged(int phs);
    void lambdaOffsetChanged(int val);
    void lambdaNumChanged(int num);
    void lambdaParamsChanged();

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
    tableview *sizeSelexTimeVaryParamsView;
    tableview *sizeSelexRetainParamsView;
    tableview *sizeSelexRetainTVParamsView;
    tableview *sizeSelexDiscardParamsView;
    tableview *sizeSelexDiscardTVParamsView;
    tableview *sizeSelexMaleParamsView;
    tableview *sizeSelexMaleTVParamsView;
    equationDialog *selexSizeEqDialog;
    DialogInfo * selexSizeInfoDialog;

    tableview *ageSelexParamsView;
    tableview *ageSelexTimeVaryParamsView;
    tableview *ageSelexMaleParamsView;
    tableview *ageSelexMaleTVParamsView;
    equationDialog *selexAgeEqDialog;
    DialogInfo * selexAgeInfoDialog;

    tableview *lambdaView;


    void refreshFleetNames();
    void connectFleet ();
    void disconnectFleet ();
};

#endif // FLEET_WIDGET_H

