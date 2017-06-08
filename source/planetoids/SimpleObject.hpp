#pragma once

#ifndef _SIMPLEOBJECT_HPP
#define _SIMPLEOBJECT_HPP

#include "RenderObject.hpp"
#include "Mesh.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

namespace planetoids
{
    class SimpleObject : public RenderObject
    {
        public:
            SimpleObject();
            virtual ~SimpleObject();

            void render(std::shared_ptr<Camera> & camera);
			void update();
    };
}

#endif
