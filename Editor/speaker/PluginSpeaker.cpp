#include <PluginSpeaker.h>
#include <WidgetScene.h>
#include <QMouseEvent>

QString ComponentSpeaker::getName()
{
   return "Speaker";
}
QWidget* ComponentSpeaker::getWidget()
{
   return uispeaker;
}
Easy2D::Speaker* ComponentSpeaker::getSpeaker()
{
    if(getLastObject())
        return getLastObject()->getComponent<Easy2D::Speaker>();
    return nullptr;
}
QWidget* ComponentSpeaker::buildGUI(QWidget *obj)
{
    if(uispeaker) delete uispeaker;
    uispeaker=new UISpeaker(this);
    applay=false;
    circleSelected=false;
    return uispeaker;
}
void ComponentSpeaker::onCreate()
{
    uispeaker=nullptr;
    applay=false;
    circleSelected=false;
}
void ComponentSpeaker::onRun(Easy2D::Object* obj,float dt)
{
    //not ui?
    if(!uispeaker) return;
    auto speaker=getSpeaker();
    if(!speaker) return;

    if(applay)
    {
        speaker->setRadius(uispeaker->getRadius());
        speaker->volume(uispeaker->getVolume());
        speaker->set2D(uispeaker->get2D());
        speaker->setPlayOnAttach(uispeaker->getPlayOnAttach());
        if(uispeaker->getLoop()) speaker->enableLoop();
        else                     speaker->disableLoop();

        applay=false;
    }

    if(speaker->getRadius()!=uispeaker->getRadius())
        uispeaker->setRadius(speaker->getRadius());

    if(speaker->volume()!=uispeaker->getVolume())
        uispeaker->setVolume(speaker->volume());

    if(speaker->getPlayOnAttach()!=uispeaker->getPlayOnAttach())
        uispeaker->setPlayOnAttach(speaker->getPlayOnAttach());

    if(speaker->is2D()!=uispeaker->get2D())
        uispeaker->set2D(speaker->is2D());

    if(speaker->isLoop()!=uispeaker->getLoop())
        uispeaker->setLoop(speaker->isLoop());

    auto sound=speaker->getSound();
    if(sound)
    {
        QString sName(sound->getName().c_str());
        if(sName != uispeaker->getSound())
        {
            uispeaker->setSound(sName);
        }
    }
}
void ComponentSpeaker::onDrawGui()
{
    auto speaker=getSpeaker();
    if(!speaker) return;
    if(!speaker->is2D()) return;

    auto pos=getLastObject()->getPosition(true);
    float radius=speaker->getRadius();

    getScene()->drawCircle(pos,
                           radius,
                           Easy2D::Color(255,0,0,255));

}
QString ComponentSpeaker::selectSound()
{
    auto speaker=getSpeaker();
    if(!speaker) return "";

    auto sound=speaker->getSound();
    QString defaultResource;
    if(sound) defaultResource=sound->getName().c_str();

    QString name=resources(defaultResource,"sounds");

    if(name!="")
    {
        speaker->setSound(resources()->load<Easy2D::Sound>(name.toUtf8().data()));
        return name;
    }
    return defaultResource;
}
//input managment
void ComponentSpeaker::onMousePressEvent(QMouseEvent* event)
{
    auto speaker=getSpeaker();
    if(speaker && event->button()==Qt::LeftButton)
    {
        //get pick pos
        Easy2D::Vec2 mousePos(event->localPos().x(),event->localPos().y());
        Easy2D::Vec2 pikCam=getScene()->getCamera().getWMouse(mousePos);
        Easy2D::Vec2 point=getLastObject()->getPosition(true);

        float radius=speaker->getRadius();
        float rmin=radius-5.0f*getScene()->getCamera().getZoom();
        float rmax=radius+5.0f*getScene()->getCamera().getZoom();

        circleSelected=
                point.distancePow2(pikCam) > rmin*rmin &&
                point.distancePow2(pikCam) < rmax*rmax ;
    }
}
void ComponentSpeaker::onMouseMoveEvent(QMouseEvent* event)
{
    auto speaker=getSpeaker();
    if(speaker && circleSelected)
    {
        Easy2D::Vec2 mousePos(event->localPos().x(),event->localPos().y());
        Easy2D::Vec2 pikCam=getScene()->getCamera().getWMouse(mousePos);
        Easy2D::Vec2 point=getLastObject()->getPosition(true);
        speaker->setRadius((pikCam-point).length());
    }
}
void ComponentSpeaker::onMouseReleaseEvent(QMouseEvent* event)
{
    if(circleSelected && event->button()==Qt::LeftButton)
    {
        circleSelected=false;
    }
}

void ComponentSpeaker::onDestoy()
{
    if(uispeaker)
        delete uispeaker;
}
void ComponentSpeaker::doSpeaker()
{
    applay=true;
}
