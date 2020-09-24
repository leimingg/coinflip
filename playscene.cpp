#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include <mypushbutton.h>
#include <QTimer>
#include <QLabel>
#include <mycoin.h>
#include <dataconfig.h>
#include <QPropertyAnimation>
#include <QSound>

/*PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
{

}
*/

PlayScene::PlayScene(int levelNum){
    QString str=QString("Enter %1 level").arg(levelNum);
    qDebug()<<str;
    this->levelIndex=levelNum;

    this->setFixedSize(320,588);
    this->setWindowIcon(QPixmap("：/res/Coin0001.png"));
    this->setWindowTitle("Coin Flip");

    QMenuBar *bar=menuBar();
    setMenuBar(bar);
    //create menu bar
    QMenu *startMenu=bar->addMenu("Start");
    QAction *quitAction=startMenu->addAction("Exit");
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    QSound *backSound=new QSound(":/res/BackButtonSound.wav",this);
    QSound *flipSound=new QSound(":/res/ConFlipSound.wav",this);
    QSound *winSound=new QSound(":/res/LevelWinSound.wav",this);

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
    //Display current level
    QLabel *label =new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("Times New Roman");
    font.setPointSize(20);
    QString str1=QString("Level:%1").arg(this->levelIndex);
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(30,this->height()-50,120,50);

    dataConfig config;
    //initialze every level's 2D array
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            this->gameArray[i][j]=config.mData[this->levelIndex][i][j];
        }
    }

    //Display success picture
    QLabel* winLabel =new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());

    //Display coin background
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            QPixmap pix=QPixmap(":/res/BoardNode.png");
            QLabel* label=new QLabel;
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57+i*50,200+j*50);

            //create golden coin
           QString str;
            if(this->gameArray[i][j]==1){
               //display golden coin
                str=":/res/Coin0001.png";
            }else{
                //display silver coin
                str=":/res/Coin0008.png";
            }
            MyCoin* coin =new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*50,204+j*50);

            coin->posX=i;
            coin->posY=j;
            coin->flag=this->gameArray[i][j];//1 is front,0 is back

            coinBtn[i][j]=coin;
            connect(coin,&MyCoin::clicked,[=](){
                flipSound->play();
                for(int i=0;i<4;i++){
                    for(int j=0;j<4;j++){
                        this->coinBtn[i][j]->isWin=true;
                    }
                }
                coin->changeFlag();
                this->gameArray[i][j]=this->gameArray[i][j]==0?1:0;

                QTimer::singleShot(300,this,[=](){
                    //flip coin around
                    //flip right side
                    if(coin->posX+1<=3){
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY]=this->gameArray[coin->posX+1][coin->posY]==0?1:0;
                    }
                    //flip left side
                    if(coin->posX-1>=0){
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY]=this->gameArray[coin->posX-1][coin->posY]==0?1:0;
                    }
                    //flip top side
                    if(coin->posY+1<=3){
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1]=this->gameArray[coin->posX][coin->posY+1]==0?1:0;
                    }
                    //flip down side
                    if(coin->posY-1>=0){
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1]=this->gameArray[coin->posX][coin->posY-1]==0?1:0;
                    }

                    for(int i=0;i<4;i++){
                        for(int j=0;j<4;j++){
                            this->coinBtn[i][j]->isWin=false;
                        }
                    }

                    //determine is win
                    this->isWin=true;
                    for(int i=0;i<4;i++){
                        for(int j=0;j<4;j++){
                            if(coinBtn[i][j]->flag==false){
                                isWin=false;
                                break;
                            }

                        }
                    }
                    if(this->isWin==true){
                        winSound->play();
                        qDebug()<<"Win";
                        //reset all push buttons as true. No response and return directly if push button again
                        for(int i=0;i<4;i++){
                            for(int j=0;j<4;j++){
                                coinBtn[i][j]->isWin=true;

                            }
                        }

                        //display success picture
                        QPropertyAnimation * animation=new QPropertyAnimation(winLabel,"geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }
                });

            });

        }
    }
}

void PlayScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    //create background
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //load title
    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
