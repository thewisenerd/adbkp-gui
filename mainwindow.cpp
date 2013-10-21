#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <myQWidget.h>
#include <QPainter>
#include <QMouseEvent>
#include <QCheckBox>
#include <QProcess>
#include <QDebug>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

bool multi_touch = false;
QProcess *exec = new QProcess();

QString himax;
QString keypad;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    recalibrate(); //auto-recalibrate at startup

    // The *screen* box
    myQWidget *bg_img = new myQWidget(this);
    bg_img->setGeometry(160,5,320,480);
    bg_img->setCursor(Qt::CrossCursor);
    bg_img->show();

    // Refresh button
    QPushButton *upd_bg_btn = new QPushButton(this);
    upd_bg_btn->setGeometry(510, 180, 100, 28);
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

    // refresh button
    connect(upd_bg_btn, SIGNAL(clicked()), bg_img, SLOT(update_bg()));

    // touch type select
    this->connect(this->ui->multi_chk, SIGNAL(clicked()), SLOT(touch_type_set()));

    // recalibrate
    //this->connect(this->ui->recalibrate_btn, SIGNAL(clicked()), SLOT(recalibrate()));

    // send text
    this->connect(this->ui->send_text, SIGNAL(clicked()), SLOT(sendtext()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recalibrate() {
    system("timeout 1s adb shell getevent -c 1 > ./keycodes.txt");

    ifstream keycodes("keycodes.txt");
    std::string keycodes_buf;
    int line=0;

    while (!keycodes.eof()) {
        std::getline(keycodes, keycodes_buf, '\n');
        line += 1;
        if (keycodes_buf.find("himax-touchscreen") != std::string::npos) {
            break;
        }
    }

    line -= 1;
    keycodes.clear();
    keycodes.seekg(0, ios::beg);

    for (int i=0; i<line; i++){
        std::getline(keycodes, keycodes_buf, '\n');
    }
    int pos = keycodes_buf.find("/dev");

    std::string himax_buf = keycodes_buf.substr(pos);
    himax = QString::fromStdString(himax_buf);

    std::cout<<"Captured touchscreen event at: "<<himax.toStdString()<<std::endl;

    keycodes.clear();
    keycodes.seekg(0, ios::beg);
    line=0;

    while (!keycodes.eof()) {
        std::getline(keycodes, keycodes_buf, '\n');
        line += 1;
        if (keycodes_buf.find("pico-keypad") != std::string::npos) {
            break;
        }
    }

    line -= 1;
    keycodes.clear();
    keycodes.seekg(0, ios::beg);

    for (int i=0; i<line; i++){
        std::getline(keycodes, keycodes_buf, '\n');
    }
    pos = keycodes_buf.find("/dev");

    std::string keypad_buf = keycodes_buf.substr(pos);
    keypad = QString::fromStdString(keypad_buf);

    std::cout<<"Captured keypad event at: "<<keypad.toStdString()<<std::endl;

    if (remove("keycodes.txt")!=0) {
        std::cout<<"Error removing keycodes.txt"<<std::endl;
        std::cout<<"You can safely remove keycodes.txt manually, later."<<std::endl;
    }
}


void MainWindow::sendtext() {
    exec->execute("adb shell \"input text \""+this->ui->line_send->text()+"\"\"");
    this->ui->line_send->setText("");
}

void MainWindow::touch_type_set() {
    if (multi_touch==false){
        multi_touch=true;
    }
    else {
        multi_touch=false;
    }
}

void myQWidget::update_bg() {
    setStyleSheet("background-image: url(./ph.png);");
    system("adb shell screencap -p | sed 's/\r$//' > ph.png");
    setStyle(QApplication::style());
}

myQWidget::myQWidget(QWidget *parent):QWidget(parent) {
    setStyleSheet("background-image: url(:/files/black.png);");
}

void myQWidget::mousePressEvent(QMouseEvent *e) {
    if (multi_touch==false) {
            exec->execute("adb shell \"sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().x()))/320)*1024) + 0.5F)))+" && sendevent "+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().y()))/480)*910) + 0.5F)))+" && sendevent "+himax+" 0 0 0\"");
                }
    else {
        exec->execute("adb shell \"sendevent "+himax+" 3 57 0 && sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().x()))/320)*1024) + 0.5F)))+" && sendevent "+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().y()))/480)*910) + 0.5F)))+" && sendevent "+himax+" 0 2 0 && sendevent "+himax+" 3 57 1 && sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(320-e->pos().x()))/320)*1024) + 0.5F)))+" && sendevent "+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(480-e->pos().y()))/480)*910) + 0.5F)))+" && sendevent "+himax+" 0 2 0 && sendevent "+himax+" 0 0 0\"");
    }
}

