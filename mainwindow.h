#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

public slots:
    void rom_select();
    void touch_type_set();
    void pwr_btn_click_start();
    void pwr_btn_click_end();
    void vol_up_btn_click_start();
    void vol_up_btn_click_end();
    void vol_dn_btn_click_start();
    void vol_dn_btn_click_end();
    void home_btn_click_start();
    void home_btn_click_end();
    void menu_btn_click_start();
    void menu_btn_click_end();
    void back_btn_click_start();
    void back_btn_click_end();
    void search_btn_click_start();
    void search_btn_click_end();
    void unlock_btn_click();
    void recalibrate();
};

#endif // MAINWINDOW_H
