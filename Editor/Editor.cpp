#include "Editor.h"
#include "ui_Editor.h"
#include <QIcon>
#include <QProcess>
#include <QFileDialog>
#include <AboutDialog.h>
#include <AddObjectDialog.h>
#include <RenameObjectDialog.h>
#include <AddResourceDialog.h>
#include <ManagerResourcesList.h>
#include <ScelectResourceDialog.h>
#include <CreateSceneDialog.h>
#include <SelectStringDialog.h>
#include <ProjectUtility.h>


//global values
QGLContext* Editor::staticGLContext=nullptr;

Editor::Editor(const QString& resourcePath,
               const QString &configFile,
               QWidget *parent)
	: QMainWindow(parent)
    , ui(new Ui::Editor())
    , configFile(configFile)
    , target(nullptr)
{
	//init context
	QGLFormat qglFormat;
    qglFormat.setProfile(QGLFormat::CoreProfile);
    qglFormat.setSampleBuffers(true);
    if(!staticGLContext)
        staticGLContext=new QGLContext(qglFormat);

    //init ui
    ui->setupUi(this);

    //globals
    //set validetor
    ui->leGravity_x->setValidator(new QDoubleValidator);
    ui->leGravity_y->setValidator(new QDoubleValidator);
    ui->leMetersInPixel->setValidator(new QDoubleValidator);

    //init state EDITOR_MODE
    ui->pushPlay->setEnabled(true);
    ui->pushEditor->setEnabled(false);
    state=EDITOR_MODE;

	//init OpenGL
	context()->makeCurrent();
	Easy2DInit::initOpenGL();
    Easy2DInit::setQTWidget(ui->EngineWindow);

	//init reources
	Easy2D::String rsgPath(resourcePath.toUtf8().data());
	resources=new Easy2D::ResourcesGroup("main",rsgPath);

	//setup tree scene
	treeModelScene=new TreeScene(this);
    ui->treeScene->setWindowTitle(QObject::tr("Scene"));
    ui->treeScene->setModel(treeModelScene);
    ui->treeScene->setSelectionMode(QAbstractItemView::SingleSelection);
    treeModelScene->setTreeView(ui->treeScene);

    //setup page components
    componentsTabs=new ComponentsTabs(this,ui->tabComponents);

	//load elements
	treeModelScene->setReourceGroup(*resources);

	//init engine
    //ui->EngineWindow->setMouseTracking(true);
    ui->EngineWindow->setSceneNode(treeModelScene->getSceneNode());
    ui->EngineWindow->setEditor(this);
    //set globals
    setGlobalFromTable(*resources->load<Easy2D::Table>("scene"));
    //this->setMouseTracking(true);

	//setup dialogs
	aboutDialog=new AboutDialog(this);
	addObjectDialog=new AddObjectDialog(treeModelScene,this);
    renameObjectDialog=new RenameObjectDialog(this);
	addResourceDialog=new AddResourceDialog(this);

    //resources ui
	resourcesTabs=
    new ResourcesGroupTabs(ui->listTextures,
                           ui->listMeshes,
                           ui->listFrameSets,
                           ui->listTables,
                           ui->listFonts,
                           ui->listSounds,
                           ui->listScripts,
                           ui->listShaders,
						   resources->getTable());
	resourcesTabs->setNodeTarget(treeModelScene->getSceneNode());


	//events
    //[menu events]
    attachMenu();
	//[tree view scene events]
	attachTreeViewSceneEvents();
	//[widget scene events]
	attachWidgetScene();
	//[menu bar events]
	attachMenuBarEvents();
	//[tool box resources events]
	attachToolBoxResourceEvents();
    //[components buttoms events]
    attachComponentsEvents();
    //[globals events]
    attachGlobalsEvent();
    //update callback
    ui->EngineWindow->setOnRun
   ([this](float dt)
    {
        if(target)
            componentsTabs->componentsManager().onRun(target,dt);
    });
    ui->EngineWindow->setOnEnterPlayMode
   ([this]()
    {
        componentsTabs->componentsManager().onEnterPlayMode();
    });
    ui->EngineWindow->setOnExitPlayMode
    ([this]()
     {
         componentsTabs->componentsManager().onExitPlayMode();
     });
    ui->EngineWindow->setOnDrawGui
    ([this]()
     {
        if(target)
            componentsTabs->componentsManager().onDrawGui();
     });
    //input callbacks
    ui->EngineWindow->setOnMousePress
    ([this](QMouseEvent* event)
    {
        componentsTabs->componentsManager().onMousePressEvent(event);
    });
    ui->EngineWindow->setOnMouseMove
    ([this](QMouseEvent* event)
    {
        componentsTabs->componentsManager().onMouseMoveEvent(event);
    });
    ui->EngineWindow->setOnMouseRelease
    ([this](QMouseEvent* event)
    {
        componentsTabs->componentsManager().onMouseReleaseEvent(event);
    });
    ui->EngineWindow->setOnDoubleClick
    ([this](QMouseEvent* event)
    {
        componentsTabs->componentsManager().onMouseDoubleClickEvent(event);
    });
    ui->EngineWindow->setOnMouseWell
    ([this](QWheelEvent* event)
    {
        componentsTabs->componentsManager().onWheelEvent(event);
    });

}
//set target
void Editor::setTarget(Easy2D::Object* obj,bool force)
{
    ui->EngineWindow->lock();

        if(force) treeModelScene->selectObject(obj);
        ui->EngineWindow->setSelectObject(obj);
        componentsTabs->setTarget(obj);
        target=obj;
        //reset
        componentsTabs->
        componentsManager().
        resetLastObjectInComponents();
        //
    ui->EngineWindow->unlock();
}

