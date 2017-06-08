#version 110

uniform vec3 u_camera_v3Position;

uniform vec3 u_light_v3Position;
uniform vec3 u_light_v3Direction;
uniform vec4 u_light_v4Diffuse;
uniform vec4 u_light_v4Specular;
uniform float u_light_fIntensity;

uniform sampler2D u_s2dPlanetTex0;

varying vec3 var_v3Position;
varying vec2 var_v2Texcoord;
varying vec3 var_v3Normal;
varying vec4 var_v4RayleighColor;
varying vec4 var_v4MieColor;


void main()
{
    vec3 L = normalize(u_light_v3Position - var_v3Position);
    vec3 H = normalize(u_camera_v3Position - var_v3Position);
    vec3 N = normalize(var_v3Normal);

    vec4 v4MainColor = var_v4RayleighColor + texture2D(u_s2dPlanetTex0, var_v2Texcoord) * var_v4MieColor;
    vec4 v4Diffuse = v4MainColor * u_light_v4Diffuse * clamp(dot(N, L) + 0.5, 0.0, 1.0);
    vec4 v4Specular = v4MainColor * u_light_v4Specular * pow(max(dot(N, H), 0.0), 20.0);

    gl_FragColor = v4Diffuse + v4Specular;
}

/*
void main()
{
    vec3 L = normalize(u_light_v3Position - var_v3Position);
    vec3 H = normalize(u_camera_v3Position - var_v3Position);
    vec3 N = normalize(var_v3Normal);

    vec4 v4MainColor = var_v4RayleighColor + texture2D(u_s2dPlanetTex0, var_v2Texcoord) * var_v4MieColor;
    vec4 v4Diffuse = v4MainColor * u_light_v4Diffuse * clamp(dot(N, L) + 0.5, 0.0, 1.0);
    vec4 v4Specular = v4MainColor * u_light_v4Specular * pow(max(dot(N, H), 0.0), 20.0);

    gl_FragColor = v4Diffuse + v4Specular;
}
*/