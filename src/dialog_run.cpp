#include "dialog_run.h"
#include "ui_dialog_run.h"
#include "dialog_view.h"
#include "dialog_fileview.h"

#include <QFileDialog>
#include <QProcess>
#include <QDebug>

Dialog_run::Dialog_run(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_run)
{
    ui->setupUi(this);
    ui->pushButton_increase->setVisible(false);
    ui->pushButton_decrease->setVisible(false);
    ui->pushButton_pause->setVisible(false);

    ui->plainTextEdit_output->setCenterOnScroll(true);

    ui->pushButton_run->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);

    connect (ui->pushButton_changeExe, SIGNAL(clicked()), SLOT(changeExe()));
    connect (ui->pushButton_options, SIGNAL(clicked()), SLOT(showOptions()));
    connect (ui->pushButton_run, SIGNAL(clicked()), SLOT(startRun()));
    connect (ui->pushButton_pause, SIGNAL(clicked()), SLOT(pauseRun()));
    connect (ui->pushButton_stop, SIGNAL(clicked()), SLOT(stopRun()));
    connect (ui->pushButton_cancel, SIGNAL(clicked()), SLOT(rejected()));
    connect (ui->pushButton_rptCharts, SIGNAL(clicked()), SLOT(showRptCharts()));
//    ui->pushButton_RCharts->setEnabled(false);
    connect (ui->pushButton_RCharts, SIGNAL(clicked()), SLOT(generateRCharts()));
    connect (ui->pushButton_showWarn, SIGNAL(clicked()), SLOT(showWarnFile()));
    connect (ui->pushButton_showEcho, SIGNAL(clicked()), SLOT(showEchoFile()));

    connect (ui->pushButton_increase, SIGNAL(clicked()), SLOT(increaseFont()));
    connect (ui->pushButton_decrease, SIGNAL(clicked()), SLOT(decreaseFont()));

    warnview = new Dialog_fileView(this);//("Warning.sso", this);
    warnview->hide();
    echoview = new Dialog_fileView(this);
    echoview->hide();

    charts = new dialogSummaryOutput(this);
    charts->hide();

    nohess = false;
    shess = false;
    est = false;
    noest = false;
    lprof = false;
    mcdiag = false;
    runoptions = new Dialog_runoptions(this);
    runoptions->hide();
    connect (runoptions, SIGNAL(doNoHess(bool)), SLOT(setNohess(bool)));
    connect (runoptions, SIGNAL(doSparseHess(bool)), SLOT(setShess(bool)));
    connect (runoptions, SIGNAL(doNoEst(bool)), SLOT(setNoest(bool)));
    connect (runoptions, SIGNAL(doEstOnly(bool)), SLOT(setEst(bool)));
    connect (runoptions, SIGNAL(doLikeProf(bool)), SLOT(setLprof(bool)));
    connect (runoptions, SIGNAL(doMCDiag(bool)), SLOT(setMcdiag(bool)));
    connect (runoptions, SIGNAL(accepted()), SLOT(applyOptions()));
    connect (runoptions, SIGNAL(rejected()), SLOT(resetOptions()));

    stocksynth = new QProcess(this);
    connect (stocksynth, SIGNAL(started()), SLOT(runStarted()));
    connect (stocksynth, SIGNAL(readyReadStandardOutput()), SLOT(stdOutput()));
    connect (stocksynth, SIGNAL(readyReadStandardError()), SLOT(stdError()));
    connect (stocksynth, SIGNAL(finished(int)), SLOT(runCompleted(int)));

    dfont = QFont(fontInfo().family(), fontInfo().pointSize());
    setFontSize(9);
    ui->pushButton_run->setDefault(true);
}

Dialog_run::~Dialog_run()
{
    delete ui;
    delete stocksynth;
}

void Dialog_run::showRptCharts()
{
    charts->show();
    charts->refreshData();

}

//PATH=%PATH%;R_exe
//rscript -e "r4ss::SS_plots(r4ss::SS_output(getwd(),verbose=FALSE))"

