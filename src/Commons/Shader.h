#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

// シェーダクラス
// *コンパイル後、シェーダプログラムを作成してリンクする
class Shader
{
public:
    Shader();
    ~Shader();

    // ロード処理
    bool Load(const std::string& vertName, const std::string& fragName);
    void Unload();

    void SetActive();

private:
    // コンパイル処理
    bool CompileShader(const std::string& fileName,
                       GLenum shaderType,
                       GLuint& outShader);

    // コンパイルに成功したか？
    bool IsCompiled(GLuint shader);

    // 頂点、フラグメントプログラムのリンクを確認
    bool IsValidProgram();

    // シェーダのIDを格納
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};
