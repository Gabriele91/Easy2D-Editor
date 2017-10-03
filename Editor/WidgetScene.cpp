#include <WidgetScene.h>
#include <Editor.h>
#include <QMouseEvent>
#include <QDir>
#include <QDebug>
#include <QImageReader>

WidgetScene::WidgetScene(QWidget *parent)
    :QGLWidget(Editor::context(),parent)
    ,Scene(false)
    ,editor(nullptr)
	,timer(this)
    ,sceneNode(nullptr)
	,target(nullptr)
    ,state(OBJECT_UNSELECTED)
    ,cbOnStart([](){})
    ,cbOnRun([](float){})
    ,cbOnEnd([](){})
    ,cbOnEnterPlayMode([](){})
    ,cbOnExitPlayMode([](){})
    ,cbOnDrawGui( [](){} )
    ,cbOnMousePress( [](QMouseEvent *event){} )
    ,cbOnMouseMove( [](QMouseEvent *event){} )
    ,cbOnMouseRelease( [](QMouseEvent *event){} )
    ,cbOnMouseWell( [](QWheelEvent *event){} )
    ,cbOnDoubleClick( [](QMouseEvent *event){} )
{  
	//allocs
	camera=new Easy2D::Camera();
	//init update
	timer.setInterval(DELTATIME*1000.0);//60 fps
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateThreadGL()));
    timer.start();
}
WidgetScene::~WidgetScene()
{
    timer.stop();
	//call end
	end();
	//delete
	delete camera;
	//clear scene object (no delete)
	clearObjects(false);
}
//init editor
void WidgetScene::setEditor(Editor* argeditor)
{
    editor=argeditor;
    lastEditorState=(int)editor->getState();
}
//update OpenGL thread
void WidgetScene::updateThreadGL()
{
    context()->makeCurrent();
    updateGL();
}
//Object managments
void WidgetScene::setSceneNode(Easy2D::Object *argscene)
{
    if(sceneNode) eraseObject(sceneNode);
    sceneNode=argscene;
    addObject(sceneNode);
}
void WidgetScene::setSelectObject(Easy2D::Object* argtarget)
{
    if(target==argtarget)
    {
        goToTarget();
    }
    else
    {
        state=OBJECT_UNSELECTED;
        target=argtarget;
    }
}
void WidgetScene::unselectObject()
{
    state=OBJECT_UNSELECTED;
    target=nullptr;
}
Easy2D::Object* WidgetScene::getSelectObject()
{
	return target;
}
bool hasObject(Easy2D::Object* target,Easy2D::Object* obj)
{
	if(!target) return false;
	if(target==obj) return true;
	for(auto child:*target)
	{
		if(hasObject(child,obj))
			return true;
	}
	return false;
}
bool WidgetScene::isInSelectedObject(Easy2D::Object* obj)
{
	if(!target) return false;
	return hasObject(target,obj);
}
void WidgetScene::goToTarget()
{
    if(target)
    {
        //go to here
        Easy2D::AABox2 box;
        getTargetAABox(box);
        //camera pos
        camera->setPosition(-box.getCenter());
        setZoom((box.getSize().length()/camera->getViewport().length())*4);
    }
}
//aabb utilities
bool WidgetScene::getAABox(Easy2D::Object* obj,Easy2D::AABox2& retBox)
{
    if(obj)
    {
        auto renderable=obj->getComponent<Easy2D::Renderable>();
        if(renderable && renderable->getMesh())
        {
            const Easy2D::AABox2& box = renderable->getMesh()->getAABox();
            retBox = renderable->canTransform() ?
                     box.applay(obj->getGlobalMatrix()) : box;
        }
        else
        {
            retBox.addPoint(obj->getPosition(true));
        }
        return true;
/*
            return true;
        }
*/
    }
    return false;
}
bool WidgetScene::getGlobalAABox(Easy2D::Object* obj,Easy2D::AABox2& outBox)
{
    //vars
    bool added=false;
    Easy2D::AABox2 box;

    //get this aabb
    if(getAABox(obj,box))
    {
        added=true;
        outBox.addBox(box);
    }

    //add child aabb
    for(auto child:*obj)
    {
        added=getGlobalAABox(child,outBox)||added;
    }

    //return
    return added;
}
bool WidgetScene::getTargetAABox(Easy2D::AABox2& box)
{
    if(target)
    {
        //boolean
        bool added=getGlobalAABox(target,box);
        //size
        auto size=box.getSize().getAbs();
        const float epsilon=1e-6;
        //too smaller?
        if(size.x < epsilon ||
           size.y < epsilon ||
           Easy2D::Math::isinf(size.x) ||
           Easy2D::Math::isinf(size.y))
        {
            float size=Easy2D::Math::clamp(50.0f*camera->getZoom(),10000.0f,10.0f);
            box.setBox(target->getPosition(true),Easy2D::Vec2(size,size));
        }
        return added;
    }
    return false;
}
//set camera zoom
void WidgetScene::setZoom(float nzoom)
{
    camera->setZoom(Easy2D::Math::max(nzoom,0.01f));
    float zoomceild=std::ceilf(nzoom);
    grid->setScale(Easy2D::Vec2(32,32)*zoomceild);
}

