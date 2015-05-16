#include <PluginBody.h>

inline bool ComponentBody::collisionLine(const Easy2D::Vec2& start,
                                         const Easy2D::Vec2& end,
                                         const Easy2D::Vec2& center,
                                         float r)
{
    auto d=start-end;
    auto f=end-center;

    float a = d.dot( d ) ;
    float b = 2*f.dot( d ) ;
    float c = f.dot( f ) - r*r ;

    float discriminant = b*b-4*a*c;

    if( discriminant >= 0 )
    {
      discriminant = sqrt( discriminant );
      float t1 = (-b - discriminant)/(2*a);
      float t2 = (-b + discriminant)/(2*a);

      if( t1 >= 0 && t1 <= 1 ) return true ;
      if( t2 >= 0 && t2 <= 1 ) return true ;
    }

    return false ;
}

//name
QString ComponentBody::getName()
{
   return "Body";
}
//get widget ui
QWidget* ComponentBody::getWidget()
{
   return uibody;
}
//get body
Easy2D::Body* ComponentBody::getBody()
{
    if(getLastObject()) return getLastObject()->getComponent<Easy2D::Body>();
    return nullptr;
}
//build ui
QWidget* ComponentBody::buildGUI(QWidget*)
{
    if(uibody) delete uibody;
    //init ui
    uibody=new UIBody(this);
    //init ui
    initUi=true;
    applay=false;
    editShape=EditShape();
    return uibody;
}
//on load dll
void ComponentBody::onCreate()
{
    uibody=nullptr;
    initUi=false;
}

//on run
void ComponentBody::onRun(Easy2D::Object* obj,float dt)
{
    //ui?
    if(!uibody) return;
    //get component
    auto body=getBody();
    //else return
    if(!body) return;
    //set ui
    if(initUi)
    {
        uibody->initUIBody(body);
        //update
        initUi=false;
    }
    //update component
    if(applay)
    {
        uibody->applayToBody(body);
        //update shape
        if(editShape.isEditable)
        {
           uibody->applayToShape(body,
                                 editShape.shape);
        }
        //update
        applay=false;
    }
    //update vertex / circle ui
    if(editShape.pointSelect<0)
    {
        uibody->disableUIVertex();
    }
}

//on draw ui
void ComponentBody::onDrawGui()
{
    auto body=getBody();
    if(body)
    {
        //standard color
        Easy2D::Color colorPoint(0,255,0,255);
        Easy2D::Color colorLine(0,255,0,128);
        //for all shapes
        for(Easy2D::Shape sp : *body)
        {
            //in edit mode change color
           if(editShape.isEditable)
           {
               //draw green
               if(editShape.shape==sp)
               {
                   //draw blue radius
                   if(editShape.radiusSelect)
                       colorLine = Easy2D::Color (0,0,255,128);
                   else
                       colorLine = Easy2D::Color (0,255,0,128);
                   //
                   colorPoint = Easy2D::Color (0,255,0,255);
               }
               //draw red
               else
               {
                   colorLine = Easy2D::Color (255,0,0,128);
                   colorPoint = Easy2D::Color (255,0,0,128);
               }
           }
           //select type
           switch (body->getShapeType(sp))
           {
               case Easy2D::Body::CIRCLE:
                   drawBodyCircle(body,sp,colorLine,colorPoint);
               break;
               case Easy2D::Body::POLYGON:
                   drawBodyPolygon(body,sp,colorLine,colorPoint);
               break;
               case Easy2D::Body::CHAIN:
                   drawBodyChain(body,sp,colorLine,colorPoint);
               break;
               default:
               break;
           }
        };
        //edit mode, draw points
        if(editShape.isEditable && editShape.pointSelect>-1)
        {
            Easy2D::Mat4 m4RT(getRTMatrix());
            getScene()->drawPoint2D(m4RT.mul2D(editShape.points[editShape.pointSelect]));
        }
    }
}

//destroy dll
void ComponentBody::onDestoy()
{
    if(uibody)
        delete uibody;
}

//uibody change
void ComponentBody::applayChange()
{
    applay=true;
}

