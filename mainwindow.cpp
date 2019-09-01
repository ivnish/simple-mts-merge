#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

#include <QDebug>

static QString str_inputdir;
static QString str_outputdir;
static QString str_pathToFFMPEG;
static QProcess * ffmpeg;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_btn_inputdir_clicked()
{
    str_inputdir = QFileDialog::getExistingDirectory(this, tr("Select directory with MTS files"), "", QFileDialog::ShowDirsOnly);
    ui->le_inputdir->setText(str_inputdir);
}

void MainWindow::on_btn_outputdir_clicked()
{
    str_outputdir = QFileDialog::getExistingDirectory(this, tr("Select output directory"), "", QFileDialog::ShowDirsOnly);
    ui->le_outputdir->setText(str_outputdir);
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus status)
{
   delete ffmpeg;

   ui->btn_inputdir->setEnabled(true);
   ui->btn_outputdir->setEnabled(true);
   ui->btn_merge->setEnabled(true);
   //ui->cb_compress->setEnabled(true);

   ui->progressBar->setMaximum(1);

   QMessageBox::information(nullptr, tr("Info"), tr("Files successfully merged!"));
}

void MainWindow::on_btn_merge_clicked()
{
    if (str_inputdir.isEmpty())
    {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Please select directory with MTS files"));
        return;
    }

    if (str_outputdir.isEmpty())
    {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Please select output directory"));
        return;
    }

    ui->btn_inputdir->setEnabled(false);
    ui->btn_outputdir->setEnabled(false);
    ui->btn_merge->setEnabled(false);
    ui->cb_compress->setEnabled(false);

    QDir directory(str_inputdir);
    QStringList sl_fileNames = directory.entryList(QStringList() << "*.mts" << "*.MTS", QDir::Files);
    QString str_fileNames;
    foreach(QString filename, sl_fileNames) {
      str_fileNames += filename + "|";
    }

    str_pathToFFMPEG = QApplication::applicationDirPath() + "/ffmpeg/ffmpeg.exe";

    QStringList sl_arguments;
    sl_arguments << "-i" << "concat:" + str_fileNames << "-vcodec" << "copy" << "-acodec" << "copy" << str_outputdir + "/output.ts";

    ui->progressBar->setMaximum(0);

    ffmpeg = new QProcess(this);
    ffmpeg->setWorkingDirectory(str_inputdir);
    ffmpeg->setProgram(str_pathToFFMPEG);
    ffmpeg->setArguments(sl_arguments);
    ffmpeg->start();

    connect(ffmpeg, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "Simple MTS Merge 0.1\n\nby Andrei Ivnitskii (ivnish.dev@gmail.com)\n\nhttps://github.com/ivnish/simple-mts-merge");
}
