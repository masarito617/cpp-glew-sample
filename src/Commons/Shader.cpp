#include "Shader.h"
#include <SDL.h>
#include <fstream>
#include <sstream>
#include "../Game.h"
#include "../Actors/Camera.h"

Shader::Shader(const ShaderType type, float specPower)
:mType(type)
,mShaderProgram(0)
,mVertexShader(0)
,mFragShader(0)
,mSpecPower(specPower)
{}

Shader::~Shader()
{}

bool Shader::Load(Game* game)
{
    // コンパイルを行う
    if (!CompileShader(game->GetShaderPath() + GetVertFileName(), GL_VERTEX_SHADER, mVertexShader)
    || !CompileShader(game->GetShaderPath() + GetFragFileName(), GL_FRAGMENT_SHADER, mFragShader))
    {
        return false;
    }

    // 頂点シェーダ、フラグメントシェーダをリンクして
    // シェーダプログラムを作成
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragShader);
    glLinkProgram(mShaderProgram);

    // 成功したかどうか？
    if (!IsValidProgram())
    {
        return false;
    }
    return true;
}

void Shader::Unload()
{
    glDeleteProgram(mShaderProgram);
    glDeleteShader(mVertexShader);
    glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
    // 作成したシェーダプログラムを使用
    glUseProgram(mShaderProgram);
}

bool Shader::CompileShader(const std::string& filePath, GLenum shaderType, GLuint& outShader)
{
    // ファイルを開く
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open())
    {
        SDL_Log("Failed open shader.");
        return false;
    }

    // 全てのテキストを１つの文字列に読み込む
    std::stringstream stream;
    stream << shaderFile.rdbuf();
    std::string contents = stream.str();
    const char* contentsChar = contents.c_str();

    // 指定されたタイプのシェーダを作成
    outShader = glCreateShader(shaderType);
    glShaderSource(outShader, 1, &(contentsChar), nullptr);
    glCompileShader(outShader);

    // 成功したかどうか？
    if (!IsCompiled(outShader))
    {
        SDL_Log("Failed compile shader.");
        return false;
    }
    return true;
}

bool Shader::IsCompiled(GLuint shader)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        SDL_Log("gl compile status false.");
        return false;
    }
    return true;
}

bool Shader::IsValidProgram()
{
    GLint status;
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        SDL_Log("gl link status false.");
        return false;
    }
    return true;
}

std::string Shader::GetVertFileName() const
{
    std::string fileName;
    switch (mType) {
        case ShaderType::BASIC:
            fileName = "BasicVert.glsl";
            break;
        case ShaderType::SPRITE:
            fileName = "SpriteVert.glsl";
            break;
        case ShaderType::LAMBERT:
            fileName = "LambertVert.glsl";
            break;
        case ShaderType::PHONG:
            fileName = "PhongVert.glsl";
            break;
    }
    return fileName;
}

std::string Shader::GetFragFileName() const
{
    std::string fileName;
    switch (mType) {
        case ShaderType::BASIC:
            fileName = "BasicFrag.glsl";
            break;
        case ShaderType::SPRITE:
            fileName = "SpriteFrag.glsl";
            break;
        case ShaderType::LAMBERT:
            fileName = "LambertFrag.glsl";
            break;
        case ShaderType::PHONG:
            fileName = "PhongFrag.glsl";
            break;
    }
    return fileName;
}

// ワールド座標uniform設定
void Shader::SetWorldTransformUniform(const Matrix4& would)
{
    SetMatrixUniform(UNIFORM_WOULD_TRANSFORM_NAME, would);
}

// クリップ座標uniform設定
void Shader::SetViewProjectionUniform(const Matrix4& viewProjection)
{
    SetMatrixUniform(UNIFORM_VIEW_PROJECTION_NAME, viewProjection);
}

// ライティングuniform設定
void Shader::SetLightingUniform(const Renderer* renderer)
{
    switch (mType) {
        case ShaderType::BASIC:
        case ShaderType::SPRITE:
            // 設定しない
            break;
        case ShaderType::LAMBERT:
            SetVectorUniform(UNIFORM_AMBIENT_COLOR, renderer->GetAmbientLight());
            SetVectorUniform(UNIFORM_DIR_LIGHT_DIRECTION, renderer->GetDirLightDirection());
            SetVectorUniform(UNIFORM_DIR_LIGHT_DIFFUSE_COLOR, renderer->GetDirLightDiffuseColor());
            break;
        case ShaderType::PHONG:
            SetVectorUniform(UNIFORM_CAMERA_POS, renderer->GetCamera()->GetPosition());
            SetVectorUniform(UNIFORM_AMBIENT_COLOR, renderer->GetAmbientLight());
            SetVectorUniform(UNIFORM_DIR_LIGHT_DIRECTION, renderer->GetDirLightDirection());
            SetVectorUniform(UNIFORM_DIR_LIGHT_DIFFUSE_COLOR, renderer->GetDirLightDiffuseColor());
            SetVectorUniform(UNIFORM_DIR_LIGHT_SPEC_COLOR, renderer->GetDirLightSpecColor());
            SetFloatUniform(UNIFORM_SPEC_POWER, mSpecPower);
            break;
    }
}

// 指定された名前のuniformを設定
void Shader::SetMatrixUniform(const char *name, const Matrix4 &matrix)
{
    GLuint location = glGetUniformLocation(mShaderProgram, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, matrix.GetMatrixFloatPtr());
}
void Shader::SetVectorUniform(const char *name, const Vector3 &vector)
{
    GLuint location = glGetUniformLocation(mShaderProgram, name);
    glUniform3fv(location, 1, vector.GetAsFloatPtr());
}
void Shader::SetFloatUniform(const char* name, float value)
{
    GLuint location = glGetUniformLocation(mShaderProgram, name);
    glUniform1f(location, value);
}
