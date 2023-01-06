#include "_Core.h"

#include "component/FontRenderer.h"

namespace core {

    //FONTRENDERER
    FontRenderer::FontRenderer() {
        // set renderer to FONTRENDERER
        this->typeID = std::string("font_renderer");
        // pascal you are funny
        LOG_CORE_DEBUG("FOUND FONT RENDERER");
    }

    FontRenderer::~FontRenderer() {

    }

    void FontRenderer::start() {

    }

    void FontRenderer::update(float deltaTime) {

    }

}