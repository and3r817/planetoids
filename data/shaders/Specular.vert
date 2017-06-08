#version 110

attribute vec3 attr_v3Position;
attribute vec2 attr_v2Texcoord;
attribute vec3 attr_v3Normal;

uniform mat4 u_trans_m4Model;
uniform mat4 u_trans_m4View;
uniform mat4 u_trans_m4ModelViewProj;

varying vec3 var_v3Position;
varying vec2 var_v2Texcoord;
varying vec3 var_v3Normal;


void main()
{
	var_v3Position = attr_v3Position;
    var_v2Texcoord = attr_v2Texcoord;
    var_v3Normal = (u_trans_m4Model * vec4(attr_v3Normal,0.0)).xyz;

    gl_Position = u_trans_m4ModelViewProj * vec4(attr_v3Position, 1.0);
}