//select object
bool Editor::selectObject(Easy2D::Object* obj)
{
    if(treeModelScene->selectObject(obj))
    {
        setTarget(obj);
        return true;
    }
    return false;
}

//engine
WidgetScene* Editor::getWidgetScene()
{
    return ui->EngineWindow;
}

//lock engine update
void Editor::lock()
{
    ui->EngineWindow->lock();
}

//unlock engine update
void Editor::unlock()
{
    ui->EngineWindow->unlock();
}

//update ui globals
void Editor::updateUIGLobals()
{
    ui->leGravity_x->setText(QString::number(ui->EngineWindow->getGravity().x));
    ui->leGravity_y->setText(QString::number(ui->EngineWindow->getGravity().y));
    ui->leMetersInPixel->setText(QString::number(ui->EngineWindow->getMetersInPixel()));
    ui->sbAmbientColor_r->setValue(ui->EngineWindow->getAmbientLight().r);
    ui->sbAmbientColor_g->setValue(ui->EngineWindow->getAmbientLight().g);
    ui->sbAmbientColor_b->setValue(ui->EngineWindow->getAmbientLight().b);
    ui->sbAmbientColor_a->setValue(ui->EngineWindow->getAmbientLight().a);
    ui->sbClearColor_r->setValue(ui->EngineWindow->getClearColor().r);
    ui->sbClearColor_g->setValue(ui->EngineWindow->getClearColor().g);
    ui->sbClearColor_b->setValue(ui->EngineWindow->getClearColor().b);
    ui->sbClearColor_a->setValue(ui->EngineWindow->getClearColor().a);
}
//set global values from table
void Editor::setGlobalFromTable(const Easy2D::Table& scene)
{
    //tmp widget engine
    auto* engine = ui->EngineWindow;

    engine->setGravity(scene.getVector2D("gravity",engine->getGravity()));
    engine->setMetersInPixel(scene.getFloat("metersInPixel",engine->getMetersInPixel()));

    //tmp color
    Easy2D::Color color;

    color = scene.getColor("ambientLight",engine->getAmbientLight());

    engine->setAmbientLight(color);

    color = scene.getColor("clearColor",engine->getClearColor());

    engine->setClearColor(color);

    updateUIGLobals();
}

//close editor
//virtual close callback
void Editor::closeEvent(QCloseEvent* event)
{
    lock();
}

//select resource dialog
QString Editor::selectResource(const QString& defaultrs,
                               const QString& name)
{
    ScelectResourceDialog dialog
            (
             &resources->getTable().getTable(name.toUtf8().data()),
             defaultrs,
             this
            );
    return dialog.doExec();
}
QString Editor::selectResource(const QString& defaultrs,
                               const QString& name,
                               const QStringList &filter)
{
    ScelectResourceDialog dialog
            (
             filter,
             &resources->getTable().getTable(name.toUtf8().data()),
             defaultrs,
             this
            );
    return dialog.doExec();
}
//rutine: open a project
void Editor::openProject(const QString& file)
{
    if(QFile::exists(file))
    {
        //save path
        QString resourcePath=file;
        Easy2D::String rsgPath(resourcePath.toUtf8().data());
        //stop engine
        ui->EngineWindow->lock();
        //delete old scene object
        ui->EngineWindow->eraseObject(treeModelScene->getSceneNode());
        //delete resource
        delete resources;
        //new resource
        resources=new Easy2D::ResourcesGroup("main",rsgPath);
        //set new resource
        treeModelScene->setReourceGroup(*resources);
        //reset target
        setTarget(nullptr);
        //reset resources tab
        resourcesTabs->changeResources(resources->getTable());
        resourcesTabs->setNodeTarget(treeModelScene->getSceneNode());
        //reset engine
        ui->EngineWindow->setSceneNode(treeModelScene->getSceneNode());
        //update global settings ui
        setGlobalFromTable(*resources->load<Easy2D::Table>("scene"));
        //start engine
        ui->EngineWindow->unlock();
        //change default open
        Easy2D::Table config;
        Easy2DInit::readTable(configFile,config);
        config.set("path",rsgPath);
        Easy2DInit::saveTable(configFile,config);
    }
}

