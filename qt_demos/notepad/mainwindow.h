#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <QFontDialog>
#include <QColorDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QPrintPreviewDialog>
#include <qDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionPrint_triggered();

    void on_actionClose_triggered();

    void on_actionFont_triggered();

    void on_actionFontColor_triggered();

    void on_actionAboutNotepad_triggered();


    void setFontBold(bool bold) const;
    void setFontUnderline(bool underline);
    void setFontItalic(bool italic);

    void on_actionBackground_Color_triggered();

    void on_actionPrint_Preview_triggered();

    void on_actionFind_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFileName;

    void saveFile(QFile &file);
};
#endif // MAINWINDOW_H
