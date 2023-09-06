#include "cellbutton.h"
#include <QMimeData>

cellbutton::cellbutton(int *b, bool *f, int p, QWidget *parent) : type_button(b), first_operation(f), po(p), QPushButton(parent) {
    int zoom = 20;
    setAcceptDrops(true);
    setStyleSheet("background-color: rgb(215, 215, 215)");
    setMinimumSize(zoom, zoom);
    setMaximumSize(zoom, zoom);
}

void cellbutton::mousePressEvent(QMouseEvent *e) {
    if (e->button() != Qt::LeftButton && e->button() != Qt::RightButton) {
        return;
    }
    *(first_operation) = true;
    *(type_button) = e->button();
    QDrag *drag = new QDrag(this);
    QMimeData *md = new QMimeData();
    drag->setMimeData(md);
    drag->exec(Qt::CopyAction | Qt::MoveAction);
    emit released();
}

void cellbutton::dragEnterEvent(QDragEnterEvent *e) {
    if (*(type_button) == Qt::LeftButton) {
        emit filled(po);
    }
    else {
        emit scratched(po);
    }
}
