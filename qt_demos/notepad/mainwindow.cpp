#include "mainwindow.h"
#include "finddialog.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)  : QMainWindow(parent)    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionBold,&QAction::triggered,this,&MainWindow::setFontBold);
    connect(ui->actionItalic,&QAction::triggered,this,&MainWindow::setFontItalic);
    connect(ui->actionUnderline,&QAction::triggered,this,&MainWindow::setFontUnderline);

    connect(ui->actionAboutQt,&QAction::triggered,[=](){
        QMessageBox::aboutQt(this,"QMessageBox");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    currentFileName.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open the File");
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    currentFileName = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Cannot open file:"+file.errorString());
        return;
    }

    setWindowTitle(file.fileName());
    QTextStream textStream(&file);
    QString content = textStream.readAll();
    ui->textEdit->setText(content);
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    if (currentFileName.isEmpty()){
        // currentFileName =  QFileDialog::getSaveFileName(this,"Save File");
        QMessageBox::warning(this,"Warning","Nothing need to save");
        return;
    }
    QFile file(currentFileName);
    saveFile(file);
}


void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    currentFileName = fileName;

    saveFile(file);
}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printDev;
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;

    ui->textEdit->print(&printDev);
}


void MainWindow::on_actionClose_triggered()
{
    ui->textEdit->clear();
    setWindowTitle("Simple Note");
    currentFileName="";
}


void MainWindow::on_actionFont_triggered()

{
    bool fontSelected;
    QFont font =  QFontDialog::getFont(&fontSelected,this);
    if (fontSelected) ui->textEdit->setFont(font);
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor fontColor = QColorDialog::getColor(Qt::white,this);
    if( fontColor.isValid())
    {
        ui->textEdit->setTextColor(fontColor);
    }
}


void MainWindow::on_actionBackground_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    if( color.isValid())  {
        QString backgroundColor = QString("background: rgba(%1,%2,%3,01);").arg(color.red()).arg(color.green()).arg(color.blue());
        ui->textEdit->setStyleSheet(backgroundColor);
        ui->textEdit->setText(backgroundColor);
    }
}

void MainWindow::on_actionAboutNotepad_triggered()
{
    QMessageBox::about(this, tr("About Notepad"),
                       tr("The <b>Notepad</b> example demonstrates how to code a basic text editor using QtWidgets"));
}


void MainWindow::setFontUnderline(bool underline)
{
    ui->textEdit->setFontUnderline(underline);
}

void MainWindow::setFontItalic(bool italic)
{
    ui->textEdit->setFontItalic(italic);
}

void MainWindow::setFontBold(bool bold) const {
    bold ? ui->textEdit->setFontWeight(QFont::Bold) : ui->textEdit->setFontWeight(QFont::Normal);
}

void MainWindow::saveFile(QFile &file)
{
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(currentFileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}



void MainWindow::on_actionPrint_Preview_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, ui->textEdit, &QTextEdit::print);
    preview.exec();
}


void MainWindow::on_actionFind_triggered()
{
    FindDialog dialog;
    dialog.exec();
}