//rutine: change state
void Editor::setState(State newState)
{
    switch (newState)
    {
    case EDITOR_MODE:
        /////////////////////////////////////
        // SETUP UI
        //disable
        ui->pushEditor->setEnabled(false);
        //enable
        ui->pushPlay->setEnabled(true);
        ui->tabResources->setEnabled(true);
        ui->menuFile->setEnabled(true);
        //save mode
        state=EDITOR_MODE;
        /////////////////////////////////////
        //LOAD STATE
        //lock
        ui->EngineWindow->lock();
            //set open gl context
            context()->makeCurrent();
            //delete old scene
            ui->EngineWindow->eraseObject(treeModelScene->getSceneNode());
            //reset world
            ui->EngineWindow->resetWorld();
            //reload scene
            treeModelScene->changeSceneTable(saveState.map);
            ui->EngineWindow->metaDataDeserialize(saveState.metadata);
            //change obj refs
            resourcesTabs->setNodeTarget(treeModelScene->getSceneNode());
            ui->EngineWindow->setSceneNode(treeModelScene->getSceneNode());
            //reset camera
            ui->EngineWindow->getCamera().setPosition(saveState.campos,true);
            ui->EngineWindow->getCamera().setRotation(saveState.camrot,true);
            ui->EngineWindow->getCamera().setScale(saveState.camscale,true);
            ui->EngineWindow->getCamera().setZoom(saveState.camzoom);
            //change selection
            setTarget(nullptr);
        //unlock
        ui->EngineWindow->unlock();
        /////////////////////////////////////
        break;
    case PLAY_MODE:
        /////////////////////////////////////
        //SETUP UI
        //disable
        ui->pushPlay->setEnabled(false);
        ui->tabResources->setEnabled(false);
        ui->menuFile->setEnabled(false);
        //enable
        ui->pushEditor->setEnabled(true);
        //save mode
        state=PLAY_MODE;
        /////////////////////////////////////
        //SAVE STATE
        //lock
        ui->EngineWindow->lock();
            //save
            ui->EngineWindow->metaDataSerialize(saveState.metadata);
            saveState.map=Easy2D::Table(&resources->getManager<Easy2D::Table>());
            treeModelScene->serialize(saveState.map.createTable("Objects"));
            saveState.campos=ui->EngineWindow->getCamera().getPosition(true);
            saveState.camrot=ui->EngineWindow->getCamera().getRotation(true);
            saveState.camscale=ui->EngineWindow->getCamera().getScale(true);
            saveState.camzoom=ui->EngineWindow->getCamera().getZoom();
            //change selection
            setTarget(nullptr);
        //unlock
        ui->EngineWindow->unlock();
        /////////////////////////////////////
        break;
    default: break;
    }
}

