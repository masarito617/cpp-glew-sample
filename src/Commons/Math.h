#pragma once
#include <random>
#include <cmath>
#include <iostream>

// *計算処理をまとめたライブラリ

// 二次元ベクトル
class Vector2
{
public:
    float x, y;

    Vector2()
    :x(0.0f)
    ,y(0.0f)
    {}

    Vector2(float inX, float inY)
    :x(inX)
    ,y(inY)
    {}

    // operator calculate
    friend Vector2 operator+(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x + b.x, a.y + b.y);
    }
    friend Vector2 operator-(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x - b.x, a.y - b.y);
    }
    friend Vector2 operator*(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x * b.x, a.y * b.y);
    }
    friend Vector2 operator*(float scalar, const Vector2& vec)
    {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }
    Vector2& operator+=(const Vector2& vec)
    {
        x += vec.x;
        y += vec.y;
        return *this;
    }
    Vector2& operator-=(const Vector2& vec)
    {
        x -= vec.x;
        y -= vec.y;
        return *this;
    }

    // ベクトルの長さ
    float Length() const
    {
        return (sqrtf(x*x + y*y));
    }
};

// 三次元ベクトル
class Vector3
{
public:
    float x, y, z;

    Vector3()
    :x(0.0f)
    ,y(0.0f)
    ,z(0.0f)
    {}

    Vector3(float inX, float inY, float inZ)
    :x(inX)
    ,y(inY)
    ,z(inZ)
    {}

    const float* GetAsFloatPtr() const
    {
        return reinterpret_cast<const float*>(&x);
    }

    // operator calculate
    friend Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x+b.x, a.y+b.y, a.z+b.z);
    }
    friend Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x-b.x, a.y-b.y, a.z-b.z);
    }
    friend Vector3 operator*(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x*b.x, a.y*b.y, a.z*b.z);
    }
    friend Vector3 operator*(float scalar, const Vector3& vec)
    {
        return Vector3(vec.x*scalar, vec.y*scalar, vec.z*scalar);
    }
    Vector3& operator+=(const Vector3& vec)
    {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }
    Vector3& operator-=(const Vector3& vec)
    {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
    }

    // ベクトルの長さ
    float Length() const
    {
        return (sqrt(x*x + y*y + z*z));
    }

    // ベクトルの正規化
    static Vector3 Normalize(const Vector3& vec)
    {
        Vector3 temp = vec;
        float length = temp.Length();
        temp.x /= length;
        temp.y /= length;
        temp.z /= length;
        return temp;
    }

    // ベクトル同士の内積
    static float Dot(const Vector3& a, const Vector3& b)
    {
        return (a.x*b.x + a.y*b.y + a.z*b.z);
    }

    // ベクトル同士の外積
    static Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        Vector3 temp;
        temp.x = a.y*b.z - a.z*b.y;
        temp.y = a.z*b.x - a.x*b.z;
        temp.z = a.x*b.y - a.y*b.x;
        return temp;
    }
};

// クォータニオン
class Quaternion
{
public:
    float x, y, z; // ベクトル qv
    float w;       // スカラー qs

    Quaternion()
    :x(0.0f)
    ,y(0.0f)
    ,z(0.0f)
    ,w(1.0f)
    {}

    Quaternion(float inX, float inY, float inZ, float inW)
    :x(inX)
    ,y(inY)
    ,z(inZ)
    ,w(inW)
    {}

    // 正規化された回転軸と回転角からクォータニオンを計算する
    Quaternion(const Vector3& axis, float angle)
    {
        // qv = axis * sin(angle/2.0f)
        float scalar = sinf(angle / 2.0f);
        x = axis.x * scalar;
        y = axis.y * scalar;
        z = axis.z * scalar;
        // qs = cos(angle/2.0f)
        w = cosf(angle / 2.0f);
    }

    // クォータニオン結合処理
    // qにpを加えたクォータニオンを返却する
    static Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
    {
        Vector3 qv(q.x, q.y, q.z);
        Vector3 pv(p.x, p.y, p.z);

        // グラスマン積でベクトルとスカラを計算する（引数順で計算）
        Vector3 outVec = p.w*qv + q.w*pv + Vector3::Cross(pv, qv);
        float outW = p.w*q.w - Vector3::Dot(pv, qv);

        // クォータニオンとして返却
        return Quaternion(outVec.x, outVec.y, outVec.z, outW);
    }

