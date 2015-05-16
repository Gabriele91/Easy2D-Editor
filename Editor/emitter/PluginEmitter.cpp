#include <PluginEmitter.h>

QString ComponentEmitter::getName()
{
   return "Emitter";
}
QWidget*  ComponentEmitter::getWidget()
{
   return uiemitter;
}
QWidget*  ComponentEmitter::buildGUI(QWidget *obj)
{
    if(uiemitter) delete uiemitter;
    //init ui
    uiemitter=new UIEmitter(this);
    initUi=true;
    applay=false;
    applayGlobals=false;
    return uiemitter;
}

Easy2D::Emitter* ComponentEmitter::getEmitter()
{
    if(!getLastObject()) return nullptr;
    return getLastObject()->getComponent<Easy2D::Emitter>();
}

void ComponentEmitter::onCreate()
{
    uiemitter=nullptr;
    initUi=true;
    applay=false;
    applayGlobals=false;
}
void ComponentEmitter::onRun(Easy2D::Object* obj,float dt)
{
    //ui?
    if(!uiemitter) return;
    //get component
    auto emitter=getEmitter();
    //else return
    if(!emitter) return;
    //update controll ui
    uiemitter->updateControllUI(*emitter);
    //set ui
    if(initUi)
    {
        uiemitter->updateUI(*emitter);
        //update
        initUi=false;
    }
    //update component
    if(applay)
    {
        uiemitter->updateComponent(*emitter);
        applay=false;
    }
    if(applayGlobals)
    {
        uiemitter->updateComponentGlobals(*emitter);
        applayGlobals=false;
    }
    //update ui

}
void ComponentEmitter::onDestoy()
{
    if(uiemitter)
        delete uiemitter;
}

void ComponentEmitter::applayGlobalsChange()
{
    applayGlobals=true;
}
void  ComponentEmitter::applayChange()
{
    applay=true;
}

bool  ComponentEmitter::setTexture(const QString& name)
{
    //get component
    auto emitter=getEmitter();
    //exist sprite component ?
    if(!emitter) return false;
    //lock editor
    lock();
    //enable this context
    context()->makeCurrent();
    //load texture
    emitter->setTexture(resources()->load<Easy2D::Texture>(name.toUtf8().data()));
    //unlock editor
    unlock();
    //done
    return true;
}
QString ComponentEmitter::getTextureName()
{

    //get component
    auto emitter=getEmitter();
    //exist sprite component ?
    if(!emitter) return "";
    //exist texture?
    if(!emitter->getTexture()) return "";
    //texture name
    return emitter->getTexture()->getName().c_str();
}
QString ComponentEmitter::selectTexture()
{
    return resources(getTextureName(),"textures");
}

void ComponentEmitter::restart()
{
    //get component
    auto emitter=getEmitter();
    //exist sprite component ?
    if(!emitter) return;
    //restart
    emitter->restart();
}
void ComponentEmitter::play()
{
    //get component
    auto emitter=getEmitter();
    //exist sprite component ?
    if(!emitter) return;
    //restart
    emitter->setPause(false);
}
void ComponentEmitter::pause()
{
    //get component
    auto emitter=getEmitter();
    //exist sprite component ?
    if(!emitter) return;
    //restart
    emitter->setPause(true);
}