//reset event
void Editor::resetEvent()
{
    //whant restart?
    QPixmap e2dicon(QString::fromUtf8(":/E2DLogo32.png"));
    QMessageBox msgBox;
    msgBox.setWindowTitle("Save");
    msgBox.setText("You are sure?");
    msgBox.addButton(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIconPixmap(e2dicon);
    auto reply=msgBox.exec();
    //
    if (reply == QMessageBox::Yes)
    {
        //change default open
        Easy2D::Table config;
        Easy2DInit::readTable(configFile,config);
        config.set("startUp",0.0f);
        Easy2DInit::saveTable(configFile,config);
        //close
        close();
        QApplication::exit(RESTART_APP_CODE);
    }
}
//new event
void Editor::newEvent()
{
    //none
    CreateSceneDialog dialog;
    dialog.exec("New Scene");
    QDir path(dialog.getLastPath());
    QString projectPath;

    if(!path.exists())
    {
        QMessageBox messageBox(this);
        messageBox.setFixedSize(200,200);
        messageBox.critical(0,"Error","The path doesn't exist");
        messageBox.open();
    }
    else
    {
        if(!path.mkdir(dialog.getLastName()))
        {
            QMessageBox messageBox(this);
            messageBox.setFixedSize(200,200);
            messageBox.critical(0,"Error","Invalid project name");
            messageBox.open();
        }
        else
        {
            if(ProjectUtility::createProject(dialog.getLastPath(),
                                              dialog.getLastName(),
                                              projectPath))
            {
                openProject(projectPath);
            }
            else
            {
                //rejected
                QMessageBox messageBox(this);
                messageBox.setFixedSize(200,200);
                messageBox.critical(0,"Error","Can't create a new project");
                messageBox.exec();
            }
        }
    }

}
//open event
void Editor::openEvent()
{
    //"Easy2D (*.rs.e2d)"
    QString file=QFileDialog::getOpenFileName(this,
                                              "Open File...",
                                              QString(),
                                              "Easy2D (*.rs.e2d)",
                                              0,
                                              QFileDialog::DontUseNativeDialog);
    if(QFile::exists(file))
    {
        openProject(file);
    }
    else
    {
        QMessageBox messageBox(this);
        messageBox.setFixedSize(200,200);
        messageBox.critical(0,"Error","invalid file");
        messageBox.open();
    }
}
//save event
void Editor::saveEvent()
{
   //whant save?
   QPixmap e2dicon(QString::fromUtf8(":/E2DLogo32.png"));
   QMessageBox msgBox;
   msgBox.setWindowTitle("Save");
   msgBox.setText("You are sure?");
   msgBox.addButton(QMessageBox::Yes);
   msgBox.addButton(QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::Yes);
   msgBox.setIconPixmap(e2dicon);
   auto reply=msgBox.exec();
   //
   if (reply == QMessageBox::Yes)
   {
       ui->EngineWindow->lock();
       /* SAVE SCENE */
       //get objects
       Easy2D::Table table;
       table.set("gravity",ui->EngineWindow->getGravity());
       table.set("metersInPixel",ui->EngineWindow->getMetersInPixel());
       table.set("ambientLight",ui->EngineWindow->getAmbientLight().toVec4());
       table.set("clearColor",ui->EngineWindow->getClearColor().toVec4());
       treeModelScene->serialize(table.createTable("Objects"));
       auto scene=table.serialize();
       //open a file
       auto pathTables=resources->getTable()
                                 .getTable("tables")
                                 .getPath()
                                 .getDirectory();
       QFile sfile(QString::fromUtf8(pathTables+"/scene.table.e2d"));
       sfile.open(QIODevice::WriteOnly);
       //write
       sfile.write((const char*)scene,scene.byteSize());
       //close
       sfile.close();
       /* SAVE RESOURCE FILE */
       //serialize
       Easy2D::String strResources("{\n");
       //add tables
       Easy2D::Table& tableRs=resources->getTable();
       //append all table
       for(auto value:tableRs)
       {
           Easy2D::Table& aTable=value.second->get<Easy2D::Table>();
           if(aTable.getPath().getPath()==tableRs.getPath().getPath())
           {
               strResources+=value.first.string()+"=\n"+aTable.serialize()+"\n";
           }
           else
           {
               Easy2DInit::saveTable(QString::fromUtf8(aTable.getPath().getPath()),//dir
                                     aTable);                   //table
               strResources+=value.first.string()+"=<"+aTable
                                                       .getPath()
                                                       .getRelativePathTo(tableRs.getPath())
                                                       .getPath()+">\n";
           }
       }
       //
       strResources+="}";
       //write
       QFile rfile(QString::fromUtf8(resources->getTable().getPath().getPath()));
       rfile.open(QIODevice::WriteOnly);
       //write
       rfile.write(strResources,strResources.byteSize());
       //close
       rfile.close();
       /* END SAVE SCENE */
       ui->EngineWindow->unlock();
   }

}
//close event
void Editor::closeEvent()
{
    this->close();
}

//attach menu events
void Editor::attachMenu()
{
    //new event
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newEvent()));
    //open event
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openEvent()));
    //save event
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveEvent()));
    //reset event
    connect(ui->actionReset,SIGNAL(triggered()),this,SLOT(resetEvent()));
    //exit event
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(closeEvent()));
}

