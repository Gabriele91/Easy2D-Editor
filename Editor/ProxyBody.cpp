#include "ProxyBody.h"


class IProxyBody : public ProxyBody
{

    Easy2D::Body* body;

public:

    IProxyBody(Easy2D::Body* body):body(body)
    {
    }
    virtual bool isValid() const;
    /*
    * Listener
    */
    virtual void setBeginListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                  Easy2D::Body::Info& info)> cbBegin);
    virtual void setEndListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                Easy2D::Body::Info& info)> cbEnd);
    virtual void setPreSolverListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                      Easy2D::Body::Info& info,const
                                                      Easy2D::Body::Manifold& oldmf)> cbPreSolver);
    virtual void setPostSolverListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                       Easy2D::Body::Info& info,
                                                       const Easy2D::Body::Impulse& impulse)> cbPostSolver);
    /*
    * body
    */
    virtual void setType(Easy2D::Body::Type);
    virtual Easy2D::Body::Type getType() const;

    virtual void setActive(bool);
    virtual bool getActive() const;

    virtual void setAwake(bool);
    virtual bool getAwake() const;

    virtual void setBullet(bool);
    virtual bool getBullet() const;

    virtual void setEnableScale(bool);
    virtual bool getEnableScale() const;
    //void setMass(float); in fixtures
    virtual float getMass() const;
    //get world
    virtual Easy2D::Vec2 getWorldCenter() const;
    virtual Easy2D::Vec2 getWorldPoint(const Easy2D::Vec2& local) const;
    //get local
    virtual Easy2D::Vec2 getLocalVector(const Easy2D::Vec2& worldVector) const;
    virtual Easy2D::Vec2 getLocalPoint(const Easy2D::Vec2& worldPoint) const;
    virtual Easy2D::Vec2 getLocalCenter() const;

    //velocity
    virtual void setLinearVelocity(const Easy2D::Vec2&);
    virtual Easy2D::Vec2 getLinearVelocity() const;
    virtual Easy2D::Vec2 getLinearVelocityFromWorldPoint(const Easy2D::Vec2& world) const;
    virtual Easy2D::Vec2 getLinearVelocityFromLocalPoint(const Easy2D::Vec2& local) const;

    virtual void setLinearDamping(float);
    virtual float getLinearDamping() const;

    virtual void setAngularVelocity(float);
    virtual float getAngularVelocity() const;

    virtual void setAngularDamping(float);
    virtual float getAngularDamping() const;

    virtual void setFixedAngle(bool);
    virtual bool getFixedAngle() const;

    virtual void setGravityScale(float);
    virtual float getGravityScale() const;

    virtual void setSleepingAllowed(bool);
    virtual bool getSleepingAllowed() const;

    /**
    * applay
    */
    virtual void applyForce(const Easy2D::Vec2& force, const Easy2D::Vec2& point, bool wake);
    virtual void applyForceToCenter(const Easy2D::Vec2& force, bool wake);
    virtual void applyTorque(float torque, bool wake);
    virtual void applyLinearImpulse(const Easy2D::Vec2& impulse, const Easy2D::Vec2& point, bool wake);
    virtual void applyAngularImpulse(float impulse, bool wake);
    /*
    * Groups
    */
    virtual void setCollisionGroupMask( Easy2D::byte16 groupMask );
    virtual Easy2D::byte16 getCollisionGroupMask() const;

    virtual void setCollisionLayerMask( Easy2D::byte16 layerMask );
    virtual Easy2D::byte16 getCollisionLayerMask() const;
    /*
    * Shapes
    */
    virtual Easy2D::Shape createCircleShape(float radius,
                                            const Easy2D::Vec2& pos=Easy2D::Vec2::ZERO);
    virtual Easy2D::Shape createBoxShape(const Easy2D::Vec2& size,
                                         const Easy2D::Vec2& pos=Easy2D::Vec2::ZERO,
                                         float angle=0.0);
    virtual Easy2D::Shape createPolygonShape(const std::vector<Easy2D::Vec2>& points);
    virtual Easy2D::Shape createChainShape( const std::vector<Easy2D::Vec2>& points );
    virtual Easy2D::Shape createChainShape( const std::vector<Easy2D::Vec2>& points,
                                            bool startp ,
                                            const Easy2D::Vec2& adjacentStartPoint,
                                            bool endp,
                                            const Easy2D::Vec2& adjacentEndPoint );
   virtual Easy2D::Shape createEdgeShape( const Easy2D::Vec2& localPositionStart,
                                          const Easy2D::Vec2& localPositionEnd);
   virtual Easy2D::Shape createEdgeShape( const Easy2D::Vec2& localPositionStart,
                                          const Easy2D::Vec2& localPositionEnd,
                                          const bool hasAdjacentLocalPositionStart,
                                          const Easy2D::Vec2& adjacentLocalPositionStart,
                                          const bool hasAdjacentLocalPositionEnd,
                                          const Easy2D::Vec2& adjacentLocalPositionEnd );

    virtual void setShapeDensity(Easy2D::Shape index,float density);
    virtual float getShapeDensity(Easy2D::Shape index) const;

    virtual void setShapeFriction(Easy2D::Shape index,float friction);
    virtual float getShapeFriction(Easy2D::Shape index) const;

    virtual void setShapeRestitution(Easy2D::Shape index,float restitution);
    virtual float getShapeRestitution(Easy2D::Shape index) const;

    virtual void setShapeIsSensor(Easy2D::Shape index,bool isSensor);
    virtual bool getShapeIsSensor(Easy2D::Shape index) const;

    /**
    * Shape geometry info
    */
    virtual size_t       countShape() const;
    virtual Easy2D::Body::GeometryType getShapeType(Easy2D::Shape index) const;
    //it
    virtual Easy2D::Body::ShapeIterator begin();
    virtual Easy2D::Body::ShapeIterator end();
    virtual Easy2D::Body::CShapeIterator begin() const;
    virtual Easy2D::Body::CShapeIterator cbegin() const;
    virtual Easy2D::Body::CShapeIterator end() const;
    virtual Easy2D::Body::CShapeIterator cend() const;
    //circle
    virtual Easy2D::Vec2  getCirclePosition(Easy2D::Shape index) const;
    virtual float getCircleRadius(Easy2D::Shape index) const;
    //polygon
    virtual void  getPolygonPoints(Easy2D::Shape index,std::vector<Easy2D::Vec2>& points) const;
    //chain
    virtual void  getChainPoints(Easy2D::Shape index,std::vector<Easy2D::Vec2>& points) const;
    virtual bool  getChainStartPoint(Easy2D::Shape index,Easy2D::Vec2& point) const;
    virtual bool  getChainEndPoint(Easy2D::Shape index,Easy2D::Vec2& point) const;
    //edge
    virtual void  getEdgePoints(Easy2D::Shape index,Easy2D::Vec2& start,Easy2D::Vec2& end) const;
    virtual bool  getEdgeStartPoint(Easy2D::Shape index,Easy2D::Vec2& point) const;
    virtual bool  getEdgeEndPoint(Easy2D::Shape index,Easy2D::Vec2& point) const;

    //Geometry change
    //Circle
    virtual void changeCirclePosition(Easy2D::Shape index,const Easy2D::Vec2& pos);
    virtual void changeCircleRadius(Easy2D::Shape index,float r);
    //Polygon
    virtual void changePolygonPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points);
    //Chain
    virtual void changeChainPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points);
    virtual void changeChainPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points,
                                   bool startp,
                                   const Easy2D::Vec2& adjacentStartPoint,
                                   bool endp,
                                   const Easy2D::Vec2& adjacentEndPoint);


    //delete a shape
    virtual void deleteShape(Easy2D::Shape index);
};
///
ProxyBody::ptr ProxyBody::createProxyBody(Easy2D::Body* body)
{
    return ProxyBody::ptr(new IProxyBody(body));
}
///
bool IProxyBody::isValid() const
{
    return body!=nullptr;
}
/*
* Listener
*/
void IProxyBody::setBeginListener(DFUNCTION<void (Easy2D::Object* tocollide,
                                                 const Easy2D::Body::Info& info)> cbBegin)
{
    body->setBeginListener(cbBegin);
}
void IProxyBody::setEndListener(DFUNCTION<void (Easy2D::Object* tocollide,
                                               const Easy2D::Body::Info& info)> cbEnd)
{
    body->setEndListener(cbEnd);
}
void IProxyBody::setPreSolverListener(DFUNCTION<void (Easy2D::Object* tocollide,
                                                     const Easy2D::Body::Info& info,
                                                     const Easy2D::Body::Manifold& oldmf)> cbPreSolver)
{
    body->setPreSolverListener(cbPreSolver);
}
void IProxyBody::setPostSolverListener(DFUNCTION<void (Easy2D::Object* tocollide,
                                                      const Easy2D::Body::Info& info,
                                                      const Easy2D::Body::Impulse& impulse)> cbPostSolver)
{
    body->setPostSolverListener(cbPostSolver);
}
/*
* body
*/
void IProxyBody::setType(Easy2D::Body::Type type)
{
    body->setType(type);
}
Easy2D::Body::Type IProxyBody::getType() const
{
    return body->getType();
}

