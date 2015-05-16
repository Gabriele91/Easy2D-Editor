#include <ComponentsManager.h>
#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QObject>
#include <Editor.h>
#include <WidgetScene.h>
#include <QApplication.h>
#include <SelectStringDialog.h>
#include <Easy2D.h>

#include <ComponentsInterface.h>
#include <transform/PluginTransform.h>
#include <parallax/PluginParallax.h>
#include <sprite/PluginSprite.h>
#include <animatedSprite/PluginAnimatedSprite.h>
#include <emitter/PluginEmitter.h>
#include <speaker/PluginSpeaker.h>
#include <body/PluginBody.h>

void ComponentsManager::insert(ComponentsInterface* component)
{
    component->editor=this->editor;
    component->onCreate();
    components.insert(component->getName(),component);
}

ComponentsManager::ComponentsManager(Editor* editor)
:editor(editor)
{
    insert(new ComponentTransform());
    insert(new ComponentParallax());
    insert(new ComponentSprite());
    insert(new ComponentEmitter());
    insert(new ComponentBody());
    insert(new ComponentAnimatedSprite());
    insert(new ComponentSpeaker());
}
ComponentsManager::~ComponentsManager()
{
    for(auto component:components)
    {
        component->onDestoy();
        delete component;
    }
}

void ComponentsManager::onRun(Easy2D::Object* obj,float dt)
{
    //update
    for(auto component:components)
        component->run(obj,dt);
}
void ComponentsManager::onEnterPlayMode()
{
    //all components
    for(auto component:components)
        component->onEnterPlayMode();
}
void ComponentsManager::onDrawGui()
{
    for(auto component:components)
        component->onDrawGui();
}
void ComponentsManager::onExitPlayMode()
{
    //all components
    for(auto component:components)
        component->onExitPlayMode();
}

void ComponentsManager::onMouseDoubleClickEvent(QMouseEvent *event)
{
    //all components
    for(auto component:components)
        component->onMouseDoubleClickEvent(event);
}
void ComponentsManager::onMousePressEvent(QMouseEvent *event)
{
    //all components
    for(auto component:components)
        component->onMousePressEvent(event);
}
void ComponentsManager::onMouseMoveEvent(QMouseEvent *event)
{
    //all components
    for(auto component:components)
        component->onMouseMoveEvent(event);
}
void ComponentsManager::onMouseReleaseEvent(QMouseEvent *event)
{
    //all components
    for(auto component:components)
        component->onMouseReleaseEvent(event);
}
void ComponentsManager::onWheelEvent(QWheelEvent* event)
{
    //all components
    for(auto component:components)
        component->onWheelEvent(event);
}
void ComponentsManager::resetLastObjectInComponents()
{
    //all components
    for(auto component:components)
        component->lastObjectToNullptr();
}
QStringList ComponentsManager::componentsList() const
{
    //output
    QStringList slist;
    //loop
    for (CCIterator
         it  = components.begin();
         it != components.end();
         ++it)
    {
        slist << it.key();
    }
    //return
    return slist;
}

