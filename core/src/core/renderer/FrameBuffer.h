#pragma once
#include "_Core.h"
#include "utility.h"

namespace core {


	enum class FramebufferTexFormat
	{
		None = 0,

		//COLORS
		RGBA8,

		//DEPTH
		DEPTH24STECIL8,

		//DEFAULT
		Depth = DEPTH24STECIL8
	};

	struct FramebufferTexSpecification 
	{
		FramebufferTexSpecification() = default;
		FramebufferTexSpecification(FramebufferTexFormat format)
			: format(format) { };

		FramebufferTexFormat format;
	};

	struct FramebufferAttachSpecification 
	{
		FramebufferAttachSpecification() = default;
		FramebufferAttachSpecification(std::initializer_list<FramebufferTexSpecification> attachments) 
			: attachments(attachments) { }

		std::vector<FramebufferTexSpecification> attachments;
	};

	struct FramebufferSpecification 
	{
		FramebufferAttachSpecification attachments;

		uint32_t width, height;
		uint32_t samples = 1;
	};

	
	class FrameBuffer
	{
	private:
		FramebufferSpecification specification;
		unsigned int fboID = 0;
		unsigned int rbo = 0;
		uint32_t color;
		uint32_t depth;
	public:
		FrameBuffer(const FramebufferSpecification& specification);
		~FrameBuffer();

		uint32_t GetColorID() const { return color; }

		void Invalidate();
		void Resize(unsigned int width, unsigned int height);

		void Bind();
		void Unbind();

		FramebufferSpecification& GetProperties() { return specification; }
	};
}


