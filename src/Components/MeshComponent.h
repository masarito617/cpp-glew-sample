#pragma once
#include "Component.h"

// メッシュコンポーネントクラス
class MeshComponent : public Component
{
public:
    MeshComponent(class Actor* actor);
    ~MeshComponent();

    virtual void Draw();

protected:
    class Mesh* mMesh;
    class Shader* mShader;

public:
    virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
    virtual void SetShader(class Shader* shader) { mShader = shader; }

};