void Dialog_run::generateRCharts()
{
    QFile Rexe(R_exe);
    QFileInfo Rinfo(R_exe);

    if (R_exe.isEmpty()) {
        QMessageBox::information(this,tr("Missing R executable"), tr("Use Main window - Options menu to find R executable"));
    }
    else {
        QDir bindir(qApp->applicationDirPath());
        QDir Rdir(Rinfo.dir());
        QString line(QString("\"%1/").arg(Rdir.path()));
        line.append("rscript\" -e \"r4ss::SS_plots(r4ss::SS_output(getwd(),verbose=FALSE))\"\n\n");
        ui->plainTextEdit_output->clear();
        ui->plainTextEdit_error->clear();
        ui->plainTextEdit_output->appendPlainText(line);
        stocksynth->start(line, QIODevice::ReadOnly);
    }
}

void Dialog_run::showWarnFile()
{
    QString dir (ui->label_directory->text());
    warnview->viewFile(dir + QString("/warning.sso"));
    warnview->show();
}

void Dialog_run::showEchoFile()
{
    QString dir (ui->label_directory->text());
    echoview->viewFile(dir + QString("/echoinput.sso"));
    echoview->show();
}

void Dialog_run::runSS()
{
    QString ss(ui->label_executable->text());
    QString opts(ui->lineEdit_options->text());
    QString line;

    QString cmd("cmd.exe /k");

    ui->plainTextEdit_output->clear();
    ui->plainTextEdit_error->clear();

    line = QString (QString("%1 %2\n").arg(ss, opts));
    ui->plainTextEdit_output->appendPlainText(line);
    // run command in box
    stocksynth->start(line, QIODevice::ReadOnly);
}

void Dialog_run::runStarted()
{
    setUiEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
    running = true;
}

void Dialog_run::finishOutput()
{
    running = false;
}

void Dialog_run::runCompleted(int code)
{
    QString message;

    while (stocksynth->canReadLine())
    {
        output = stocksynth->readLine(BUFFER_SIZE);
        ui->plainTextEdit_output->appendPlainText(output.left(output.indexOf('\n')));
    }

    setUiEnabled(true);
    if (code)
    {
        QProcess::ProcessError err = stocksynth->error();
        switch (err)
        {
        case QProcess::FailedToStart:
            message = QString ("Stock Synthesis failed to start\n");
            break;
        case QProcess::Crashed:
            message = QString ("Stock Synthesis crashed.\n");
            break;
        case QProcess::Timedout:
            message = QString ("Stock Synthesis timed out.\n");
            break;
        case QProcess::ReadError:
            message = QString ("Error reading output from Stock Synthesis.\n");
            break;
        case QProcess::UnknownError:
        default:
            message = QString ("Unknown error running Stock Synthesis.\n");
        }
        ui->pushButton_showWarn->setDefault(true);
    }
    else
    {
        message = QString("Stock Synthesis executed with no errors.\n");

        ui->pushButton_rptCharts->setDefault(true);
    }
    ui->plainTextEdit_error->insertPlainText(message);
    ui->plainTextEdit_error->scroll(0,1);

    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
}

void Dialog_run::startRun()
{
    ui->pushButton_run->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
    runSS();
}

void Dialog_run::pauseRun()
{
    stocksynth->write("\0x18");
    ui->pushButton_run->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
}

void Dialog_run::stopRun()
{
    stocksynth->kill();
    ui->pushButton_run->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
}

void Dialog_run::rejected()
{
    reject();
}

void Dialog_run::setDir(QString dir)
{
    QString warnfile(dir + QChar('/') + QString("Warning.sso"));
    stocksynth->setWorkingDirectory(dir);
    ui->label_directory->setText(dir);
}

void Dialog_run::changeExe()
{
    QString filename;
    QString dir = ui->label_executable->text().section('/', 0, -2);
 //   dir = QApplication::applicationDirPath();
    filename = (QFileDialog::getOpenFileName (this, tr("Executable"),
        dir, tr("applications (*.exe)")));
    setExe(filename);
    emit executableChanged(filename);
}

void Dialog_run::showOptions()
{
    runoptions->show();
    ui->pushButton_run->setDefault(true);
}

