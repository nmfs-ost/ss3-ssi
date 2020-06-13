#ifndef DIALOGCHOSEREPORT_H
#define DIALOGCHOSEREPORT_H

#include <QDialog>

namespace Ui {
class DialogChoseReport;
}

class DialogChoseReport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChoseReport(QWidget *parent = nullptr);
    ~DialogChoseReport();

    void connectCheckBoxes();
    void disconnectCheckBoxes();

    void setReports(bool value);
    QList<bool> &getReports();

    int getRepDetail() const;
    void setRepDetail(int value);

    bool getAllReps() const;
    void setAllReps(bool value);

    bool getMinimReps() const;
    void setMinimReps(bool value);

    bool getBriefReps() const;
    void setBriefReps(bool value);

    bool getCustmReps() const;
    void setCustmReps(bool value);
    void setCustomReport(int value);

    void setCheckboxes();
    void setRadioButtons();

    bool isMinim() const;
    bool isAll() const;
    bool isBrief() const;

public slots:
    void clearReports();
    void close();

    void changeMinimReps(bool value);
    void changeAllReps(bool value);
    void changeBriefReps(bool value);
    void changeCheckbox(bool value = true);

signals:
    void detailChanged(int value);

private:
    Ui::DialogChoseReport *ui;

    int repDetail;
    bool minimReps;
    bool allReps;
    bool briefReps;
    bool custmReps;

    QList<bool> reports;

};

#endif // DIALOGCHOSEREPORT_H
