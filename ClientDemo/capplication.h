#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsBlurEffect>
#include <QDebug>
#include <QLocalSocket>
#include <QWidget>
#include <QThread>
#include <QLabel>
#include <QPicture>

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
    unsigned int opacity; // Opacity
    char title[128]; // Surface Title
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
    char title[128]; // Surface Title
}SurfaceTitleStruct;

// Send Surface Opacity
#define SURFACE_OPACITY 5
typedef struct{
    unsigned int type = SURFACE_OPACITY; // Message type
    unsigned int id; // Surface Id
    unsigned int opacity; // Surface Opacity
}SurfaceOpacityStruct;

// Send Surface blur Request
#define SURFACE_BLUR 6
typedef struct{
    unsigned int type = SURFACE_BLUR; // Message type
    unsigned int id; // Surface Id
    bool activate; // Turn ON/OFF blur
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

// Surface Blur Image
#define SURFACE_BLUR_IMAGE 2
typedef struct{
    unsigned int type; // Message type
    unsigned int id; // Surface desitnation
    unsigned int width; // Pixels
    unsigned int height; // Pixels
    unsigned char pixels[4*64*64]; // 64x64 Image (RGBA 255)
}SurfaceBlurImageStruct;

#endif


#ifndef CWIDGET
#define CWIDGET

/* ------------------ CWidget ------------------ */

class CWidget : public QWidget
{

    Q_OBJECT

public:

    // Constructor
    CWidget(CWidget *parent = nullptr)
    {
        // Wrongly used to identify a surface
        QWidget::setWindowTitle(QString::number(winId()));

        // Deletes blue Qt frame
        setWindowFlags(Qt::FramelessWindowHint);

        // Asigns parent
        QWidget::setParent(parent);

        // Configure blur
        blurImage->hide();
        blurEffect->setEnabled(false);
        blurEffect->setBlurRadius(20);
        blurImage->setScaledContents(true);
        blurImage->setGraphicsEffect(blurEffect);

    }

    // Assign and send position to Crystals
    void move(const QPoint &pos)
    {
        QWidget::move(pos);
        positionChanged(pos);
        blurArrived();
    }
    void move(int x, int y)
    {
        QWidget::move(x,y);
        positionChanged(QPoint(x,y));
        blurArrived();
    }

    // Sends title to Crystals
    void setWindowTitle(const QString &title)
    {
        localTitle = title;
        titleChanged(title);
    }

    // Gets local title
    QString windowTitle()
    {
        return localTitle;
    }

    // Resize
    void resize(const QSize &size)
    {
        QWidget::resize(size);
        blurArrived();
    }

    // Resize
    void resize(int x, int y)
    {
        QWidget::resize(x,y);
        blurArrived();
    }

    // Sends surface mode
    void setMode(unsigned int mode = WINDOW_MODE)
    {
        localMode = mode;
        modeChanged(mode);
    }

    // Gets surface mode
    unsigned int mode()
    {
        return localMode;
    }

    // Turn On/OFF blur
    void setBlur(bool mode)
    {
        localBlur = mode;
        blurStateChanged(mode);
        blurEffect->setEnabled(mode);
        if(mode)
        {
            blurArrived();
            blurImage->show();
        }
        else
        {
            blurImage->hide();
        }
    }

    // Gets blur state
    bool blurState()
    {
        return localBlur;
    }

    // Refresh blur properties
    void blurArrived()
    {
        blurImage->move(0,0);
        blurImage->resize(size());
    }

    // Set surface Opacity
    void setWindowOpacity(uint opacity = 255)
    {
        localOpacity = opacity;
        opacityChanged(opacity);
    }

    // Gets surface opacity
    uint windowOpacity()
    {
        return localOpacity;
    }

    bool registeredSurface = false;
    QLabel *blurImage = new QLabel(this);

signals:
    void positionChanged(const QPoint &pos);
    void titleChanged(QString);
    void modeChanged(uint);
    void opacityChanged(uint);
    void blurStateChanged(bool);

private:
    QString localTitle = "";
    uint localMode = WINDOW_MODE;
    uint localOpacity = 255;
    bool localBlur = false;
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect();



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
    QImage blur;

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
                    conf.role = widget->mode();
                    strcpy(conf.title,widget->windowTitle().toUtf8());

                    // Copy message to a char pointer
                    char data[sizeof(SurfaceConfigStruct)];
                    memcpy(data,&conf,sizeof(SurfaceConfigStruct));

                    // Send message
                    socket->write(data,sizeof(SurfaceConfigStruct));

                    // Connects widget's events
                    connect(widget,SIGNAL(positionChanged(QPoint)),this,SLOT(sendPosition(QPoint)));
                    connect(widget,SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));
                    connect(widget,SIGNAL(modeChanged(uint)),this,SLOT(modeChanged(uint)));
                    connect(widget,SIGNAL(blurStateChanged(bool)),this,SLOT(blurStateChanged(bool)));
                    connect(widget,SIGNAL(opacityChanged(uint)),this,SLOT(opacityChanged(uint)));


                    return;
                }

            }
        }break;
        case SURFACE_BLUR_IMAGE:{

            // Parse Message
            SurfaceBlurImageStruct *reply = (SurfaceBlurImageStruct*)message.data();

            // Search the registered widget
            Q_FOREACH(QWidget *wid,allWidgets())
            {
                if(wid->winId() == reply->id)
                {
                    CWidget *widget = qobject_cast<CWidget*>(wid);
                    blur = QImage(QSize(reply->width,reply->height),QImage::Format_RGB16);
                    qDebug() << "W:" + QString::number(reply->width);
                    qDebug() << "H:" + QString::number(reply->height);

                    int i = 0;
                    for(int y = 0; y<reply->height ;y++)
                    {
                        for(int x = 0; x<reply->width ;x++)
                        {
                            uint r = reply->pixels[i];
                            uint g = reply->pixels[i+1];
                            uint b = reply->pixels[i+2];
                            blur.setPixelColor(x,y,QColor(r,g,b));
                            i+=4;
                        }
                    }
                    widget->blurImage->setPixmap(QPixmap::fromImage(blur));
                    widget->blurArrived();
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

    void titleChanged(QString title)
    {
        CWidget *widget = qobject_cast<CWidget*>(sender());

        // Send surface title to Crystals
        SurfaceTitleStruct message;
        message.id = widget->winId();
        strcpy(message.title,title.toUtf8());

        // Copy message to a char pointer
        char data[sizeof(SurfaceTitleStruct)];
        memcpy(data,&message,sizeof(SurfaceTitleStruct));

        // Send message
        socket->write(data,sizeof(SurfaceTitleStruct));
    }

    void modeChanged(uint mode)
    {
        CWidget *widget = qobject_cast<CWidget*>(sender());

        // Send surface title to Crystals
        SurfaceRoleStruct message;
        message.id = widget->winId();
        message.role = mode;

        // Copy message to a char pointer
        char data[sizeof(SurfaceRoleStruct)];
        memcpy(data,&message,sizeof(SurfaceRoleStruct));

        // Send message
        socket->write(data,sizeof(SurfaceRoleStruct));
    }

    void opacityChanged(uint opacity)
    {
        CWidget *widget = qobject_cast<CWidget*>(sender());

        // Send surface title to Crystals
        SurfaceOpacityStruct message;
        message.id = widget->winId();
        message.opacity = opacity;

        // Copy message to a char pointer
        char data[sizeof(SurfaceOpacityStruct)];
        memcpy(data,&message,sizeof(SurfaceOpacityStruct));

        // Send message
        socket->write(data,sizeof(SurfaceOpacityStruct));
    }

    void blurStateChanged(bool mode)
    {
        CWidget *widget = qobject_cast<CWidget*>(sender());

        // Send surface title to Crystals
        SurfaceBlurStruct message;
        message.id = widget->winId();
        message.activate = mode;

        // Copy message to a char pointer
        char data[sizeof(SurfaceBlurStruct)];
        memcpy(data,&message,sizeof(SurfaceBlurStruct));

        // Send message
        socket->write(data,sizeof(SurfaceBlurStruct));
    }

signals:
    void run();

};

#endif