//init opengl
void WidgetScene::initializeGL()
{
    //enable default state
    Easy2D::RenderContext::setDefaultRenderState();
    //enable context
    context()->makeCurrent();
    //init render
    Render::init();
	//init
	setCamera(camera);
	//call start
	start();	
}
void WidgetScene::paintEvent(QPaintEvent *event)
{
}
//draw opengl
void WidgetScene::paintGL()
{
    //editor?
    if(!editor) return;
    //enable context
    context()->makeCurrent();
    //update cameta
    camera->updateProjection();
    /////////////////////////////////////////////////////////////////////////////
    if(lastEditorState==Editor::EDITOR_MODE && editor->getState()==Editor::PLAY_MODE)
    {
        cbOnEnterPlayMode();
    }
    if(lastEditorState==Editor::PLAY_MODE && editor->getState()==Editor::EDITOR_MODE)
    {
        cbOnExitPlayMode();
    }
    lastEditorState=editor->getState();
    /////////////////////////////////////////////////////////////////////////////
    // save ambient color
    Easy2D::Color lightAmbientColor=getAmbientLight();
    /////////////////////////////////////////////////////////////////////////////
    //if is in select mode
    if(target && state==OBJECT_SELECTED_EDITOR)
    {
        setAmbientLight(Easy2D::Color(64,64,64,64));
    }
    /////////////////////////////////////////////////////////////////////////////
    switch (editor->getState())
    {
    case Editor::EDITOR_MODE:
        // EDITOR MODE
        //draw
        {
            //update editor loop (no components)
            onRun(DELTATIME);
            //update particles components
            updateParticles(sceneNode,DELTATIME);
            //render is only if have some objects
            if(getObjects().size())
            {
                //rebuild queue
                buildQueue(getObjects());
                //draw scene
                Render::draw();
            }
        }
        break;
    case Editor::PLAY_MODE:
        // PLAY MODE
        //update and draw
        {
            //run
            run(DELTATIME);
        }
        break;
    default: break;
    }
    /////////////////////////////////////////////////////////////////////////////
    //reset ambient color
    setAmbientLight(lightAmbientColor);
    /////////////////////////////////////////////////////////////////////////////
    //if is in select mode
    if(target && state==OBJECT_SELECTED_EDITOR)
    {
        //list objects
        std::list<Easy2D::Object*> listObjs;
        listObjs.push_back(target);
        //rebuild queue
        buildQueue(listObjs);
        //save clear state
        bool cEnable=getClearIsEnable();
        setEnableClear(false);
        //draw scene
        Render::draw();
        //reset state
        setEnableClear(cEnable);
    }
    /////////////////////////////////////////////////////////////////////////////
    //draw editor
    auto state=Easy2D::RenderContext::getRenderState();
    drawEditorObjects();
    Easy2D::RenderContext::setRenderState(state);
    //////////////////////////////////////////////////
}
void WidgetScene::resizeGL(int w, int h)
{
}
//draw editor info
void WidgetScene::drawEditorObjects()
{
    //////////////////////////////////////////////////////////////////
    //set projection matrix
    Easy2D::RenderContext::setProjection(camera->getProjection());
    //set view port
    Easy2D::RenderContext::setViewport(Easy2D::Vec4(0,0,camera->getViewport().x,
                                                        camera->getViewport().y));
    //set model view matrix
    Easy2D::RenderContext::setView(camera->getGlobalMatrix());
    Easy2D::RenderContext::setModel(Easy2D::Mat4::IDENTITY);
    //set states
    Easy2D::RenderContext::setCullFace(Easy2D::DISABLE);
    Easy2D::RenderContext::setBlend(true);
    //set blend function
    Easy2D::RenderContext::setBlendFunction(Easy2D::BLEND::SRC::ALPHA,
                                            Easy2D::BLEND::ONE::MINUS::SRC::ALPHA);
    //texture
    Easy2D::RenderContext::setTexture(false);
    //clients
    Easy2D::RenderContext::setClientState(true, false, false, false);
    //////////////////////////////////////////////////////////////////
    if(target &&
       state!=OBJECT_SELECTED_EDITOR) //no object editor mode
    {
        //globals
        auto rotation=target->getRotation(true);
        auto position=target->getPosition(true);
        //dec box
        Easy2D::AABox2 box;
        getTargetAABox(box);
        //draw box
        drawAABox(box);
        //points:
        auto size=box.getSize();
        drawPoint2D(box.getCenter()-size);
        drawPoint2D(box.getCenter()+Easy2D::Vec2(-size.x, size.y));
        drawPoint2D(box.getCenter()+Easy2D::Vec2( size.x,-size.y));
        drawPoint2D(box.getCenter()+size);

        //matrix
        Easy2D::Mat4 mRot;
        mRot.setRotZ(rotation);
        //mul
        auto up=mRot.mul2D(Easy2D::Vec2(0,128)*camera->getZoom());
        auto left=mRot.mul2D(Easy2D::Vec2(128,0)*camera->getZoom());
        //draw line up
        Easy2D::RenderContext::drawLine(position,position+up,Easy2D::Color(255,0,0,255));
        //draw line left
        Easy2D::RenderContext::drawLine(position,position+left,Easy2D::Color(0,255,0,255));
        //point
        drawPoint2D(target->getPosition(true));
    }
    //////////////////////////////////////////////////////////////////
    //callback
    cbOnDrawGui();
    //////////////////////////////////////////////////////////////////
}
//update particles
void WidgetScene::updateParticles(Easy2D::Object* obj,float dt)
{
    //get emitter
    auto emitter=obj->getComponent<Easy2D::Emitter>();
    //emitter
    if(emitter)
    {
        emitter->onRun(dt);
    }
    //childs emitter
    for(auto child:*obj)
    {
        updateParticles(child,dt);
    }
}
//clear color
void WidgetScene::setClearColor(const Easy2D::Color& color)
{
    clearColor=color;
}
Easy2D::Color WidgetScene::getClearColor()
{
    return clearColor;
}
//editor object mode
void WidgetScene::editorObjectMode(bool eom)
{
    if(target)
    {
        if(eom)
        {
            state=OBJECT_SELECTED_EDITOR;
        }
        else
        {
            state=OBJECT_UNSELECTED;
        }
    }
}
bool WidgetScene::isInEditorObjectMode()
{
    if(target)
    {
        return state==OBJECT_SELECTED_EDITOR;
    }
    return false;
}

