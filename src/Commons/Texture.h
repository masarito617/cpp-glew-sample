#pragma once
#include <string>
#include <SDL_image.h>

// テクスチャクラス
class Texture {
public:
    Texture();
    ~Texture();

    bool Load(const std::string& fileName);
    void Unload();
    void SetActive();

private:
    unsigned int mTextureID;
    class SDL_Surface* mTexture;

public:
    int GetWidth() const { return mTexture ? mTexture->w : 0; }
    int GetHeight() const { return mTexture ? mTexture->h : 0; }

};
