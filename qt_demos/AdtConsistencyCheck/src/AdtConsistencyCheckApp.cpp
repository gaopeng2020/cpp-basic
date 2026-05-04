#include "AdtConsistencyCheckApp.h"
#include <QFileDialog>
#include <common-utils/core.h>
#include "DataTypeConsistencyCheck.h"

using namespace common_utils::core;

AdtConsistencyCheckApp::AdtConsistencyCheckApp(QWidget* parent) : QMainWindow(parent), ui(new Ui::AdtConsistencyCheckApp) {
    ui->setupUi(this);
    this->dataTypeChecker = std::make_unique<DataTypeConsistencyCheck>();
}

void AdtConsistencyCheckApp::on_pb_open_clicked() {
    // 获取上次打开的位置
    const std::string open_dir = Core::getLastOpenDir("");
    fileName =
        QFileDialog::getOpenFileName(this, "Open the File", QString::fromStdString(open_dir), "Excel (*.xlsx);;All Files (*)");
    if (fileName.isEmpty()) return;

    // 存储新位置
    Core::getLastOpenDir(fileName.toStdString());
    setWindowTitle(fileName);

    ui->textEdit->clear();
    const QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->textEdit->append("[" + currentTime + "]" + fileName + " 正在开始一致性检查，请稍后！");

    ui->lineEdit->setText(fileName);
    this->dataTypeChecker->set_file_path(fileName.toStdString());
    this->dataTypeChecker->readDataType();
    outputErrors2TextEdit();
}

void AdtConsistencyCheckApp::outputErrors2TextEdit() const {

    // 将错误信息输出到 textEdit 控件
    if (const auto errors = dataTypeChecker->getErrors(); !errors.empty()) {
        std::string allErrors;
        for (const auto& error : errors) {
            if (error.empty()) continue;
            allErrors += error + "\n";
        }
        ui->textEdit->append(QString::fromStdString(allErrors));
        const QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        ui->textEdit->append("[" + currentTime + "]" + fileName + " 已完成检查，请按照提示修改！");
    } else {
        ui->textEdit->append(fileName + " 没有发现已知的错误");
    }
}

void AdtConsistencyCheckApp::on_actionOpen_triggered() { on_pb_open_clicked(); }

void AdtConsistencyCheckApp::on_actionSave_triggered() const {
    // 将错误信息保存到text文件，与校验文件位置相同
    if (!this->fileName.isEmpty()) {
        const QString logName =
            QFileInfo(this->fileName).absolutePath() + "/" + QFileInfo(this->fileName).completeBaseName() + "_check_result.txt";
        QFile file(logName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // 如果文件存在，先清除原来的内容后再写入
            if (file.exists()) file.resize(0);

            QTextStream out(&file);
            out << ui->textEdit->toPlainText();
            file.close();
            // 在textEdit中提示保存成功，textEdit跳转到最后一行
            ui->textEdit->moveCursor(QTextCursor::End);
            const QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            ui->textEdit->append("[" + currentTime + "] 检查结果保存成功，与输入文件同目录！");
        }
    }
}

void AdtConsistencyCheckApp::on_actionClear_triggered() const {
    // 检查textEdit是否为空
    if (!ui->textEdit->toPlainText().isEmpty()) ui->textEdit->clear();
}

void AdtConsistencyCheckApp::on_actionRefresh_triggered() const {
    ui->textEdit->clear();
    const QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->textEdit->append("[Info " + currentTime + "]" + fileName + " 正在开始一致性检查，请稍后！");
    if (!this->fileName.isEmpty()) this->dataTypeChecker->readDataType();
    outputErrors2TextEdit();
}
