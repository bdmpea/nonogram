#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMouseEvent>

class cellbutton : public QPushButton {
    Q_OBJECT

 private:
    int po;
    int *type_button;
    bool *first_operation;

 public:
    cellbutton(int *b, bool *f, int p, QWidget *parent = 0);
    
 protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void filled(int pos);
    void scratched(int pos);
};

#endif // PUSHBUTTON_H
