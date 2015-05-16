#ifndef PLUGINTRANSFORM_H
#define PLUGINTRANSFORM_H
#include <UITransform.h>
#include <ComponentsInterface.h>

class ComponentTransform : public ComponentsInterface
{
public:

    virtual QString getName();
    virtual QWidget* getWidget();
    QWidget* buildGUI(QWidget *obj);

    virtual void onCreate();
    virtual void onRun(Easy2D::Object* obj,float dt);
    virtual void onDestoy();
    void doTransform();

private:

    UITransform* uitranform;
    bool applayTransform;

};


#endif // PLUGINTRANSFORM_H
