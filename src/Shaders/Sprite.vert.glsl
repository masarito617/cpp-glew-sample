#version 330

// ワールド変換座標、ビュー変換座標を受け取る
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 位置座標、テクスチャ座標
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main() {
    vec4 pos = vec4(inPosition, 1.0);
    // ワールド空間 -> クリップ空間
    gl_Position = pos * uWorldTransform * uViewProj;
    // テクスチャ座標を渡す
    fragTexCoord = inTexCoord;
}
