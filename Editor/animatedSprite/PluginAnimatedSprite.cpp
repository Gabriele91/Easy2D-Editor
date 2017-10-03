#include <PluginAnimatedSprite.h>

QString ComponentAnimatedSprite::getName()
{
   return "AnimatedSprite";
}

QWidget* ComponentAnimatedSprite::getWidget()
{
   return uisprite;
}
//get animation sprite
Easy2D::AnimatedSprite* ComponentAnimatedSprite::getAnimatedSprite()
{
    if(getLastObject()) return getLastObject()->getComponent<Easy2D::AnimatedSprite>();
    return nullptr;
}
QWidget* ComponentAnimatedSprite::buildGUI(QWidget *obj)
{
    if(uisprite) delete uisprite;
    uisprite=new UIAnimatedSprite(this);
    applay=false;
    applayBuild=true;
    return uisprite;
}

void ComponentAnimatedSprite::onCreate()
{
    uisprite=nullptr;
    applayBuild=false;
    applay=false;
}

void ComponentAnimatedSprite::onRun(Easy2D::Object* obj,float dt)
{
    //ui?
    if(!uisprite) return;
    //get component
    auto sprite=getAnimatedSprite();
    //update
    if(sprite)
    {
        if(applayBuild)
        {
            if(sprite->countAnimations())
                uisprite->initAnimations(getAnimations(),
                                         sprite->getCurrentAnimation(),
                                         sprite->getCurrentFrame());
            applayBuild=false;
        }

        if(applay)
        {

            if(uisprite->getCullface()!=sprite->getCullFace())
                sprite->setCull(uisprite->getCullface());

            if(uisprite->getColor()!=sprite->getColor())
                sprite->setColor(uisprite->getColor());

            if(uisprite->getBlendSrc()!=sprite->getBlendSrc() ||
               uisprite->getBlendDst()!=sprite->getBlendDst() )
                sprite->setBlend(uisprite->getBlendSrc(),uisprite->getBlendDst());

            applay=false;
        }

        if(uisprite->getCullface()!=sprite->getCullFace())
            uisprite->setCullface(sprite->getCullFace());

        if(uisprite->getColor()!=sprite->getColor())
            uisprite->setColor(sprite->getColor());

        if(uisprite->getBlendSrc()!=sprite->getBlendSrc() ||
           uisprite->getBlendDst()!=sprite->getBlendDst() )
            uisprite->setBlend(sprite->getBlendSrc(),sprite->getBlendDst());

    }
}
void ComponentAnimatedSprite::onDestoy()
{
    if(uisprite)
        delete uisprite;
}
void ComponentAnimatedSprite::onChange(Easy2D::Object* oldObj,
                                       Easy2D::Object* newObj)
{
    applay=false;
}
//return a object animation by name
Animation ComponentAnimatedSprite::getAnimation(const QString& qname)
{
    //frames
    auto animations=getAnimations();
    //qstring to e2string
    Easy2D::String name(qname.toUtf8().data());
    //search
    for(int i=0;i!=animations.frames.size();++i)
    {
        if(animations.frames[i]->getName()==name)
        {
            return Animation(animations.frames[i],
                             animations.times[i],
                             animations.loops[i]);
        }
    }
    return Animation();
}
//get list of object animation
Animations ComponentAnimatedSprite::getAnimations()
{
    Animations animations;
    //get component
    auto sprite=getAnimatedSprite();
    //exist sprite component ?
    if(!sprite) return animations;
    //get frames
    sprite->getFrameSets(animations.frames);
    //resize
    animations.times.resize(animations.frames.size());
    animations.loops.resize(animations.frames.size());
    //get times
    for(int count=0;count!=animations.frames.size();++count)
    {
        animations.times[count]=sprite->getTotalTime(count);
        animations.loops[count]=sprite->getLoop(count);
    }
    //texture name
    return animations;
}
//set corrent animation
int ComponentAnimatedSprite::setDefaultAnimation(int anim)
{
    //last object?
    if(!getLastObject()) return 0;
    //get component
    auto sprite=getAnimatedSprite();
    //safe change
    int max=sprite->countAnimations() ? sprite->countAnimations()-1 : 0;
    int newAnim=Easy2D::Math::clamp(anim,max,0);
    sprite->setAnimation(newAnim);
    //return anim id
    return newAnim;
}
//set current frame
int ComponentAnimatedSprite::setFrame(int frame)
{
    //last object?
    if(!getLastObject()) return 0;
    //get component
    auto sprite=getAnimatedSprite();
    //safe change
    int max=sprite->getCurrentAnimationSize() ? sprite->getCurrentAnimationSize()-1 : 0;
    int newFrame=Easy2D::Math::clamp(frame,max,0);
    sprite->setFrame(newFrame);
    //return frame id
    return newFrame;
}
//return list of animation's name
QStringList ComponentAnimatedSprite::getFrameSetsNames()
{
    //List
    QStringList list;
    //get name of frame
    auto animations=getAnimations();
    for(const auto& frame : animations.frames)
    {
        list << QString::fromUtf8(frame->getName());
    }

    return list;
}
//load a frame set
QString ComponentAnimatedSprite::selectFrameSet()
{
    return resources("","frameSets");
}
QString ComponentAnimatedSprite::selectFrameSet(const QString& filter)
{
    QStringList list; list << filter;
    return resources("","frameSets", list);
}
QString ComponentAnimatedSprite::selectFrameSetNotLoaded()
{
    return resources("","frameSets", getFrameSetsNames());
}
//load a animation
Animation ComponentAnimatedSprite::selectAnimation()
{
    Animation anout;
    QString frameSet=selectFrameSet();
    if(frameSet.size())
    {
        anout.frame=resources()->load<Easy2D::FrameSet>(frameSet.toUtf8().data());
        anout.time=anout.frame->getDefaultTime();
        anout.loop=false;
    }
    return anout;
}
Animation ComponentAnimatedSprite::selectAnimation(const QString& filter)
{
    Animation anout;
    QString frameSet=selectFrameSet(filter);
    if(frameSet.size())
    {
        anout.frame=resources()->load<Easy2D::FrameSet>(frameSet.toUtf8().data());
        anout.time=anout.frame->getDefaultTime();
        anout.loop=false;
    }
    return anout;
}
Animation ComponentAnimatedSprite::selectAnimationNotLoaded(const QString& filter)
{
    Animation anout;
    QString frameSet=selectFrameSetNotLoaded();
    if(frameSet.size())
    {
        anout.frame=resources()->load<Easy2D::FrameSet>(frameSet.toUtf8().data());
        anout.time=anout.frame->getDefaultTime();
        anout.loop=false;
    }
    return anout;
}
//update change
void ComponentAnimatedSprite::applayChange()
{
    applay=true;
}
//update animations list
void ComponentAnimatedSprite::applayFrames()
{
    //last object?
    if(!getLastObject()) return;
    //get component
    auto sprite=getAnimatedSprite();
    //lock editor
    lock();
    //enable this context
    context()->makeCurrent();
    //load texture/frames
    Animations animations;
    uisprite->getAnimations(animations);
    //
    int selectedAnimation=sprite->getCurrentAnimation();
    sprite->clearAnimations();
    //
    for(size_t i=0;i!=animations.frames.size();++i)
    {
        sprite->addAnimation(animations.frames[i],
                             animations.times[i],
                             animations.loops[i]);
    }
    //safe
    if(selectedAnimation<sprite->countAnimations())
        sprite->setAnimation(selectedAnimation);
    else if(sprite->countAnimations())
        sprite->setAnimation(0);
    //unlock editor
    unlock();
}
