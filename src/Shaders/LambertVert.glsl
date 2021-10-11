#version 330

uniform mat4 uWorldTransform; // ワールド変換座標
uniform mat4 uViewProjection; // ビュー射影行列

layout(location = 0) in vec3 inPosition; // 位置座標
layout(location = 1) in vec3 inNormal;   // 法線座標
layout(location = 2) in vec2 inTexCoord; // UV座標

// テクスチャ情報
out vec2 fragTexCoord; // 位置座標
out vec3 fragNormal;   // 法線座標
out vec3 fragWorldPos; // ワールド座標

void main() {
    // w成分を加える
    vec4 pos = vec4(inPosition, 1.0);
    // クリップ空間座標に変換して設定
    gl_Position = uViewProjection * uWorldTransform * pos;

    // テクスチャ情報を設定
    fragTexCoord = inTexCoord;
    // ワールド座標に変換する
    fragNormal = (uWorldTransform * vec4(inNormal, 0.0f)).xyz;
}