//attach treeView scene events
void Editor::attachTreeViewSceneEvents()
{
	//scene tree events
    ui->treeScene->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//[scene event:scelect]
    connect( ui->treeScene, &QTreeView::clicked,this,
	[this](const QModelIndex& item) {
		if(item.isValid())
		{
			Easy2D::Object* obj=(Easy2D::Object*)(item.internalPointer());
			if(obj)	
				setTarget(obj);
		}
    });
    //[scene event:rename]
    connect(ui->pushRenameObject, &QPushButton::clicked, this,
    [this](bool)
    {
        this->lock();
        renameObjectDialog->exec(treeModelScene->currentObject());
        this->unlock();
    });
    //[scene event:copy]
    connect(ui->pushCopyObject, &QPushButton::clicked, this,
    [this](bool){
        if(target)
        {
            //lock engine
            this->lock();
            //get info object
            Easy2D::Table theObjTable(&resources->getManager<Easy2D::Table>());
            auto parenMode=target->getParentMode();
            target->serialize(theObjTable);
            //search new name
            bool loop=true;
            int i=0;
            Easy2D::String newName;
            while(loop)
            {
                loop=false;
                newName=target->getName()+(i++);
                for(auto child:*target->getParent())
                {
                    if(child->getName()==newName)
                    {
                        loop=true;
                        break;
                    }
                }
            }
            //new object
            auto nobj=new Easy2D::Object;
            nobj->deserialize(theObjTable.getTable(target->getName()));
            nobj->setName(newName);
            //set new name
            //append
            target->getParent()->addChild(nobj,parenMode);
            //update tree
            treeModelScene->reset();
            //new target
            setTarget(nobj,true);
            //lock engine
            this->unlock();
        }
    });
	//[scene event:delete]
    connect(ui->pushDeleteObject, &QPushButton::clicked, this,
	[this](bool){
        //lock engine
        this->lock();
		//if selected
        if(ui->EngineWindow->isInSelectedObject(treeModelScene->getObjectSelected()))
		{
			//unselected
            ui->EngineWindow->setSelectObject(nullptr);
		}
		//unselect from componenets view
		setTarget(nullptr);
		//delete obj
		treeModelScene->deleteObject();
        //lock engine
        this->unlock();
	});

}

//attach widget scene
void Editor::attachWidgetScene()
{
	//event "selected"
    connect(ui->treeScene, &QTreeView::clicked, this, [this]() {
        /* Event in tree items */
        /* this->setTarget(treeModelScene->getObjectSelected()); */
    });
    //event edit
    connect(ui->pushEditor,&QPushButton::clicked, this, [this](){
        setState(EDITOR_MODE);
    });
    //event play
    connect(ui->pushPlay,&QPushButton::clicked, this, [this](){
        setState(PLAY_MODE);
    });
}

//attach menuBar Events
void Editor::attachMenuBarEvents()
{
	//[exit]
    connect(ui->actionExit, &QAction::triggered, this, [this]() {
		close();
	});
	//[Edit:add object]
    connect(ui->actionObject, &QAction::triggered, this, [this]() {
		addObjectDialog->exec();
	});
	//[about]
    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
		aboutDialog->exec();
	});
}

