#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H

#include <iostream>
#include "Component.h"

class TestComponent : public Component
{
private:
public:
    TestComponent()
    {
        std::cout << owner->gameObject.name << "Component is construct \n";
    }
    ~TestComponent() {}

    void Initialize() override
    {
        std::cout << owner->gameObject.name << " is initialize.. \n";
    }
    void Update(float deltaTime)
    {
        std::cout << owner->gameObject.name << " is update \n";
    }
    void Render(ShaderProgram *shader)
    {
        std::cout << owner->gameObject.name << " is render \n";
    }
};

#endif // TESTCOMPONENT_H