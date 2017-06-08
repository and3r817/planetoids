#include "Heightmap.hpp"

namespace planetoids
{
    Heightmap::Heightmap(std::vector<char> & data, unsigned int size)
    {
        m_Size = size;

        std::vector<std::vector<float> > v(size, std::vector<float>(size, 0.0f));
        m_Heights = v;
        v.clear();

		auto k = 0;
        for(auto i = 0; i < m_Size; ++i)
        {
            for(auto j = 0; j < m_Size; ++j)
            {
                m_Heights[j][i] = glm::clamp(data[k] / 255.0f, 0.0f,1.0f);
				++k;
            }
        }
    }


    Heightmap::~Heightmap()
    {
        m_Heights.clear();
    }


    float Heightmap::getHeightAt(glm::vec2 & coord)
    {
// 		glm::ivec2 p0 = glm::ivec2(glm::clamp(glm::trunc(coord), 1, m_Size - 1));
// 		glm::ivec2 p1 = glm::ivec2(glm::clamp(p0 + 1, 1, m_Size - 1));
// 
// 		glm::vec2 d = coord - glm::vec2(p0);

		glm::ivec2 c = glm::ivec2(coord);
		if (c.x > 0)
		{
			c.x -= 1;
		}

		if (c.y > 0)
		{
			c.y -= 1;
		}
		return m_Heights[(int)c.x][(int)c.y];
		/*
        if(d.y > d.x)
        {
            return m_Heights[p1.y][p0.x] + (1.0f - d.y) * (m_Heights[p0.y][p0.x] - m_Heights[p1.y][p0.x]) + d.x * (m_Heights[p1.y][p1.x] - m_Heights[p1.y][p0.x]);
        }
        else
        {
            return m_Heights[p0.y][p1.x] + d.y * (m_Heights[p1.y][p1.x] - m_Heights[p0.y][p1.x]) + (1.0f - d.x) * (m_Heights[p0.y][p0.x] - m_Heights[p0.y][p1.x]);
        }
		*/
    }
}