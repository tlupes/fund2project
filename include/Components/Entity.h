#ifndef ENTITY_H
#define ENTITY_H

#include<list>
#include<unordered_map>
#include<memory>
#include<typeindex>
#include<vector>
#include<functional>
#include <sstream>

#include "Components/EventDefines.h"

class AudioComponent;
class RenderComponent;
class IDComponent;
class InputComponent;
class MovementComponent;
class PhysicsComponent;
class WorldPositionComponent;
class StatsComponent;
class TargetComponent;
class ScriptComponent;
class ComponentBase;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

/// Contains all the components an entity has
class Entity
{
    public:
        //Constructors and Destructors
        Entity(int ID);
        virtual ~Entity();

        //Component Management
        AudioComponent* audio;
        RenderComponent* render;
        IDComponent* identification;
        InputComponent* input;
        MovementComponent* movement;
        PhysicsComponent* physics;
        WorldPositionComponent* position;
        StatsComponent* stats;
        TargetComponent* target;

        void addScript(ScriptComponent* s)      {scripts.push_back(s);}
        void removeScript(ScriptComponent* s)   {scripts.remove(s);}
        std::list<ScriptComponent*> getScripts() {return scripts;}

        int getID() {return _ID;}

        //Deferred Delete logic
        void setDelete() {_delete = true;}
        int isDeleted() {return _delete;}

        // Listener Logic
        typedef std::function<void(Events, std::vector<std::string>, Entity*)> listener;
        typedef std::function<void(ComponentBase&, Events, std::vector<std::string>, Entity*)> unboundListener;

        void addListener(std::type_index toListenTo, listener toCall);
        //void addListener(std::type_index toListenTo, ComponentBase* obj, unboundListener toCall);
        void removeListener(std::type_index toListenTo, listener toCall);
        void callListeners(std::type_index origin, Events event, std::vector<std::string> message = std::vector<std::string>());
    protected:
        int _ID;
        bool _delete;
    private:
        std::list<ScriptComponent*> scripts;
        std::unordered_map<std::type_index, std::list<listener> > componentListeners;
};

#endif // ENTITY_H
