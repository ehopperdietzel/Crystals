#ifndef WINDOW_H
#define WINDOW_H

#include "capplication.h"
#include <QBoxLayout>
#include <QPushButton>

class Window : public CWidget
{
    Q_OBJECT
public:
    Window();
    QPushButton *mov = new QPushButton("Mover");
    QPushButton *cre = new QPushButton("Crear");
    QPushButton *del = new QPushButton("Eliminar");
    QHBoxLayout *layout = new QHBoxLayout(this);
public slots:
    void moveWindow();
    void deleteWindow();
    void createWindow();
};

#endif // WINDOW_H
