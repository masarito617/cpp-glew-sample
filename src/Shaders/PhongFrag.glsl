#version 330

uniform sampler2D uTexture; // テクスチャ（自動で設定される）
uniform vec3 uCameraPos;    // カメラ座標
uniform float uSpecPower;   // 鏡面反射指数 a
uniform vec3 uAmbientColor; // 環境色 ka

// 平行光源
struct DirectionalLight
{
    vec3 mDirection;    // 向き
    vec3 mDiffuseColor; // 拡散反射色 kd
    vec3 mSpecColor;    // 鏡面反射色 ks
};
uniform DirectionalLight uDirLight;

in vec2 fragTexCoord; // 位置座標
in vec3 fragNormal;   // 法線座標
in vec3 fragWorldPos; // ワールド座標

out vec4 outColor;

void main() {
    // 計算に必要な向きを正規化して求める
    vec3 N = normalize(fragNormal);                // 法線
    vec3 L = normalize(-uDirLight.mDirection);     // 表面→光源
    vec3 V = normalize(uCameraPos - fragWorldPos); // 表面→視点
    vec3 R = normalize(reflect(-L, N));            // Nを軸としてLを反射させたもの

    // Phong反射色の計算
    // ベースとなる環境色を設定
    vec3 Phong = uAmbientColor; // ka
    // N・Lが0より大きい場合
    float NdotL = dot(N, L);
    if (NdotL > 0)
    {
        // 拡散反射色、鏡面反射色を加える
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL; // kd * N・L
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower); // ks * (R・V)^a
        Phong += Diffuse + Specular;
    }
    // テクスチャ色 * Phong反射色
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
