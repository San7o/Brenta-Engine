#include "catch_amalgamated.hpp"
#include "world.h"

#include <algorithm>
#include <unordered_map>
#include <any>

using namespace ECS::Types;

TEST_CASE("Get ECS without inizialization")
{
    SECTION("Get ECS world entities without inizialization")
    {
        std::set<Entity>* entities = ECS::World::getEntities();
        REQUIRE(entities == nullptr);
    }

    SECTION("Get ECS world systems without inizialization")
    {
        std::set<SPtr<System>, CompareSharedPtr>* systems = ECS::World::getSystems();
        REQUIRE(systems == nullptr);
    }

    SECTION("Get ECS world resources without inizialization")
    {
        UMap<ResourceName, Resource>* resources = ECS::World::getResources();
        REQUIRE(resources == nullptr);
    }

    SECTION("Get ECS world components without inizialization")
    {
        UMapVec<ComponentName, Component>* components = ECS::World::getComponents();
        REQUIRE(components == nullptr);
    }
}

TEST_CASE("Get ECS from world after initialization")
{
    ECS::World::Init();

    SECTION("Get ECS world entities after initialization")
    {
        std::set<Entity>* entities = ECS::World::getEntities();
        REQUIRE(entities != nullptr);
        REQUIRE(entities->empty());
    }

    SECTION("Get ECS world systems after initialization")
    {
        std::set<SPtr<System>, CompareSharedPtr>* systems = ECS::World::getSystems();
        REQUIRE(systems != nullptr);
        REQUIRE(systems->empty());
    }

    SECTION("Get ECS world resources after initialization")
    {
        UMap<ResourceName, Resource>* resources = ECS::World::getResources();
        REQUIRE(resources != nullptr);
        REQUIRE(resources->empty());
    }

    SECTION("Get ECS world components after initialization")
    {
        UMapVec<ComponentName, Component>* components = ECS::World::getComponents();
        REQUIRE(components != nullptr);
        REQUIRE(components->empty());
    }

    ECS::World::Delete();
}

TEST_CASE("Add and remove entities to ECS world")
{
    ECS::World::Init();
    
    SECTION("Add and remove an entity to ECS world")
    {
        Entity entity = ECS::World::NewEntity();
        REQUIRE(entity != -1);

        std::set<Entity>* entities = ECS::World::getEntities();
        REQUIRE(entities != nullptr);
        REQUIRE(entities->size() == 1);
        REQUIRE(entities->find(entity) != entities->end());

        ECS::World::RemoveEntity(entity);

        entities = ECS::World::getEntities();
        REQUIRE(entities != nullptr);
        REQUIRE(entities->empty());
        REQUIRE(entities->size() == 0);
        REQUIRE(entities->find(entity) == entities->end());
    }

    SECTION("Add multiple entities and remove them to ECS world")
    {
        Entity entity1 = ECS::World::NewEntity();
        Entity entity2 = ECS::World::NewEntity();
        Entity entity3 = ECS::World::NewEntity();

        REQUIRE(entity1 != -1);
        REQUIRE(entity2 != -1);
        REQUIRE(entity3 != -1);

        std::set<Entity>* entities = ECS::World::getEntities();
        REQUIRE(entities != nullptr);
        REQUIRE(entities->size() == 3);
        REQUIRE(entities->find(entity1) != entities->end());
        REQUIRE(entities->find(entity2) != entities->end());
        REQUIRE(entities->find(entity3) != entities->end());

        ECS::World::RemoveEntity(entity1);
        ECS::World::RemoveEntity(entity2);
        ECS::World::RemoveEntity(entity3);
        REQUIRE(entities->size() == 0);
    }

    ECS::World::Delete();
}

TEST_CASE("Add and remove components from the ECS world")
{
    ECS::World::Init();
        
    SECTION("Add and remove a component to ECS world")
    {
        Entity e = ECS::World::NewEntity();
        auto component = std::make_shared<Component>();
        ECS::World::AddComponent(e, "test", component);

        UMapVec<ComponentName, Component>* components = ECS::World::getComponents();
        REQUIRE(components != nullptr);
        REQUIRE(components->size() == 1);
        REQUIRE(components->find("test") != components->end());
        REQUIRE(*(components->at("test").at(0).get()) == *component);

        ECS::World::RemoveComponent(*component);
        REQUIRE(components->at("test").size() == 0);
    }

    SECTION("Add multiple components and remove them to ECS world")
    {
        Entity e = ECS::World::NewEntity();
        auto component1 = std::make_shared<Component>();
        auto component2 = std::make_shared<Component>();
        auto component3 = std::make_shared<Component>();

        ECS::World::AddComponent(e, "test1", component1);
        ECS::World::AddComponent(e, "test1", component2);
        ECS::World::AddComponent(e, "test3", component3);

        UMapVec<ComponentName, Component>* components = ECS::World::getComponents();
        REQUIRE(components != nullptr);
        REQUIRE(components->size() == 2);
        REQUIRE(components->find("test1") != components->end());
        REQUIRE(components->find("test1") != components->end());
        REQUIRE(components->find("test3") != components->end());
        REQUIRE(components->at("test1").size() == 2);
        REQUIRE(components->at("test3").size() == 1);

        ECS::World::RemoveComponent(*component1);
        ECS::World::RemoveComponent(*component2);
        ECS::World::RemoveComponent(*component3);
        REQUIRE(components->at("test1").size() == 0);
        REQUIRE(components->at("test3").size() == 0);
    }

    SECTION("Add, modify and remove custom component")
    {
        /* Create custom component */
        struct CustomComponent : Component {
            int value;
            CustomComponent(int value) : value(value) {}
        };

        Entity e = ECS::World::NewEntity();
        auto custom_component = std::make_shared<CustomComponent>(10);

        /* Add custom component to ECS world */
        ECS::World::AddComponent(e, "CustomComponent", custom_component);

        UMapVec<ComponentName, Component>* components = ECS::World::getComponents();
        REQUIRE(components != nullptr);
        REQUIRE(components->size() == 1);
        REQUIRE(components->find("CustomComponent") != components->end());
        REQUIRE(components->at("CustomComponent").size() == 1);
        REQUIRE(*(components->at("CustomComponent").at(0)) == *custom_component);

        /* Modify custom component */
        custom_component->value = 20;

        /* Get the custom component from the ECS world */
        CustomComponent* custom_component_instance =            
            static_cast<CustomComponent*>(components->at("CustomComponent").at(0).get());
        REQUIRE(custom_component_instance->value == 20);

        ECS::World::RemoveComponent(*custom_component);
        REQUIRE(components->at("CustomComponent").size() == 0);
    }

    ECS::World::Delete();
}