//void applay vertex slected
void ComponentBody::applayVertex()
{
    if(getBody() &&
       editShape.pointSelect>-1  &&
       editShape.isEditable)
    {
      uibody->applayVertex(editShape.points[editShape.pointSelect]);
      updateVertexs();
    }
}

//update corrent shape vertexs
void ComponentBody::updateVertexs()
{
    auto body=getBody();
    if(body && editShape.isEditable)
    switch (body->getShapeType(editShape.shape))
    {
        case Easy2D::Body::CIRCLE:
            body->changeCirclePosition(editShape.shape,editShape.points[0]);
        break;
        case Easy2D::Body::POLYGON:
            body->changePolygonPoints(editShape.shape,editShape.points);
        break;
        case Easy2D::Body::CHAIN:
            /*** CHAIN CALL b2Alloc, unsafe */
            body->changeChainPoints(editShape.shape,editShape.points);
        break;
        default:
        break;
    };
}

//add a shape
void ComponentBody::addShape()
{
    auto body=getBody();
    if(body)
    {
        //lock editor
        lock();
        //
        QStringList shapesType;
        shapesType << "Circle" ;
        shapesType << "Polygon" ;
        shapesType << "Chain" ;
        QString select=openDialogStringList(shapesType,"Circle");

        if(select=="Circle")
        {
            body->createCircleShape(100.0f);
        }
        else if(select=="Polygon")
        {
            body->createBoxShape(Easy2D::Vec2::ONE*100.0f);
        }
        else if(select=="Chain")
        {
            std::vector<Easy2D::Vec2> vec;
            vec.push_back(Easy2D::Vec2(-50.0f,-50.0f));
            vec.push_back(Easy2D::Vec2( 50.0f, 50.0f));
            body->createChainShape(vec);
        }
        //update ui
        uibody->updateListShape(body);
        //unlock editor
        unlock();
        //
    }
}

//enable edit mode
void ComponentBody::enabeEditMode(Easy2D::Shape shape)
{
    getScene()->editorObjectMode(true);
    getScene()->goToTarget();
    //save status
    editShape=EditShape();
    editShape.isEditable=true;
    editShape.shape=shape;
    //enable ui
    uibody->enableUIShape(getBody(),shape);
}

//disable edit mode
void ComponentBody::disableEditMode()
{
    getScene()->editorObjectMode(false);
    //edit shape
    editShape=EditShape();
    //disable ui
    uibody->disableUIShape();
    uibody->disableUIVertex();
}

//is editable
inline bool ComponentBody::isEditable(const Easy2D::Body *body) const
{
   return body && editShape.isEditable;
}

//delete selected shape
void ComponentBody::deleteCurrentShape()
{
    //get body
    auto body=getBody();
    //
    if(isEditable(body))
    {
        //lock editor
        lock();
        //delete
        body->deleteShape(editShape.shape);
        //disable editor mode
        disableEditMode();
        //update ui
        uibody->updateListShape(body);
        //unlock editor
        unlock();
    }
}

//input event
void ComponentBody::onWheelEvent(QWheelEvent* event)
{
    if(getScene()->isInEditorObjectMode())
    {
        auto camera=getScene()->getCamera();
        //event
        float delta=event->angleDelta().y();
        float nzoom=camera.getZoom()+(delta*0.001);
        getScene()->setZoom(Easy2D::Math::max(nzoom,0.01f));
    }
}

