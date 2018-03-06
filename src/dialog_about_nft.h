/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef DIALOG_ABOUT_NFT_H
#define DIALOG_ABOUT_NFT_H

#include <QDialog>

namespace Ui {
class Dialog_About_NFT;
}

class Dialog_About_NFT : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_About_NFT(QWidget *parent = 0);
    ~Dialog_About_NFT();

signals:
    void show_webpage (QString pg);

public slots:
    void goToWebpage ();

private:
    Ui::Dialog_About_NFT *ui;
};

#endif // DIALOG_ABOUT_NFT_H
