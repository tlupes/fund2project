#ifndef WORLDPOSITIONCOMPONENT_H
#define WORLDPOSITIONCOMPONENT_H

#include "Components/ComponentBase.h"
#include <SFML/System/Vector2.hpp>

class PhysicsComponent;

///Simple components which contains information about the location of the object defined by the component collection
class WorldPositionComponent : public ComponentBase {
    public:
        ///Constructor
        /** \param ID The ID of the associated componnets
        \param position The XY coordinate (In world coordinates)
        \param layer the rendering layer of the object
        \param rotation the rotation of the object **/
        WorldPositionComponent(sf::Vector2f position = sf::Vector2f(0,0), int layer = 0, float rotation = 0);

        /// Copy Constructor
        WorldPositionComponent(WorldPositionComponent& old);

        void go(sf::Time frameTime, Entity* entity);

        void setPosition(sf::Vector2f input, PhysicsComponent* = nullptr, bool awaken = true);
        ///Moves the object
        /** IE (5,5) will move the object 5 to the left and 5 down **/
        void move(sf::Vector2f input, PhysicsComponent* = nullptr, bool awaken = true);
        sf::Vector2f getPosition() {
            return position;}
        void setRotation(float input, PhysicsComponent* = nullptr, bool awaken = true);
        float getRotation() {return rotation;}
        int getLayer();
        void setLayer(int);
    protected:
    private:
        sf::Vector2f position;
        float rotation;
        int layer;
        static int PPM;
};

#endif // WORLDPOSITIONCOMPONENT_H
