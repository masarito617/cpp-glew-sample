#version 330

uniform mat4 uWorldTransform; // ワールド変換座標
uniform mat4 uViewProjection; // ビュー射影行列

in vec3 inPosition;

void main() {
    // w成分を加える
    vec4 pos = vec4(inPosition, 1.0);
    // ローカル座標 * ワールド変換座標 * ビュー射影行列
    // を逆に計算して、クリップ空間座標に変換
    gl_Position = uViewProjection * uWorldTransform * pos;
}
