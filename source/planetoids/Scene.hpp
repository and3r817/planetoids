#pragma once

#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "Light.hpp"
#include "Planet.hpp"
#include "Platform.hpp"
#include "RenderObject.hpp"

namespace planetoids
{
    class Scene
    {
        public:
            Scene();
            virtual ~Scene();

            void addLight(std::shared_ptr<Light> & light);

            void addObject(std::shared_ptr<RenderObject> mesh);

            void addObject(std::shared_ptr<Planet> & planet);

            void clear();

            std::vector<std::shared_ptr<Light>> getLights();

            unsigned int getLightsCount();

            std::vector<std::shared_ptr<RenderObject>> getObjects();

            unsigned int getObjectsCount();

            std::vector<std::shared_ptr<Planet>> getPlanets();

            unsigned int getPlanetsCount();

            void removeLight(std::shared_ptr<Light> & light);

            void removeObject(std::shared_ptr<RenderObject> & mesh);

            void removeObject(std::shared_ptr<Planet> & planet);

        private:
            std::vector<std::shared_ptr<Light>> m_Lights;
            std::vector<std::shared_ptr<RenderObject>> m_Objects;
            std::vector<std::shared_ptr<Planet>> m_Planets;
    };
}

#endif