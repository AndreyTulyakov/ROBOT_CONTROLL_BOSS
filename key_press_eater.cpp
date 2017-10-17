#include "key_press_eater.h"
#include <QKeyEvent>
#include <QDebug>

KeyPressEater::KeyPressEater(QObject *parent) : QObject(parent)
{

}

bool KeyPressEater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->isAutoRepeat() == false)
        {
            int key = keyEvent->key();

            if (event->type() == QEvent::KeyPress)
            {
                emit signalKeyPress(key);
            }

            if (event->type() == QEvent::KeyRelease)
            {
                emit signalKeyRelease(key);
            }

            // qDebug("Ate key press %d", keyEvent->key());
            return true;
        }
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}
