#include "input_file.h"
#include "dialoginputerror.h"
#include <QMessageBox>

ss_file::ss_file(QString name, QObject *parent) :
    QFile(parent)
{
    inputErrDialog = new DialogInputError();
    setFileName(name);
    reset();

    connect (this, SIGNAL(stopReadingFile()), SLOT(reset()));
}

ss_file::~ss_file ()
{
    delete inputErrDialog;
    delete current_line;
    delete current_tokens;
}

void ss_file::setFileName(const QString &filename) {
    inputErrDialog->setFileName(filename);
    QFile::setFileName(filename);
}

bool ss_file::reset()
{
    // reset and close
    bool okay = true;
    line_num = 0;
    wait = false;
    okay = true;
    stop = false;
    current_line = new QString("");
    current_tokens = new QStringList(*current_line);
    current_tokens->clear();
    if (isOpen()) {
        QFile::reset(); //seek(0);
        close();
    }
    return okay; //QFile::reset();
}

QString ss_file::read_line()
{
    QString line("EOF");

    if (isOpen() && !atEnd())
    {
        QByteArray qba = readLine(MAX_LINE_LENGTH);
        line_num++;
        inputErrDialog->setLineNum(line_num);
        line = QString (qba);
    }
    return line;
}

void ss_file::skip_line()
{
    if (current_line->count() > 0)
        current_line->clear();
    if (current_tokens->count() > 0)
        current_tokens->clear();
}

bool ss_file::at_eol()
{
    bool flag = false;
    if (current_tokens->count() == 0)
        flag = true;
    return flag;
}

QStringList ss_file::read_comments()
{
    QString cmt, token (get_next_token());
    comments.clear();

    while (token.startsWith("#"))
    {
        if (current_line->startsWith("#C"))
        {
            cmt = current_line->section("#C", 1, -1);
            while (cmt.startsWith(' '))
                cmt = cmt.section(' ', 1, -1);
            while (cmt.endsWith('\n') || cmt.endsWith('\r'))
                cmt.chop(1);
            comments.append(cmt);
        }
        skip_line();
        token = get_next_token();
    }
    get_line_tokens(current_line);
    return comments;
}

void ss_file::set_comments(QStringList cmts)
{
    comments.clear();
    for (int i = 0; i < cmts.count(); i++)
    {
        comments.append(QString(cmts.at(i)));
    }
}

int ss_file::write_comments()
{
    int chars = 0;
    QString cmt, line;
    for (int i = 0; i < comments.count(); i++)
    {
//        cmt = comments.at(i);
        line = QString("#C %1").arg(comments.at(i));
        chars = writeline (line);
    }
    return chars;
}

int ss_file::writeline(QString str)
{
    int chars = 0;
    if (!str.isEmpty())
        chars += write (str.toUtf8());
    chars += newline();
    return chars;
}

int ss_file::writechar (QChar chr)
{
    char ch = chr.toLatin1();
    int chars = 0;
    if (!chr.isNull())
    {
        write (&ch);
        chars = 1;
    }
    return  chars;
}

int ss_file::newline()
{
    return write (ENDLINE);
}

/* Get the next line from the input file,
 * and separate by tabs and spaces into separate tokens. */
QStringList *ss_file::get_line_tokens()
{
    QString line(read_line());
    if (current_line->count() > 0)
        current_line->clear();
//    line = ;
    current_line->append(line);

    return get_line_tokens(current_line);
}

QStringList *ss_file::get_line_tokens(QString *line)
{
    QString last;
    QStringList cl;

    cl = line->split('\t', QString::SkipEmptyParts);
    current_tokens->clear();
    for (int i = 0; i < cl.count(); i++)
        current_tokens->append(cl.at(i).split (' ', QString::SkipEmptyParts));

    if (!current_tokens->isEmpty()) {
        last = current_tokens->takeLast();
        if (last.compare("\n") != 0)
        {
            last = last.split ('\n', QString::SkipEmptyParts).takeFirst();
            if (last.compare("\r") != 0)
                last = last.split ('\r', QString::SkipEmptyParts).takeFirst();
            else
                last.clear();
        }
        else
            last.clear();
        if (!last.isEmpty())
           current_tokens->append(last);
        if (current_tokens->count() == 0)
            get_line_tokens();
    }
    setNumTokens();

    return current_tokens;
}

int ss_file::getNumTokens()
{
    return current_line_num_tokens;
}

void ss_file::setNumTokens()
{
    current_line_num_tokens = 0;
    if (!current_tokens->isEmpty()) {
    for (int i = 0; i < current_tokens->count(); i++)
    {
        if (QString(current_tokens->at(i)).startsWith('#'))
            break;
        current_line_num_tokens++;
    }
    }
}

/*QString ss_file::get_next_value()
{
    QString tk (get_next_token());
    while (tk.startsWith("#"))
    {
        skip_line();
        tk = get_next_token();
    }
    return tk;
}*/

