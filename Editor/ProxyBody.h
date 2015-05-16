#ifndef PROXYBODY_H
#define PROXYBODY_H

#include <Easy2D.h>
class ProxyBody
{
protected:

    ProxyBody(){}

public:
    //////////////////////////////
    typedef DS_PTR<ProxyBody> ptr;
    //////////////////////////////
    static ProxyBody::ptr createProxyBody(Easy2D::Body* body);
    //////////////////////////////
    virtual bool isValid() const=0;
    /*
    * Listener
    */
    virtual void setBeginListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                  Easy2D::Body::Info& info)> cbBegin)=0;

    virtual void setEndListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                Easy2D::Body::Info& info)> cbEnd)=0;

    virtual void setPreSolverListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                      Easy2D::Body::Info& info,const
                                                      Easy2D::Body::Manifold& oldmf)> cbPreSolver)=0;

    virtual void setPostSolverListener(DFUNCTION<void (Easy2D::Object* tocollide,const
                                                       Easy2D::Body::Info& info,
                                                       const Easy2D::Body::Impulse& impulse)> cbPostSolver)=0;
    /*
    * body
    */
    virtual void setType(Easy2D::Body::Type)=0;
    virtual Easy2D::Body::Type getType() const=0;

    virtual void setActive(bool)=0;
    virtual bool getActive() const=0;

    virtual void setAwake(bool)=0;
    virtual bool getAwake() const=0;

    virtual void setBullet(bool)=0;
    virtual bool getBullet() const=0;

    virtual void setEnableScale(bool)=0;
    virtual bool getEnableScale() const=0;
    //void setMass(float); in fixtures
    virtual float getMass() const=0;
    //get world
    virtual Easy2D::Vec2 getWorldCenter() const=0;
    virtual Easy2D::Vec2 getWorldPoint(const Easy2D::Vec2& local) const=0;
    //get local
    virtual Easy2D::Vec2 getLocalVector(const Easy2D::Vec2& worldVector) const=0;
    virtual Easy2D::Vec2 getLocalPoint(const Easy2D::Vec2& worldPoint) const=0;
    virtual Easy2D::Vec2 getLocalCenter() const=0;

    //velocity
    virtual void setLinearVelocity(const Easy2D::Vec2&)=0;
    virtual Easy2D::Vec2 getLinearVelocity() const=0;
    virtual Easy2D::Vec2 getLinearVelocityFromWorldPoint(const Easy2D::Vec2& world) const=0;
    virtual Easy2D::Vec2 getLinearVelocityFromLocalPoint(const Easy2D::Vec2& local) const=0;

    virtual void setLinearDamping(float)=0;
    virtual float getLinearDamping() const=0;

    virtual void setAngularVelocity(float)=0;
    virtual float getAngularVelocity() const=0;

    virtual void setAngularDamping(float)=0;
    virtual float getAngularDamping() const=0;

    virtual void setFixedAngle(bool)=0;
    virtual bool getFixedAngle() const=0;

    virtual void setGravityScale(float)=0;
    virtual float getGravityScale() const=0;

    virtual void setSleepingAllowed(bool)=0;
    virtual bool getSleepingAllowed() const=0;

    /**
    * applay
    */
    virtual void applyForce(const Easy2D::Vec2& force, const Easy2D::Vec2& point, bool wake)=0;
    virtual void applyForceToCenter(const Easy2D::Vec2& force, bool wake)=0;
    virtual void applyTorque(float torque, bool wake)=0;
    virtual void applyLinearImpulse(const Easy2D::Vec2& impulse, const Easy2D::Vec2& point, bool wake)=0;
    virtual void applyAngularImpulse(float impulse, bool wake)=0;
    /*
    * Groups
    */
    virtual void setCollisionGroupMask( Easy2D::byte16 groupMask )=0;
    virtual Easy2D::byte16 getCollisionGroupMask() const=0;

    virtual void setCollisionLayerMask( Easy2D::byte16 layerMask )=0;
    virtual Easy2D::byte16 getCollisionLayerMask() const=0;
    /*
    * Shapes
    */
    virtual Easy2D::Shape createCircleShape(float radius,
                                            const Easy2D::Vec2& pos=Easy2D::Vec2::ZERO)=0;
    virtual Easy2D::Shape createBoxShape(const Easy2D::Vec2& size,
                                         const Easy2D::Vec2& pos=Easy2D::Vec2::ZERO,
                                         float angle=0.0)=0;
    virtual Easy2D::Shape createPolygonShape(const std::vector<Easy2D::Vec2>& points)=0;
    virtual Easy2D::Shape createChainShape( const std::vector<Easy2D::Vec2>& points )=0;
    virtual Easy2D::Shape createChainShape( const std::vector<Easy2D::Vec2>& points,
                                            bool startp ,
                                            const Easy2D::Vec2& adjacentStartPoint,
                                            bool endp,
                                            const Easy2D::Vec2& adjacentEndPoint )=0;
   virtual Easy2D::Shape createEdgeShape( const Easy2D::Vec2& localPositionStart,
                                          const Easy2D::Vec2& localPositionEnd)=0;
   virtual Easy2D::Shape createEdgeShape( const Easy2D::Vec2& localPositionStart,
                                          const Easy2D::Vec2& localPositionEnd,
                                          const bool hasAdjacentLocalPositionStart,
                                          const Easy2D::Vec2& adjacentLocalPositionStart,
                                          const bool hasAdjacentLocalPositionEnd,
                                          const Easy2D::Vec2& adjacentLocalPositionEnd )=0;

    virtual void setShapeDensity(Easy2D::Shape index,float density)=0;
    virtual float getShapeDensity(Easy2D::Shape index) const=0;

    virtual void setShapeFriction(Easy2D::Shape index,float friction)=0;
    virtual float getShapeFriction(Easy2D::Shape index) const=0;

    virtual void setShapeRestitution(Easy2D::Shape index,float restitution)=0;
    virtual float getShapeRestitution(Easy2D::Shape index) const=0;

    virtual void setShapeIsSensor(Easy2D::Shape index,bool isSensor)=0;
    virtual bool getShapeIsSensor(Easy2D::Shape index) const=0;

    /**
    * Shape geometry info
    */
    virtual size_t       countShape() const=0;
    virtual Easy2D::Body::GeometryType getShapeType(Easy2D::Shape index) const=0;
    //it
    virtual Easy2D::Body::ShapeIterator begin()=0;
    virtual Easy2D::Body::ShapeIterator end()=0;
    virtual Easy2D::Body::CShapeIterator begin() const=0;
    virtual Easy2D::Body::CShapeIterator cbegin() const=0;
    virtual Easy2D::Body::CShapeIterator end() const=0;
    virtual Easy2D::Body::CShapeIterator cend() const=0;
    //circle
    virtual Easy2D::Vec2  getCirclePosition(Easy2D::Shape index) const=0;
    virtual float getCircleRadius(Easy2D::Shape index) const=0;
    //polygon
    virtual void  getPolygonPoints(Easy2D::Shape index,std::vector<Easy2D::Vec2>& points) const=0;
    //chain
    virtual void  getChainPoints(Easy2D::Shape index,std::vector<Easy2D::Vec2>& points) const=0;
    virtual bool  getChainStartPoint(Easy2D::Shape index,Easy2D::Vec2& point) const=0;
    virtual bool  getChainEndPoint(Easy2D::Shape index,Easy2D::Vec2& point) const=0;
    //edge
    virtual void  getEdgePoints(Easy2D::Shape index,Easy2D::Vec2& start,Easy2D::Vec2& end) const=0;
    virtual bool  getEdgeStartPoint(Easy2D::Shape index,Easy2D::Vec2& point) const=0;
    virtual bool  getEdgeEndPoint(Easy2D::Shape index,Easy2D::Vec2& point) const=0;

    //Geometry change
    //Circle
    virtual void changeCirclePosition(Easy2D::Shape index,const Easy2D::Vec2& pos)=0;
    virtual void changeCircleRadius(Easy2D::Shape index,float r)=0;
    //Polygon
    virtual void changePolygonPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points)=0;
    //Chain
    virtual void changeChainPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points)=0;
    virtual void changeChainPoints(Easy2D::Shape index,const std::vector<Easy2D::Vec2>& points,
                                   bool startp,
                                   const Easy2D::Vec2& adjacentStartPoint,
                                   bool endp,
                                   const Easy2D::Vec2& adjacentEndPoint)=0;


    //delete a shape
    virtual void deleteShape(Easy2D::Shape index)=0;
};


#endif // PROXYBODY_H
