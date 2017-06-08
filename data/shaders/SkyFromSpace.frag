#version 110

uniform vec3 u_light_v3Position;
uniform vec3 u_light_v3Direction;
uniform float u_light_fIntensity;
uniform vec4 u_light_v4Diffuse;

uniform float u_atm_fAsymmetryFactor;
uniform float u_atm_fAsymmetryFactor2;

varying vec3 var_v3Direction;
varying vec4 var_v4MieColor;
varying vec4 var_v4RayleighColor;


float getRayleighPhase(float fCos)
{
    return 0.75 * (1.0 + fCos * fCos);//(2.0 + 0.5 * fCos * fCos);
}


float getMiePhase(float fCos, float fG, float fG2)
{
    return 1.5 * ((1.0 - fG2) / (2.0 + fG2)) * (1.0 + fCos * fCos) / pow(1.0 + fG2 - 2.0 * fG * fCos, 1.5);
}


vec4 getFinalColor(float fRayleighPhase, float fMiePhase)
{
    vec4 color = fRayleighPhase * var_v4RayleighColor + fMiePhase * var_v4MieColor;
    //color.a = max(max(color.r, color.g), color.b);

    return 1.0 - exp(-2.0 * color);
}


void main()
{
    float fCos = dot(u_light_v3Direction, var_v3Direction) / length(var_v3Direction);
    float fRayleighPhase = getRayleighPhase(fCos);
    float fMiePhase = getMiePhase(fCos, u_atm_fAsymmetryFactor, u_atm_fAsymmetryFactor2);

    gl_FragColor = getFinalColor(fRayleighPhase, fMiePhase);
}
