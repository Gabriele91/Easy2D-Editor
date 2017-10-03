#include <PluginSprite.h>

QString  ComponentSprite::getName()
{
   return "Sprite";
}
QWidget* ComponentSprite::getWidget()
{
   return uisprite;
}
QWidget* ComponentSprite::buildGUI(QWidget* obj)
{
    if(uisprite) delete uisprite;
    uisprite=new UISprite(this);
    applay=false;
    return uisprite;
}

void ComponentSprite::onCreate()
{
    uisprite=nullptr;
}
void ComponentSprite::onRun(Easy2D::Object* obj,float dt)
{
    //ui?
    if(!uisprite) return;
    //get component
    auto sprite=obj->getComponent<Easy2D::Sprite>();
    //update
    if(sprite)
    {

        if(applay)
        {
            if(uisprite->getTexture()!="" &&
               uisprite->getTexture()!=getTextureName())
            {
                //lock editor
                lock();
                //enable this context
                context()->makeCurrent();
                //load texture
                auto texture=getTexture(uisprite->getTexture());
                //set texture
                sprite->setTexture(texture);
                //unlock editor
                unlock();
            }
            if(uisprite->getCullface()!=sprite->getCullFace())
                sprite->setCull(uisprite->getCullface());

            if(uisprite->getColor()!=sprite->getColor())
                sprite->setColor(uisprite->getColor());

            if(uisprite->getBlendSrc()!=sprite->getBlendSrc() ||
               uisprite->getBlendDst()!=sprite->getBlendDst() )
                sprite->setBlend(uisprite->getBlendSrc(),uisprite->getBlendDst());

            applay=false;
        }


        if(uisprite->getTexture()!=getTextureName())
            uisprite->setTexture(getTextureName());

        if(uisprite->getCullface()!=sprite->getCullFace())
            uisprite->setCullface(sprite->getCullFace());

        if(uisprite->getColor()!=sprite->getColor())
            uisprite->setColor(sprite->getColor());

        if(uisprite->getBlendSrc()!=sprite->getBlendSrc() ||
           uisprite->getBlendDst()!=sprite->getBlendDst() )
            uisprite->setBlend(sprite->getBlendSrc(),sprite->getBlendDst());
    }
}
void ComponentSprite::onDestoy()
{
    if(uisprite)
        delete uisprite;
}
void ComponentSprite::onChange(Easy2D::Object* oldObj,
                               Easy2D::Object* newObj)
{
    applay=false;
}

Easy2D::Texture::ptr ComponentSprite::getTexture(const QString& name)
{
    return resources()->load<Easy2D::Texture>(name.toUtf8().data());
}
QString ComponentSprite::getTextureName()
{
    //last object?
    if(!getLastObject()) return "";
    //get component
    auto sprite=getLastObject()->getComponent<Easy2D::Sprite>();
    //exist sprite component ?
    if(!sprite) return "";
    //exist texture?
    if(!sprite->getTexture()) return "";
    //texture name
    return QString::fromUtf8(sprite->getTexture()->getName());
}
QString ComponentSprite::selectTexture()
{
    return resources(getTextureName(),"textures");
}
void ComponentSprite::applayChange()
{
    applay=true;
}

