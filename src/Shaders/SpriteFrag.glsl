#version 330

uniform sampler2D uTexture; // テクスチャ（自動で設定される）

in vec2 fragTexCoord;

out vec4 outColor;

void main() {
    // テクスチャを出力
    outColor = texture(uTexture, fragTexCoord);
}
