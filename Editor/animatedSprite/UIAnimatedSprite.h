#ifndef UIANIMATEDSPRITE_H
#define UIANIMATEDSPRITE_H

#include <Easy2D.h>
#include <QWidget>


namespace Ui
{
    class UIAnimatedSprite;
}

class Animation;
class Animations;
class ComponentAnimatedSprite;
class UIAnimatedSprite : public QWidget
{
    Q_OBJECT

public:
    explicit UIAnimatedSprite(ComponentAnimatedSprite *csprite);
    ~UIAnimatedSprite();

    //frames
    void initAnimations(const Animations& anim,
                        int animDefault,
                        int frameDefault);
    void pushAnimation(const Animation& anim);
    void getAnimations(Animations &anim);
    void clearAnimations();
    //cullface
    void setCullface(Easy2D::CullFace type);
    Easy2D::CullFace getCullface();
    //colors
    void setColor(const Easy2D::Color& color);
    Easy2D::Color getColor();
    //blend
    void setBlend(int blendsrc,int blenddst);
    int getBlendSrc();
    int getBlendDst();

public slots:

    //update frame color
    void updateFrameColor();
    //update change
    void applayChange();
    void applayFrames();
    //set default animation
    void applayDefaultAnim(QWidget* widget);

private:

    Ui::UIAnimatedSprite *ui;
    ComponentAnimatedSprite* csprite;
    std::vector<QWidget*> frameSets;

    struct DefaultValues
    {
        int animation;
        int frame;
        DefaultValues()
        {
            animation=0;
            frame=0;
        }
    };
    //default values
    DefaultValues vdefault;

    //update default value
    void deleteDefault(int animDelete);
    void updateDefaultUI();

    //add frame set
    void addFrameSet(const QString& name,
                     float defaultTime,
                     bool loop);
};

#endif // UIANIMATEDSPRITE_H
