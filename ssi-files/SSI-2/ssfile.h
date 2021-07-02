#ifndef SSFILE_H
#define SSFILE_H

#include <QFile>
#include <QObject>

class ssFile : public QFile
{
public:
    explicit ssFile(QObject *parent = nullptr);

    void readLine();
    QString getNextToken();
    QString getLine();

    void readComments();
    QStringList getComments();
    QString getCommentString();

private:
    int lineNum;
    QString line;
    QStringList lineTokens;

    QStringList comments;
};

#endif // SSFILE_H