//edit shape
void ComponentBody::selectOrAddAPoint(const Easy2D::Vec2& pikCam)
{
    //mat
    Easy2D::Mat4 m4RT(getRTMatrix());
    //reset selected point
    editShape.pointSelect=-1;
    //point distance
    float dinstance=5.0f*getScene()->getCamera().getZoom();
    //tmp body
    auto body=getBody();
    //all case
    switch (body->getShapeType(editShape.shape))
    {
    case Easy2D::Body::CIRCLE:
    {
        //get
        editShape.points.resize(1);
        editShape.points[0]=body->getCirclePosition(editShape.shape);
        //determinate a collision
        auto point=m4RT.mul2D(editShape.points[0]);
        if(point.distancePow2(pikCam) < dinstance*dinstance)
        {
            editShape.pointSelect=0;
            //enable vertex ui
            uibody->enableUIVertex(editShape.points[0]);
        }
        else
        {
            float radius=body->getCircleRadius(editShape.shape);
            float rmin=radius-5.0f*getScene()->getCamera().getZoom();
            float rmax=radius+5.0f*getScene()->getCamera().getZoom();
            editShape.radiusSelect=
                    point.distancePow2(pikCam) > rmin*rmin &&
                    point.distancePow2(pikCam) < rmax*rmax ;
        }
    }
    break;
    case Easy2D::Body::POLYGON:
    {
        //get
        body->getPolygonPoints(editShape.shape,editShape.points);
        //npoints
        int size=editShape.points.size();
        //determinate a collision
        for(int i=0;i<size;++i)
        {
            auto point=m4RT.mul2D(editShape.points[i]);
            if(point.distancePow2(pikCam) < dinstance*dinstance)
            {
                editShape.pointSelect=i;
                //enable vertex ui
                uibody->enableUIVertex(editShape.points[i]);
                //break loop
                break;
            }
        }
        //line point
        if(editShape.pointSelect<0 && size < 8 )
        {
            for(int i=0;i<size;++i)
            {
                auto start=editShape.points[i];
                auto end=editShape.points[(i+1)%size];
                if(collisionLine(m4RT.mul2D(start),
                                 m4RT.mul2D(end),
                                 pikCam,dinstance))
                {
                    //select point
                    editShape.pointSelect=i+1;
                    //insert
                    editShape.points.insert(editShape.points.begin()+i+1,
                                            m4RT.getInverse().mul2D(pikCam));
                    //enable vertex ui
                    uibody->enableUIVertex(editShape.points[i+1]);
                    //break
                    break;
                }
            }
        }
    }
    break;
    case Easy2D::Body::CHAIN:
    {
        //get
        body->getChainPoints(editShape.shape,editShape.points);
        //npoints
        int size=editShape.points.size();
        //determinate a collision
        for(int i=0;i<editShape.points.size();++i)
        {
            auto point=m4RT.mul2D(editShape.points[i]);
            if(point.distancePow2(pikCam) < dinstance*dinstance)
            {
                editShape.pointSelect=i;
                //enable vertex ui
                uibody->enableUIVertex(editShape.points[i]);
                //break loop
                break;
            }
        }
        //line point
        if(editShape.pointSelect<0)
        {
            for(int i=0;i<size;++i)
            {
                auto start=editShape.points[i];
                auto end=editShape.points[(i+1)%size];
                if(collisionLine(m4RT.mul2D(start),
                                 m4RT.mul2D(end),
                                 pikCam,dinstance))
                {
                    //select point
                    editShape.pointSelect=i+1;
                    //insert
                    editShape.points.insert(editShape.points.begin()+i+1,
                                            m4RT.getInverse().mul2D(pikCam));
                    //enable vertex ui
                    uibody->enableUIVertex(editShape.points[i+1]);
                    //break
                    break;
                }
            }
        }
    }
    break;
    default:
    break;
    }
}
void ComponentBody::deleteAPoint(const Easy2D::Vec2& pikCam)
{
    //reset selected point
    editShape.pointSelect=-1;
    //point distance
    float dinstance=5.0f*getScene()->getCamera().getZoom();
    //point size
    int size=0;
    //get tmp body
    auto body=getBody();
    //all case
    switch (body->getShapeType(editShape.shape))
    {
    case Easy2D::Body::CIRCLE: return; break;
    case Easy2D::Body::POLYGON:
    //get
    body->getPolygonPoints(editShape.shape,editShape.points);
    break;
    case Easy2D::Body::CHAIN:
    //get
    body->getChainPoints(editShape.shape,editShape.points);
    break;
    default: break;
    }
    //get matrix
    Easy2D::Mat4 m4RT(getRTMatrix());
    //npoints
    size=editShape.points.size();
    //determinate a collision
    for(int i=0;i<size;++i)
    {
        auto point=m4RT.mul2D(editShape.points[i]);
        if(point.distancePow2(pikCam) < dinstance*dinstance)
        {
            //delete
            editShape.points.erase(editShape.points.begin()+i);
            //disable vertex ui
            uibody->disableUIVertex();
            //update component
            updateVertexs();
            //break loop
            break;
        }
    }
}

