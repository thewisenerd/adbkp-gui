#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <myQWidget.h>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include <math.h>
#include <sstream>

using namespace std;

int xcor = 0, ycor = 0, x2cor = 0, y2cor = 0, xval = 0, yval = 0, x2val = 0, y2val = 0;

// Default CM9 config
std::string himax="event3";
std::string keypad="event4";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->lp_pwr_btn->setToolTip("Long Press Power Button");
    myQWidget *bg_img = new myQWidget(this);
    bg_img->setGeometry(160,5,320,480);
    bg_img->show();
    QPushButton *upd_bg_btn = new QPushButton(this);
    upd_bg_btn->setGeometry(510, 200, 100, 28);
    upd_bg_btn->setText("Refresh");
    upd_bg_btn->setToolTip("Refresh Screen");
    upd_bg_btn->show();
    this->ui->pwr_btn->setToolTip("Press Power Button");
    this->ui->vol_up_btn->setToolTip("Press Volume-Up Button");
    this->ui->vol_dn_btn->setToolTip("Press Volume-Down Button");
    this->ui->unlock_btn->setToolTip("Perform Basic Unlock gesture");
    this->connect(this->ui->pwr_btn, SIGNAL(pressed()), this, SLOT(pwr_btn_click_start()));
    this->connect(this->ui->pwr_btn, SIGNAL(released()), this, SLOT(pwr_btn_click_end()));
    this->connect(this->ui->vol_up_btn, SIGNAL(clicked()), this, SLOT(vol_up_btn_click()));
    this->connect(this->ui->vol_dn_btn, SIGNAL(clicked()), this, SLOT(vol_dn_btn_click()));
    this->connect(this->ui->home_btn, SIGNAL(clicked()), this, SLOT(home_btn_click()));
    this->connect(this->ui->menu_btn, SIGNAL(clicked()), this, SLOT(menu_btn_click()));
    this->connect(this->ui->back_btn, SIGNAL(clicked()), this, SLOT(back_btn_click()));
    this->connect(this->ui->search_btn, SIGNAL(clicked()), this, SLOT(search_btn_click()));
    this->connect(this->ui->unlock_btn, SIGNAL(clicked()), this, SLOT(unlock_btn_click()));
    this->connect(this->ui->lp_pwr_btn, SIGNAL(clicked()), this, SLOT(lp_pwr_btn_click()));
    connect(upd_bg_btn, SIGNAL(clicked()), bg_img, SLOT(update_bg()));
    this->connect(this->ui->rom_select, SIGNAL(currentIndexChanged(int)), SLOT(rom_select()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::rom_select() {
    QString rom_name=this->ui->rom_select->currentText();
    if (rom_name=="CM9") {
        himax="event3";
        keypad="event4";
    }
    else if (rom_name=="MiniCM9"){
        himax="event4";
        keypad="event3";
    }
    string temp=rom_name.toStdString();
    system(("echo "+temp).c_str());
}

void myQWidget::update_bg(){
    setStyleSheet("background-image: url(./ph.png);");
    system("./bin/adb shell screencap -p | sed 's/\r$//' > ph.png");
    setStyle(QApplication::style());
    ;
}

myQWidget::myQWidget(QWidget *parent) :
    QWidget(parent)
{
    connect( this, SIGNAL( clicked() ), this, SLOT( slotClicked() ) );
    setStyleSheet("background-image: url(./black.png);");
}

void myQWidget::slotClicked() {
    bg_img_press();
}

void myQWidget::mousePressEvent ( QMouseEvent * event )
{
    xcor=event->pos().x();
    ycor=event->pos().y();
}

void myQWidget::mouseReleaseEvent ( QMouseEvent *e )
{
    x2cor=e->pos().x();
    y2cor=e->pos().y();
    emit clicked();
}

void myQWidget::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

// Custom Functions for ease of use :)
// Round number function.
int roundn(float x)
{
    return static_cast<int>(floor(x + 0.5f));
}
// integer to string conversion.
string inttoa(int a) {
    string str;
    ostringstream temp;
    temp<<a;
    return temp.str();
}

void send_tclick(string x, string y){
    system(("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+x+" && sendevent /dev/input/"+himax+" 3 54 "+y+" && sendevent /dev/input/"+himax+" 0 0 0 && sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"").c_str());
}

void send_tswipe(string x, string y, string x2, string y2){
    system(("./bin/adb shell \"sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+x+" && sendevent /dev/input/"+himax+" 3 54 "+y+" && sendevent /dev/input/"+himax+" 0 0 0 && sendevent /dev/input/"+himax+" 3 48 1 && sendevent /dev/input/"+himax+" 3 53 "+x2+" && sendevent /dev/input/"+himax+" 3 54 "+y2+" && sendevent /dev/input/"+himax+" 0 0 0 && sendevent /dev/input/"+himax+" 3 48 0 && sendevent /dev/input/"+himax+" 0 0 0\"").c_str());
}

void myQWidget::bg_img_press(){
    int xcortemp=xcor, ycortemp=ycor, xcor2temp=x2cor, ycor2temp=y2cor;
    if ((xcortemp==xcor2temp) && (ycortemp==ycor2temp)){
        double xcordbl=xcor;
        double ycordbl=ycor;
        float xcorflt=(xcordbl/320)*1024;
        xval=round(xcorflt);
        float ycorflt=(ycordbl/480)*910;
        yval=round(ycorflt);
        string tempstrx=inttoa(xval);
        string tempstry=inttoa(yval);
        send_tclick(tempstrx, tempstry);
    }
    else {
        double xcordbl=xcor, ycordbl=ycor, xcor2dbl=x2cor, ycor2dbl=y2cor;
        float xcorflt=(xcordbl/320)*1024;
        xval=round(xcorflt);
        float ycorflt=(ycordbl/480)*910;
        yval=round(ycorflt);
        float xcor2flt=(xcor2dbl/320)*1024;
        x2val=round(xcor2flt);
        float ycor2flt=(ycor2dbl/480)*910;
        y2val=round(ycor2flt);
        string tempstrx=inttoa(xval);
        string tempstry=inttoa(yval);
        string tempstr2x=inttoa(x2val);
        string tempstr2y=inttoa(y2val);
        send_tswipe(tempstrx, tempstry, tempstr2x, tempstr2y);

    }
}

void MainWindow::pwr_btn_click_start(){
    system(("./bin/adb shell \"sendevent /dev/input/"+keypad+" 1 116 1\"").c_str());
}

void MainWindow::pwr_btn_click_end(){
    system(("./bin/adb shell \"sendevent dev/input/"+keypad+" 1 116 0\"").c_str());
}

void MainWindow::lp_pwr_btn_click(){
    system("./bin/adb shell \"sendevent /dev/input/event4 1 116 1 && sleep 2.0 && sendevent  dev/input/event4 1 116 0\"");
}

void MainWindow::vol_up_btn_click(){
    system("./bin/adb shell \"sendevent /dev/input/event4 1 115 1 && sendevent  dev/input/event4 1 115 0\"");
}

void MainWindow::vol_dn_btn_click(){
    system("./bin/adb shell \"sendevent /dev/input/event4 1 114 1 && sendevent  dev/input/event4 1 114 0\"");
}

void MainWindow::home_btn_click(){
    system("./bin/adb shell \"sendevent /dev/input/event3 3 48 1 && sendevent /dev/input/event3 3 53 125 && sendevent /dev/input/event3 3 54 950 && sendevent /dev/input/event3 0 0 0 && sendevent /dev/input/event3 3 48 0 && sendevent /dev/input/event3 0 0 0\"");
}

void MainWindow::menu_btn_click(){
    system("./bin/adb shell \"sendevent /dev/input/event3 3 48 1 && sendevent /dev/input/event3 3 53 375 && sendevent /dev/input/event3 3 54 950 && sendevent /dev/input/event3 0 0 0 && sendevent /dev/input/event3 3 48 0 && sendevent /dev/input/event3 0 0 0\"");
}

void MainWindow::back_btn_click(){
    system("./bin/adb shell \"sendevent /dev/input/event3 3 48 1 && sendevent /dev/input/event3 3 53 675 && sendevent /dev/input/event3 3 54 950 && sendevent /dev/input/event3 0 0 0 && sendevent /dev/input/event3 3 48 0 && sendevent /dev/input/event3 0 0 0\"");
}

void MainWindow::search_btn_click(){
    system("./bin/adb shell \"sendevent /dev/input/event3 3 48 1 && sendevent /dev/input/event3 3 53 925 && sendevent /dev/input/event3 3 54 950 && sendevent /dev/input/event3 0 0 0 && sendevent /dev/input/event3 3 48 0 && sendevent /dev/input/event3 0 0 0\"");
}

void MainWindow::unlock_btn_click() {
    system("./bin/adb shell \"sendevent /dev/input/event3 3 48 1 && sendevent /dev/input/event3 3 53 512 && sendevent /dev/input/event3 3 54 758 && sendevent /dev/input/event3 0 0 0 && sendevent /dev/input/event3 3 48 1 && sendevent /dev/input/event3 3 53 880 && sendevent /dev/input/event3 3 54 758 && sendevent /dev/input/event3 0 0 0 && sendevent /dev/input/event3 3 48 0 && sendevent /dev/input/event3 0 0 0\"");
}
