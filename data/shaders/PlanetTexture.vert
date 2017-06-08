#version 110

attribute vec3 attr_v3Position;
attribute vec2 attr_v2Texcoord;

varying vec2 var_v2Texcoord;


void main()
{

    var_v2Texcoord = (attr_v2Texcoord + 1.0) * 0.5;

	gl_Position = vec4(attr_v3Position, 1.0);
}
