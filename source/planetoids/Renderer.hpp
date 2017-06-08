#pragma once

#ifndef _REDERER_HPP
#define _REDERER_HPP

#include "Error.hpp"
#include "Camera.hpp"
#include "Platform.hpp"
#include "Scene.hpp"
#include "Singleton.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"

namespace planetoids
{
    class Renderer : public Singleton<Renderer>
    {
        public:
            Renderer();
            virtual ~Renderer();

            void render(std::shared_ptr<Scene> & scene);
			void update(std::shared_ptr<Scene> & scene);

			void setMainCamera(std::shared_ptr<Camera> & camera);

            void clearColor(float r, float g, float b, float a);

            void clearScreen();

            float aspectRatio();

            unsigned int getFSAAsamples();

            bool getVsync();

            glm::ivec2 getWindowSize();

            std::string getWindowTitle();

            void setFSAAsamples(unsigned int val);

            void setPolygonMode(bool isWireframe);

            void setSkybox(std::shared_ptr<Skybox> & skybox);

            void setVsync(bool val);

            void setWindowTitle(std::string val);

            void showWindow(std::string title, glm::ivec2 & size);

            void swapBuffers();

        private:
            std::shared_ptr<Skybox> m_Skybox;
			std::shared_ptr<Camera> m_MainCamera;

			bool m_ShadowsEnabled;
			std::shared_ptr<Camera> m_ShadowCamera;
			std::shared_ptr<Shader> m_ShadowShader;
			//FrameBuffer * m_FrameBuffer;

            unsigned int m_FSAAsamples;
            bool m_FullScreen;
            bool m_VSync;
            std::string m_WindowTitle;
            glm::ivec2 m_WindowSize;
    };
}

#endif