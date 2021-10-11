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
        BASIC,   // テクスチャ無し（青色）
        SPRITE,  // テクスチャ有り
        LAMBERT, // ランバート反射モデル
        PHONG,   // フォン反射モデル
    };

    Shader(const ShaderType type, float specPower = 10.0f);
    ~Shader();

    bool Load(class Game* game);
    void Unload();
    void SetActive();

    std::string GetVertFileName() const;
    std::string GetFragFileName() const;

    // uniformへの設定処理
    void SetWorldTransformUniform(const class Matrix4& would);          // ワールド座標
    void SetViewProjectionUniform(const class Matrix4& viewProjection); // クリップ座標
    void SetLightingUniform(const class Renderer* renderer);            // ライティング関連

    // uniform名
    const char* UNIFORM_VIEW_PROJECTION_NAME = "uViewProjection";
    const char* UNIFORM_WOULD_TRANSFORM_NAME = "uWorldTransform";
    const char* UNIFORM_CAMERA_POS = "uCameraPos";
    const char* UNIFORM_AMBIENT_COLOR = "uAmbientColor";
    const char* UNIFORM_DIR_LIGHT_DIRECTION     = "uDirLight.mDirection";
    const char* UNIFORM_DIR_LIGHT_DIFFUSE_COLOR = "uDirLight.mDiffuseColor";
    const char* UNIFORM_DIR_LIGHT_SPEC_COLOR = "uDirLight.mSpecColor";
    const char* UNIFORM_SPEC_POWER = "uSpecPower";

private:
    // コンパイル処理
    bool CompileShader(const std::string& fileName,
                       GLenum shaderType,
                       GLuint& outShader);

    // コンパイルに成功したか？
    bool IsCompiled(GLuint shader);

    // 頂点、フラグメントプログラムのリンクを確認
    bool IsValidProgram();

    // uniformへの設定処理
    void SetMatrixUniform(const char* name, const Matrix4& matrix);
    void SetVectorUniform(const char* name, const Vector3& vector);
    void SetFloatUniform(const char* name, float value);

    // シェーダタイプ
    ShaderType mType;

    // シェーダのIDを格納
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;

    // ライティングパラメータ
    float mSpecPower; // 鏡面反射指数 a
};
