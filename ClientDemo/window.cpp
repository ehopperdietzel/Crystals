#include "window.h"

Window::Window()
{
  setStyleSheet("font-family:Lato");
  move(100,100);
  connect(mov,SIGNAL(pressed()),this,SLOT(moveWindow()));
  connect(cre,SIGNAL(pressed()),this,SLOT(createWindow()));
  connect(del,SIGNAL(pressed()),this,SLOT(deleteWindow()));
  layout->addWidget(mov);
  layout->addWidget(cre);
  layout->addWidget(del);
  show();
}
void Window::moveWindow()
{
    move(pos() + QPoint(10,10));
}
void Window::createWindow()
{
    Window *w = new Window();
}
void Window::deleteWindow()
{
    deleteLater();
}
