#ifndef QTBASICEXERCICE_H
#define QTBASICEXERCICE_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QTextEdit>
#include <QDockWidget>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>
#include <QString>
#include <QDebug>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class QtBasicExercice; }
QT_END_NAMESPACE

class QtBasicExercice : public QMainWindow
{
    Q_OBJECT

public:
    QtBasicExercice(QWidget *parent = nullptr);
    ~QtBasicExercice();

private slots:
    void on_pushButton_clicked();

private:
    void on_pb_ok_released();

private:
    QPushButton pb_ok;
    QPushButton *pb_close;
    Ui::QtBasicExercice *ui;
};
#endif // QTBASICEXERCICE_H
