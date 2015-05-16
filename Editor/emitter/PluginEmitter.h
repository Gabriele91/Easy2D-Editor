#ifndef PLUGINEMITTER_H
#define PLUGINEMITTER_H

#include <UIEmitter.h>
#include <ComponentsInterface.h>

class ComponentEmitter : public ComponentsInterface
{

public:

    virtual QString getName();
    virtual QWidget* getWidget();
    virtual QWidget* buildGUI(QWidget* obj);

    virtual void onCreate();
    virtual void onRun(Easy2D::Object* obj,float dt);
    virtual void onDestoy();

    void applayGlobalsChange();
    void applayChange();

    bool setTexture(const QString& name);
    QString getTextureName();
    QString selectTexture();
    Easy2D::Emitter* getEmitter();

    void restart();
    void play();
    void pause();

private:

    UIEmitter* uiemitter;
    bool applay;
    bool applayGlobals;
    bool initUi;

};

#endif // PLUGINEMITTER_H
