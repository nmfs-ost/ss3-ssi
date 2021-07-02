#include "ssfile.h"

ssFile::ssFile(QObject *parent) : QFile(parent)
{

}

void ssFile::readLine()
{

}

QString ssFile::getNextToken()
{
    return lineTokens.takeFirst();
}

QString ssFile::getLine()
{
    return line;
}

void ssFile::readComments()
{

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
