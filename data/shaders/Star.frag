#version 110

uniform vec3 u_camera_v3Position;

uniform sampler2D u_s2dMainTex0;

varying vec3 var_v3Position;
varying vec2 var_v2Texcoord;
varying vec3 var_v3Normal;


void main()
{
    gl_FragColor = texture2D(u_s2dMainTex0, var_v2Texcoord);
}
