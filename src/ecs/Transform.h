#ifndef TRANSFORM_H
#define TRANSFORM_H


#include "Component.h"
#include "../math/Math.h"

namespace ct
{

class Transform : public Component
{
private:
public:
    Vector2 position;
    float scale;
    float rotation;

    Transform();
    ~Transform();

    void Start() override;
    void Handle_Events() override;
    void Update() override;
    void Render() override;
};
} // namespace ct

#endif // TRANSFORM_H