//attach toolBox/Resources
#define RETURN_YES 0
#define RETURN_ALL 1
#define RETURN_NO 2
int deleteAResourceMessage(const QString& name)
{
	QIcon allertIco(":allert_window.png");
	QMessageBox msgBox;
	msgBox.setWindowIcon(allertIco);
	msgBox.setWindowTitle("Warning");
    msgBox.setText("This "+name+" is used by some objects, do you want to delete those objects?");
	msgBox.addButton("Only the objects", QMessageBox::YesRole);
	msgBox.addButton("Objects and their childs", QMessageBox::NoRole);
    msgBox.addButton("No", QMessageBox::RejectRole);
    return msgBox.exec();
}
void Editor::attachToolBoxResourceEvents()
{
	//[resources event:add texture]
    connect(ui->addTexture,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add Texture",
                                   "Images (*.png *.bmp *.tga)",
                                   &resources->getTable().getTable("textures"))==QDialog::Accepted)
		{
			if(addResourceDialog->getLastPath().size())
			{
				QString name=addResourceDialog->getLastName();
				name=name.replace(" ","_");
				if(name.size())
				{
					resourcesTabs->addTexture(name,addResourceDialog->getLastPath());
				}
			}
		}
	});
	//[resources event:delete texture]
    connect(ui->deleteTexture,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listTextures->currentItem();
		if(current)
		{
			QString rsname=current->text();
			bool deleteRs=true;
			bool deleteAll=true;
			if(!resourcesTabs->canDeleteTexture(rsname))
			{
				auto  reply = deleteAResourceMessage("texture");
				deleteRs=(reply!=RETURN_NO);
				deleteAll=(reply==RETURN_ALL);
			}
			//delete rs?
			if(deleteRs)
			{
				//delete resource
				resourcesTabs->deleteTexture(rsname,deleteAll);
				//reset ui
				treeModelScene->reset();
			}
		}
	});
	//[resources event:add mesh]
    connect(ui->addMesh,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add Mesh",
                                   "Meshes (*.mesh.e2d)",
                                   &resources->getTable().getTable("meshes"))==QDialog::Accepted)
		{
			if(addResourceDialog->getLastPath().size())
			{
				QString name=addResourceDialog->getLastName();
				name=name.replace(" ","_");
				if(name.size())
				{
					resourcesTabs->addMesh(name,addResourceDialog->getLastPath());
				}
			}
		}
	});	
	//[resources event:delete mesh]
    connect(ui->deleteMesh,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listMeshes->currentItem();
		if(current)
		{
			QString rsname=current->text();
			bool deleteRs=true;
			bool deleteAll=true;
			if(!resourcesTabs->canDeleteMesh(rsname))
			{
				auto  reply = deleteAResourceMessage("mesh");
				deleteRs=(reply!=RETURN_NO);
				deleteAll=(reply==RETURN_ALL);
			}
			//delete rs?
			if(deleteRs)
			{
				resourcesTabs->deleteMesh(rsname,deleteAll);
				//reset ui
				treeModelScene->reset();
			}
		}
	});
	//[resources event:add frameset]
    connect(ui->addFrameSet,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add FrameSet",
                                   "FrameSets (*.frameset.e2d *.frameSet.e2d)",
                                   &resources->getTable().getTable("frameSets"))==QDialog::Accepted)
		{
			if(addResourceDialog->getLastPath().size())
			{
				QString name=addResourceDialog->getLastName();
				name=name.replace(" ","_");
				if(name.size())
				{
					resourcesTabs->addFrameSet(name,addResourceDialog->getLastPath());
				}
			}
		}
	});
	//[resources event:delete frameSet]
    connect(ui->deleteFrameSet,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listFrameSets->currentItem();
		if(current)
		{
			QString rsname=current->text();
			bool deleteRs=true;
			bool deleteAll=true;
			if(!resourcesTabs->canDeleteFrameSet(rsname))
			{
				auto  reply = deleteAResourceMessage("frameSet");
				deleteRs=(reply!=RETURN_NO);
				deleteAll=(reply==RETURN_ALL);
			}
			//delete rs?
			if(deleteRs)
			{
				resourcesTabs->deleteFrameSet(rsname,deleteAll);
				//reset ui
				treeModelScene->reset();
			}
		}
	});
	//[resources event:add table]
    connect(ui->addTable,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add Table",
                                   "Tables (*.table.e2d)",
                                   &resources->getTable().getTable("tables"))==QDialog::Accepted)
		{
			if(addResourceDialog->getLastPath().size())
			{
				QString name=addResourceDialog->getLastName();
				name=name.replace(" ","_");
				if(name.size())
				{
					resourcesTabs->addTable(name,addResourceDialog->getLastPath());
				}
			}
		}
	});
	//[resources event:delete table]
    connect(ui->deleteTable,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listTables->currentItem();
		if(current)
		{
			QString rsname=current->text();
			bool deleteRs=true;
			bool deleteAll=true;
			if(!resourcesTabs->canDeleteTable(rsname))
			{
				auto  reply = deleteAResourceMessage("table");
				deleteRs=(reply!=RETURN_NO);
				deleteAll=(reply==RETURN_ALL);
			}
			//delete rs?
			if(deleteRs)
			{
				resourcesTabs->deleteTable(rsname,deleteAll);
				//reset ui
				treeModelScene->reset();
			}
		}
	});
	//[resources event:add font]
    connect(ui->addFont,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add Font",
                                   "Fonts (*.font.e2d *.fnt)",
                                   &resources->getTable().getTable("fonts"))==QDialog::Accepted)
		{
			if(addResourceDialog->getLastPath().size())
			{
				QString name=addResourceDialog->getLastName();
				name=name.replace(" ","_");
				if(name.size())
				{
					resourcesTabs->addFont(name,addResourceDialog->getLastPath());
				}
			}
		}
	});
	//[resources event:delete font]
    connect(ui->deleteFont,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listFonts->currentItem();
		if(current)
		{
			QString rsname=current->text();
			bool deleteRs=true;
			bool deleteAll=true;
			if(!resourcesTabs->canDeleteFont(rsname))
			{
				auto  reply = deleteAResourceMessage("font");
				deleteRs=(reply!=RETURN_NO);
				deleteAll=(reply==RETURN_ALL);
			}
			//delete rs?
			if(deleteRs)
			{
				resourcesTabs->deleteFont(rsname,deleteAll);
				//reset ui
				treeModelScene->reset();
			}
		}
	});
	//[resources event:add sound]
    connect(ui->addSound,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add Sound",
                                   "Sounds (*.wav)",
                                   &resources->getTable().getTable("sounds"))==QDialog::Accepted)
		{
			if(addResourceDialog->getLastPath().size())
			{
				QString name=addResourceDialog->getLastName();
				name=name.replace(" ","_");
				if(name.size())
				{
					resourcesTabs->addSound(name,addResourceDialog->getLastPath());
				}
			}
		}
	});
	//[resources event:delete sound]
    connect(ui->deleteSound,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listSounds->currentItem();
		if(current)
		{
			QString rsname=current->text();
			bool deleteRs=true;
			bool deleteAll=true;
			if(!resourcesTabs->canDeleteSound(rsname))
			{
				auto  reply = deleteAResourceMessage("sound");
				deleteRs=(reply!=RETURN_NO);
				deleteAll=(reply==RETURN_ALL);
			}
			//delete rs?
			if(deleteRs)
			{
				resourcesTabs->deleteSound(rsname,deleteAll);
				//reset ui
				treeModelScene->reset();
			}
		}
	});
	//[resources event:add script]
    connect(ui->addScript,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add Script",
                                   "Scripts (*.lua)",
                                   &resources->getTable().getTable("scripts"))==QDialog::Accepted)
		{
			if(addResourceDialog->getLastPath().size())
			{
				QString name=addResourceDialog->getLastName();
				name=name.replace(" ","_");
				if(name.size())
				{
					resourcesTabs->addScript(name,addResourceDialog->getLastPath());
				}
			}
		}
	});
	//[resources event:delete script]
    connect(ui->deleteScript,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listScripts->currentItem();
		if(current)
		{
			QString rsname=current->text();
			bool deleteRs=true;
			bool deleteAll=true;
			if(!resourcesTabs->canDeleteScript(rsname))
			{
				auto  reply = deleteAResourceMessage("script");
				deleteRs=(reply!=RETURN_NO);
				deleteAll=(reply==RETURN_ALL);
			}
			//delete rs?
			if(deleteRs)
			{
				resourcesTabs->deleteScript(rsname,deleteAll);
				//reset ui
				treeModelScene->reset();
			}
		}
	});
    //[resources event:add shader]
    connect(ui->addShader,&QPushButton::clicked,this,[this](bool){
        if(addResourceDialog->exec("Add Shader",
                                   "Shaders (*.fx.glsl)",
                                   &resources->getTable().getTable("shaders"))==QDialog::Accepted)
        {
            if(addResourceDialog->getLastPath().size())
            {
                QString name=addResourceDialog->getLastName();
                name=name.replace(" ","_");
                if(name.size())
                {
                    resourcesTabs->addShader(name,addResourceDialog->getLastPath());
                }
            }
        }
    });
    //[resources event:delete shader]
    connect(ui->deleteShader,&QPushButton::clicked,this,[this](bool){
        auto current=ui->listShaders->currentItem();
        if(current)
        {
            QString rsname=current->text();
            bool deleteRs=true;
            bool deleteAll=true;
            if(!resourcesTabs->canDeleteShader(rsname))
            {
                auto  reply = deleteAResourceMessage("shader");
                deleteRs=(reply!=RETURN_NO);
                deleteAll=(reply==RETURN_ALL);
            }
            //delete rs?
            if(deleteRs)
            {
                resourcesTabs->deleteShader(rsname,deleteAll);
                //reset ui
                treeModelScene->reset();
            }
        }
    });

}