QString ss_file::get_next_value(QString prompt)
{
    QString msg ("Found EOF when ");
    QString tk (get_next_token());
    while (tk.startsWith("#"))
    {
        skip_line();
        tk = get_next_token();
    }
    if (tk.compare("EOF") == 0)
    {
        if (prompt.isEmpty())
            msg.append(QString("reading "));
        else
            msg.append(QString("looking for %1 in ").arg(prompt));
        msg.append(QString("file %2.").arg(this->fileName()));
        msg.append(QString("\n\tLine: %1").arg(QString::number(line_num)));
        QMessageBox::warning(nullptr, tr("Input File unexpected EOF"), msg);
        emit stopReadingFile("EOF");
    }
    return tk;
}

QString ss_file::get_next_token()
{
    QString tk("EOF");
    /*if (okay)*/ {
        while (current_tokens->count() == 0)
        {
            get_line_tokens();
        }
        tk = current_tokens->takeFirst();
    }
    return tk;
}

QString ss_file::get_next_token(QString line)
{
    if (line.isEmpty())
        get_line_tokens();
    else
        get_line_tokens(&line);
    while (current_tokens->count() == 0)
    {
        get_line_tokens();
    }

    return current_tokens->takeFirst();
}

void ss_file::return_token(QString tokn)
{
    current_tokens->prepend(tokn);
}

QString ss_file::get_line()
{
    if (current_line->isEmpty())
        get_line_tokens();
    QString line (*current_line);
//    skip_line();
    return line;
}

void ss_file::append_file_line_info (QString &txt)
{
    txt.append(QString ("\n  File: %1  Line: %2").arg(fileName(), QString::number (line_num)));
}

int ss_file::error (QString err)
{
    okay = false;
    append_file_line_info (err);
    int btn = QMessageBox::warning((QWidget*)parent(), tr("Error"), err, QMessageBox::Cancel | QMessageBox::Ok);
    return btn;
}


int ss_file::message(QString msg)
{
    append_file_line_info (msg);
    int btn = QMessageBox::information((QWidget*)parent(), tr("Information"), msg, QMessageBox::Ok);
    return btn;
}

int ss_file::write_val(int i_val, int spcng, QString info)
{
    QString val = QString::number(i_val);
    return write_val (val, spcng, info);
}

int ss_file::write_val(double f_val, int spcng, QString info)
{
    QString val = QString::number(f_val);
    return write_val (val, spcng, info);
}

int ss_file::write_val(QString val, int spcng, QString info)
{
    if (!info.isEmpty())
    {
        do {
            val.append(' ');
        } while (val.count() < spcng);
        val.append(QString("#_%1").arg(info));
    }
    return writeline(val);
}

int ss_file::write_vector(QStringList vect, int spcng, QString info)
{
    QString line ("");
    QString item;

    int num = vect.count();
    for (int i = 0; i < num; i++)
    {
        item = vect.at(i);
        while (item.count() < spcng)
            item.prepend (' ');
        line.append(QString("%1 ").arg(item));
    }
    if (!info.isEmpty())
        line.append(QString("# %1").arg(info));
    return writeline(line);
}

float ss_file::getFloatValue(QString desc, float min, float max, float dfault)
{
    float value;
    float result = -1;
    /*if (okay)*/ {
        value = get_next_value(desc).toFloat();
        result = checkFloatValue(value, desc, min, max, dfault);
    }
    return result;
}

float ss_file::checkFloatValue(float val, QString desc, float min, float max, float dfault)
{
    while (val < min ||
            val > max)
    {
        inputErrDialog->getNewFloatValue(val, desc, min, max, dfault);
        inputErrDialog->exec();
        val = inputErrDialog->getFloatValue();
        if (val < -999990 || val > 999999)
            break;
    }
    checkErrCode(val);
    return val;
}

int ss_file::getIntValue(QString desc, int min, int max, int dfault)
{
    int value;
    int result = -1000000;
    /*if (okay)*/ {
        value = get_next_value(desc).toInt();
        result = checkIntValue(value, desc, min, max, dfault);
    }
    return result;
}

int ss_file::checkIntValue(int val, QString desc, int min, int max, int dfault)
{
    /*if (okay)*/ {
        while (val < min ||
                val > max)
        {
            inputErrDialog->getNewIntValue(val, desc, min, max, dfault);
            inputErrDialog->exec();
            val = inputErrDialog->getIntValue();
            if (val < -999990 || val < -999999)
                break;
        }
        checkErrCode(val);
    }
    return val;
}

void ss_file::checkErrCode(float code) {
    int icode = static_cast<int>(code + .5);
    checkErrCode(icode);
}

void ss_file::checkErrCode(int code) {
    switch (code) {
    case -1000000:
        QMessageBox::warning(nullptr, tr("Stopped Reading on User Input"),
                     tr(QString("Reading file %1 has stopped at line %2.").arg(fileName(), QString::number(line_num)).toUtf8()));
        reset();
        okay = false;
        stop = true;
        emit stopReadingFile("Stop");
        break;

    case -999999:
        QMessageBox::critical(nullptr, tr("Exit on Reading Error"),
                    tr("Leaving the application while reading files."));
        reset();
        okay = false;
        exit(code);
    }
}

bool ss_file::getOkay() const
{
    return okay;
}

void ss_file::setOkay(bool value)
{
    okay = value;
}

bool ss_file::getStop() const
{
    return stop;
}

void ss_file::setStop(bool value)
{
    stop = value;
}
