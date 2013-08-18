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

private slots:
    void pwr_btn_click();
    void vol_up_btn_click();
    void vol_dn_btn_click();
    void home_btn_click();
    void menu_btn_click();
    void back_btn_click();
    void search_btn_click();
    void unlock_btn_click();
    void lp_pwr_btn_click();
};

#endif // MAINWINDOW_H
