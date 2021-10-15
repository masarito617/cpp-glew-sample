#version 330

uniform sampler2D uTexture; // テクスチャ（自動で設定される）
uniform vec3 uAmbientColor; // 環境色 ka

// 平行光源
struct DirectionalLight
{
    vec3 mDirection;    // 向き
    vec3 mDiffuseColor; // 拡散反射色 kd
};
uniform DirectionalLight uDirLight;

in vec2 fragTexCoord; // 位置座標
in vec3 fragNormal;   // 法線座標

out vec4 outColor;

void main() {
    // 計算に必要な向きを正規化して求める
    vec3 N = normalize(fragNormal);                // 法線
    vec3 L = normalize(-uDirLight.mDirection);     // 表面→光源

    // 反射色の計算
    // ベースとなる環境色を設定
    vec3 Lambert = uAmbientColor; // ka
    // N・Lが0より大きい場合
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        // 拡散反射色を加える
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL; // kd * N・L
        Lambert += Diffuse;
    }
    // テクスチャ色 * 反射色
    outColor = texture(uTexture, fragTexCoord) * vec4(Lambert, 1.0f);
}
