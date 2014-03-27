#include "Components/Physics/SimpleBoxPhysics.h"
#include "Components/ComponentManager.h"
#include "Options.h"
#include "physics/PhysicsEngine.h"

SimpleBoxPhysics::SimpleBoxPhysics(unsigned int ID, int x, int y, bool isStatic, bool rotatable, bool roundedCorners, bool isSensor) : PhysicsComponent(ID)
{
    if(isStatic)
        physBodyDef.type = b2_staticBody;
    else
        physBodyDef.type = b2_dynamicBody;
    physBodyDef.position.Set(1,1);
    physBodyDef.angle = 0;
    physBodyDef.fixedRotation = !rotatable;
    physBody = physEng->_world->CreateBody(&physBodyDef);
    if(roundedCorners) {
        b2Vec2 boxVertices[8];
        float ax = .5f*x/pixelsPerMeter;
        float ay = .5f*y/pixelsPerMeter;
        boxVertices[0].Set(-ax*.95f, -ay);
        boxVertices[1].Set(-ax, -ay*.95f);
        boxVertices[2].Set(ax*.95f, -ay);
        boxVertices[3].Set(ax, -ay*.95f);
        boxVertices[4].Set(ax, ay*.95f);
        boxVertices[5].Set(ax*.95f, ay);
        boxVertices[6].Set(-ax*.95f, ay);
        boxVertices[7].Set(-ax, ay*.95f);
        /*boxVertices[0].Set(-0.5f, -0.75f);
        boxVertices[1].Set(-0.25f, -0.975f);
        boxVertices[2].Set(0.25f, -0.975f);
        boxVertices[3].Set(0.5f, -0.75f);
        boxVertices[4].Set(0.5f, 0.9f);
        boxVertices[5].Set(0.4f, 0.975f);
        boxVertices[6].Set(-0.4f, 0.975f);
        boxVertices[7].Set(-0.5f, 0.9f);*/

        boxShape.Set(boxVertices, 8);
        footListener = NULL;
    }
    else {
        boxShape.SetAsBox(.5*x/pixelsPerMeter,.5*y/pixelsPerMeter);
    }
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    boxFixtureDef.friction = 1;
    boxFixtureDef.isSensor = isSensor;
    physBody->CreateFixture(&boxFixtureDef);
    screenHeight = atoi(Options::instance().get("screen_height").c_str());
    WorldPositionComponent* position = ComponentManager::getInst().posSym.getComponent(getID());

    if(!isStatic && !rotatable) { //All of the sensors!!!
        //foot
        boxShape.SetAsBox(x*.45/pixelsPerMeter, 0.1, b2Vec2(0,-y/(2.0f*pixelsPerMeter)), 0);
        boxFixtureDef.isSensor = true;
        b2Fixture* footSensorFixture = physBody->CreateFixture(&boxFixtureDef);
        footSensorFixture->SetUserData( (void*)(getID()*10+1) );
        footListener = new FootContactListener(getID()*10+1, this);
        physEng->contactListeners.addListener(footListener);
        //head
        boxShape.SetAsBox(x*.45/pixelsPerMeter, 0.1, b2Vec2(0,y/(2.0f*pixelsPerMeter)), 0);
        boxFixtureDef.isSensor = true;
        b2Fixture* headSensorFixture = physBody->CreateFixture(&boxFixtureDef);
        headSensorFixture->SetUserData( (void*)(getID()*10+2) );
        headListener = new FootContactListener(getID()*10+2, this);
        physEng->contactListeners.addListener(headListener);
        //left
        boxShape.SetAsBox(.1f, y*.45/pixelsPerMeter, b2Vec2(-x/(2.0f*pixelsPerMeter),0), 0);
        boxFixtureDef.isSensor = true;
        b2Fixture* leftSensorFixture = physBody->CreateFixture(&boxFixtureDef);
        leftSensorFixture->SetUserData( (void*)(getID()*10+3) );
        leftListener = new FootContactListener(getID()*10+3, this);
        physEng->contactListeners.addListener(leftListener);
        //right
        boxShape.SetAsBox(.1f, y*.45/pixelsPerMeter, b2Vec2(x/(2.0f*pixelsPerMeter),0), 0);
        boxFixtureDef.isSensor = true;
        b2Fixture* rightSensorFixture = physBody->CreateFixture(&boxFixtureDef);
        rightSensorFixture->SetUserData( (void*)(getID()*10+4) );
        rightListener = new FootContactListener(getID()*10+4, this);
        physEng->contactListeners.addListener(rightListener);
    }

    if(position!=NULL) {
        physBody->SetTransform(b2Vec2(position->getPosition().x/pixelsPerMeter, -position->getPosition().y/pixelsPerMeter),physBody->GetAngle());
    }
}

SimpleBoxPhysics::~SimpleBoxPhysics()
{
    //dtor
}

bool SimpleBoxPhysics::onGround() {
    if(footListener!=NULL)
        return footListener->onGround();
    else
        return true;
}

bool SimpleBoxPhysics::onLeft() {
    if(left!=NULL)
        return leftListener->onGround();
    else
        return true;
}

bool SimpleBoxPhysics::onRight() {
    if(rightListener!=NULL)
        return rightListener->onGround();
    else
        return true;
}

bool SimpleBoxPhysics::onTop() {
    if(headListener!=NULL)
        return headListener->onGround();
    else
        return true;
}

void SimpleBoxPhysics::go(sf::Time frameTime) {
    WorldPositionComponent* position = ComponentManager::getInst().posSym.getComponent(getID());
    //The body is the one that contains the position, velocity, etc. not the body definition
    //screenHeight
    //Times 32, as 32 pixels is ~one meter
    position->setPosition(sf::Vector2f((physBody->GetPosition().x)*pixelsPerMeter, -((physBody->GetPosition().y)*pixelsPerMeter)),false);
    position->setRotation(physBody->GetAngle());
    //cout << physBody->GetPosition().x << " " << physBody->GetPosition().y << " " << physBodyDef.awake << endl;
}

void SimpleBoxPhysics::setRotatable(bool input) {
    //physBodyDef.fixedRotation = !input;
}


void FootContactListener::BeginContact(b2Contact* contact) {
  //check if fixture A was the foot sensor
  void* fixtureUserData = contact->GetFixtureA()->GetUserData();
  if ( (int)fixtureUserData == findID )
    if( !contact->GetFixtureB()->IsSensor())
      onGroundNum++;
  //check if fixture B was the foot sensor
  fixtureUserData = contact->GetFixtureB()->GetUserData();
  if ( (int)fixtureUserData == findID )
    if(!contact->GetFixtureA()->IsSensor())
      onGroundNum++;
}

void FootContactListener::EndContact(b2Contact* contact) {
  //check if fixture A was the foot sensor
  void* fixtureUserData = contact->GetFixtureA()->GetUserData();
  if ( (int)fixtureUserData == findID )
    if(!contact->GetFixtureB()->IsSensor())
      onGroundNum--;
  //check if fixture B was the foot sensor
  fixtureUserData = contact->GetFixtureB()->GetUserData();
  if ( (int)fixtureUserData == findID )
    if(!contact->GetFixtureA()->IsSensor())
      onGroundNum--;
}
