#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    childDirectories = new QStringList();

    // Populate model
    model = new QStringListModel(*childDirectories);
    model->setStringList(*childDirectories);

    // Add to list
    ui->listBackup->setModel(model);

    // Initialise backupdir
    backupDir = new QList<QDir>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Project_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open File"), "C://", QFileDialog::ShowDirsOnly);
    masterDirectory = &dir;
    qDebug() << "Master: " << *masterDirectory;

    parentDir = new QDir(*masterDirectory);

    setMasterLayout(*masterDirectory);
}

void MainWindow::setMasterLayout(const QString &dir)
{
    ui->labelMasterDir->setText(dir);
}

void MainWindow::on_actionQuit_triggered()
{

}

void MainWindow::on_pushButtonBrowse_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open File"), "C://", QFileDialog::ShowDirsOnly);
    newBackupDir = &dir;

    // Add to list
    childDirectories->append(*newBackupDir);

    // Populate model
    model->setStringList(*childDirectories);

    // Add to backupDir list
    QDir *newDir = new QDir(*newBackupDir);
    backupDir->append(*newDir);
}

void MainWindow::on_pushButtonBackup_clicked()
{
    QFileInfo srcFileInfo(srcFilePath);
       if (srcFileInfo.isDir()) {
           QDir targetDir(tgtFilePath);
           targetDir.cdUp();
           if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
               return false;
           QDir sourceDir(srcFilePath);
           QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
           foreach (const QString &fileName, fileNames) {
               const QString newSrcFilePath
                       = srcFilePath + QLatin1Char('/') + fileName;
               const QString newTgtFilePath
                       = tgtFilePath + QLatin1Char('/') + fileName;
               if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                   return false;
           }
       } else {
           if (!QFile::copy(srcFilePath, tgtFilePath))
               return false;
       }
       return true;
}
