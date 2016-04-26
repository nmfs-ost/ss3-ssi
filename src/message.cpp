#include "message.h"

void showInputMessage(QString msg)
{
    QMessageBox::information(0, "Reading file", msg);
}

void showOutputMessage(QString msg)
{
    QMessageBox::information(0, "Writing file", msg);
}

