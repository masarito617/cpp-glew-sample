#pragma once
#include <random>
#include <cmath>
#include <iostream>

// 共通の計算処理
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
}

// 4*4行列
class Matrix4
{
public:
    float matrix[4][4];

    Matrix4()
    {
    }

    // 行列データをコピーする
    Matrix4(float copyMatrix[4][4])
    {
        memcpy(matrix, copyMatrix, 16 * sizeof(float));
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

    // ビュー射影行列
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
};
