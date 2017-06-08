#version 110

attribute vec3 msPosition;
attribute vec3 msTexcoord;

uniform mat4 MVP;

varying vec3 wsTexCoord;

void main()
{
    wsTexCoord = msPosition;

    gl_Position = MVP * vec4(msPosition, 1.0);
}
