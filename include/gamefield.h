#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>

class gamefield: public QWidget
{
    Q_OBJECT

public:
    gamefield(QWidget *parent = nullptr);
    ~gamefield();

};

#endif // GAMEFIELD_H