//input
void ComponentBody::onMousePressEvent(QMouseEvent* event)
{

    //get pick pos
    Easy2D::Vec2 mousePos(event->localPos().x(),event->localPos().y());
    Easy2D::Vec2 pikCam=getScene()->getCamera().getWMouse(mousePos);
    auto body=getBody();
    //
    if(isEditable(body) && event->button()==Qt::LeftButton)
    {
        selectOrAddAPoint(pikCam);
    }
    else if (isEditable(body) && event->button()==Qt::RightButton)
    {
        deleteAPoint(pikCam);
    }
}
void ComponentBody::onMouseMoveEvent(QMouseEvent* event)
{
    //move camera
    //mouse pos
    Easy2D::Vec2 mousePos(event->localPos().x(),event->localPos().y());
    Easy2D::Vec2 pikCam=getScene()->getCamera().getWMouse(mousePos);
    auto& camera=getScene()->getCamera();
    //move camera
    if(QApplication::mouseButtons()==Qt::MouseButton::LeftButton)
    {
        Easy2D::Vec2 sizeWid=getScene()->getCamera().getViewport();
        const float bSize=100.0f;
        const float ibSize=1.0f/bSize+0.05;
        const Easy2D::Vec2 bsize(bSize,bSize);
        auto mindiff=-(mousePos-bsize); mindiff*=mindiff*DELTATIME*ibSize;
        auto maxdiff=-bsize+(sizeWid-mousePos); maxdiff*=-maxdiff*DELTATIME*ibSize;
        if((mousePos.x-bsize.x) < 0) camera.setTranslation(Easy2D::Vec2(mindiff.x,0));
        if((mousePos.x+bsize.x) > sizeWid.x) camera.setTranslation(Easy2D::Vec2(maxdiff.x,0));
        if((mousePos.y-bsize.y) < 0) camera.setTranslation(Easy2D::Vec2(0,-mindiff.y));
        if((mousePos.y+bsize.y) > sizeWid.y) camera.setTranslation(Easy2D::Vec2(0,-maxdiff.y));
    }
    //tmp body
    auto body=getBody();
    //move
    if(isEditable(body) && QApplication::mouseButtons()==Qt::LeftButton)
    {

        if(editShape.radiusSelect)
        {
            //Easy2D::Body::CIRCLE
            auto point=getRTMatrix().mul2D(editShape.points[0]);
            auto distance=(pikCam-point).length();
            body->changeCircleRadius(editShape.shape,distance);
        }
        else if (editShape.pointSelect>-1)
        switch (body->getShapeType(editShape.shape))
        {
            case Easy2D::Body::CIRCLE:
            case Easy2D::Body::POLYGON:
            case Easy2D::Body::CHAIN:
            editShape.points[editShape.pointSelect]=
                    getRTMatrix()
                   .getInverse()
                   .mul2D(pikCam);
            uibody->setUIVertex(editShape.points[editShape.pointSelect]);
            break;
            default:
            break;
        };
    }
}
void ComponentBody::onMouseReleaseEvent(QMouseEvent* event)
{
    //save tmp body
    auto body=getBody();
    //
    if(isEditable(body) && event->button()==Qt::LeftButton)
    {
        if(editShape.radiusSelect)
        {
            //Easy2D::Body::CIRCLE
            editShape.radiusSelect=false;
        }
        else if (editShape.pointSelect>-1)
            updateVertexs();
    }
}

//get matrix
Easy2D::Mat4 ComponentBody::getRTMatrix()
{
    if(!getLastObject()) return Easy2D::Mat4::IDENTITY;
    Easy2D::Mat4 m4RT;
    m4RT.setRotZ(getLastObject()->getRotation(true));
    m4RT.unsafeTranslation(getLastObject()->getPosition(true));


    return m4RT;
}

