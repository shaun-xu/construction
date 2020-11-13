#ifndef SHOW_H

#define SHOW_H

#include <QImage>

#include <QWidget>

#include <QPainter>

#include <QPaintEvent>

class showImage:public QWidget

{

public:

    showImage(QWidget *parent ,uchar *text, unsigned int width, unsigned int height );

    ~showImage();

    QImage *show1;

    void paintEvent(QPaintEvent *e);

private:
    char * m_pImageData;

};

#endif // SHOW_H


