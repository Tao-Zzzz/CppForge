#version 330 core
out vec4 FragColor;

in vec3 vertexPos; // 从顶点着色器接收的顶点位置

void main()
{
    FragColor = vec4(vertexPos, 1.0); // 将顶点位置作为颜色
}