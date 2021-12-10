/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef INPUT_FILE_H
#define INPUT_FILE_H

#include <QFile>
#include <QStringList>
#include "dialoginputerror.h"

#define MAX_LINE_LENGTH 4096

#define END_OF_DATA       999  // signifies end of data in files
#define END_OF_LIST     -9999  // signifies end of data values

#define EXIT_SSI      -999999  // exit the program
#define STOP_READING -1000000  // stop reading files

#ifdef WIN32
 #define ENDLINE ("\r\n")
#else
 #ifdef MACOS
  #define ENDLINE ("\r")
 #else
  #define ENDLINE ("\n")
 #endif
#endif

class ss_file : public QFile
{
    Q_OBJECT
public:
    explicit ss_file(QString name, QObject *parent = nullptr);
    ~ss_file();

    void setFileName (const QString &fname);
    QString getShortFileName() {return filename;}
    QString getDirectory() {return directory;}

    QString get_next_value (QString prompt = QString(""));
    QString get_next_token();
    QString get_next_token(QString line);
    void return_token (QString tokn);
    int getNumTokens ();
    void setNumTokens ();

    QString get_line();
    int getLineNum() {return line_num;}
    void resetLineNum () {line_num = 0;}

    void skip_line();

    QStringList comments;
    QString read_line();
    QStringList read_comments();
    QStringList get_comments() {return comments;}
    void set_comments(QStringList cmts);
    int write_comments();
    int newline();
    int writeline(QString str = QString(""));
    int writeTerminator(int num);
    int writechar(QChar chr);
    int read_int (QString info = QString(""));
    float read_float (QString info = QString(""));

    int write_val (int i_val, int spcng = 0, QString info = QString(""));
    int write_val (double f_val, int spcng = 0, QString info = QString(""));
    int write_val (QString val, int spcng = 0, QString info = QString(""));

    int write_vector (QStringList vect, int spcng = 0, QString info = QString(""));

    float getFloatValue (QString desc, float min, float max, float dfault);
    float checkFloatValue (float val, QString desc, float min, float max, float dfault);
    int   getIntValue (QString desc, int min, int max, int dfault);
    int   checkIntValue (int val, QString desc, int min, int max, int dfault);
    void  stopWait ();
    QString getStringValue (QString desc, QString dfault = QString(""));

    bool at_eol ();
    void checkErrCode(float code);
    void checkErrCode(int code);

    bool getOkay() const;
    void setOkay(bool value);

    bool getStop() const;
    void setStop(bool value);

private:
    QString filename;
    QString directory;
    int line_num;
    bool wait;
    bool okay;
    bool stop;
    QString *current_line;
    QStringList *current_tokens;
    int current_line_num_tokens;

protected:
    QStringList *get_line_tokens();
    QStringList *get_line_tokens(QString *line);

    void append_file_line_info (QString &txt);

    DialogInputError *inputErrDialog;

signals:
    void stopReadingFile(QString info = QString(""));

public slots:
    bool reset ();
    int error (QString msg);
    int message (QString msg);

};

#endif // INPUT_FILE_H