//draw
void ComponentBody::drawBodyCircle(const Easy2D::Body* body,
                                   Easy2D::Shape index,
                                   const Easy2D::Color& lineColor,
                                   const Easy2D::Color& pointColor)
{
    //draw
    drawCircle(body->getCirclePosition(index),
               body->getCircleRadius(index),
               lineColor,
               pointColor);
}
void ComponentBody::drawBodyPolygon(const Easy2D::Body* body,
                                    Easy2D::Shape index,
                                    const Easy2D::Color& lineColor,
                                    const Easy2D::Color& pointColor)
{
    std::vector<Easy2D::Vec2> points;
    body->getPolygonPoints(index,points);
    drawPolygon(points,
                true,
                lineColor,
                pointColor);
}
void ComponentBody::drawBodyChain(const Easy2D::Body* body,
                                  Easy2D::Shape index,
                                  const Easy2D::Color& lineColor,
                                  const Easy2D::Color& pointColor)
{
    std::vector<Easy2D::Vec2> points;
    body->getChainPoints(index,points);

    Easy2D::Vec2 pstart,pend;

    drawChain(points,
              body->getChainStartPoint(index,pstart),pstart,
              body->getChainEndPoint(index,pend),pend,
              lineColor,
              pointColor);
}

//utility
inline void ComponentBody::drawCircle(const Easy2D::Vec2& pos,
                                      float radius,
                                      const Easy2D::Color& lineColor,
                                      const Easy2D::Color& pointColor)
{
    Easy2D::Mat4 m4RT(getRTMatrix());
    //calc pos
    Easy2D::Vec2 posCircle=m4RT.mul2D(pos);
    //draw
    getScene()->drawPoint2D(posCircle,
                            Easy2D::Color(255,0,0,128),
                            pointColor);
    getScene()->drawCircle(posCircle,
                           radius,
                           lineColor);

}
inline void ComponentBody::drawPolygon(std::vector<Easy2D::Vec2> points,
                                       bool close,
                                       const Easy2D::Color& lineColor,
                                       const Easy2D::Color& pointColor)
{

    Easy2D::Mat4 m4RT(getRTMatrix());

    for(auto& point : points) point=m4RT.mul2D(point);

    getScene()->drawPathPoints(points,
                                     close,
                                     lineColor,
                                     Easy2D::Color(255,0,0,128),
                                     pointColor);
}
inline void ComponentBody::drawChain(std::vector<Easy2D::Vec2> points
                                    ,bool bStart
                                    ,const Easy2D::Vec2& start
                                    ,bool bEnd
                                    ,const Easy2D::Vec2& end
                                    ,const Easy2D::Color& lineColor
                                    ,const Easy2D::Color& pointColor)
{
    /*** matrix ***/
    Easy2D::Mat4 m4RT(getRTMatrix());
    /*** points ***/
    for(auto& point : points)
    {
        point=m4RT.mul2D(point);
    }
    /*** draw ***/
    getScene()->drawPathPoints(points,
                               false,
                               lineColor,
                               Easy2D::Color(255,0,0,128),
                               pointColor);
    /*** NORMALS ***/
    float camzoom=getScene()->getCamera().getZoom();
    for(int i=0; i<points.size()-1; ++i)
    {
        //point
        auto center=(points[i]+points[i+1])/2.0f;
        //normal
        auto dxy=points[i]-points[i+1];
        Easy2D::Vec2 normal(dxy.y,-dxy.x);
        normal.normalize();
        //draw
        getScene()->drawLine(center,
                             center+normal*20.0f*camzoom,
                             Easy2D::Color(0,0,255,128));
    }

    //rt
    if(bStart)
    {

        getScene()->drawPoint2D(m4RT.mul2D(start),
                                Easy2D::Color(0,0,255,128),
                                Easy2D::Color(0,0,255,255));
        getScene()->drawLine(m4RT.mul2D(start),
                             m4RT.mul2D(points[0]),
                             Easy2D::Color(0,0,255,128));
    }
    if(bEnd)
    {

        getScene()->drawPoint2D(m4RT.mul2D(end),
                                Easy2D::Color(0,0,255,128),
                                Easy2D::Color(0,0,255,255));
        getScene()->drawLine(m4RT.mul2D(end),
                             m4RT.mul2D(points[points.size()-1]),
                             Easy2D::Color(0,0,255,128));
    }
}

