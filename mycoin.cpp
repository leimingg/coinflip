#include "mycoin.h"
#include <QDebug>
/*MyCoin::MyCoin(QWidget *parent) : QPushButton(parent)
{

}*/

MyCoin::MyCoin(QString btnImg){
    QPixmap pix;
    bool ret=pix.load(btnImg);
    if(!ret){
        QString str=QString("Failed to load Picture %1").arg(btnImg);
        qDebug()<<str;
        return;

    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{boarder:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    //initialize timer object
    timer1=new QTimer(this);
    timer2=new QTimer(this);

    //listen signal of fliping coin from front to back
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{boarder:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //reset min
        if(this->min>this->max){
            this->min=1;
            isAnimation=false;
            timer1->stop();
        }
    });

    //listen signal of fliping coin from back to front
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{boarder:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //reset min
        if(this->max<this->min){
            this->max=8;
            isAnimation=false;
            timer2->stop();
        }
    });

}

void MyCoin::mousePressEvent(QMouseEvent *e){
    if(this->isAnimation|| this->isWin){
        return;
    }else{
        QPushButton::mousePressEvent(e);
    }
}

void MyCoin::changeFlag(){
    //change to back if it is front
    if(this->flag){
        timer1->start(30);
        isAnimation=true;
        this->flag=false;

    }else{//change to front if it is back
        timer2->start(30);
        isAnimation=true;
        this->flag=true;
    }
}