void IProxyBody::setActive(bool active)
{
    body->setActive(active);
}
bool IProxyBody::getActive() const
{
    return body->getActive();
}

void IProxyBody::setAwake(bool awake)
{
    body->setAwake(awake);
}
bool IProxyBody::getAwake() const
{
    return body->getAwake();
}

void IProxyBody::setBullet(bool bullet)
{
    return body->setBullet(bullet);
}
bool IProxyBody::getBullet() const
{
    return body->getBullet();
}


void IProxyBody::setEnableScale(bool escale)
{
    body->setEnableScale(escale);
}
bool IProxyBody::getEnableScale() const
{
    return body->getEnableScale();
}

float IProxyBody::getMass() const
{
    return body->getMass();
}
//get world
Easy2D::Vec2 IProxyBody::getWorldCenter() const
{
    return body->getWorldCenter();
}
Easy2D::Vec2 IProxyBody::getWorldPoint(const Easy2D::Vec2& local) const
{
    return body->getWorldPoint(local);
}
//get local
Easy2D::Vec2 IProxyBody::getLocalVector(const Easy2D::Vec2& worldVector) const
{
    return body->getLocalVector(worldVector);
}
Easy2D::Vec2 IProxyBody::getLocalPoint(const Easy2D::Vec2& worldPoint) const
{
    return body->getLocalPoint(worldPoint);
}
Easy2D::Vec2 IProxyBody::getLocalCenter() const
{
    return body->getLocalCenter();
}

