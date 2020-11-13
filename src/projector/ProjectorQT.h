#ifndef PROJECTORQT_H
#define PROJECTORQT_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QPoint>
#include <QtMath>
#include <QMouseEvent>
#include "Projector.h"
#include "ShowImage.h"



class ProjectorQT: public QWidget, public Projector
{
    Q_OBJECT
public:
    ProjectorQT(unsigned int screen);

    virtual void setPattern(unsigned int patternNumber, const unsigned char *tex, unsigned int texWidth, unsigned int texHeight) ;
    virtual void displayPattern(unsigned int patternNumber) ;
    // Upload and display pattern on the fly
    virtual void displayTexture(const unsigned char *tex, unsigned int width, unsigned int height) ;
    // Monochrome color display
    virtual void displayBlack() ;
    virtual void displayWhite() ;
    virtual void getScreenRes(unsigned int *nx, unsigned int *ny) ;
signals:

public slots:

private:
    std::map<unsigned int,showImage *>   m_mImages;
    unsigned int m_nCurrent;
};

#endif // PROJECTORQT_H
