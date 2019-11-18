﻿#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aInstanceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = aInstanceMatrix * vec4(aPos, 1.0);
}