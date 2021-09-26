#version 330

uniform mat4 uWorldTransform; // ワールド変換座標
uniform mat4 uViewProjection; // ビュー射影行列

layout(location = 0) in vec3 inPosition; // 位置座標
layout(location = 1) in vec3 inNormal;   // 法線座標
layout(location = 2) in vec2 inTexCoord; // UV座標

out vec2 fragTexCoord;

void main() {
    // w成分を加える
    vec4 pos = vec4(inPosition, 1.0);
    // ローカル座標 * ワールド変換座標 * ビュー射影行列
    // を逆に計算して、クリップ空間座標に変換
    gl_Position = uViewProjection * uWorldTransform * pos;
    // テクスチャ座標を設定
    fragTexCoord = inTexCoord;
}