//events
void WidgetScene::mouseDoubleClickEvent(QMouseEvent *event)
{
    //callback
    cbOnDoubleClick(event);
    //no object editor mode
    if(state==OBJECT_SELECTED_EDITOR) return;
    //event
    if(event->button()==Qt::MouseButton::LeftButton)
    {
        //mouse pos
        Easy2D::Vec2 mousePos(event->localPos().x(),event->localPos().y());
        //pick list
        auto listOPicked=picking(camera->getWMouse(mousePos));
        //get last object
        auto objSelected=listOPicked[listOPicked.size()-1];
        //if not selected
        if(!(objSelected && editor->selectObject(objSelected)))
        {
            //unselect
            unselectObject();
        }
    }
}
void WidgetScene::mousePressEvent(QMouseEvent *event)
{
    //callback
    cbOnMousePress(event);
    //no object editor mode
    if(state==OBJECT_SELECTED_EDITOR) return;

    //unselected
    if(!target)
    {
        state=OBJECT_UNSELECTED;
        return;
    }
    Easy2D::AABox2 box;
    if(!getTargetAABox(box))
    {
        state=OBJECT_UNSELECTED;
        return;
    }

	//mouse pos
	Easy2D::Vec2 mousePos(event->localPos().x(),event->localPos().y());
	//get pos
	pikCam=camera->getWMouse(mousePos);
	//pos
	pikPos=target->getPosition(true);
	//rot
	pikRot=target->getRotation(true);
	//scale
	pikScale=target->getScale(true);
    //if not picked
    if(!box.isIntersection(pikCam))
    {
        //unselect
        state=OBJECT_UNSELECTED;
        return;
    }
    //else
    else
    {
        switch (event->button())
        {
            case Qt::MouseButton::LeftButton: state=OBJECT_SELECTED_TRANSLATE; break;
            case Qt::MouseButton::RightButton: state=OBJECT_SELECTED_ROTATION; break;
            case Qt::MouseButton::MidButton: state=OBJECT_SELECTED_SCALE; break;
            default: return; break;
        }
    }
}
//on move mouse event
void WidgetScene::mouseMoveEvent(QMouseEvent * event)
{
    //event
    QPoint posMouse = mapFromGlobal(QCursor::pos());
    if(!rect().contains(posMouse)) return;
    //callback
    if(event)
    {
        cbOnMouseMove(event);
    }
    else
    {
        QMouseEvent *efake=
        new QMouseEvent(QEvent::MouseMove,posMouse,
                        Qt::NoButton,
                        Qt::NoButton,
                        Qt::KeyboardModifier::NoModifier);
        cbOnMouseMove(efake);
        delete efake;
    }
    //no object editor mode
    if(state==OBJECT_SELECTED_EDITOR) return;

	//mouse pos
	Easy2D::Vec2 mousePos(posMouse.x(),posMouse.y());
	Easy2D::Vec2 sizeWid(width(),height());
	auto mouseW=camera->getWMouse(mousePos);
    //move camera
	if(QApplication::mouseButtons()==Qt::MouseButton::LeftButton)
	{
		const float bSize=100.0f;
		const float ibSize=1.0f/bSize+0.05;
		const Easy2D::Vec2 bsize(bSize,bSize);
		auto mindiff=-(mousePos-bsize); mindiff*=mindiff*DELTATIME*ibSize;
		auto maxdiff=-bsize+(sizeWid-mousePos); maxdiff*=-maxdiff*DELTATIME*ibSize;
		if((mousePos.x-bsize.x) < 0) camera->setTranslation(Easy2D::Vec2(mindiff.x,0));
		if((mousePos.x+bsize.x) > sizeWid.x) camera->setTranslation(Easy2D::Vec2(maxdiff.x,0));
		if((mousePos.y-bsize.y) < 0) camera->setTranslation(Easy2D::Vec2(0,-mindiff.y));
		if((mousePos.y+bsize.y) > sizeWid.y) camera->setTranslation(Easy2D::Vec2(0,-maxdiff.y));
	}
	//state manager
	switch(state)
	{
	case OBJECT_SELECTED_TRANSLATE:
		 //
         target->setPosition(mouseW-(pikCam-pikPos),true);
		 //
	break;
	case OBJECT_SELECTED_ROTATION:
		{
			auto startPointToCenter=pikCam-target->getPosition(true);
            auto endPointToCenter=mouseW-target->getPosition(true);
			//angle
            Easy2D::Angle angle=endPointToCenter.direction()-startPointToCenter.direction();
            //float angleDeg=Easy2D::Math::todeg(angle);
			//set angle
            target->setRotation(angle+pikRot,true);
        }
	break;
	case OBJECT_SELECTED_SCALE:
		{
			auto diff=(mouseW-pikCam)/32.0f;
			target->setScale(pikScale+diff,true);
		}
	break;
    default: return; break;
	}
}
//well event
void WidgetScene::wheelEvent(QWheelEvent* event)
{
    //callback
    cbOnMouseWell(event);
    //no object editor mode
    if(state==OBJECT_SELECTED_EDITOR) return;

    //event
	float delta=event->angleDelta().y();
    float nzoom=camera->getZoom()+(delta*0.001);
    setZoom(nzoom);
}
//on release event
void WidgetScene::mouseReleaseEvent(QMouseEvent *event)
{
    //callback
    cbOnMouseRelease(event);
    //no object editor mode
    if(state==OBJECT_SELECTED_EDITOR) return;

    //event
	switch (event->button())
	{
	case Qt::MouseButton::LeftButton:
	case Qt::MouseButton::MidButton:
	case Qt::MouseButton::RightButton: 
		state=OBJECT_UNSELECTED;
	break;
	default: return; break;
	}
}

