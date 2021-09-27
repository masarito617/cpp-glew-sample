#pragma once
#include "Component.h"

// メッシュコンポーネントクラス
class MeshComponent : public Component
{
public:
    MeshComponent(class Actor* actor);
    ~MeshComponent();

    virtual void Draw(class Shader* shader);

protected:
    class Mesh* mMesh;

public:
    virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }

};