void myQWidget::mouseReleaseEvent( QMouseEvent *e) {
    e->ignore();
    exec->execute("adb shell \"sendevent "+himax+" 3 48 0 && sendevent "+himax+" 0 0 0\"");
}

void myQWidget::mouseMoveEvent(QMouseEvent *e) {
    if (multi_touch==false) {
        exec->execute("adb shell \"sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().x()))/320)*1024) + 0.5F)))+" && sendevent "+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().y()))/480)*910) + 0.5F)))+" && sendevent "+himax+" 0 0 0\"");
    }
    else {
        exec->execute("adb shell \"sendevent "+himax+" 3 57 0 && sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().x()))/320)*1024) + 0.5F)))+" && sendevent "+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(e->pos().y()))/480)*910) + 0.5F)))+" && sendevent "+himax+" 0 2 0 && sendevent "+himax+" 3 57 1 && sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 "+QString::number(static_cast<int>(floor((((static_cast<double>(320-e->pos().x()))/320)*1024) + 0.5F)))+" && sendevent "+himax+" 3 54 "+QString::number(static_cast<int>(floor((((static_cast<double>(480-e->pos().y()))/480)*910) + 0.5F)))+" && sendevent "+himax+" 0 2 0 && sendevent "+himax+" 0 0 0\"");
    }
}

void myQWidget::paintEvent(QPaintEvent *) {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainWindow::pwr_btn_click_start() {
    exec->execute("echo \"sendevent "+keypad+" 1 116 1 && sendevent "+keypad+" 0 0 0\"");
}
void MainWindow::pwr_btn_click_end() {
    exec->execute("echo \"adb shell sendevent "+keypad+" 1 116 0 && sendevent "+keypad+" 0 0 0\"");
}
void MainWindow::vol_up_btn_click_start() {
    exec->execute("echo \"adb shell sendevent "+keypad+" 1 115 1 && sendevent "+keypad+" 0 0 0\"");
}
void MainWindow::vol_up_btn_click_end() {
    exec->execute("adb shell \"sendevent "+keypad+" 1 115 0 && sendevent "+keypad+" 0 0 0\"");
}
void MainWindow::vol_dn_btn_click_start() {
    exec->execute("adb shell \"sendevent "+keypad+" 1 114 1 && sendevent "+keypad+" 0 0 0\"");
}
void MainWindow::vol_dn_btn_click_end() {
    exec->execute("adb shell \"sendevent "+keypad+" 1 114 0 && sendevent "+keypad+" 0 0 0\"");
}
void MainWindow::home_btn_click_start() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 125 && sendevent "+himax+" 3 54 950 && sendevent "+himax+" 0 0 0\"");
}
void MainWindow::home_btn_click_end() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 0 && sendevent "+himax+" 0 0 0\"");
}
void MainWindow::menu_btn_click_start() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 375 && sendevent "+himax+" 3 54 950 && sendevent "+himax+" 0 0 0\"");
}
void MainWindow::menu_btn_click_end() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 0 && sendevent "+himax+" 0 0 0\"");
}
void MainWindow::back_btn_click_start() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 675 && sendevent "+himax+" 3 54 950 && sendevent "+himax+" 0 0 0\"");
}
void MainWindow::back_btn_click_end() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 0 && sendevent "+himax+" 0 0 0\"");
}
void MainWindow::search_btn_click_start() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 1 && sendevent "+himax+" 3 53 925 && sendevent "+himax+" 3 54 950 && sendevent "+himax+" 0 0 0\"");
}
void MainWindow::search_btn_click_end() {
    exec->execute("adb shell \"sendevent "+himax+" 3 48 0 && sendevent "+himax+" 0 0 0\"");
}
