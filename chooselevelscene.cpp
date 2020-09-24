#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include <mypushbutton.h>
#include <QtDebug>
#include <QTimer>
#include <QLabel>
#include <QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //set icon
    this->setFixedSize(320,588);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //set title
    this->setWindowTitle("Select Level");
    QMenuBar *bar=menuBar();
    setMenuBar(bar);

    //create menu bar
    QMenu *startMenu=bar->addMenu("Start");
    QAction *quitAction=startMenu->addAction("Exit");
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    QSound * chooseSound = new QSound(":/res/TapButtonSound.wav",this);
    QSound * backSound = new QSound(":/res/BackButtonSound.wav",this);
    //create back button
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //Back button
    connect(backBtn,&MyPushButton::clicked,[=](){
        //qDebug()<<"Click Back Button";
        backSound->play();
        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });

    });

    for(int i=0;i<20;i++){
        MyPushButton*menuBtn=new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25+i%4*70,130+i/4*70);

        //listen every button click event
        connect(menuBtn,&MyPushButton::clicked,[=](){
            chooseSound->play();
            QString str=QString("You selected %1 Level").arg(i+1);


            //enter game
            this->hide();
            play=new PlayScene(i+1);//create game scene
            play->show();
            play->setGeometry(this->geometry());
            connect(play,&PlayScene::chooseSceneBack,[=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play=NULL;
            });

        });
        QLabel *label=new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25+i%4*70,130+i/4*70);
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent *){
    //load background
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //load title
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix.width(),pix.height(),pix);
}
