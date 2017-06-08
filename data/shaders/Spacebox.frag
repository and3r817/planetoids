#version 110

uniform samplerCube u_scCubemap;

varying vec3 wsTexCoord;

void main()
{
    gl_FragColor = textureCube(u_scCubemap, wsTexCoord);
}