#ifndef PLUGINSPEAKER_H
#define PLUGINSPEAKER_H
#include <UISpeaker.h>
#include <ComponentsInterface.h>

class ComponentSpeaker : public ComponentsInterface
{
public:

    virtual QString getName();
    virtual QWidget* getWidget();
    QWidget* buildGUI(QWidget *obj);
    Easy2D::Speaker* getSpeaker();

    virtual void onCreate();
    virtual void onRun(Easy2D::Object* obj,float dt);
    virtual void onDrawGui();
    virtual void onDestoy();
    QString selectSound();
    void doSpeaker();
    //input managment
    void onMousePressEvent(QMouseEvent* event);
    void onMouseMoveEvent(QMouseEvent* event);
    void onMouseReleaseEvent(QMouseEvent* event);

private:

    UISpeaker* uispeaker;
    bool applay, circleSelected;

};


#endif // PLUGINTRANSFORM_H
