#include "compositor.h"

Compositor::Compositor():QWaylandCompositor()
{
    create();

    setScreenResolution(QSize(1500,800));

    connect(this, &QWaylandCompositor::surfaceCreated, this, &Compositor::onSurfaceCreated);
    connect(this, &QWaylandCompositor::subsurfaceChanged, this, &Compositor::onSubsurfaceChanged);

    connect(defaultSeat(), &QWaylandSeat::cursorSurfaceRequest, this, &Compositor::adjustCursorSurface);
    connect(defaultSeat()->drag(), &QWaylandDrag::dragStarted, this, &Compositor::startDrag);

}


void Compositor::onSurfaceCreated(QWaylandSurface *surface)
{
    connect(surface, &QWaylandSurface::surfaceDestroyed, this, &Compositor::surfaceDestroyed);
    connect(surface, &QWaylandSurface::hasContentChanged, this, &Compositor::surfaceHasContentChanged);
    connect(surface, &QWaylandSurface::redraw, this, &Compositor::triggerRender);
    connect(surface, &QWaylandSurface::subsurfacePositionChanged, this, &Compositor::onSubsurfacePositionChanged);
    View *view = new View(this);
    view->setSurface(surface);
    view->setOutput(outputFor(window));
    views << view;
    connect(view, &QWaylandView::surfaceDestroyed, this, &Compositor::viewSurfaceDestroyed);
}

void Compositor::surfaceHasContentChanged()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    if (surface->hasContent()) {
        if (surface->role() == QWaylandWlShellSurface::role()){
            defaultSeat()->setKeyboardFocus(surface);
        }
    }
    triggerRender();
}

void Compositor::surfaceDestroyed()
{
    triggerRender();
}

void Compositor::viewSurfaceDestroyed()
{
    View *view = qobject_cast<View*>(sender());
    view->setBufferLocked(true);
    views.removeAll(view);
    delete view;
}


View * Compositor::findView(const QWaylandSurface *s) const
{
    Q_FOREACH (View* view, views) {
        if (view->surface() == s)
            return view;
    }
    return Q_NULLPTR;
}


void Compositor::onSubsurfaceChanged(QWaylandSurface *child, QWaylandSurface *parent)
{
    View *view = findView(child);
    View *parentView = findView(parent);
    view->setParentView(parentView);
}

void Compositor::onSubsurfacePositionChanged(const QPoint &position)
{
    qDebug()<<position;
    QWaylandSurface *surface = qobject_cast<QWaylandSurface*>(sender());
    if (!surface)
        return;

    View *view = findView(surface);
    view->setPosition(position);
    triggerRender();
}

void Compositor::triggerRender()
{
    window->requestUpdate();
}

void Compositor::startRender()
{
    QWaylandOutput *out = defaultOutput();
    if (out)
        out->frameStarted();
}

void Compositor::endRender()
{
    QWaylandOutput *out = defaultOutput();
    if (out)
        out->sendFrameCallbacks();
}

void Compositor::updateCursor()
{
    cursor.advance();
    QImage image = cursor.currentBuffer().image();
    if (!image.isNull())
        window->setCursor(QCursor(QPixmap::fromImage(image), m_cursorHotspotX, m_cursorHotspotY));
}

void Compositor::adjustCursorSurface(QWaylandSurface *surface, int hotspotX, int hotspotY)
{
    if ((cursor.surface() != surface)) {
        if (cursor.surface())
            disconnect(cursor.surface(), &QWaylandSurface::redraw, this, &Compositor::updateCursor);
        if (surface)
            connect(surface, &QWaylandSurface::redraw, this, &Compositor::updateCursor);
    }

    cursor.setSurface(surface);
    m_cursorHotspotX = hotspotX;
    m_cursorHotspotY = hotspotY;

    if (surface && surface->hasContent())
        updateCursor();
}



void Compositor::handleMouseEvent(QWaylandView *target, QMouseEvent *me)
{

    QWaylandSeat *input = defaultSeat();
    QWaylandSurface *surface = target ? target->surface() : nullptr;
    switch (me->type()) {
        case QEvent::MouseButtonPress:
            input->sendMousePressEvent(me->button());
            if (surface != input->keyboardFocus()) {
                if (surface == nullptr) {
                    input->setKeyboardFocus(surface);
                }
            }
            break;
    case QEvent::MouseButtonRelease:
         input->sendMouseReleaseEvent(me->button());
         break;
    case QEvent::MouseMove:
        input->sendMouseMoveEvent(target, me->localPos(), me->globalPos());
    default:
        break;
    }
}



void Compositor::startDrag()
{
    QWaylandDrag *currentDrag = defaultSeat()->drag();
    Q_ASSERT(currentDrag);
    View *iconView = findView(currentDrag->icon());
    iconView->setPosition(window->mapFromGlobal(QCursor::pos()));

    emit dragStarted(iconView);
}

void Compositor::handleDrag(View *target, QMouseEvent *me)
{
    QPointF pos = me->localPos();
    QWaylandSurface *surface = 0;
    if (target) {
        pos -= target->position();
        surface = target->surface();
    }
    QWaylandDrag *currentDrag = defaultSeat()->drag();
    currentDrag->dragMove(surface, pos);
    if (me->buttons() == Qt::NoButton) {
        views.removeOne(findView(currentDrag->icon()));
        currentDrag->drop();
    }
}




static int findEndOfChildTree(const QList<View*> &list, int index)
{
    int n = list.count();
    View *parent = list.at(index);
    while (index + 1 < n) {
        if (list.at(index+1)->parentView() != parent)
            break;
        index = findEndOfChildTree(list, index + 1);
    }
    return index;
}

void Compositor::raise(View *view)
{
    int startPos = views.indexOf(view);
    int endPos = findEndOfChildTree(views, startPos);

    int n = views.count();
    int tail =  n - endPos - 1;

    //bubble sort: move the child tree to the end of the list
    for (int i = 0; i < tail; i++) {
        int source = endPos + 1 + i;
        int dest = startPos + i;
        for (int j = source; j > dest; j--)
            views.swap(j, j-1);
    }
}

void Compositor::setScreenResolution(QSize size)
{
    window->resize(size);
    QWaylandOutputMode mode = QWaylandOutputMode(size, 60000);
    output->addMode(mode, true);
    output->setCurrentMode(mode);
}
