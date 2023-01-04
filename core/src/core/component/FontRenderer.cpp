#include "_Core.h"

#include "component/FontRenderer.h"

namespace core {

    //FONTRENDERER
    FontRenderer::FontRenderer() {
        // set renderer to FONTRENDERER
        this->typeID = std::string("font_renderer");
        // pascal you are funny
        Logger::Log("FOUND FONT RENDERER", Logger::Error);
    }

    FontRenderer::~FontRenderer() {

    }

    void FontRenderer::start() {

    }

    void FontRenderer::update(float deltaTime) {

    }

}