    // クォータニオンでベクトルを回転させる
    static Vector3 RotateVec(const Vector3& v, const Quaternion& q)
    {
        // TODO なぜこの式になるのか？行列での回転ではだめ？
        // vec = 2.0*qv・(qv・v + qw*v）
        Vector3 qv(q.x, q.y, q.z);
        Vector3 retVec = v;
        retVec += 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, v) + q.w*v);
        return retVec;
    }
};

// 4*4行列
class Matrix4
{
public:
    float matrix[4][4];

    Matrix4()
    {
    }

    // 行列データをコピーする
    Matrix4(float inMatrix[4][4])
    {
        memcpy(matrix, inMatrix, 16 * sizeof(float));
    }

    // 行列の中身をログ出力
    void PrintMatrix()
    {
        for (int i = 0; i <= 3; i++) {
            for (int j = 0; j <= 3; j++) {
                std::cout << matrix[i][j] << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // 行列データのポインタを返す
    const float* GetMatrixFloatPtr() const
    {
        return reinterpret_cast<const float*>(&matrix[0][0]);
    }

    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
    {
        // 4*4行列の計算
        // |a11 a12 a13 a14| |b11 b12 b13 b14|
        // |a21 a22 a23 a24| |b21 b22 b23 b14|
        // |a31 a32 a33 a34| |b31 b32 b33 b14|
        // a11b11+a12b21+a13b31+a14b41, a11b12+a12b22+a13b32+a14b42, a11b13+a12b23+a13b33+a14b43, a11b14+a12b24+a13b34+a14b44
        // a21b11+a22b21+a23b31+a24b41, a21b12+a22b22+a23b32+a24b42, a21b13+a22b23+a23b33+a24b43, a21b14+a22b24+a23b34+a24b44
        // a31b11+a32b21+a33b31+a34b41, a31b12+a32b22+a33b32+a34b42, a31b13+a32b23+a33b33+a34b43, a31b14+a32b24+a33b34+a34b44
        // a41b11+a42b21+a43b31+a44b41, a41b12+a42b22+a43b32+a44b42, a41b13+a42b23+a43b33+a44b43, a41b14+a42b24+a43b34+a44b44
        Matrix4 ret;
        // row1
        ret.matrix[0][0] = a.matrix[0][0]*b.matrix[0][0] + a.matrix[0][1]*b.matrix[1][0] + a.matrix[0][2]*b.matrix[2][0] + a.matrix[0][3]*b.matrix[3][0];
        ret.matrix[0][1] = a.matrix[0][0]*b.matrix[0][1] + a.matrix[0][1]*b.matrix[1][1] + a.matrix[0][2]*b.matrix[2][1] + a.matrix[0][3]*b.matrix[3][1];
        ret.matrix[0][2] = a.matrix[0][0]*b.matrix[0][2] + a.matrix[0][1]*b.matrix[1][2] + a.matrix[0][2]*b.matrix[2][2] + a.matrix[0][3]*b.matrix[3][2];
        ret.matrix[0][3] = a.matrix[0][0]*b.matrix[0][3] + a.matrix[0][1]*b.matrix[1][3] + a.matrix[0][2]*b.matrix[2][3] + a.matrix[0][3]*b.matrix[3][3];
        // row2
        ret.matrix[1][0] = a.matrix[1][0]*b.matrix[0][0] + a.matrix[1][1]*b.matrix[1][0] + a.matrix[1][2]*b.matrix[2][0] + a.matrix[1][3]*b.matrix[3][0];
        ret.matrix[1][1] = a.matrix[1][0]*b.matrix[0][1] + a.matrix[1][1]*b.matrix[1][1] + a.matrix[1][2]*b.matrix[2][1] + a.matrix[1][3]*b.matrix[3][1];
        ret.matrix[1][2] = a.matrix[1][0]*b.matrix[0][2] + a.matrix[1][1]*b.matrix[1][2] + a.matrix[1][2]*b.matrix[2][2] + a.matrix[1][3]*b.matrix[3][2];
        ret.matrix[1][3] = a.matrix[1][0]*b.matrix[0][3] + a.matrix[1][1]*b.matrix[1][3] + a.matrix[1][2]*b.matrix[2][3] + a.matrix[1][3]*b.matrix[3][3];
        // row3
        ret.matrix[2][0] = a.matrix[2][0]*b.matrix[0][0] + a.matrix[2][1]*b.matrix[1][0] + a.matrix[2][2]*b.matrix[2][0] + a.matrix[2][3]*b.matrix[3][0];
        ret.matrix[2][1] = a.matrix[2][0]*b.matrix[0][1] + a.matrix[2][1]*b.matrix[1][1] + a.matrix[2][2]*b.matrix[2][1] + a.matrix[2][3]*b.matrix[3][1];
        ret.matrix[2][2] = a.matrix[2][0]*b.matrix[0][2] + a.matrix[2][1]*b.matrix[1][2] + a.matrix[2][2]*b.matrix[2][2] + a.matrix[2][3]*b.matrix[3][2];
        ret.matrix[2][3] = a.matrix[2][0]*b.matrix[0][3] + a.matrix[2][1]*b.matrix[1][3] + a.matrix[2][2]*b.matrix[2][3] + a.matrix[2][3]*b.matrix[3][3];
        // row4
        ret.matrix[3][0] = a.matrix[3][0]*b.matrix[0][0] + a.matrix[3][1]*b.matrix[1][0] + a.matrix[3][2]*b.matrix[2][0] + a.matrix[3][3]*b.matrix[3][0];
        ret.matrix[3][1] = a.matrix[3][0]*b.matrix[0][1] + a.matrix[3][1]*b.matrix[1][1] + a.matrix[3][2]*b.matrix[2][1] + a.matrix[3][3]*b.matrix[3][1];
        ret.matrix[3][2] = a.matrix[3][0]*b.matrix[0][2] + a.matrix[3][1]*b.matrix[1][2] + a.matrix[3][2]*b.matrix[2][2] + a.matrix[3][3]*b.matrix[3][2];
        ret.matrix[3][3] = a.matrix[3][0]*b.matrix[0][3] + a.matrix[3][1]*b.matrix[1][3] + a.matrix[3][2]*b.matrix[2][3] + a.matrix[3][3]*b.matrix[3][3];
        return ret;
    }

    Matrix4 operator*=(const Matrix4& a)
    {
        *this = *this * a;
        return *this;
    }

    // スケール行列
    static Matrix4 CreateScale(float x, float y, float z)
    {
        float temp[4][4] =
        {
            {    x, 0.0f, 0.0f, 0.0f },
            { 0.0f,    y, 0.0f, 0.0f },
            { 0.0f, 0.0f,    z, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // 回転行列
    static Matrix4 CreateRotationX(float radians)
    {
        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, cosf(radians), -sinf(radians), 0.0f },
            { 0.0f, sinf(radians), cosf(radians), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }
    static Matrix4 CreateRotationY(float radians)
    {
        float temp[4][4] =
        {
            { cosf(radians), 0.0f, sinf(radians), 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { -sinf(radians), 0.0f, cosf(radians), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }
    static Matrix4 CreateRotationZ(float radians)
    {
        float temp[4][4] =
        {
            { cosf(radians), -sinf(radians), 0.0f, 0.0f },
            { sinf(radians), cosf(radians), 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // 平行移動行列
    static Matrix4 CreateTranslation(float x, float y, float z)
    {
        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f, x },
            { 0.0f, 1.0f, 0.0f, y },
            { 0.0f, 0.0f, 1.0f, z },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // クォータニオン行列
    static Matrix4 CreateQuaternion(const class Quaternion& q)
    {
        // 単位クォータニオンの場合の式になる
        // | 1-2qy^2-2qz^2 2qxqy-2qwqz   2qxqz+2qwqy   0 |
        // | 2qxqy+2qwqz   1-2qx^2-2qz^2 2qyqz-2qwqx   0 |
        // | 2qxqz-2qwqy   2qyqz+2qwqx   1-2qx^2-2qy^2 0 |
        // | 0             0             0             1 |
        float temp[4][4];
        // row1
        temp[0][0] = 1.0f - 2.0f*q.y*q.y - 2.0f*q.z*q.z;
        temp[0][1] = 2.0f*q.x*q.y - 2.0f*q.w*q.z;
        temp[0][2] = 2.0f*q.x*q.z + 2.0f*q.w*q.y;
        temp[0][3] = 0.0f;
        // row2
        temp[1][0] = 2.0f*q.x*q.y + 2.0f*q.w*q.z;
        temp[1][1] = 1.0f - 2.0f*q.x*q.x - 2.0f*q.z*q.z;
        temp[1][2] = 2.0f*q.y*q.z - 2.0f*q.w*q.x;
        temp[1][3] = 0.0f;
        // row3
        temp[2][0] = 2.0f*q.x*q.z - 2.0f*q.w*q.y;
        temp[2][1] = 2.0f*q.y*q.z + 2.0f*q.w*q.x;
        temp[2][2] = 1.0f - 2.0f*q.x*q.x - 2.0f*q.y*q.y;
        temp[2][3] = 0.0f;
        // row4
        temp[3][0] = 0.0f;
        temp[3][1] = 0.0f;
        temp[3][2] = 0.0f;
        temp[3][3] = 1.0f;
        return Matrix4(temp);
    }

    // ビュー射影行列（2D用）
    static Matrix4 CreateSimpleViewProjection(float width, float height)
    {
        float temp[4][4] =
        {
            { 2.0f/width, 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f/height, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // ビュー行列
    // eye：自身の位置
    // target：注視対象の位置
    // up：上方向ベクトル
    static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
    {
        Vector3 k = Vector3::Normalize(target - eye);
        Vector3 i = Vector3::Normalize(Vector3::Cross(up, k));
        Vector3 j = Vector3::Normalize(Vector3::Cross(k, i));
        Vector3 t;
        t.x = -Vector3::Dot(i, eye);
        t.y = -Vector3::Dot(j, eye);
        t.z = -Vector3::Dot(k, eye);

        float temp[4][4] =
        {
            { i.x,  i.y,  i.z,  t.x },
            { j.x,  j.y,  j.z,  t.y },
            { k.x,  k.y,  k.z,  t.z },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // 正射影行列
    // near、far：近接、遠方の見える範囲
    static Matrix4 CreateOrtho(float width, float height,
                               float near, float far)
    {
        float temp[4][4] =
        {
            { 2.0f/width, 0.0f,        0.0f, 0.0f },
            { 0.0f,       2.0f/height, 0.0f, 0.0f },
            { 0.0f,       0.0f,        1.0f/(far-near), near/(near-far) },
            { 0.0f,       0.0f,        0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // 透視射影行列
    // fov：縦方向に画面に入る範囲（垂直画角）
    // near、far：近接、遠方の見える範囲
    static Matrix4 CreatePerspectiveFOV(float fov, float width, float height,
                                        float near, float far)
    {
        // yScale = cot(fov/2.0f)
        // xScale = yScale・(height/width)
        float yScale = 1.0f / tanf(fov/2.0f);
        float xScale = yScale * height / width;

        float temp[4][4] =
        {
            { xScale, 0.0f,   0.0f, 0.0f },
            { 0.0f,   yScale, 0.0f, 0.0f },
            { 0.0f,   0.0f,   far/(far-near), -near*far/(far-near) },
            { 0.0f,   0.0f,   1.0f, 0.0f },
        };
        return Matrix4(temp);
    }
};

// 共通の定数及び計算処理
// namespaceとして定義
namespace Math
{
    const float Pi = 3.1415926535f;

    // degrees -> radians
    inline float ToRadians(float degrees)
    {
        return degrees * Pi / 180.0f;
    }

    // radians -> degrees
    inline float ToDegrees(float radians)
    {
        return radians * 180.0f / Pi;
    }

    // random
    static std::random_device seed_gen;
    static std::default_random_engine randomEngine(seed_gen());
    inline float GetRand(float min, float max)
    {
        std::uniform_real_distribution<> dist(min, max);
        return dist(Math::randomEngine);
    }

    // vec2
    static const Vector2 VEC2_ZERO   = Vector2(0.0f, 0.0f);
    // vec3
    static const Vector3 VEC3_ZERO   = Vector3(0.0f, 0.0f, 0.0f);
    static const Vector3 VEC3_UNIT   = Vector3(1.0f, 1.0f, 1.0f);
    static const Vector3 VEC3_UNIT_X = Vector3(1.0f, 0.0f, 0.0f);
    static const Vector3 VEC3_UNIT_Y = Vector3(0.0f, 1.0f, 0.0f);
    static const Vector3 VEC3_UNIT_Z = Vector3(0.0f, 0.0f, 1.0f);
}
