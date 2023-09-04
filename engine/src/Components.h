#pragma once
#include "core/component/CameraComponent.h"
#include "core/component/CubeRenderer.h"
#include "core/component/DataComponent.h"
#include "core/component/LightComponent.h"
#include "core/component/LineComponent.h"
#include "core/component/ScriptComponent.h"
#include "core/component/SpriteComponent.h"
#include "core/component/TextComponent.h"
#include "core/component/TransformComponent.h"
#include "core/component/2D/SpriteSheet.h"

namespace Paper {

    template<typename... Component>
    struct ComponentGroup
    {
    };

    using AllComponents =
        ComponentGroup<TransformComponent,
        SpriteComponent, LineComponent, TextComponent,
        CameraComponent, ScriptComponent>;

}