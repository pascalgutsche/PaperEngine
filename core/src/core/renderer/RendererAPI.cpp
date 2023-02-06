#include "_Core.h"
#include "RendererAPI.h"

#include <glad/glad.h>


namespace core
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(message); return;
		}

		CORE_ASSERT(false, "Unknown severity level!");
	}

	void RendererAPI::Init()
	{
#ifdef BUILD_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RendererAPI::Clear(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void RendererAPI::DrawElements(Shr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		vertexArray->GetVertexBuffer()->Bind();
		vertexArray->GetElementBuffer()->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetElementBuffer()->GetElementCount(), GL_UNSIGNED_INT, nullptr);
	}

	Shr<RendererAPI> RendererAPI::CreateAPI()
	{
		return MakeShr<RendererAPI>();
	}
}
