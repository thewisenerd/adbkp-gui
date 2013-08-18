#ifndef MYQWIDGET_H
#define MYQWIDGET_H

#include <QWidget>

class myQWidget : public QWidget
{
    Q_OBJECT
public:
    explicit myQWidget(QWidget *parent = 0);

signals:
    void clicked();
public slots:
    void slotClicked();
    void bg_img_press();
    void update_bg();
protected:
    void mousePressEvent ( QMouseEvent * event ) ;
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);

};

#endif // MYQWIDGET_H
