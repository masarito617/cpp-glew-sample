#include "Texture.h"
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

Texture::Texture()
:mTextureID(0)
,mTexture(nullptr)
{}

Texture::~Texture()
{}

bool Texture::Load(const std::string &filePath)
{
    // ファイル読込
    mTexture = IMG_Load(filePath.c_str());
    if (!mTexture)
    {
        SDL_Log("Failed load texture.");
        return false;
    }

    // RGBフォーマットの指定
    int rgbFormat = GL_RGB;
    if (mTexture->format->BitsPerPixel >= 4) rgbFormat = GL_RGBA;

    // テクスチャオブジェクトの作成
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, rgbFormat, mTexture->w, mTexture->h, 0, rgbFormat,
                 GL_UNSIGNED_BYTE, mTexture->pixels);

    // バイリニアフィルタを有効にする
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return true;
}

void Texture::Unload()
{
    glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
