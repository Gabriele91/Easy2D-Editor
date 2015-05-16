#ifndef PLUGINSPRITE_H
#define PLUGINSPRITE_H
#define  SHAREDLIB_LIBRARY
#include <Editor.h>
#include <UISprite.h>
#include <QGLContext>
#include <QOpenGLContext>
#include <ComponentsInterface.h>

class ComponentSprite : public ComponentsInterface
{
public:

    virtual QString  getName();
    virtual QWidget* getWidget();
    virtual QWidget* buildGUI(QWidget* obj);

    virtual void onCreate();
    virtual void onRun(Easy2D::Object* obj,float dt);
    virtual void onDestoy();
    virtual void onChange(Easy2D::Object* oldObj,
                          Easy2D::Object* newObj);

    Easy2D::Texture::ptr getTexture(const QString& name);
    QString getTextureName();
    QString selectTexture();
    void applayChange();

private:
    UISprite* uisprite;
    bool applay;
};


#endif // PLUGINSPRITE_H
