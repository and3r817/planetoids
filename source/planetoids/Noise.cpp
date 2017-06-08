#include "Noise.hpp"

#undef max
#undef min

namespace planetoids
{
	glm::vec3 mod289(glm::vec3 & x)
	{
		return x - glm::floor(x * (1.0f / 289.0f)) * 289.0f;
	}


	glm::vec2 mod289(glm::vec2 & x)
	{
		return x - glm::floor(x * (1.0f / 289.0f)) * 289.0f;
	}


	glm::vec3 permute(glm::vec3 & x)
	{
		return mod289(((x * 34.0f) + 1.0f) * x);
	}


	float snoise(glm::vec2 & v)
	{
		const glm::vec4 C = glm::vec4(0.211324865405187f, 0.366025403784439f, -0.577350269189626f, 0.024390243902439f);

		glm::vec2 i = glm::floor(v + glm::dot(v, glm::vec2(C.yy)));
		glm::vec2 x0 = v - i + glm::dot(i, glm::vec2(C.xx));

		glm::vec2 i1 = (x0.x > x0.y) ? glm::vec2(1.0f, 0.0f) : glm::vec2(0.0f, 1.0f);

		glm::vec4 x12 = x0.xyxy + C.xxzz;
		x12.xy -= i1;

		i = mod289(i);
		glm::vec3 p = permute(permute(i.y + glm::vec3(0.0f, i1.y, 1.0f)) + i.x + glm::vec3(0.0f, i1.x, 1.0f));

		glm::vec3 m = glm::max(0.5f - glm::vec3(glm::dot(x0, x0), glm::dot(glm::vec2(x12.xy), glm::vec2(x12.xy)), glm::dot(glm::vec2(x12.zw), glm::vec2(x12.zw))), 0.0f);
		m = m * m;
		m = m * m;

		glm::vec3 x = 2.0f * glm::fract(p * C.www) - 1.0f;
		glm::vec3 h = glm::abs(x) - 0.5f;
		glm::vec3 ox = glm::floor(x + 0.5f);
		glm::vec3 a0 = x - ox;

		m *= 1.79284291400159f - 0.85373472095314f * (a0 * a0 + h * h);

		glm::vec3 g;
		g.x  = a0.x  * x0.x  + h.x  * x0.y;
		g.yz = a0.yz * x12.xz + h.yz * x12.yw;

		return 130.0f * glm::dot(m, g);
	}


    float simplexNoise1(glm::vec2 & coord, unsigned int octaves)
    {
        float n = 0.0f;

        float persistence = 0.15f;

        for(int i = 1; i < octaves; ++i)
        {
            float frequency = std::pow(2.0f, i);
            float amplitude = std::pow(persistence, i);

            n += amplitude * glm::simplex(glm::vec3(coord * frequency, 0.5f));
        }

        return n;
    }


	float simplexNoise2(glm::vec2 & coord, float size)
	{
		float v = 0.0;
		float s = size;

		while(size >= 2.0)
		{
			v += snoise(coord / size) * size;
			size /= 4.0f;
		}

		return glm::clamp(0.7f * v / s, 0.0f, 2.0f);
	}
	/*
    float simplexNoise2(glm::vec2 & coord, float size)
    {
		
        float v = 0.0f;
        float s = size;

        while(size >= 2.0)
        {
            v += snoise(coord / size) * size;
            size /= 4.0f;
        }

        return glm::sqrt(glm::clamp(4.0f * v / s, 0.0f, 1.0f));
		//return glm::pow(1.0f - glm::abs(v / s), 2.0f);
    }*/
}