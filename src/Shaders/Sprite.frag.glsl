#version 330

// テクスチャを受け取る
uniform sampler2D uTexture;

// テクスチャ座標
in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    // テクスチャの色を設定
    outColor = texture(uTexture, fragTexCoord);
}
