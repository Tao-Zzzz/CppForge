#version 330 core
layout (location = 0) in vec3 aPos; // 顶点位置

uniform float xOffset; // 水平偏移量

out vec3 vertexPos; // 输出顶点位置到片段着色器

void main()
{
    // 上下翻转：将 y 坐标取反
    vec3 flippedPos = vec3(aPos.x, -aPos.y, aPos.z);
    // 应用水平偏移
    flippedPos.x += xOffset;
    gl_Position = vec4(flippedPos, 1.0);
    vertexPos = aPos; // 直接输出原始顶点位置（未翻转，未偏移）
}