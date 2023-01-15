#pragma once
#include "_Core.h"

#include "generic/GameObject.h"
#include "generic/Transform.h"

namespace core {

    struct GameObjectMap {
        std::vector<std::vector<GameObject*>> map;
        int width = 0;
        int height = 0;
        int ZERO_OFFSET_X = 0;
        int ZERO_OFFSET_Y = 0;

        void resize(int width, int height);
        std::vector<std::vector<GameObject*>>& getMap();

    };

    class Mesh {
    private:
        GameObjectMap coord;
        int zIndex;

        int mapCol = 0;
        int mapRow = 0;
        int indexRep0_X = 0;
        int indexRep0_Y = 0;
        Transform* lastTransform;


    public:
        Mesh(Transform transform, int zIndex);
        ~Mesh() = default;

        Transform transform;

        void addGameObject(GameObject* gameObject, int x, int y);
        void update(float dt);

    };

}