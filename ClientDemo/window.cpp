#include "window.h"

Window::Window()
{
  setStyleSheet("font-family:Lato;background:red");
  move(100,100);
  connect(mov,SIGNAL(pressed()),this,SLOT(moveWindow()));
  connect(cre,SIGNAL(pressed()),this,SLOT(createWindow()));
  connect(del,SIGNAL(pressed()),this,SLOT(deleteWindow()));
  connect(mod,SIGNAL(pressed()),this,SLOT(changeMode()));
  connect(opa,SIGNAL(pressed()),this,SLOT(changeOpacity()));
  connect(tit,SIGNAL(pressed()),this,SLOT(changeTitle()));
  connect(siz,SIGNAL(pressed()),this,SLOT(changeSize()));
  connect(time,SIGNAL(timeout()),this,SLOT(timeStopped()));
  layout->addWidget(mov);
  layout->addWidget(siz);
  layout->addWidget(cre);
  layout->addWidget(mod);
  layout->addWidget(opa);
  layout->addWidget(tit);
  layout->addWidget(del);
}
void Window::moveWindow()
{
    time->start(0);
}
void Window::createWindow()
{
    Window *w = new Window();
    w->show();
}

void Window::changeOpacity()
{
    setWindowOpacity(windowOpacity()-10);
}

void Window::changeMode()
{
    setMode(DOCK_MODE);
}

void Window::changeTitle()
{
    setWindowTitle("Sup bitches!");
}

void Window::changeSize()
{
    resize(width() + 10,height() + 10);
}

void Window::deleteWindow()
{
    close();
}
void Window::timeStopped()
{
    move(pos() + QPoint(1,1));
    time->start(1);
}