//attach components Events
void Editor::attachComponentsEvents()
{
    //[components event:add components]
    connect(ui->addComponent,&QPushButton::clicked,this,[this](bool){
        if(this->target)
        {
            QStringList components(this->componentsTabs
                                       ->componentsManager()
                                        .componentsList());
            //list to remove
            QStringList toRemove;
            //remove fake component
            toRemove << "Transform";
            //delete olready added components (by family id)
            for(auto e2cmp:target->getComponents())
            {
                //get id
                uint id=Easy2D::ComponentMap::getFamily(e2cmp.second->getComponentName());
                //remove id
                for(const auto& component : components)
                {
                    uint localid=Easy2D::ComponentMap::getFamily(component.toUtf8().data());
                    if(localid==id)
                    {
                        toRemove << component;
                    }
                }
            }
            //remove all
            for(const auto& remove : toRemove)
                components.removeAll(remove);
            //open dialog
            SelectStringDialog dialog(components,"",this);
            QString res=dialog.doExec();
            //add new component
            if(res.size())
            {
                //stop engine
                ui->EngineWindow->lock();
                //save and reset target
                auto tmpTarget=this->target;
                this->setTarget(nullptr);
                //add component
                tmpTarget->component(res.toUtf8().data());
                //set old target
                this->setTarget(tmpTarget);
                //start engine
                ui->EngineWindow->unlock();
            }
        }
    });
    //[components event:remove components]
    connect(ui->removeComponent,&QPushButton::clicked,this,[this](bool){
        QStringList components;
        //add components
        for(auto e2cmp:target->getComponents())
        {
            components << e2cmp.second->getComponentName();
        }
        //open dialog
        if(components.size())
        {
            //open dialog
            SelectStringDialog dialog(components,"",this);
            QString res=dialog.doExec();
            //delete a component
            if(res.size())
            {
                //stop engine
                ui->EngineWindow->lock();
                //save and reset target
                auto tmpTarget=this->target;
                this->setTarget(nullptr);
                //add component
                delete tmpTarget->removeComponent(res.toUtf8().data());
                //set old target
                this->setTarget(tmpTarget);
                //start engine
                ui->EngineWindow->unlock();
            }
        }
    });
}


