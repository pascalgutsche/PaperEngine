#pragma once
#include "component/PhysicsComponent.h"
#include "component/CameraComponent.h"
#include "component/DataComponent.h"
#include "component/LineComponent.h"
#include "component/ScriptComponent.h"
#include "component/SpriteComponent.h"
#include "component/TextComponent.h"
#include "component/TransformComponent.h"

namespace Paper {

    template<typename... Component>
    struct ComponentGroup
    {
    };

    using AllComponents =
        ComponentGroup<TransformComponent,
        SpriteComponent, LineComponent, TextComponent,
        CameraComponent, ScriptComponent,
		Rigidbody2DComponent, Collider2DComponent>;

}