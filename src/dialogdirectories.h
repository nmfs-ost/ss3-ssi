#ifndef DIALOGDIRECTORIES_H
#define DIALOGDIRECTORIES_H

#include <QDialog>
#include <QString>

namespace Ui {
class DialogDirectories;
}

class DialogDirectories : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDirectories(QWidget *parent = nullptr);
    ~DialogDirectories();

    QString &getDirectory (int value);
    void setDirectory (const QString &value);
    QString &selectDirectory(QString dir);

    QString getModel1() const;
    void setModel1(const QString &value);

    QString getModel2() const;
    void setModel2(const QString &value);

public slots:
    void selectDir1();
    void selectDir2();

    void accept();
    void reject();

signals:
    void accepted();

private:
    Ui::DialogDirectories *ui;

    QString model1;
    QString model2;
    QString newDir;
};

#endif // DIALOGDIRECTORIES_H
