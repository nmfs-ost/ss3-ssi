#include "ssfile.h"
#include "metadata.h"

#define MAXLINE 2048

ssFile::ssFile(QObject *parent) : QFile(parent)
{
    lineTokens.clear();
    line.clear();
}

void ssFile::readNextLine()
{
    if (atEnd()) {
        line = QString("EOF");
    }
    else {
        QByteArray ary = readLine(MAXLINE);
        line = QString(ary);
    }
    lineTokens = line.simplified().split(' ', QString::SkipEmptyParts);
}

QString ssFile::getNextToken(QString prompt)
{
    while (lineTokens.isEmpty() || lineTokens.first().startsWith('#'))
    {
        if (lineTokens.first().startsWith("#C"))
        {
            comments.append(line);
        }
        readNextLine();
    }
    return lineTokens.takeFirst();
}

QString ssFile::getLine(QString prompt)
{
    return line;
}

void ssFile::readHeader()
{
    readNextLine();
    while (lineTokens.first().startsWith("#"))
    {
        if (lineTokens.first().startsWith("#C"))
        {
            comments.append(line);
        }
        readNextLine();
    }
}

int ssFile::writeHeader()
{
    int chars = 0;
    QByteArray hdr;
    QStringList hdrs;
    hdrs.append("# stuff\n");
    hdrs.append("# more stuff\n");
    for (int i = 0; i < hdrs.count(); i++)
    {
        hdr = hdrs.at(i).toUtf8();
        chars += write(hdr);
    }
    for (int i = 0; i < comments.count(); i++)
    {
        hdr = comments.at(i).toUtf8();
        hdr.append('\n');
        chars += write(hdr);
    }
    return chars;
}

QStringList ssFile::getComments()
{
    return comments;
}

QString ssFile::getCommentString()
{
    QString commentsString;
    if (!comments.isEmpty())
    {
        commentsString = comments.at(0);
        for (int i = 1; i < comments.count(); i++)
        {
            commentsString.append('\n');
            commentsString.append(comments.at(i));
        }
    }
    return commentsString;
}

void ssFile::nextLine()
{
    line.clear();
    lineTokens.clear();
}


QStringList ssFile::readStringList(int num, QString prompt)
{
    QStringList result;
    for (int i = 0; i < num; i++)
    {
        result.append(getNextToken(prompt));
    }

    return result;
}
