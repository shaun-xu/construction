#include "ProjectorQT.h"
#include "qpainter.h"
#include "ShowImage.h"
#include "qapplication.h"
#include "qscreen.h"
#include "qwindow.h"


extern QScreen * projector_screen;
ProjectorQT::ProjectorQT(unsigned int num):Projector()
{
    QWidget();
    show();
//    QWindow *handle = windowHandle();
//    windowHandle()->setScreen(projector_screen);
    QRect  rect= projector_screen->geometry();
    setGeometry(rect);
    showFullScreen();
//    windowHandle()->setScreen(projector_screen);

    m_nCurrent =0;
    this->resize(projector_screen->size());
}


void ProjectorQT::setPattern(unsigned int patternNumber, const unsigned char *tex, unsigned int texWidth, unsigned int texHeight)
{
//    std::map<unsigned int, QImage*>::iterator itr = m_mImages.find(patternNumber);
//    if(itr != m_mImages.end())
//    {
//        delete  itr->second;
//    }
//    QImage *tempImage=new QImage(tex,texWidth,texHeight,QImage::Format_RGB888);
    std::cout << "projector width=" <<this->size().width()<<",height="<<this->size().height()<< std::endl;
    showImage  *img = new showImage(this, (uchar *)tex,texWidth,texHeight);
    m_mImages.insert(std::map<unsigned int,showImage *>::value_type(patternNumber, img));
    img->resize(this->size());
    m_nCurrent=patternNumber;
//    ImageLable_m->show();


}
void ProjectorQT::displayPattern(unsigned int patternNumber)
{

    for(std::map<unsigned int,showImage *>::iterator ir=m_mImages.begin(); ir != m_mImages.end() ; ir++)
    {
        showImage * img = ir->second;

        img->hide();
    }
    std::map<unsigned int, showImage *>::iterator itr = m_mImages.find(patternNumber);
    if( itr != m_mImages.end())
    {
        showImage * img = itr->second;
        img->setGeometry(0,0,this->width(),this->height());
        img->show();
    }

}
// Upload and display pattern on the fly
void ProjectorQT::displayTexture(const unsigned char *tex, unsigned int width, unsigned int height)
{

}
// Monochrome color display
void ProjectorQT::displayBlack()
{

}
void ProjectorQT::displayWhite()
{

}
void ProjectorQT::getScreenRes(unsigned int *nx, unsigned int *ny)
{

    *nx = projector_screen->size().width();
    *ny = projector_screen->size().height();
}

