
#ifndef COMPONENTSINTERFACE_H
#define COMPONENTSINTERFACE_H

#include <QWidget>
#include <Easy2D.h>
#include <functional>
#include <QGLContext>
#include <QtCore/QtGlobal>
//class declaretion
class Editor;
class WidgetScene;
class SceneComponentCallBack;
class ComponentsInterface;
class ComponentsManager;

//interface plugin
class ComponentsInterface
{

    friend class ComponentsManager;

    //ptr object
    Easy2D::Object* lastObject{ nullptr };
    Editor* editor{ nullptr };
    //editor methos
    DFUNCTION<Easy2D::ResourcesGroup*(void)> getResourcesGroup{ nullptr };
    DFUNCTION<QString (const QString&,const QString&)> selectRerouce;
    DFUNCTION<QString (const QString&,const QString&,const QStringList&)> selectRerouceFilter;
    DFUNCTION<void(void)>  lockEditor;
    DFUNCTION<void(void)>  unlockEditor;
    DFUNCTION<QGLContext*(void)>  contextEditor;
    //run method
    void run(Easy2D::Object* obj,float dt)
    {
        if(lastObject!=obj)
        {
            onChange(lastObject,obj);
            lastObject=obj;
        }
        onRun(obj,dt);
    }
    //to nullptr
    void lastObjectToNullptr()
    {
        onChange(lastObject,nullptr);
        lastObject=nullptr;
    }

public:

    //component info
    virtual QString getName()=0;
    virtual QWidget* getWidget()=0;
    virtual QWidget* buildGUI(QWidget *obj)=0;
    virtual QWidget* buildGUI(){ return this->buildGUI(nullptr); };
    //update
    virtual void onCreate()=0;
    virtual void onRun(Easy2D::Object* obj,float dt)=0;
    virtual void onDrawGui(){};
    virtual void onDestoy()=0;
    //change
    virtual void onChange(Easy2D::Object* oldObj,Easy2D::Object* newObj){}
    virtual void onEnterPlayMode(){}
    virtual void onExitPlayMode(){}
    //events
    virtual void onMouseDoubleClickEvent(QMouseEvent *event){}
    virtual void onMousePressEvent(QMouseEvent *event){}
    virtual void onMouseMoveEvent(QMouseEvent *event){}
    virtual void onMouseReleaseEvent(QMouseEvent *event){}
    virtual void onWheelEvent(QWheelEvent* event){}
    //get last object
    virtual Easy2D::Object* getLastObject();
    virtual Editor* getEditor();
    virtual WidgetScene* getScene();
    //ops
    virtual void lock();
    virtual void unlock();
    virtual QGLContext* context();
    //dialogs
    QString openDialogStringList(const QStringList& list,
                                 const QString& sdefault);
    //resources
    virtual QString resources(const QString& defaults,const QString& name);
    virtual QString resources(const QString& defaults,const QString& name,const QStringList& filters);
    virtual Easy2D::ResourcesGroup* resources();
};



#endif // COMPONENTSINTERFACE_H
