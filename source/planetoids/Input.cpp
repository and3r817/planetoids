#include "Input.hpp"

namespace planetoids
{
    Input::Input()
    {
    }


    Input::~Input()
    {
    }


	void Input::setMousePosition(glm::ivec2 & position)
	{
		glfwSetMousePos(position.x, position.y);
	}


	void Input::lockMousePosition()
	{
		setMousePosition(Renderer::ptr()->getWindowSize() / 2);
	}


    glm::ivec2 Input::mousePosition()
    {
        auto mpos = glm::ivec2(0);
        glfwGetMousePos(&mpos.x, &mpos.y);

        return mpos;
    }


    int Input::mouseWeelDelta()
    {
        return glfwGetMouseWheel();
    }


    bool Input::mouseButtonPress(int mouseButton)
    {
        if(glfwGetMouseButton(mouseButton) == GLFW_PRESS)
        {
            return true;
        }

        return false;
    }


    bool Input::mouseButtonUp(int mouseButton)
    {
        if(glfwGetMouseButton(mouseButton) == GLFW_RELEASE)
        {
            return true;
        }

        return false;
    }


    bool Input::keyPress(int key)
    {
        if(glfwGetKey(key) == GLFW_PRESS)
        {
            return true;
        }

        return false;
    }


    bool Input::keyUp(int key)
    {
        if(glfwGetKey(key) == GLFW_RELEASE)
        {
            return true;
        }

        return false;
    }
}