//virtual methos
void WidgetScene::onStart()
{
	//new obj
	grid=new Easy2D::Object();
	//create randerable
	auto* rable=new Easy2D::Renderable();
	//blend
	rable->
    enableBlend();
	rable->
    setBlend(Easy2D::BLEND::SRC::ALPHA,
             Easy2D::BLEND::ONE::MINUS::SRC::ALPHA);
	//add mesh
    auto mesh=new Easy2D::Mesh();
	for(int x=-50;x<=50;++x)
	for(int y=-50;y<=50;++y)
	{
		//seg1
		mesh->addVertex(x,y,0,0);
		mesh->addVertex(x+1,y,0,0);
		//seg2
		mesh->addVertex(x+1,y,0,0);
		mesh->addVertex(x+1,y+1,0,0);
		//seg3
		mesh->addVertex(x+1,y+1,0,0);
		mesh->addVertex(x,y+1,0,0);
		//seg4
		mesh->addVertex(x,y+1,0,0);
		mesh->addVertex(x,y,0,0);
	};
    mesh->setDrawMode(Easy2D::LINES);
	mesh->build();
	rable->setMesh(Easy2D::Mesh::ptr(mesh));
	mesh->setBatching(false);
	//add texture
	auto texture=new Easy2D::Texture();
    //load
    QImage gridImage;
    gridImage.load( ":/grid_texture.png" );
    //gridImage.load( "://grid_texture.png" );
    //gridImage.load( "assets/images/grid_texture.png" );
    //to opengl format
    QImage gridImageGL=QGLWidget::convertToGLFormat(gridImage);
	//to easy 2D texture
    texture->loadFromBinaryData(
		gridImageGL.bits(),
		gridImageGL.width(),
		gridImageGL.height(),
		GL_RGBA,
		GL_RGBA
		);
	texture->bilinear(false);
	//texture->mipmaps(false);
	//to renderable
	rable->setTexture(Easy2D::Texture::ptr(texture));
	//add component
	grid->addComponent(rable);
	//add into scene
    addObject(grid);
	//set info
	grid->setScale(Easy2D::Vec2(32,32));
	grid->setZ(~0x7FFFFFFF);
    //callback
    cbOnStart();
}
void WidgetScene::onRun(float dt)
{
	//clear color
    setClear(editor->getState()==Editor::EDITOR_MODE ?
             Easy2D::Color::GRAY :
             clearColor);
	//update events onmove
    if(this->hasFocus())
        mouseMoveEvent(nullptr);
    //callback
    cbOnRun(dt);
}
void WidgetScene::onEnd()
{
    //callback
    cbOnEnd();
}

