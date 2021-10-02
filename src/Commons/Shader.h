#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

// シェーダクラス
// *コンパイル後、シェーダプログラムを作成してリンクする
class Shader
{
public:
    // シェーダタイプ
    enum ShaderType
    {
        BASIC,  // 青色で出力
        SPRITE, // テクスチャ付きで出力
        PHONG,  // テクスチャ+フォン反射で出力
    };

    Shader(const ShaderType type);
    ~Shader();

    // ロード処理
    bool Load(class Game* game);
    void Unload();

    void SetActive();

    void SetLightingUniform();

    // Uniformへの設定処理
    void SetMatrixUniform(const char* name, const Matrix4& matrix);
    void SetVectorUniform(const char* name, const Vector3& vector);
    void SetFloatUniform(const char* name, float value);

    std::string GetVertFileName() const;
    std::string GetFragFileName() const;

    // Uniform名
    static const char* UNIFORM_VIEW_PROJECTION_NAME;
    static const char* UNIFORM_WOULD_TRANSFORM_NAME;


private:
    // コンパイル処理
    bool CompileShader(const std::string& fileName,
                       GLenum shaderType,
                       GLuint& outShader);

    // コンパイルに成功したか？
    bool IsCompiled(GLuint shader);

    // 頂点、フラグメントプログラムのリンクを確認
    bool IsValidProgram();

    // シェーダタイプ
    ShaderType mType;

    // シェーダのIDを格納
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};