//velocity
void IProxyBody::setLinearVelocity(const Easy2D::Vec2& velocity)
{
    body->setLinearVelocity(velocity);
}
Easy2D::Vec2 IProxyBody::getLinearVelocity() const
{
    return body->getLinearVelocity();
}
Easy2D::Vec2 IProxyBody::getLinearVelocityFromWorldPoint(const Easy2D::Vec2& world) const
{
    return body->getLinearVelocityFromWorldPoint(world);
}
Easy2D::Vec2 IProxyBody::getLinearVelocityFromLocalPoint(const Easy2D::Vec2& local) const
{
    return body->getLinearVelocityFromLocalPoint(local);
}

void IProxyBody::setLinearDamping(float damping)
{
    body->setLinearDamping(damping);
}
float IProxyBody::getLinearDamping() const
{
    return body->getLinearDamping();
}

void IProxyBody::setAngularVelocity(float angular)
{
    body->setAngularVelocity(angular);
}
float IProxyBody::getAngularVelocity() const
{
    return body->getAngularVelocity();
}

void IProxyBody::setAngularDamping(float adamping)
{
    body->setAngularDamping(adamping);
}
float IProxyBody::getAngularDamping() const
{
    return body->getAngularDamping();
}

void IProxyBody::setFixedAngle(bool fangle)
{
    body->setFixedAngle(fangle);
}
bool IProxyBody::getFixedAngle() const
{
    return body->getFixedAngle();
}

void IProxyBody::setGravityScale(float scale)
{
    body->setGravityScale(scale);
}
float IProxyBody::getGravityScale() const
{
    return body->getGravityScale();
}

