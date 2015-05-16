#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets/QMainWindow>
#include <Easy2DInit.h>
#include <QApplication>
#include <QGLContext>
#include <ResourcesGroupTabs.h>
#include <ComponentsTabs.h>
#include <TreeScene.h>

////////////////////////////////
const int RESTART_APP_CODE = 1000;
////////////////////////////////
class AboutDialog;
class AddObjectDialog;
class RenameObjectDialog;
class AddResourceDialog;
class WidgetScene;
////////////////////////////////
namespace Ui
{
    class Editor;
}
////////////////////////////////
/// \brief The Editor class
class Editor : public QMainWindow
{
	Q_OBJECT

public:

    enum State
    {
        EDITOR_MODE,
        PLAY_MODE
    };

    Editor(const QString& resourcePath,
           const QString& configFile,
		   QWidget *parent = 0);
	~Editor();

	//attach treeView scene events
	void attachTreeViewSceneEvents();

    //attach menu
    void attachMenu();

	//attach widget scene
	void attachWidgetScene();

	//attach menuBar Events
	void attachMenuBarEvents();

	//attach toolBox/Resources
	void attachToolBoxResourceEvents();

    //attach components Events
    void attachComponentsEvents();

    //attach globals Events
    void attachGlobalsEvent();

    //state
    State getState()
    {
        return state;
    }

    //global context
    static QGLContext* context()
    {
        return staticGLContext;
    }

	//set target
    void setTarget(Easy2D::Object* obj, bool force=false);

    //select target
    bool selectObject(Easy2D::Object* obj);

    //engine
    WidgetScene* getWidgetScene();

    //lock engine update
    void lock();

    //unlock engine update
    void unlock();

    //current resources group
    Easy2D::ResourcesGroup* getResourcesGroup()
    {
        return resources;
    }

    //open and select a resource
    QString selectResource(const QString &defaultrs,
                           const QString &name);
    QString selectResource(const QString &defaultrs,
                           const QString &name,
                           const QStringList& filter);


    //virtual close callback
    virtual void closeEvent(QCloseEvent* event);

public slots:

    //reset event
    void resetEvent();
    //new event
    void newEvent();
    //open event
    void openEvent();
    //save event
    void saveEvent();
    //close event
    void closeEvent();

private:
	//the opengl context
	static QGLContext* staticGLContext;
    //save ui
    Ui::Editor* ui;
    //save state
    State state;
    //configure file
    QString configFile;
    //selected object
    Easy2D::Object* target;
	//resources
	Easy2D::ResourcesGroup* resources;
	ResourcesGroupTabs* resourcesTabs;
	//models
	TreeScene* treeModelScene;
	//editor manager
	ComponentsTabs* componentsTabs;
	//dialogs
	AboutDialog* aboutDialog;
	AddObjectDialog* addObjectDialog;
	RenameObjectDialog* renameObjectDialog;
	AddResourceDialog* addResourceDialog;
    //utilities
    void openProject(const QString& project);
    void setState(State newState);
    //update globals
    void updateUIGLobals();
    void setGlobalFromTable(const Easy2D::Table& table);
    //save map state
    struct SaveState
    {
        Easy2D::Table metadata;
        Easy2D::Table map;
        Easy2D::Vec2  campos;
        Easy2D::Vec2  camscale;
        Easy2D::Angle camrot;
        float         camzoom;
    }saveState;
};

#endif // EDITOR_H
