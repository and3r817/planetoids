#version 110

uniform vec3 u_camera_v3Position;

uniform vec3 u_light_v3Position;
uniform vec3 u_light_v3Direction;
uniform vec4 u_light_v4Diffuse;
uniform vec4 u_light_v4Specular;
uniform float u_light_fIntensity;

uniform float u_fTime;

uniform sampler2D u_s2dWaterTex;

varying vec3 var_v3Position;
varying vec2 var_v2Texcoord;
varying vec3 var_v3Normal;


vec3 mod289(vec3 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}


vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}


vec4 permute(vec4 x)
{
    return mod289(((x * 34.0) + 1.0) * x);
}


vec4 taylorInvSqrt(vec4 r)
{
    return 1.89284291400159 - 0.65373472095314 * r;
}


float snoise(vec3 v)
{
    const vec2 C = vec2(1.0 / 6.0, 1.0 / 3.0);
    const vec4 D = vec4(0.0, 0.5, 1.0, 2.0);

    vec3 i = floor(v + dot(v, C.yyy));
    vec3 x0 = v - i + dot(i, C.xxx);

    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min(g.xyz, l.zxy);
    vec3 i2 = max(g.xyz, l.zxy);

    vec3 x1 = x0 - i1 + C.xxx;
    vec3 x2 = x0 - i2 + C.yyy;
    vec3 x3 = x0 - D.yyy;

    i = mod289(i);
    vec4 p = permute(permute(permute(i.z + vec4(0.0, i1.z, i2.z, 1.0)) + i.y + vec4(0.0, i1.y, i2.y, 1.0)) + i.x + vec4(0.0, i1.x, i2.x, 1.0));

    float n_ = 0.142857142857;
    vec3  ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z * ns.z);

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_);

    vec4 x = x_ * ns.x + ns.yyyy;
    vec4 y = y_ * ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4(x.xy, y.xy);
    vec4 b1 = vec4(x.zw, y.zw);

    vec4 s0 = floor(b0) * 2.0 + 1.0;
    vec4 s1 = floor(b1) * 2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

    vec3 p0 = vec3(a0.xy, h.x);
    vec3 p1 = vec3(a0.zw, h.y);
    vec3 p2 = vec3(a1.xy, h.z);
    vec3 p3 = vec3(a1.zw, h.w);

    vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    vec4 m = max(0.6 - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    m = m * m;
    return 42.0 * dot(m * m, vec4(dot(p0, x0), dot(p1, x1), dot(p2, x2), dot(p3, x3)));
}


float waterNoise3(vec3 coord)
{
    float n = 0.0;

    n += 0.3 * abs(snoise(coord));
    //n += 0.0625 * abs(snoise(coord * 28.0));

    return n;
}

void main()
{
    /*
        vec3 coord = vec3(var_v2Texcoord, -u_fTime);
        float n = waterNoise3(coord);

    	vec3 L = normalize(u_light_v3Position);
        vec3 H = normalize(u_camera_v3Position - var_v3Position);
        vec3 N = normalize(-var_v3Normal);

    	vec4 v4MainTex = vec4(vec3(n+0.4, n+0.4, n+0.4) * vec3(0.11, 0.34, 0.88), 1.0);
        vec4 v4Diffuse = v4MainTex * clamp(dot(N, L), 0.0, 1.0);

        gl_FragColor = v4Diffuse;
    	gl_FragColor.a = 0.5;
    */
	/*
    float fVal = texture2D(u_s2dWaterTex, var_v2Texcoord).r;
    float fValU = texture2D(u_s2dWaterTex, var_v2Texcoord + vec2(1.0 / 256.0, 0.0)).r;
    float fValV = texture2D(u_s2dWaterTex, var_v2Texcoord + vec2(0.0, 1.0 / 256.0)).r;
    vec3 v3Bump = vec3(fVal - fValU, 0.01, fVal - fValV);

    vec3 L = normalize(u_light_v3Position - var_v3Position);
    vec3 H = normalize(u_camera_v3Position - var_v3Position);
    vec3 N = normalize(v3Bump);

    vec4 v4MainColor = vec4(0.11, 0.34, 0.88, 1.0);
    vec4 v4Diffuse = v4MainColor * clamp(dot(N, L), 0.0, 1.0);

    gl_FragColor = v4Diffuse;
	*/

	vec3 L = normalize(u_light_v3Position - var_v3Position);
    vec3 H = normalize(u_camera_v3Position - var_v3Position);
    vec3 N = normalize(var_v3Normal);

	vec4 v4MainTex = vec4(0.11, 0.34, 0.88, 1.0);
    vec4 v4Diffuse = v4MainTex * u_light_v4Diffuse * clamp(dot(N, L), 0.0, 1.0);
    vec4 v4Specular = v4MainTex * u_light_v4Specular * pow(max(dot(N, H), 0.0), 20.0);

    vec3 v3NormalTex = texture2D(u_s2dWaterTex, var_v2Texcoord).xyz - 0.35;
    float fBump = sqrt(max(0.01, dot(u_light_v3Direction, v3NormalTex) / length(v3NormalTex)));

    gl_FragColor = (v4Diffuse + v4Specular)  * u_light_fIntensity;
}