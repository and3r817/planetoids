#version 110

attribute vec3 attr_v3Position;
attribute vec2 attr_v2Texcoord;
attribute vec3 attr_v3Normal;

uniform mat4 u_trans_m4ModelView;
uniform mat4 u_trans_m4Proj;

uniform vec3 u_v3Position;

varying vec3 var_v3Position;
varying vec2 var_v2Texcoord;
varying vec3 var_v3Normal;


void main()
{
    var_v3Position = attr_v3Position;
    var_v2Texcoord = attr_v2Texcoord;

    gl_Position = u_trans_m4Proj * (vec4(attr_v3Position * 200.0, 1.0) + vec4(u_trans_m4ModelView[3].xyz, 0.0));
}