void IProxyBody::setSleepingAllowed(bool sallowed)
{
    body->setSleepingAllowed(sallowed);
}
bool IProxyBody::getSleepingAllowed() const
{
    return body->getSleepingAllowed();
}

/**
* applay
*/
void IProxyBody::applyForce(const Easy2D::Vec2& force, const Easy2D::Vec2& point, bool wake)
{
    body->applyForce(force,point,wake);
}
void IProxyBody::applyForceToCenter(const Easy2D::Vec2& force, bool wake)
{
    body->applyForceToCenter(force,wake);
}
void IProxyBody::applyTorque(float torque, bool wake)
{
    body->applyTorque(torque,wake);
}
void IProxyBody::applyLinearImpulse(const Easy2D::Vec2& impulse, const Easy2D::Vec2& point, bool wake)
{
    body->applyLinearImpulse(impulse,point,wake);
}
void IProxyBody::applyAngularImpulse(float impulse, bool wake)
{
    body->applyAngularImpulse(impulse,wake);
}
/*
* Groups
*/
void IProxyBody::setCollisionGroupMask( Easy2D::byte16 groupMask )
{
    body->setCollisionGroupMask(groupMask);
}
Easy2D::byte16 IProxyBody::getCollisionGroupMask() const
{
    return body->getCollisionGroupMask();
}

void IProxyBody::setCollisionLayerMask( Easy2D::byte16 layerMask )
{
    body->setCollisionLayerMask(layerMask);
}
Easy2D::byte16 IProxyBody::getCollisionLayerMask() const
{
    return body->getCollisionLayerMask();
}
/*
* Shapes
*/
Easy2D::Shape IProxyBody::createCircleShape(float radius,
                                           const Easy2D::Vec2& pos)
{
    return body->createCircleShape(radius,pos);
}
Easy2D::Shape IProxyBody::createBoxShape(const Easy2D::Vec2& size,
                                        const Easy2D::Vec2& pos,
                                        float angle)
{
    return body->createBoxShape(size,pos,angle);
}
Easy2D::Shape IProxyBody::createPolygonShape(const std::vector<Easy2D::Vec2>& points)
{
    return body->createPolygonShape(points);
}
Easy2D::Shape IProxyBody::createChainShape( const std::vector<Easy2D::Vec2>& points )
{
    return body->createChainShape(points);
}
Easy2D::Shape IProxyBody::createChainShape( const std::vector<Easy2D::Vec2>& points,
                                           bool startp ,
                                           const Easy2D::Vec2& adjacentStartPoint,
                                           bool endp,
                                           const Easy2D::Vec2& adjacentEndPoint )
{
    return body->createChainShape(points,
                                  startp,adjacentStartPoint,
                                  endp,adjacentEndPoint);
}
Easy2D::Shape IProxyBody::createEdgeShape( const Easy2D::Vec2& localPositionStart,
                                          const Easy2D::Vec2& localPositionEnd)
{
    return body->createEdgeShape(localPositionStart,localPositionEnd);
}
Easy2D::Shape IProxyBody::createEdgeShape( const Easy2D::Vec2& localPositionStart,
                                      const Easy2D::Vec2& localPositionEnd,
                                      const bool hasAdjacentLocalPositionStart,
                                      const Easy2D::Vec2& adjacentLocalPositionStart,
                                      const bool hasAdjacentLocalPositionEnd,
                                      const Easy2D::Vec2& adjacentLocalPositionEnd )
{
    return body->createEdgeShape(localPositionStart,localPositionEnd,
                                 hasAdjacentLocalPositionStart,adjacentLocalPositionStart,
                                 hasAdjacentLocalPositionEnd,adjacentLocalPositionEnd);
}

void IProxyBody::setShapeDensity(Easy2D::Shape index,float density)
{
    body->setShapeDensity(index,density);
}
float IProxyBody::getShapeDensity(Easy2D::Shape index) const
{
    return body->getShapeDensity(index);
}

void IProxyBody::setShapeFriction(Easy2D::Shape index,float friction)
{
    body->setShapeFriction(index,friction);
}
float IProxyBody::getShapeFriction(Easy2D::Shape index) const
{
    return body->getShapeFriction(index);
}