/*** DRAW UTILITIES ***/
void WidgetScene::drawPoint2D(const Easy2D::Vec2& p,
                              const Easy2D::Color& colorE,
                              const Easy2D::Color& colorI)
{
    Easy2D::RenderContext::drawCircle(p,
                                      5*camera->getZoom(),
                                      colorE);
    Easy2D::RenderContext::drawFillCircle(p,
                                          4*camera->getZoom(),
                                          colorI);
}
void WidgetScene::drawLine(const Easy2D::Vec2& p1,
                           const Easy2D::Vec2& p2,
                           const Easy2D::Color& color)
{
    Easy2D::RenderContext::drawLine(p1,p2,color);
}
void WidgetScene::drawAABox(const Easy2D::AABox2& box, const Easy2D::Color &color)
{
    Easy2D::RenderContext::drawBox(box,color);
}
void WidgetScene::drawCircle(const Easy2D::Vec2& p,
                             float r,
                             const Easy2D::Color &color)
{
    Easy2D::RenderContext::drawCircle(p,r,color);
}
void WidgetScene::drawPath(const std::vector<Easy2D::Vec2>& points,
                           bool close,
                           const Easy2D::Color &color)
{
    drawPath(points,Easy2D::Vec2::ZERO,close,color);
}
void WidgetScene::drawPath(const std::vector<Easy2D::Vec2>& points,
                           const Easy2D::Vec2& position,
                           bool close,
                           const Easy2D::Color &color)
{
    //no draw
    if(points.size()<2) return;
    //lines
    for(size_t i=1;i!=points.size();++i)
    {
        Easy2D::RenderContext::drawLine(position+points[i-1],position+points[i],color);
    }
    //close
    if(close)
    {
        Easy2D::RenderContext::drawLine(position+points[points.size()-1],position+points[0],color);
    }
}
void WidgetScene::drawPathPoints(const std::vector<Easy2D::Vec2>& points,
                                 bool close,
                                 const Easy2D::Color& color,
                                 const Easy2D::Color& colorE,
                                 const Easy2D::Color& colorI)
{
    drawPathPoints(points,Easy2D::Vec2::ZERO,close,color, colorE, colorI);
}
void WidgetScene::drawPathPoints(const std::vector<Easy2D::Vec2>& points,
                                 const Easy2D::Vec2& position,
                                 bool close,
                                 const Easy2D::Color& color,
                                 const Easy2D::Color& colorE,
                                 const Easy2D::Color& colorI)
{
    //no draw
    if(points.size()<2) return;
    //first point
    drawPoint2D(position+points[0],colorE,colorI);
    //lines and points
    for(size_t i=1;i!=points.size();++i)
    {
        Easy2D::RenderContext::drawLine(position+points[i-1],position+points[i],color);
        drawPoint2D(position+points[i],colorE,colorI);
    }
    //close
    if(close)
    {
        Easy2D::RenderContext::drawLine(position+points[points.size()-1],position+points[0],color);
    }
}


















/*
#if 0
{
	auto startPointToCenter=pikCam-target->getPosition(true);
	auto endPointToCenter=mouseW-target->getPosition(true);
	//normalize
	//startPointToCenter.normalize();
	//endPointToCenter.normalize();
	auto diff=endPointToCenter-startPointToCenter;
	//angle
	float angle=diff.direction();
	float angleDeg=Easy2D::Math::todeg(angle);
	//set angle
	target->setRotation(angleDeg+pikPos,true);
}
#endif
#if 0
{
	auto diff=mouseW-target->getPosition(true);
	//angle
	float angle=diff.direction();
	float angleDeg=Easy2D::Math::todeg(angle);
	//set angle
	target->setRotation(angleDeg+pikRot,true);
}
#endif
*/
