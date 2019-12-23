#include "Transform.h"

ct::Transform::Transform()
    : position(Vector2(0, 0)),
      scale(0),
      rotation(0)
{
}
ct::Transform::~Transform() {}

void ct::Transform::Start() {}
void ct::Transform::Handle_Events() {}
void ct::Transform::Update() {}
void ct::Transform::Render() {}