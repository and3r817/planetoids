#version 110

attribute vec3 attr_v3Position;
attribute vec2 attr_v2Texcoord;

uniform sampler2D u_s2dPlanetMask;

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

varying vec3 var_v3Position;
varying vec2 var_v2Texcoord;
varying vec3 var_v3Normal;
varying vec4 var_v4RayleighColor;
varying vec4 var_v4MieColor;


float scaleExp(float fCos)
{
    float x = 1.0 - fCos;
    return u_atm_fScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}


void main()
{
	vec3 v3Pos = attr_v3Position * ((texture2DLod(u_s2dPlanetMask, attr_v2Texcoord, 0.0).w * 0.019) + 0.996);
    vec3 v3Ray = attr_v3Position - u_camera_v3Position;
    float fFar = length(v3Ray);
    v3Ray /= fFar;

    float b = 2.0 * dot(u_camera_v3Position, v3Ray);
    float c = u_camera_fHeight2 - u_atm_fAtmosphereRadius2;
    float fDet = max(0.0, b * b - 4.0 * c);
    float fNear = 0.5 * (-b - sqrt(fDet));
    fFar -= fNear;

    vec3 v3Start = u_camera_v3Position + v3Ray * fNear;
    float fDepth = exp((u_atm_fPlanetRadius - u_atm_fAtmosphereRadius) / u_atm_fScaleDepth);
    float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);
    float fLightAngle = dot(u_light_v3Direction, v3Pos) / fFar;
    float fCameraScale = scaleExp(fCameraAngle);
    float fLightScale = scaleExp(fLightAngle);
    float fCameraOffset = fDepth * fCameraScale;
    float fTemp = (fLightScale + fCameraScale);

    float fSampleLength = length(v3Pos) / u_atm_fSamples;
    float fScaledLength = fSampleLength * u_atm_fScale;
    vec3 v3SampleRay = v3Ray * fSampleLength;
    vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

    vec3 v3Color = vec3(0.0, 0.0, 0.0);
    vec3 v3Attenuate = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < u_atm_nSamples; ++i)
    {
        float fHeight = max(u_atm_fPlanetRadius, length(v3SamplePoint));
        float fDepth = exp(u_atm_fScaleOverScaleDepth * (u_atm_fPlanetRadius - fHeight));
        float fScatter = fDepth * fTemp - fCameraOffset;
        v3Attenuate = exp(-fScatter * (u_atm_v3InvWavelength * u_atm_fRayleighConst4PI + u_atm_fMieConst4PI));
        v3Color += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }

	var_v3Position = v3Pos;
    var_v2Texcoord = attr_v2Texcoord;
    var_v3Normal = normalize(v3Pos);

    var_v4RayleighColor = vec4(clamp(v3Color, 0.0, 7.0) * (u_atm_v3InvWavelength * (u_atm_fRayleighConst * u_light_fIntensity) + (u_atm_fMieConst * u_light_fIntensity)), 1.0);
    var_v4MieColor = vec4(clamp(v3Attenuate, 0.0, 7.0), 1.0);
	
    gl_Position = u_trans_m4ModelViewProj * vec4(v3Pos, 1.0);
}
