#ifndef COMPONENTSMANAGER_H
#define COMPONENTSMANAGER_H

#include <QMap>
#include <QStringList>
#include <ComponentsInterface.h>
///////////////////////
class Editor;
class ComponentLib;
class ComponentsTabs;
///////////////////////
class ComponentsManager
{
    friend class ComponentsTabs;
    typedef QMap<QString, ComponentsInterface*>::iterator CIterator;
    typedef QMap<QString, ComponentsInterface*>::const_iterator CCIterator;
    QMap<QString, ComponentsInterface*> components;
    void insert(ComponentsInterface*);
    Editor* editor{ nullptr };

public:
    ComponentsManager(Editor *editor);
    ~ComponentsManager();
    void onRun(Easy2D::Object* obj,float dt);
    void onDrawGui();
    void onEnterPlayMode();
    void onExitPlayMode();
    //events
    void onMouseDoubleClickEvent(QMouseEvent *event);
    void onMousePressEvent(QMouseEvent *event);
    void onMouseMoveEvent(QMouseEvent *event);
    void onMouseReleaseEvent(QMouseEvent *event);
    void onWheelEvent(QWheelEvent* event);
    //reset last object in components
    void resetLastObjectInComponents();
    //list components
    QStringList componentsList() const;
};

#endif // COMPONENTSMANAGER_H
