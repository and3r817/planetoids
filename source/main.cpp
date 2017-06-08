#include "planetoids/Cubemap.hpp"
#include "planetoids/Camera.hpp"
#include "planetoids/Input.hpp"
#include "planetoids/Light.hpp"
#include "planetoids/Renderer.hpp"
#include "planetoids/Shader.hpp"
#include "planetoids/Planet.hpp"
#include "planetoids/Platform.hpp"
#include "planetoids/Singleton.hpp"
#include "planetoids/Skybox.hpp"
#include "planetoids/Spaceship.hpp"
#include "planetoids/SimpleObject.hpp"
#include "planetoids/Sprite.hpp"
#include "planetoids/Timer.hpp"

using namespace planetoids;

int main()
{
    new Renderer();
    new Timer();
    new Input();

    Renderer::ptr()->showWindow("Planetoids", glm::ivec2(1024, 576));
    Renderer::ptr()->setVsync(true);
    Renderer::ptr()->setFSAAsamples(4);
    Renderer::ptr()->clearColor(0.0f, 0.0f, 0.0f, 1.0f);

    auto _skyboxCubemap = std::make_shared<Cubemap>("data\\spacebox01\\spacebox01");
    auto _skybox = std::make_shared<Skybox>(_skyboxCubemap);
    Renderer::ptr()->setSkybox(_skybox);

    auto _scene = std::make_shared<Scene>();

    auto _mainCamera = std::make_shared<Camera>();
    _mainCamera->perspective(60.0f, Renderer::ptr()->aspectRatio(), 1.0f, 100000.0f);
    Renderer::ptr()->setMainCamera(_mainCamera);

    auto _sunLight = std::make_shared<Light>();
    _sunLight->setPosition(glm::vec3(20000.0f, 0.0f, 0.0f));
    _sunLight->setIntensity(6.0f);

    _scene->addLight(_sunLight);

    /*
    auto _spriteMesh = Mesh::loadFromOBJ("data\\models\\plane.obj");
    auto _sunSpite = std::make_shared<Sprite>();
    _sunSpite->setGeometry(_spriteMesh);
    _sunSpite->setShader(std::make_shared<Shader>("data\\shaders\\Star"));
    _sunSpite->setMainTexture(Texture2D::loadFromFile("data\\textures\\star.tga"));
    _sunSpite->setPosition(glm::vec3(20000.0f, 0.0f, 0.0f));
    _scene->addObject(_sunSpite);
    */

    auto _planet = std::make_shared<Planet>();
    _planet->addTexture(Texture2D::loadFromFile("data\\textures\\grass0.tga"));
    _planet->addTexture(Texture2D::loadFromFile("data\\textures\\ground0.tga"));
    _scene->addObject(_planet);

    auto _spiratMesh = Mesh::loadFromOBJ("data\\models\\miningship.obj");
    auto _spirat = std::make_shared<Spaceship>(_mainCamera);
    _spirat->setGeometry(_spiratMesh);
    _spirat->setShader(std::make_shared<Shader>("data\\shaders\\Specular"));
    _spirat->setMainTexture(Texture2D::loadFromFile("data\\textures\\miningship_main.tga"));
    _spirat->setNormalTexture(Texture2D::loadFromFile("data\\textures\\miningship_normal.tga"));
    //_spirat->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    _spirat->setPosition(glm::vec3(-3.0f, -11.0f, -8955.0f));
    _mainCamera->setPosition(_spirat->rotation() * glm::vec3(0.0f, 0.0f, -7.0f) + _spirat->position());
    _mainCamera->lookAt(_spirat->position(), _spirat->up());
    _scene->addObject(_spirat);

    auto _stationMesh = Mesh::loadFromOBJ("data\\models\\kanostation.obj");
    auto _station = std::make_shared<SimpleObject>();
    _station->setGeometry(_stationMesh);
    _station->setShader(std::make_shared<Shader>("data\\shaders\\Specular"));
    _station->setMainTexture(Texture2D::loadFromFile("data\\textures\\kanostation_main.tga"));
    _station->setNormalTexture(Texture2D::loadFromFile("data\\textures\\kanostation_normal.tga"));
    _station->setPosition(glm::vec3(0.0f, 0.0f, -9000.0f));
    _scene->addObject(_station);

    auto _objMesh = Mesh::loadFromOBJ("data\\models\\morena.obj");
	auto _objMainTex = Texture2D::loadFromFile("data\\textures\\morena_main.tga");
		auto _objNormalTex = Texture2D::loadFromFile("data\\textures\\morena_normal.tga");

    auto _obj1 = std::make_shared<SimpleObject>();
    _obj1->setGeometry(_objMesh);
    _obj1->setShader(std::make_shared<Shader>("data\\shaders\\Specular"));
    _obj1->setMainTexture(_objMainTex);
    _obj1->setNormalTexture(_objNormalTex);
    _obj1->setPosition(glm::vec3(0.0f, 0.0f, -8600.0f));
    _obj1->rotate(glm::vec3(0.0f, glm::radians(105.0f), 0.0f));
    _scene->addObject(_obj1);

	auto _obj2 = std::make_shared<SimpleObject>();
	_obj2->setGeometry(_objMesh);
	_obj2->setShader(std::make_shared<Shader>("data\\shaders\\Specular"));
	_obj2->setMainTexture(_objMainTex);
	_obj2->setNormalTexture(_objNormalTex);
	_obj2->setPosition(glm::vec3(0.0f, 10.0f, -8650.0f));
	_obj2->rotate(glm::vec3(0.0f, glm::radians(105.0f), 0.0f));
	_scene->addObject(_obj2);

	auto _obj3 = std::make_shared<SimpleObject>();
	_obj3->setGeometry(_objMesh);
	_obj3->setShader(std::make_shared<Shader>("data\\shaders\\Specular"));
	_obj3->setMainTexture(_objMainTex);
	_obj3->setNormalTexture(_objNormalTex);
	_obj3->setPosition(glm::vec3(0.0f, -10.0f, -8650.0f));
	_obj3->rotate(glm::vec3(0.0f, glm::radians(105.0f), 0.0f));
	_scene->addObject(_obj3);

    do
    {
        Timer::ptr()->frameBegin();

        Renderer::ptr()->clearScreen();
        Renderer::ptr()->update(_scene);

		_obj1->translate(_obj1->foward() * Timer::ptr()->deltaTime() * 7.5f);
		_obj2->translate(_obj2->foward() * Timer::ptr()->deltaTime() * 6.0f);
		_obj3->translate(_obj3->foward() * Timer::ptr()->deltaTime() * 4.7f);

        Renderer::ptr()->render(_scene);
        Renderer::ptr()->swapBuffers();

        if(Input::ptr()->keyPress('2'))
        {
            Renderer::ptr()->setPolygonMode(true);
        }

        else if(Input::ptr()->keyPress('1'))
        {
            Renderer::ptr()->setPolygonMode(false);
        }

        //printf("FPS: %f\n", Timer::ptr()->framesPerSecond());
        //printf("Delta: %f\n", Timer::ptr()->deltaTime());
        //printf("Nodes Count: %d\n", _planet->nodesCount());
        //printf("Rendered Nodes Count: %d\n", _planet->renderedNodesCount());
        //printf("Vertices Count: %d\n", _planet->verticesCount());
        //printf("Triangles Count: %d\n", _planet->trianglesCount());

        Timer::ptr()->frameEnd();
    }

    while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));

    printf("Killing renderer... OK\n");

    return 0;
}