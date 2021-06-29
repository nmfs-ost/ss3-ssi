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
