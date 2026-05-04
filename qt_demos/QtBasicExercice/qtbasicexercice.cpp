#include "qtbasicexercice.h"
#include "ui_qtbasicexercice.h"

QtBasicExercice::QtBasicExercice(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtBasicExercice)
{
    ui->setupUi(this);
    setWindowTitle("QWidget Window");

    //pb_ok
    pb_ok.setParent(this);
    pb_ok.move(570,560);
    pb_ok.setFixedSize(100,30);
    pb_ok.setText("OK");
    //自定义槽函数
    connect(&pb_ok,&QPushButton::released,this,&QtBasicExercice::on_pb_ok_released);

    // pb_close
    pb_close = new QPushButton(this);
    pb_close->setText("Close");
    pb_close->move(675,560);
    connect(pb_close,&QPushButton::pressed,this,&QtBasicExercice::close);
    //菜单栏
    QMenuBar* p_MenuBar = menuBar();
    //菜单
    QMenu *p_File = p_MenuBar->addMenu("File");
    QMenu *p_About = p_MenuBar->addMenu("About");

    //File 菜单项
    QAction *file_New =  p_File->addAction("New");
    connect(file_New,&QAction::triggered, [=]() {
        QDialog * dialg = new QDialog();
        dialg->exec();//模态对画框，在对话框消失前不能继续使用父级窗口
        qDebug()<<"New File is created";
    });
    QAction *file_Open = p_File->addAction("Open");
    connect(file_Open,&QAction::triggered,[=](){
    const QString filter = "Images (*.png *.xpm *.jpg);;Text (*.txt);;XML files (*.xml *.arxml);;All(*.*)";
     QString fileName =   QFileDialog::getOpenFileName(this,"Open","../",filter);
     qDebug()<<"File name = "<<fileName;
    });
    //在菜单项之间添加分割线
    p_File->addSeparator();

    QAction *file_Print= p_File->addAction("Open");
    connect(file_Print,&QAction::triggered,[](){
        QDialog * dialg = new QDialog();
        dialg->show(); //非模态对画框，在对话框消失前能继续使用父级窗口
        dialg->setAttribute(Qt::WA_DeleteOnClose);//在对话框关闭时自动销毁内存，
        qDebug()<<"File was Opened";
    });

    //About 菜单项
    QAction *about_About =  p_About->addAction("About");
    connect(about_About,&QAction::triggered,[=](){
     QMessageBox::about(this,"QMessageBox","This is a static QMessageBox");
    });

    QAction *about_AboutQt =  p_About->addAction("About Qt");
    connect(about_AboutQt,&QAction::triggered,[=](){
     QMessageBox::aboutQt(this,"QMessageBox");
    });

    QAction *about_question = p_About->addAction("Question ?");
    connect(about_question,&QAction::triggered,[=](){
      int result =  QMessageBox::question(this,"Question","Do you have some questions?",QMessageBox::Yes,QMessageBox::Cancel);
      switch (result) {
        case QMessageBox::Yes:
        qDebug()<< "QMessageBox::Yes = " << result;
          break;

      case QMessageBox::Cancel:
          qDebug()<< "QMessageBox::Cancel = " << result;
            break;

      default:
          break;
      }
    });

    //Tool bar
    QToolBar *p_toolBar = addToolBar("Tool Bar");
    p_toolBar->addAction(file_New);
    p_toolBar->addAction(file_Open);

    //状态栏
    QStatusBar * stBar = statusBar();
    QLabel *label = new QLabel(this);
    label->setText("Normal text File");
     //状态栏靠左
    stBar->addWidget(label);
    //状态栏靠右
    stBar->addPermanentWidget(new QLabel("PermanentWidget",this));

    //文本编辑区
    QTextEdit * txtEdit = new QTextEdit(this);
    setCentralWidget(txtEdit);

    //浮动窗口QDockWidget
    QDockWidget *dockWin = new QDockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea,dockWin);
}

QtBasicExercice::~QtBasicExercice()
{
    delete ui;
}

void QtBasicExercice::on_pb_ok_released(){
    pb_ok.setText("Button Released");
}

void QtBasicExercice::on_pushButton_clicked()
{
    std::cout<<"======================="<<std::endl;
}

