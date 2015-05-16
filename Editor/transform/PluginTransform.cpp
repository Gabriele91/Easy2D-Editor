#include <PluginTransform.h>

QString ComponentTransform::getName()
{
   return "Transform";
}
QWidget* ComponentTransform::getWidget()
{
   return uitranform;
}
QWidget* ComponentTransform::buildGUI(QWidget *obj)
{
    if(uitranform) delete uitranform;
    uitranform=new UITransform(this);
    uitranform->setPosition(Easy2D::Vec2::ZERO);
    uitranform->setScale(Easy2D::Vec2::ONE);
    uitranform->setRotation(Easy2D::Angle());
    uitranform->setOrder(0);
    uitranform->setParentMode(Easy2D::Object::DISABLE_PARENT);
    applayTransform=false;
    return uitranform;
}
void ComponentTransform::onCreate()
{
    uitranform=nullptr;
}
void ComponentTransform::onRun(Easy2D::Object* obj,float dt)
{
    //not ui?
    if(!uitranform) return;

    if(applayTransform)
    {
        obj->setPosition(uitranform->getPosition());
        obj->setRotation(uitranform->getRotation());
        obj->setScale(uitranform->getScale());
        obj->setZ(uitranform->getOrder());
        obj->setParentMode(uitranform->getParentMode());
        applayTransform=false;
    }

    if(obj->getPosition()!=uitranform->getPosition())
        uitranform->setPosition(obj->getPosition());

    if(obj->getRotation()!=uitranform->getRotation())
        uitranform->setRotation(obj->getRotation());

    if(obj->getScale()!=uitranform->getScale())
        uitranform->setScale(obj->getScale());

    if(obj->getZ()!=uitranform->getOrder())
        uitranform->setOrder(obj->getZ());

    if(obj->getParentMode()!=uitranform->getParentMode())
        uitranform->setParentMode(obj->getParentMode());
}
void ComponentTransform::onDestoy()
{
    if(uitranform)
        delete uitranform;
}
void ComponentTransform::doTransform()
{
    applayTransform=true;
}
