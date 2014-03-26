#ifndef SIMPLEBOXPHYSICS_H
#define SIMPLEBOXPHYSICS_H

#include "Components\Physics\PhysicsComponent.h"

class FootContactListener;

class SimpleBoxPhysics : public PhysicsComponent
{
    public:
        SimpleBoxPhysics() : PhysicsComponent() {}
        SimpleBoxPhysics(unsigned int, int x = 32, int y = 32, bool isStatic = false, bool rotatable = true, bool roundedCorners = false, bool isSensor = false);
        virtual ~SimpleBoxPhysics();
        void setRotatable(bool);
        void go(sf::Time);
        bool onGround();
    protected:
    private:
        FootContactListener* footListener; // TODO (Thomas Luppi#1#03/25/14): Add sensors all the way arround
        b2BodyDef physBodyDef;
        b2PolygonShape boxShape;
        b2FixtureDef boxFixtureDef;
        int screenHeight; //Need to convert Box2d to SFML coordinates
};

class FootContactListener : public b2ContactListener
{
public:
    FootContactListener() : findID(0), origin(0) {}
    FootContactListener(unsigned int findID, PhysicsComponent* origin) : findID(findID), origin(origin), onGroundNum(0) {}
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    bool onGround() {return onGroundNum>0;}
private:
    int onGroundNum;
    unsigned int findID;
    PhysicsComponent* origin;
};

#endif // SIMPLEBOXPHYSICS_H
