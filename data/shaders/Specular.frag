#version 110

uniform vec3 u_camera_v3Position;

uniform vec3 u_light_v3Position;
uniform vec3 u_light_v3Direction;
uniform vec4 u_light_v4Ambient;
uniform vec4 u_light_v4Diffuse;
uniform vec4 u_light_v4Specular;
uniform float u_light_fIntensity;

uniform sampler2D u_s2dMainTex0;
uniform sampler2D u_s2dNormalTex0;

varying vec3 var_v3Position;
varying vec2 var_v2Texcoord;
varying vec3 var_v3Normal;


void main()
{
    vec3 L = normalize(u_light_v3Position - var_v3Position);
    vec3 H = normalize(u_camera_v3Position - var_v3Position);
    vec3 N = normalize(var_v3Normal);

	vec4 v4MainTex = texture2D(u_s2dMainTex0, var_v2Texcoord);
	vec4 v4Ambient = v4MainTex * u_light_v4Ambient;
    vec4 v4Diffuse = v4MainTex * u_light_v4Diffuse * clamp(dot(N, L), 0.0, 1.0);
    vec4 v4Specular = v4MainTex * u_light_v4Specular * pow(max(dot(N, H), 0.0), 20.0);

    vec3 v4NormalTex = texture2D(u_s2dNormalTex0, var_v2Texcoord).xyz - 0.35;
    float fBump = sqrt(max(0.01, dot(u_light_v3Direction, v4NormalTex) / length(v4NormalTex)));

    gl_FragColor = (v4Ambient + v4Diffuse + v4Specular) * fBump * u_light_fIntensity;
}
