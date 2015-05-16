#ifndef UISPEAKER_H
#define UISPEAKER_H

#include <QWidget>
#include <Easy2D.h>

namespace Ui{ class UISpeaker; };
class ComponentSpeaker;

class UISpeaker : public QWidget
{
    Q_OBJECT

public:
    explicit UISpeaker(ComponentSpeaker *cspeaker = 0);
    ~UISpeaker();

    void setRadius(float rad);
    float getRadius();

    void setVolume(float volume);
    float getVolume();

    bool get2D();
    void set2D(bool enable);

    bool getPlayOnAttach();
    void setPlayOnAttach(bool enable);

    bool getLoop();
    void setLoop(bool enable);

    void setSound(const QString& sound);
    QString getSound();
    
private:

    Ui::UISpeaker *ui;

};

#endif // TRANSFORM_H
