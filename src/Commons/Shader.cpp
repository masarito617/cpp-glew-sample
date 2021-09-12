#include "Shader.h"
#include <SDL.h>
#include <fstream>
#include <sstream>

const char* Shader::UNIFORM_VIEW_PROJECTION_NAME = "uViewProjection";
const char* Shader::UNIFORM_WOULD_TRANSFORM_NAME = "uWorldTransform";

Shader::Shader()
:mShaderProgram(0)
,mVertexShader(0)
,mFragShader(0)
{}

Shader::~Shader()
{}

bool Shader::Load(const std::string &vertFilePath, const std::string &fragFilePath)
{
    // コンパイルを行う
    if (!CompileShader(vertFilePath, GL_VERTEX_SHADER, mVertexShader)
    || !CompileShader(fragFilePath, GL_FRAGMENT_SHADER, mFragShader))
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

void Shader::SetMatrixUniform(const char *name, const Matrix4 &matrix)
{
    // 指定された名前のuniformを設定
    GLuint location = glGetUniformLocation(mShaderProgram, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, matrix.GetMatrixFloatPtr());
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
