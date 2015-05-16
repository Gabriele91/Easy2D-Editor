#include <ComponentsTabs.h>



ComponentsTabs::ComponentsTabs(Editor* editor,
                               QToolBox* parent)
                            :parent(parent)
                            ,target(nullptr)
                            ,cmanager(editor)
{
    initToolBox();
}
ComponentsTabs::~ComponentsTabs()
{
    clearMap();
}
//set target
void ComponentsTabs::setTarget(Easy2D::Object* argTarget)
{
    if(target==argTarget) return;
    target=argTarget;
    //add tool box
    initToolBox();
    //repaint
    parent->repaint();
}

//get compoenents manager
ComponentsManager& ComponentsTabs::componentsManager()
{
    return cmanager;
}

//clear map
void ComponentsTabs::clearMap()
{
    //delete all elements
    for(auto wd:mapPages)
    {
        //delete wd;
    }
    //clear
    mapPages.clear();
}

//init
void ComponentsTabs::initToolBox()
{
    //hide
    parent->blockSignals(true);
    //remove all items
    while(parent->count())
    {
        parent->removeItem(0);
    }
    //clear map
    clearMap();
    //add fake component
    if(target)
    {
        //Transform page
        newPage("Transform");
        auto comps=target->getComponents();
        for(auto cmp:comps)
        {
            newPage(cmp.second->getComponentName());
        }
    }
    else
    {
        //void page
        newPage("None");
    }
    //reshow
    parent->blockSignals(false);
}
//add a new page
void ComponentsTabs::newPage(const QString& name)
{
    QWidget* page=nullptr;
    //set widget from component manager
    auto idcmp=cmanager.components.find(name);
    if(idcmp!=cmanager.components.end())
    {
       page=(*idcmp)->buildGUI();
    }
    else
    {
       page = new QWidget();
       page->setGeometry(QRect(0, 0, 200, 222));
    }
    QString objname="page"+name;
    page->setObjectName(objname);
    page->setParent(parent);
    page->setEnabled(true);
    parent->addItem(page, name);
    //save into map
    mapPages[name]=page;
}
//get a page
QWidget* ComponentsTabs::getPage(const QString& name)
{
    auto it=mapPages.find(name);
    if(it!=mapPages.end())
    {
        return *it;
    }
    return nullptr;
}
