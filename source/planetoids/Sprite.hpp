#pragma once

#ifndef _SPRITE_HPP
#define _SPRITE_HPP

#include "RenderObject.hpp"
#include "Mesh.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

namespace planetoids
{
    class Sprite : public RenderObject
    {
        public:
            Sprite();
            virtual ~Sprite();

            void render(std::shared_ptr<Camera> & camera);
			void update();
    };
}

#endif
