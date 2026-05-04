#ifndef ADT_CONSISTENCY_CHECK_APP_H
#define ADT_CONSISTENCY_CHECK_APP_H

#include "ui_AdtConsistencyCheckApp.h"
#include "DataTypeConsistencyCheck.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui {
    class AdtConsistencyCheckApp;
}

QT_END_NAMESPACE

class AdtConsistencyCheckApp : public QMainWindow {
    Q_OBJECT

public:
    explicit AdtConsistencyCheckApp(QWidget *parent = nullptr);

    ~AdtConsistencyCheckApp() override =default;

private slots:
    void on_pb_open_clicked();
    void on_actionOpen_triggered();
    void on_actionSave_triggered() const;
    void on_actionClear_triggered() const;
    void on_actionRefresh_triggered() const;

private:
    QString fileName;
    Ui::AdtConsistencyCheckApp *ui;
    std::unique_ptr<DataTypeConsistencyCheck> dataTypeChecker;

    void outputErrors2TextEdit() const;
};
#endif // ADT_CONSISTENCY_CHECK_APP_H