TEST_CASE("Add and remove systems from the ECS world")
{
    ECS::World::Init();
    
    SECTION("Add and remove a system to ECS world")
    {
        auto system = std::make_shared<System>("test", []{});
        ECS::World::AddSystem(system);

        auto systems = ECS::World::getSystems();
        REQUIRE(systems != nullptr);
        REQUIRE(systems->size() == 1);
        REQUIRE(systems->find(system) != systems->end());

        ECS::World::RemoveSystem("test");
        REQUIRE(systems->size() == 0);
    }

    SECTION("Add multiple systems and remove them to ECS world")
    {
        auto system1 = std::make_shared<System>("test1", []{});
        auto system2 = std::make_shared<System>("test1", []{});
        auto system3 = std::make_shared<System>("test3", []{});

        ECS::World::AddSystem(system1);
        ECS::World::AddSystem(system2);
        ECS::World::AddSystem(system3);

        auto systems = ECS::World::getSystems();
        REQUIRE(systems != nullptr);
        /* No duplicate systems with the same name */
        REQUIRE(systems->size() == 2);
        REQUIRE(systems->find(system1) != systems->end());
        REQUIRE(systems->find(system3) != systems->end());

        ECS::World::RemoveSystem("test1");
        ECS::World::RemoveSystem("test3");
        REQUIRE(systems->size() == 0);
    }

    ECS::World::Delete();
}

TEST_CASE("Add and remove resources from the ECS world")
{
    ECS::World::Init();

    SECTION("Add and remove a resource to ECS world")
    {
        struct MyResource : Resource {
            int value;
            MyResource(ResourceName name, int value) : Resource(name), value(value) {}
        };
        auto resource = std::make_shared<MyResource>("myResource", 10);
        ECS::World::AddResource(resource);

        auto resources = ECS::World::getResources();
        REQUIRE(resources != nullptr);
        REQUIRE(resources->size() == 1);
        REQUIRE(resources->find("myResource") != resources->end());
        REQUIRE(resources->at("myResource") == resource);

        ECS::World::RemoveResource(resource->name);
        REQUIRE(resources->size() == 0);
    }
    
    ECS::World::Delete();
}

TEST_CASE("Query components from the ECS world")
{
    ECS::World::Init();
    
    SECTION("Add two components to the same entity and query")
    {

        Entity e = ECS::World::NewEntity();
        auto component1 = std::make_shared<Component>();
        auto component2 = std::make_shared<Component>();

        ECS::World::AddComponent(e, "Player", component1);
        ECS::World::AddComponent(e, "Transform", component2);

        std::vector<Entity> match = ECS::World::QueryComponents(
                                               {"Player", "Transform"});
        REQUIRE(match.size() == 1);
        REQUIRE(match[0] == e);
    }

    SECTION("Add three components to the same entity and others and query")
    {
        Entity e1 = ECS::World::NewEntity();
        Entity e2 = ECS::World::NewEntity();
        Entity e3 = ECS::World::NewEntity();
        ECS::World::NewEntity();
        auto component1 = std::make_shared<Component>();
        auto component2 = std::make_shared<Component>();
        auto component3 = std::make_shared<Component>();
        auto component4 = std::make_shared<Component>();
        auto component5 = std::make_shared<Component>();
        ECS::World::AddComponent(e1, "Player", component1);
        ECS::World::AddComponent(e1, "Transform", component2);
        ECS::World::AddComponent(e2, "Transform", component3);
        ECS::World::AddComponent(e3, "Health", component4);
        ECS::World::AddComponent(e1, "Health", component5);

        std::vector<Entity> match = ECS::World::QueryComponents(
                                               {"Player", "Transform"});

        REQUIRE(match.size() == 1);
        REQUIRE(match[0] == e1);
    }

    ECS::World::Delete();
}

TEST_CASE("Entity to component from the ECS world")
{
    ECS::World::Init();
    
    SECTION("Add a component to an entity and get it")
    {
        Entity e = ECS::World::NewEntity();
        auto component = std::make_shared<Component>();
        ECS::World::AddComponent(e, "Player", component);

        Component* c = ECS::World::EntityToComponent(e, "Player");
        REQUIRE(c != nullptr);
        REQUIRE(*c == *component);
    }

    SECTION("Add a component to an entity and get it with wrong name")
    {
        Entity e = ECS::World::NewEntity();
        auto component = std::make_shared<Component>();
        ECS::World::AddComponent(e, "Player", component);

        Component* c = ECS::World::EntityToComponent(e, "Transform");
        REQUIRE(c == nullptr);
    }

    ECS::World::Delete();
}
