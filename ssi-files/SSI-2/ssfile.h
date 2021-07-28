#ifndef SSFILE_H
#define SSFILE_H

#include <QFile>
#include <QObject>
#include <QStringList>
#include <QTextStream>

class ssFile : public QFile
{
public:
    explicit ssFile(QObject *parent = nullptr);

    void readNextLine();
    QString getNextToken(QString prompt = QString());
    QString getLine(QString prompt = QString());

    void readHeader();
    int writeHeader();
    QStringList getComments();
    QString getCommentString();

    void nextLine();

    QStringList readStringList(int num, QString prompt = QString());

private:
    int lineNum;
    QString line;
    QStringList lineTokens;

    QStringList comments;
};

#endif // SSFILE_H
