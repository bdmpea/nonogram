#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <QDialog>

namespace Ui {
class register_window;
}

class register_window : public QDialog
{
    Q_OBJECT

public:
    explicit register_window(QWidget *parent = nullptr);
    ~register_window();

private slots:
    void on_pushButton_sing_up_clicked();
    void on_pushButton_back_clicked();

signals:
    void firstWindow();

private:
    Ui::register_window *ui;
};

#endif // REGISTER_WINDOW_H
