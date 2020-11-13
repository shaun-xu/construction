#include "SLStudio.h"
#include <QApplication>
#include "qscreen.h"

QScreen * projector_screen =NULL;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QList<QScreen*> lists = a.screens();
    for(unsigned int i=0; i<lists.size(); i++){
        QScreen *test = lists.at(i);
        std::cout<<"screen i="<<i<<",width="<<test->size().width()<<",height="<<test->size().height()<<std::endl;
    }

    projector_screen=a.screens().at(1);
    SLStudio w;
    w.show();
    
    return a.exec();
}
