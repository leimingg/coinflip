#ifndef MYCOIN_H
#define MYCOIN_H
#include <QPushButton>
#include <QWidget>
#include <QTimer>
class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = nullptr);
    //parameter indicates golden coin or silver coin
    MyCoin(QString btnImg);

    //coin property
    int posX;
    int posY;
    bool flag;//front/back

    void changeFlag();
    QTimer *timer1;//Timer from front to back
    QTimer *timer2;//Timer from back to front
    int min=1;
    int max=8;

    bool isAnimation=false;

    //reset push and release
    void mousePressEvent(QMouseEvent *e);
     bool isWin;

signals:

public slots:
};

#endif // MYCOIN_H
