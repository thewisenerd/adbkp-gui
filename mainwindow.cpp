#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <myQWidget.h>
#include <QPainter>
#include <QMouseEvent>
#include <QCheckBox>
#include <QProcess>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool multi_touch = false;
QProcess *exec = new QProcess();

// Default CM9 config
QString himax="event3";
QString keypad="event4";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // The *screen* box
    myQWidget *bg_img = new myQWidget(this);
    bg_img->setGeometry(160,5,320,480);
    bg_img->setCursor(Qt::CrossCursor);
    bg_img->show();

    // Refresh button
    QPushButton *upd_bg_btn = new QPushButton(this);
    upd_bg_btn->setGeometry(510, 200, 100, 28);
    upd_bg_btn->setText("Refresh");
    upd_bg_btn->setToolTip("Refresh Screen");
    upd_bg_btn->setCursor(Qt::PointingHandCursor);
    upd_bg_btn->setFont(QFont("Arial", 10));
    upd_bg_btn->show();

    // Power button
    this->connect(this->ui->pwr_btn, SIGNAL(pressed()), this, SLOT(pwr_btn_click_start()));
    this->connect(this->ui->pwr_btn, SIGNAL(released()), this, SLOT(pwr_btn_click_end()));

    // Volume Up button
    this->connect(this->ui->vol_up_btn, SIGNAL(pressed()), this, SLOT(vol_up_btn_click_start()));
    this->connect(this->ui->vol_up_btn, SIGNAL(released()), this, SLOT(vol_up_btn_click_end()));

    // Volume Down button
    this->connect(this->ui->vol_dn_btn, SIGNAL(pressed()), this, SLOT(vol_dn_btn_click_start()));
    this->connect(this->ui->vol_dn_btn, SIGNAL(released()), this, SLOT(vol_dn_btn_click_end()));

    // home button
    this->connect(this->ui->home_btn, SIGNAL(pressed()), this, SLOT(home_btn_click_start()));
    this->connect(this->ui->home_btn, SIGNAL(released()), this, SLOT(home_btn_click_end()));

    // menu button
    this->connect(this->ui->menu_btn, SIGNAL(pressed()), this, SLOT(menu_btn_click_start()));
    this->connect(this->ui->menu_btn, SIGNAL(released()), this, SLOT(menu_btn_click_end()));

    // back button
    this->connect(this->ui->back_btn, SIGNAL(pressed()), this, SLOT(back_btn_click_start()));
    this->connect(this->ui->back_btn, SIGNAL(released()), this, SLOT(back_btn_click_end()));

    // search button
    this->connect(this->ui->search_btn, SIGNAL(pressed()), this, SLOT(search_btn_click_start()));
    this->connect(this->ui->search_btn, SIGNAL(released()), this, SLOT(search_btn_click_end()));

    // unlock gesture
    this->connect(this->ui->unlock_btn, SIGNAL(clicked()), this, SLOT(unlock_btn_click()));

    // refresh button
    connect(upd_bg_btn, SIGNAL(clicked()), bg_img, SLOT(update_bg()));

    // rom select
    this->connect(this->ui->rom_select, SIGNAL(currentIndexChanged(int)), SLOT(rom_select()));

    // touch type select
    this->connect(this->ui->multi_chk, SIGNAL(clicked()), SLOT(touch_type_set()));

    // recalibrate
    this->connect(this->ui->recalibrate_btn, SIGNAL(clicked()), SLOT(recalibrate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::rom_select()
{
    QString rom_name=this->ui->rom_select->currentText();
    if (rom_name=="CM9") {
        himax="event3";
        keypad="event4";
    }
    else if (rom_name=="MiniCM9"){
        himax="event4";
        keypad="event3";
    }
}

string inttoa(int a) {
    string str;
    ostringstream temp;
    temp<<a;
    return temp.str();
}

void MainWindow::recalibrate()
{
    system("timeout 0.5s ./bin/adb shell getevent > ./temp1.txt");
    system("grep -n himax-touchscreen ./temp1.txt | cut -c 1-2 > ./temp2.txt");
    string s;
    ifstream temp3("./temp2.txt");
    getline(temp3,s);
    temp3.close();

    int line1 = atoi(s.c_str());
    int line0 = line1 - 1;
    std::string temp4=inttoa(line0);
    system(("sed '"+temp4+"q;d' ./temp1.txt | cut -c 15- > ./temp5.txt").c_str());

    string himax_val;
    ifstream temp6("./temp5.txt");
    getline(temp6,himax_val);
    temp6.close();

    system("rm ./temp1.txt");
    system("rm ./temp2.txt");
    system("rm ./temp5.txt");

    himax = QString::fromUtf8(himax_val.c_str());
}

void MainWindow::touch_type_set()
{
    if (multi_touch==false){
        multi_touch=true;
    }
    else {
        multi_touch=false;
    }
}

void myQWidget::update_bg()
{
    setStyleSheet("background-image: url(./ph.png);");
    system("./bin/adb shell screencap -p | sed 's/\r$//' > ph.png");
    setStyle(QApplication::style());
}

myQWidget::myQWidget(QWidget *parent):QWidget(parent)
{
    setStyleSheet("background-image: url(./black.png);");
}

void myQWidget::mousePressEvent(QMouseEvent *event)
{
    if (multi_touch==false) {
            exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(event->pos().x()))/320)*1024) + 0.5f)))+" && sendevent /dev/input/"+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(event->pos().y()))/480)*910) + 0.5f)))+" && sendevent /dev/input/"+himax+" 0 0 0\"");
                }
    else {
        exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 57 0 && sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(event->pos().x()))/320)*1024) + 0.5f)))+" && sendevent /dev/input/"+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(event->pos().y()))/480)*910) + 0.5f)))+" && sendevent /dev/input/"+himax+" 0 2 0 && sendevent /dev/input/"+himax+" 3 57 1 && sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(320-event->pos().x()))/320)*1024) + 0.5f)))+" && sendevent /dev/input/"+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(480-event->pos().y()))/480)*910) + 0.5f)))+" && sendevent /dev/input/"+himax+" 0 2 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
    }
}