void Dialog_run::applyOptions()
{
    QString opts("");
    if (nohess)
        opts.append(QString("-nohess "));

    if (shess)
        opts.append(QString("-shess "));

    if (est)
        opts.append(QString("-est "));

    if (noest)
        opts.append(QString("-noest "));

    if (lprof)
        opts.append(QString("-lprof "));

    if (mcdiag)
        opts.append(QString("-mcdiag "));

    if (opts.isEmpty())
        resetOptions();
    else
        setOptions(opts);
}

void Dialog_run::resetOptions()
{
    QString opts("");
    ui->lineEdit_options->setText(opts);
}

void Dialog_run::setExe(QString exe)
{
    ui->label_executable->setText(exe);
}

void Dialog_run::setRExe(QString exe)
{
    R_exe = exe;
}

void Dialog_run::setOptions(QString opt)
{
    if (!opt.isEmpty())
    {
        ui->lineEdit_options->setText(opt);
    }
}

void Dialog_run::stdOutput()
{
    while (stocksynth->canReadLine())
    {
        output = stocksynth->readLine(BUFFER_SIZE);
        ui->plainTextEdit_output->appendPlainText(output.left(output.indexOf('\n')-1));
    }
}

void Dialog_run::stdError()
{
    QString message = stocksynth->readAllStandardError();
    ui->plainTextEdit_error->insertPlainText(message);
}

void Dialog_run::setUiEnabled(bool flag)
{
    ui->pushButton_changeExe->setEnabled(flag);
    ui->pushButton_options->setEnabled(flag);
    ui->lineEdit_options->setEnabled(flag);
    ui->pushButton_run->setEnabled(flag);
    ui->pushButton_rptCharts->setEnabled(flag);
    ui->pushButton_RCharts->setEnabled(flag);
    ui->pushButton_cancel->setEnabled(flag);
    ui->pushButton_showWarn->setEnabled(flag);
    ui->pushButton_showEcho->setEnabled(flag);
}

void Dialog_run::outputLine()
{
    QString line(stocksynth->readLine(BUFFER_SIZE));
    ui->plainTextEdit_output->appendPlainText(line.left(line.indexOf('\n')-1));
}

void Dialog_run::onProcessStarted()
{
    setUiEnabled(false);
}

void Dialog_run::onProcessStdOutWrite(QString szOutput)
{
#ifdef Q_OS_WIN32
    QString content;

    if (fixedPosition != 0) {
        content = szOutput.right(szOutput.count() - szOutput.indexOf('\n') - 1);
    } else {
        content = szOutput;
    }

    if (content.startsWith("\f\r\n")) {
        ui->plainTextEdit_output->clear();
        ui->plainTextEdit_output->insertPlainText(content.right(content.count() - 3));
    } else {
        ui->plainTextEdit_output->insertPlainText(content);
    }
#else
    ui->plainTextEdit_output->insertPlainText(szOutput);
#endif
    fixedPosition = ui->plainTextEdit_output->textCursor().position();
}

void Dialog_run::onProcessStdErrWrite(QString szOutput)
{
    ui->plainTextEdit_output->appendPlainText(szOutput);
    fixedPosition = ui->plainTextEdit_output->textCursor().position();
}

void Dialog_run::onProcessTerminate()
{
    setUiEnabled(true);
}

void Dialog_run::increaseFont (bool flag)
{
    int f_size = dfont.pointSize();
    if (flag)
    if (f_size < 16)
    {
        f_size += 1;
    }
    setFontSize(f_size);
}

void Dialog_run::decreaseFont(bool flag)
{
    int f_size = dfont.pointSize();
    if (flag)
    if (f_size > 8)
    {
        f_size -= 1;
    }
    setFontSize(f_size);
}

void Dialog_run::setFontSize(int fsize)
{
    dfont.setPointSize(fsize);
    setFont(dfont);
}

void Dialog_run::setMcdiag(bool value)
{
    mcdiag = value;
}

void Dialog_run::setLprof(bool value)
{
    lprof = value;
}

void Dialog_run::setNoest(bool value)
{
    noest = value;
}

void Dialog_run::setEst(bool value)
{
    est = value;
}

void Dialog_run::setShess(bool value)
{
    shess = value;
}

void Dialog_run::setNohess(bool value)
{
    nohess = value;
}