void IProxyBody::setShapeRestitution(Easy2D::Shape index,float restitution)
{
    return body->setShapeRestitution(index,restitution);
}
float IProxyBody::getShapeRestitution(Easy2D::Shape index) const
{
    return body->getShapeRestitution(index);
}

void IProxyBody::setShapeIsSensor(Easy2D::Shape index,bool isSensor)
{
    body->setShapeIsSensor(index,isSensor);
}
bool IProxyBody::getShapeIsSensor(Easy2D::Shape index) const
{
    return body->getShapeIsSensor(index);
}

/**
* Shape geometry info
*/
size_t IProxyBody::countShape() const
{
    return body->countShape();
}
Easy2D::Body::GeometryType IProxyBody::getShapeType(Easy2D::Shape index) const
{
    return body->getShapeType(index);
}
//it
Easy2D::Body::ShapeIterator IProxyBody::begin()
{
    return body->begin();
}
Easy2D::Body::ShapeIterator IProxyBody::end()
{
    return body->end();
}
Easy2D::Body::CShapeIterator IProxyBody::begin() const
{
    const Easy2D::Body& cbody=*body;
    return cbody.begin();
}
Easy2D::Body::CShapeIterator IProxyBody::cbegin() const
{
    return body->cbegin();
}
Easy2D::Body::CShapeIterator IProxyBody::end() const
{
    const Easy2D::Body& cbody=*body;
    return cbody.end();
}
Easy2D::Body::CShapeIterator IProxyBody::cend() const
{
    return body->cend();
}
//circle
Easy2D::Vec2 IProxyBody::getCirclePosition(Easy2D::Shape index) const
{
    return body->getCirclePosition(index);
}
float IProxyBody::getCircleRadius(Easy2D::Shape index) const
{
    return body->getCircleRadius(index);
}
//polygon
void  IProxyBody::getPolygonPoints(Easy2D::Shape index,std::vector<Easy2D::Vec2>& points) const
{
    body->getPolygonPoints(index,points);
}
//chain
void  IProxyBody::getChainPoints(Easy2D::Shape index,std::vector<Easy2D::Vec2>& points) const
{
    body->getChainPoints(index,points);
}
bool  IProxyBody::getChainStartPoint(Easy2D::Shape index,Easy2D::Vec2& point) const
{
    return body->getChainStartPoint(index,point);
}
bool  IProxyBody::getChainEndPoint(Easy2D::Shape index,Easy2D::Vec2& point) const
{
    return body->getChainEndPoint(index,point);
}
//edge
void  IProxyBody::getEdgePoints(Easy2D::Shape index,Easy2D::Vec2& start,Easy2D::Vec2& end) const
{
    body->getEdgePoints(index,start,end);
}
bool  IProxyBody::getEdgeStartPoint(Easy2D::Shape index,Easy2D::Vec2& point) const
{
    return body->getEdgeStartPoint(index,point);
}
bool  IProxyBody::getEdgeEndPoint(Easy2D::Shape index,Easy2D::Vec2& point) const
{
    return body->getEdgeEndPoint(index,point);
}

//Geometry change
//Circle
void IProxyBody::changeCirclePosition(Easy2D::Shape index,const Easy2D::Vec2& pos)
{
    body->changeCirclePosition(index,pos);
}
void IProxyBody::changeCircleRadius(Easy2D::Shape index,float r)
{
    body->changeCircleRadius(index,r);
}
//Polygon
void IProxyBody::changePolygonPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points)
{
    body->changePolygonPoints(index,points);
}
//Chain
void IProxyBody::changeChainPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points)
{
    body->changeChainPoints(index,points);
}
void IProxyBody::changeChainPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points,
                                  bool startp,
                                  const Easy2D::Vec2& adjacentStartPoint,
                                  bool endp,
                                  const Easy2D::Vec2& adjacentEndPoint)
{
    body->changeChainPoints(index,points,
                            startp,adjacentStartPoint,
                            endp,adjacentEndPoint);
}


//delete a shape
void IProxyBody::deleteShape(Easy2D::Shape index)
{
    body->deleteShape(index);
}