void myQWidget::mouseReleaseEvent( QMouseEvent *e)
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
}

void myQWidget::mouseMoveEvent(QMouseEvent *eve)
{
    if (multi_touch==false) {
        exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(eve->pos().x()))/320)*1024) + 0.5f)))+" && sendevent /dev/input/"+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(eve->pos().y()))/480)*910) + 0.5f)))+" && sendevent /dev/input/"+himax+" 0 0 0\"");
    }
    else {
        exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 57 0 && sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(eve->pos().x()))/320)*1024) + 0.5f)))+" && sendevent /dev/input/"+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(eve->pos().y()))/480)*910) + 0.5f)))+" && sendevent /dev/input/"+himax+" 0 2 0 && sendevent /dev/input/"+himax+" 3 57 1 && sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(320-eve->pos().x()))/320)*1024) + 0.5f)))+" && sendevent /dev/input/"+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(480-eve->pos().y()))/480)*910) + 0.5f)))+" && sendevent /dev/input/"+himax+" 0 2 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
    }
}

void myQWidget::paintEvent(QPaintEvent *)
{
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainWindow::pwr_btn_click_start()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+keypad+" 1 116 1\"");
}

void MainWindow::pwr_btn_click_end()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+keypad+" 1 116 0\"");
}

void MainWindow::vol_up_btn_click_start()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+keypad+" 1 115 1\"");
}

void MainWindow::vol_up_btn_click_end()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+keypad+" 1 115 0\"");
}

void MainWindow::vol_dn_btn_click_start()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+keypad+" 1 114 1\"");
}

void MainWindow::vol_dn_btn_click_end()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+keypad+" 1 114 0\"");
}

void MainWindow::home_btn_click_start()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 125 && sendevent /dev/input/"+himax+" 3 54 950 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::home_btn_click_end()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::menu_btn_click_start()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 375 && sendevent /dev/input/"+himax+" 3 54 950 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::menu_btn_click_end()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::back_btn_click_start()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 675 && sendevent /dev/input/"+himax+" 3 54 950 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::back_btn_click_end()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::search_btn_click_start()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 925 && sendevent /dev/input/"+himax+" 3 54 950 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::search_btn_click_end()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
void MainWindow::unlock_btn_click()
{
    exec->execute("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 512 && sendevent /dev/input/"+himax+" 3 54 758 && sendevent /dev/input/"+himax+" 0 0 0 && sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 880 && sendevent /dev/input/"+himax+" 3 54 758 && sendevent /dev/input/"+himax+" 0 0 0 && sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"");
}
