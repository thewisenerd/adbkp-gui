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
    //pwr button
    void pwr_btn_click_start();
    void pwr_btn_click_end();
    //vol_up button
    void vol_up_btn_click_start();
    void vol_up_btn_click_end();
    //vol down button
    void vol_dn_btn_click_start();
    void vol_dn_btn_click_end();
    //home button
    void home_btn_click_start();
    void home_btn_click_end();
    //menu button
    void menu_btn_click_start();
    void menu_btn_click_end();
    //back button
    void back_btn_click_start();
    void back_btn_click_end();
    //search button
    void search_btn_click_start();
    void search_btn_click_end();
    void unlock_btn_click();    
};

#endif // MAINWINDOW_H
