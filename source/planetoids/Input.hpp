#pragma once

#ifndef _INPUT_HPP
#define _INPUT_HPP

#include "Platform.hpp"
#include "Renderer.hpp"
#include "Singleton.hpp"

namespace planetoids
{
    class Input : public Singleton<Input>
    {
        public:
            Input();
            virtual ~Input();

            void setMousePosition(glm::ivec2 & position);
            void lockMousePosition();

            glm::ivec2 mousePosition();
            int mouseWeelDelta();

            bool mouseButtonPress(int mouseButton);
            bool mouseButtonUp(int mouseButton);

            bool keyPress(int key);
            bool keyUp(int key);
    };
}

#endif
