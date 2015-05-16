#ifndef PLUGINPARALLAX_H
#define PLUGINPARALLAX_H
#define  SHAREDLIB_LIBRARY
#include <Parallax.h>
#include <UIParallax.h>
#include <ComponentsInterface.h>

class ComponentParallax : public ComponentsInterface
{
public:

    virtual QString getName();
    virtual QWidget* getWidget();
    virtual QWidget* buildGUI(QWidget *obj);

    virtual void onCreate();
    virtual void onRun(Easy2D::Object* obj,float dt);
    virtual void onDestoy();

    void applayChange();

private:

    UIParallax* uiparallax;
    bool applay;

};



#endif // PLUGINPARALLAX_H
