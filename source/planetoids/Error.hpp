#pragma once

#ifndef _ERROR_HPP
#define _ERROR_HPP

#include "Platform.hpp"

namespace planetoids
{
#define OPENGL_CALL(expression) \
	{ \
		expression; \
		GLenum g_OpenGLError; \
		if ((g_OpenGLError = glGetError()) != GL_NO_ERROR) \
		{ \
			system("PAUSE"); \
			printf("OpenGL expression \"" #expression "\" error 0x%X\n", (unsigned)g_OpenGLError); \
		} \
	}
}

#endif