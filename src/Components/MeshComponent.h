#pragma once
#include "Component.h"

// メッシュコンポーネントクラス
class MeshComponent : public Component
{
public:
    MeshComponent(class Actor* actor);
    ~MeshComponent();

    virtual void Draw(class Shader* shader);
    class Texture* GetTexture(int index);

protected:
    class Mesh* mMesh;
    unsigned int mTextureIndex;

public:
    virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }

};
