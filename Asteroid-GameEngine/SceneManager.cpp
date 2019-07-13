#include "SceneManager.h"

std::vector<Shader> SceneManager::vec_ShaderProgram;
std::vector<DirectionalLight*> SceneManager::vec_DirectionlLight;
std::vector<PointLight*> SceneManager::vec_PointLight;
std::vector<Actor*> SceneManager::vec_ObjectsToRender;
std::vector<Actor*> SceneManager::Objects;
