#ifndef _COMPONENTSTABS_H
#define _COMPONENTSTABS_H

#include <QMap>
#include <QToolBox>
#include <Easy2D.h>
#include <QLineEdit>
#include <QHBoxLayout>
#include <ComponentsManager.h>
///////////////////////////////
class Editor;
///////////////////////////////
#if 0
class ComponentsTabs
{

public:

    ComponentsTabs(Editor* editor,
                   QToolBox* parent)
                :parent(parent)
                ,target(nullptr)
                ,cmanager(editor)
    {
        initToolBox();
    }
    ~ComponentsTabs()
    {
    }
    //set target
    void setTarget(Easy2D::Object* argTarget)
    {
        if(target==argTarget) return;
        target=argTarget;
        //hide components
        while(parent->count())
        {
            parent->widget(0)->hide();
            parent->removeItem(0);
        }
        //repaint
        parent->repaint();
    }

    //get compoenents manager
    ComponentsManager& componentsManager()
    {
        return cmanager;
    }

private:

    QToolBox* parent;
    Easy2D::Object* target;

    void initToolBox()
    {
        //add fake component
        //(added in ui)
        //newPage("None",new QWidget());
        //transform
        auto tranform=cmanager.components.find("Transform");
        newPage("Transform",(*tranform)->getWidget());
        //all components
        for(auto component:cmanager.components)
        {
            if(component->getName()!="Transform")
            {
                newPage(component->getName(),
                        component->getWidget());
            }
        }
    }

    void newPage(const QString& name,QWidget* page)
    {
        QString objname="page"+name;
        page->setObjectName(objname);
        page->setParent(parent);
        parent->addItem(page, name);
    }

    QWidget* getPage(const QString& name)
    {
        return nullptr;
    }

    //components from dlls
    ComponentsManager cmanager;

};
#else
class ComponentsTabs 
{

public:

    ComponentsTabs(Editor* editor,QToolBox* parent);
    ~ComponentsTabs();

    //set target
    void setTarget(Easy2D::Object* argTarget);

    //get compoenents manager
    ComponentsManager& componentsManager();

private:

    QToolBox* parent;
	Easy2D::Object* target;
	QMap<QString,QWidget*> mapPages;
    //clear map
    void clearMap();
    //init
    void initToolBox();
    //add a new page
    void newPage(const QString& name);
    //get a page
    QWidget* getPage(const QString& name);

    //components from dlls
    ComponentsManager cmanager;

};
#endif

#endif
