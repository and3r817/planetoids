#version 110

attribute vec3 attr_v3Position;
attribute vec2 attr_v2Texcoord;

uniform mat4 u_trans_m4Model;
uniform mat4 u_trans_m4View;
uniform mat4 u_trans_m4ModelViewProj;

uniform vec3 u_camera_v3Position;
uniform float u_camera_fHeight;
uniform float u_camera_fHeight2;

uniform vec3 u_light_v3Position;
uniform vec3 u_light_v3Direction;
uniform vec4 u_light_v4Diffuse;
uniform float u_light_fIntensity;

uniform vec3 u_atm_v3InvWavelength;
uniform float u_atm_fPlanetRadius;
uniform float u_atm_fPlanetRadius2;
uniform float u_atm_fAtmosphereRadius;
uniform float u_atm_fAtmosphereRadius2;
uniform float u_atm_fRayleighConst;
uniform float u_atm_fRayleighConst4PI;
uniform float u_atm_fMieConst;
uniform float u_atm_fMieConst4PI;
uniform float u_atm_fScale;
uniform float u_atm_fScaleDepth;
uniform float u_atm_fScaleOverScaleDepth;
uniform float u_atm_fSamples;
uniform int u_atm_nSamples;

varying vec3 var_v3Direction;
varying vec4 var_v4MieColor;
varying vec4 var_v4RayleighColor;


float scaleExp(float fCos)
{
    float x = 1.0 - fCos;
    return u_atm_fScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}


void main()
{
    vec3 v3Ray = attr_v3Position - u_camera_v3Position;
    float fFar = length(v3Ray);
    v3Ray /= fFar;

    vec3 v3Start = u_camera_v3Position;
    float fHeight = length(v3Start);
    float fStartAngle = dot(v3Ray, v3Start) / fHeight;
    float fStartDepth = exp(u_atm_fScaleOverScaleDepth * (u_atm_fPlanetRadius - u_camera_fHeight));
    float fStartOffset = fStartDepth * scaleExp(fStartAngle);

    float fSampleLength = fFar / u_atm_fSamples;
    float fScaledLength = fSampleLength * u_atm_fScale;
    vec3 v3SampleRay = v3Ray * fSampleLength;
    vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

    vec3 v3Color = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_atm_nSamples; ++i)
    {
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(u_atm_fScaleOverScaleDepth * (u_atm_fPlanetRadius - fHeight));
        float fLightAngle = dot(u_light_v3Direction, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth * (scaleExp(fLightAngle) - scaleExp(fCameraAngle)));
        vec3 v3Attenuate = exp(-fScatter * (u_atm_v3InvWavelength * u_atm_fRayleighConst4PI + u_atm_fMieConst4PI));
        v3Color += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }

    var_v4RayleighColor = vec4(v3Color * (u_atm_v3InvWavelength * (u_atm_fRayleighConst * u_light_fIntensity)), 1.0);
	var_v4MieColor = vec4(v3Color * (u_atm_fMieConst * u_light_fIntensity), 1.0);
    var_v3Direction = u_camera_v3Position - attr_v3Position;

    gl_Position = u_trans_m4ModelViewProj * vec4(attr_v3Position, 1.0);
}
