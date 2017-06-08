#include "Scene.hpp"

namespace planetoids
{
    Scene::Scene()
    {
    }


    Scene::~Scene()
    {
    }


    void Scene::addLight(std::shared_ptr<Light> & light)
    {
        m_Lights.push_back(light);
    }


    void Scene::addObject(std::shared_ptr<RenderObject> mesh)
    {
        m_Objects.push_back(mesh);
    }


    void Scene::addObject(std::shared_ptr<Planet> & planet)
    {
        m_Planets.push_back(planet);
    }


    void Scene::clear()
    {
        m_Lights.clear();
        m_Objects.clear();
        m_Planets.clear();
    }


    std::vector<std::shared_ptr<Light>> Scene::getLights()
    {
        return m_Lights;
    }


    unsigned int Scene::getLightsCount()
    {
        return m_Lights.size();
    }


    std::vector<std::shared_ptr<RenderObject>> Scene::getObjects()
    {
        return m_Objects;
    }


    unsigned int Scene::getObjectsCount()
    {
        return m_Objects.size();
    }


    std::vector<std::shared_ptr<Planet>> Scene::getPlanets()
    {
        return m_Planets;
    }


    unsigned int Scene::getPlanetsCount()
    {
        return m_Planets.size();
    }


    void Scene::removeLight(std::shared_ptr<Light> & light)
    {
    }


    void Scene::removeObject(std::shared_ptr<RenderObject> & planet)
    {
    }


    void Scene::removeObject(std::shared_ptr<Planet> & planet)
    {
    }
}