#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include "chooselevelscene.h"
#include <QTimer>
#include <QSound>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //config main scene and set size
    setFixedSize(320,588);

    setWindowIcon(QIcon(":/res/Coin0001.png"));
    setWindowTitle("Flip Coin");
    connect(ui->actionQuit,&QAction::triggered,[=](){
        this->close();
    });

    QSound * startSound = new QSound(":/res/TapButtonSound.wav",this);
    MyPushButton *startBtn=new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);


    chooseScene=new ChooseLevelScene;

    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,this,[=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();
        this->show();
    });

    connect(startBtn,&MyPushButton::clicked,[=](){
        qDebug()<<"Clicked Start";
        startSound->play();
        startBtn->zoom1();
        startBtn->zoom2();

        QTimer::singleShot(500,this,[=](){
            chooseScene->setGeometry(this->geometry());
            this->hide();
            chooseScene->show();


        });

        /*this->hide();
        chooseScene->show();*/
    });

}

void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}
MainScene::~MainScene()
{
    delete ui;
}

