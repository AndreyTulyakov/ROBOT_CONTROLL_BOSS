#ifndef KEY_PRESS_EATER_H
#define KEY_PRESS_EATER_H

#include <QObject>
#include <QEvent>

class KeyPressEater : public QObject
{
    Q_OBJECT
public:
    explicit KeyPressEater(QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void signalKeyPress(int key);
    void signalKeyRelease(int key);

public slots:
};

#endif // KEY_PRESS_EATER_H
