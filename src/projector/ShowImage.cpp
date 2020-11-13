#include "ShowImage.h"
#include "qscreen.h"
#include "QApplication"
#include <iostream>


showImage::showImage(QWidget *parent ,uchar *text, unsigned int width, unsigned int height):QWidget(parent)
{
    //show1.load("C:\\Users\\zoule\\Desktop\\TestUnit\\hh.jpg");//文件路径
   m_pImageData = new char[width*height*3];
   bcopy(text, m_pImageData, width*height*3);
   show1=new QImage((uchar *)m_pImageData,width,height,QImage::Format_RGB888);
}

showImage::~showImage()

{

}

void showImage::paintEvent(QPaintEvent *e)

{

    std::cout << "this width=" <<this->size().width()<<",height="<<this->size().height()<< std::endl;
    std::cout << "image width=" <<show1->size().width()<<",height="<<show1->size().height()<< std::endl;

    QPainter pp(this);

    QRect Temp(0,0,show1->size().width(),show1->size().height());

    pp.drawImage(Temp,*show1);

}


