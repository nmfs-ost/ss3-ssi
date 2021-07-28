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
//#include "equationdialog.h"
#include "dialoginfo.h"
#include "dialogselectivityequationview.h"

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
    void changeSelexSizePattern(QString text);
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
    void showSelexSizeInfo();
    void selexSizeCurveClosed ();

    void setSelexAgePattern (int pat);
    void changeSelexAgePattern(int value);
    void selexAgeParamsChanged();
    void selexAgeTVParamsChanged();
    void changeSelexAgeDiscard(int disc);
    void selexAgeDiscParamsChanged();
    void selexAgeDiscTVParamsChanged();
    void changeSelexAgeMale (int mal);
    void selexAgeMaleParamsChanged();
    void selexAgeMaleTVParamsChanged();
    void changeSelexAgeSpecial (int spc);
    void setupSelexAgeInfo();
    void selexAgeDataChanged();
    void showSelexAgeCurve(bool flag);
    void showSelexAgeInfo();
    void selexAgeCurveClosed ();

    void setUse2DAR1 (bool use);
    void changeUse2DAR1 (bool use);
    void adjust2DAR1View (bool use);
    bool getUse2DAR1 ();
    void set2DAR1 ();
    void changeDoAddToSurveyCV(bool set);
    void changeAddToSurveyCV(double value);
    void changeDoAddToDiscardSD(bool set);
    void changeAddToDiscardSD(double value);
    void changeDoAddToBdyWtCV(bool set);
    void changeAddToBdyWtCV(double value);
    void changeDoMultByLenCmpN(bool set);
    void changeMultByLenCmpN(double value);
    void changeDoMultByAgeCmpN(bool set);
    void changeMultByAgeCmpN(double value);
    void changeDoMultBySaaN(bool set);
    void changeMultBySaaN(double value);
    void changeDoMultByGenSizeCmp(bool set);
    void changeMultByGenSizeCmp(double value);

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
    DialogSelexEquationView *selexSizeEqDialog;
    DialogInfo * selexSizeInfoDialog;

    tableview *ageSelexParamsView;
    tableview *ageSelexTimeVaryParamsView;
    tableview *ageSelexRetainParamsView;
    tableview *ageSelexRetainTVParamsView;
    tableview *ageSelexDiscardParamsView;
    tableview *ageSelexDiscardTVParamsView;
    tableview *ageSelexMaleParamsView;
    tableview *ageSelexMaleTVParamsView;
    DialogSelexEquationView *selexAgeEqDialog;
    DialogInfo * selexAgeInfoDialog;

    tableview *DAR1SpecView;
    tableview *DAR1ParamView;

    void refreshFleetNames();
    void connectFleet ();
    void disconnectFleet ();
};

#endif // FLEET_WIDGET_H

