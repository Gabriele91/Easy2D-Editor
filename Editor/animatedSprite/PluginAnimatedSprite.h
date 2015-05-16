#ifndef PLUGINANIMATEDSPRITE_H
#define PLUGINANIMATEDSPRITE_H
#include <Editor.h>
#include <UIAnimatedSprite.h>
#include <QGLContext>
#include <QOpenGLContext>
#include <ComponentsInterface.h>

struct Animation
{
    Easy2D::FrameSet::ptr frame;
    float time;
    bool loop;

    Animation()
        :frame(nullptr)
        ,time(0)
        ,loop(false){}

    Animation( Easy2D::FrameSet::ptr frame,
               float time,
               bool loop)
        :frame(frame)
        ,time(time)
        ,loop(loop){}
};
struct Animations
{
    std::vector<Easy2D::FrameSet::ptr> frames;
    std::vector<float> times;
    std::vector<bool> loops;
};

class ComponentAnimatedSprite : public ComponentsInterface
{

public:



    virtual QString getName();
    virtual QWidget* getWidget();
    virtual QWidget* buildGUI(QWidget* obj);
    Easy2D::AnimatedSprite* getAnimatedSprite();

    virtual void onCreate();
    virtual void onRun(Easy2D::Object* obj,float dt);
    virtual void onDestoy();
    virtual void onChange(Easy2D::Object* oldObj,
                          Easy2D::Object* newObj);
    //return a object animation by name
    Animation getAnimation(const QString& qname);
    //get list of object animation
    Animations getAnimations();
    //set corrent animation
    int setDefaultAnimation(int anim);
    //set current frame
    int setFrame(int frame);
    //return list of animation's name
    QStringList getFrameSetsNames();
    //load a frame set
    QString selectFrameSet();
    QString selectFrameSet(const QString& filter);
    QString selectFrameSetNotLoaded();
    //load a animation
    Animation selectAnimation();
    Animation selectAnimation(const QString& filter);
    Animation selectAnimationNotLoaded(const QString& filter);
    //update change
    void applayChange();
    //update animations list
    void applayFrames();

private:

    UIAnimatedSprite* uisprite;
    bool applay;
    bool applayBuild;


};

#endif // PLUGINANIMATEDSPRITE_H