//attach globals Events
void Editor::attachGlobalsEvent()
{
    //gravity
    auto leGChanged=[this](const QString&)
    {
        float x=this->ui->leGravity_x->text().toFloat();
        float y=this->ui->leGravity_y->text().toFloat();
        this->ui->EngineWindow->setGravity(Easy2D::Vec2(x,y));
    };

    connect(ui->leGravity_x,&QLineEdit::textChanged,leGChanged);
    connect(ui->leGravity_y,&QLineEdit::textChanged,leGChanged);

    //change pixel rate
    connect(ui->leMetersInPixel,&QLineEdit::textChanged,
    [this](const QString&)
    {
        this->ui->EngineWindow->setMetersInPixel(
                    this->ui->leMetersInPixel->text().toFloat()
                    );
        this->ui->leGravity_x->blockSignals(true);
        this->ui->leGravity_y->blockSignals(true);
        /////////////////////////////////////////////////
        /// update gravity
        auto gravity=this->ui->EngineWindow->getGravity();
        this->ui->leGravity_x->setText(QString::number(gravity.x));
        this->ui->leGravity_y->setText(QString::number(gravity.y));
        /////////////////////////////////////////////////
        this->ui->leGravity_x->blockSignals(false);
        this->ui->leGravity_y->blockSignals(false);
    });

    //spin box event
    auto valueChanged=static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged);

    //change ambient color
    auto sbACChange=
    [this](int value)
    {
        ui->EngineWindow->setAmbientLight(
        Easy2D::Color(ui->sbAmbientColor_r->value(),
                      ui->sbAmbientColor_g->value(),
                      ui->sbAmbientColor_b->value(),
                      ui->sbAmbientColor_a->value())
        );
    };
    connect(ui->sbAmbientColor_r,valueChanged,sbACChange);
    connect(ui->sbAmbientColor_g,valueChanged,sbACChange);
    connect(ui->sbAmbientColor_b,valueChanged,sbACChange);
    connect(ui->sbAmbientColor_a,valueChanged,sbACChange);

    //change clear color
    auto sbCCChange=
    [this](int value)
    {
        ui->EngineWindow->setClearColor(
        Easy2D::Color(ui->sbClearColor_r->value(),
                      ui->sbClearColor_g->value(),
                      ui->sbClearColor_b->value(),
                      ui->sbClearColor_a->value())
        );
    };
    connect(ui->sbClearColor_r,valueChanged,sbCCChange);
    connect(ui->sbClearColor_g,valueChanged,sbCCChange);
    connect(ui->sbClearColor_b,valueChanged,sbCCChange);
    connect(ui->sbClearColor_a,valueChanged,sbCCChange);
}

Editor::~Editor()
{
    //lock ui engine
    //stop engine
    ui->EngineWindow->lock();
    //free root scene
    treeModelScene->safeFreeRoot();
    //delete all
	delete addObjectDialog;
	delete aboutDialog;
	delete renameObjectDialog;
	delete addResourceDialog;
	//delete manager components
	delete componentsTabs;
	//delete treeModelScene; //auto delete by qt
	delete resourcesTabs;
    delete resources;
    //release context
    Easy2DInit::releaseOpenGL();
    //delete staticGLContext; //???

    delete ui;
}
