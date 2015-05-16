#include <PluginParallax.h>


QString ComponentParallax::getName()
{
   return "Parallax";
}
QWidget* ComponentParallax::getWidget()
{
   return uiparallax;
}
QWidget* ComponentParallax::buildGUI(QWidget *obj)
{
    if(uiparallax) delete uiparallax;
    //init ui
    uiparallax=new UIParallax(this);
    uiparallax->setParallax(Easy2D::Vec2::ONE);
    applay=false;
    return uiparallax;
}

void ComponentParallax::onCreate()
{
    uiparallax=nullptr;
}
void ComponentParallax::onRun(Easy2D::Object* obj,float dt)
{
    //ui?
    if(!uiparallax) return;
    //get component
    auto parallax=obj->getComponent<Easy2D::Parallax>();
    //else return
    if(!parallax) return;

    //update component
    if(applay)
    {
        parallax->setScale(uiparallax->getParallax());
        applay=false;
    }

    //update parallax ui
    if(parallax->getScale()!=uiparallax->getParallax())
        uiparallax->setParallax(parallax->getScale());

}
void ComponentParallax::onDestoy()
{
    if(uiparallax)
        delete uiparallax;
}
void ComponentParallax::applayChange()
{
    applay=true;
}
