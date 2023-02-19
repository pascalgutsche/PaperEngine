#pragma once
#include "_Core.h"
#include "utility.h"

namespace core {

    class Texture {
    private:
        

    public:
    	virtual void Bind(unsigned int slot) = 0;
        virtual void Unbind() = 0;

        virtual int GetID() = 0;
        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual std::string GetFilePath() = 0;
        virtual std::string GetName() = 0;

        virtual ~Texture() = default;

        static Shr<Texture> CreateTexture(std::string filePath, std::string name);
    };

}