
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QLocalSocket>
#include <QWidget>
#include <QThread>

#ifndef SURFACE_MODES
#define SURFACE_MODES

#define WINDOW_MODE 0
#define FRAMELESS_MODE 1
#define TOPBAR_MODE 2
#define FILEMANAGER_MODE 3
#define DOCK_MODE 4

#endif

#ifndef MESSAGES
#define MESSAGES

/* ------------------ Out Messages Types ------------------ */

// Register App
#define REGISTER_APP 0
typedef struct{
    unsigned int type = REGISTER_APP; // Message type
    unsigned int pid; // App proccess Id
}RegisterAppStruct;

// Configure Surface
#define SURFACE_CONFIG 1
typedef struct{
    unsigned int type = SURFACE_CONFIG; // Message type
    unsigned int id; // Surface Id
    unsigned int role; // Surface Role
    unsigned int x; // X pos
    unsigned int y; // Y pos
    unsigned int titleLength;
    char title[64]; // Surface Title
}SurfaceConfigStruct;

// Send Surface Role
#define SURFACE_ROLE 2
typedef struct{
    unsigned int type = SURFACE_ROLE; // Message type
    unsigned int id; // Surface Id
    unsigned int role; // Surface Role
}SurfaceRoleStruct;

// Send Surface Position
#define SURFACE_POS 3
typedef struct{
    unsigned int type = SURFACE_POS; // Message type
    unsigned int id; // Surface Id
    int x; // X cords
    int y; // Y cords
}SurfacePosStruct;

// Send Surface Title
#define SURFACE_TITLE 4
typedef struct{
    unsigned int type = SURFACE_TITLE; // Message type
    unsigned int id; // Surface Id
    unsigned int titleLenght; // Title length
    char *title; // Surface Title
}SurfaceTitleStruct;

// Send Surface Opacity
#define SURFACE_OPACITY 5
typedef struct{
    unsigned int type = SURFACE_OPACITY; // Message type
    unsigned int id; // Surface Id
    float opacity; // Surface Opacity
}SurfaceOpacityStruct;

// Send Surface blur Request
#define SURFACE_BLUR 6
typedef struct{
    unsigned int type = SURFACE_BLUR; // Message type
    bool activate; // Turn ON/OFF blur
    unsigned int id; // Surface Id
    unsigned int childId; // Surface child Id
    int x; // X pos
    int y; // Y pos
    unsigned int width; // Width
    unsigned int height; // Height
}SurfaceBlurStruct;

// Send Surface Minimize
#define SURFACE_MINIMIZE 7
typedef struct{
    unsigned int type = SURFACE_MINIMIZE; // Message type
    unsigned int id; // Surface Id
    bool minimize; // Turn ON/OFF minimize
}SurfaceMinimizeStruct;

// Send Surface Expand
#define SURFACE_EXPAND 8
typedef struct{
    unsigned int type = SURFACE_EXPAND; // Message type
    unsigned int id; // Surface Id
    bool expand; // Turn ON/OFF expand
    unsigned int expandMode;
}SurfaceExpandStruct;

/* ------------------ In Messages Types ------------------ */

// App Registered Event
#define REGISTERED_APP 0
typedef struct{
    unsigned int type; // Message type
}RegisteredAppStruct;

// Surface Id Registered
#define REGISTERED_SURFACE 1
typedef struct{
    unsigned int type; // Message type
    unsigned int id; // Message type
}RegisteredSurfaceStruct;

#endif


#ifndef CWIDGET
#define CWIDGET

/* ------------------ CWidget ------------------ */

class CWidget : public QWidget
{

    Q_OBJECT

public:
    CWidget(CWidget *parent = nullptr)
    {
        setWindowTitle(QString::number(winId()));
        setWindowFlags(Qt::FramelessWindowHint);
        QWidget::setParent(parent);
    }
    void move(const QPoint &pos)
    {
        QWidget::move(pos);
        positionChanged(pos);
    }
    void move(int x, int y)
    {
        QWidget::move(x,y);
        positionChanged(QPoint(x,y));
    }

    bool registeredSurface = false;

signals:
    void positionChanged(const QPoint &pos);

};

#endif


#ifndef CAPPLICATION
#define CAPPLICATION

/* ------------------ CApplication ------------------ */

class CApplication : public QApplication
{
    Q_OBJECT

public:

    CApplication(int &argc,char **argv):QApplication(argc,argv){

        // Configure screen
        qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");
        setAttribute(Qt::AA_EnableHighDpiScaling);
        setAttribute(Qt::AA_UseHighDpiPixmaps);

        // Connected to Crystals
        connect(socket,SIGNAL(connected()),this,SLOT(connected()));

        // Message from Crystals
        connect(socket,SIGNAL(readyRead()),this,SLOT(messageIn()));

        // Connects to Crystals
        socket->connectToServer("com.cuarzo.crystals");

        // Wait until connect to Crystals
        socket->waitForReadyRead();
    }

    QLocalSocket *socket = new QLocalSocket(this); // Unix socket

public slots:

    void connected()
    {
        // Register App in Crystals
        RegisterAppStruct message;
        message.pid = QApplication::applicationPid();

        // Copy message to a char pointer
        char data[sizeof(RegisterAppStruct)];
        memcpy(data,&message,sizeof(RegisterAppStruct));

        // Send message
        socket->write(data,sizeof(RegisterAppStruct));

        qDebug() << "Connected to Crystals";
    }

    // Message from Crystals
    void messageIn()
    {
        // Store message
        QByteArray message = socket->readAll();

        // Message type
        unsigned int type = *(unsigned int*)message.mid(0,sizeof(unsigned int)).data();

        switch (type)
        {
        case REGISTERED_APP:{
            qDebug() << "Connected to Crystals";
        }break;
        case REGISTERED_SURFACE:{

            // Parse Message
            RegisteredSurfaceStruct *reply = (RegisteredSurfaceStruct*)message.data();

            // Search the registered widget
            Q_FOREACH(QWidget *wid,allWidgets())
            {
                if(wid->winId() == reply->id)
                {
                    CWidget *widget = qobject_cast<CWidget*>(wid);

                    // Send Surface Configuration
                    SurfaceConfigStruct conf;
                    conf.id = wid->winId();
                    conf.x = wid->pos().x();
                    conf.y = wid->pos().y();
                    conf.role = 0;
                    conf.titleLength = QString("CRYSTALS ES EL MEJOR COMPOSITORcaca").length();
                    strcpy(conf.title,QString("CRYSTALS ES EL MEJOR COMPOSITORcaca").toUtf8().data());

                    // Copy message to a char pointer
                    char data[sizeof(SurfaceConfigStruct)];
                    memcpy(data,&conf,sizeof(SurfaceConfigStruct));

                    // Send message
                    socket->write(data,sizeof(SurfaceConfigStruct));

                    connect(widget,SIGNAL(positionChanged(QPoint)),this,SLOT(sendPosition(QPoint)));

                    return;
                }

            }
        }break;
        }

    }


    // Widget events
    void sendPosition(const QPoint &pos)
    {
        CWidget *widget = qobject_cast<CWidget*>(sender());

        // Send surface pos to Crystals
        SurfacePosStruct message;
        message.id = widget->winId();
        message.x = pos.x();
        message.y = pos.y();

        // Copy message to a char pointer
        char data[sizeof(SurfacePosStruct)];
        memcpy(data,&message,sizeof(SurfacePosStruct));

        // Send message
        socket->write(data,sizeof(SurfacePosStruct));
    }
signals:
    void run();

};

#endif


