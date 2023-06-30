#include "Engine.h"
#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace ppr
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

	void OpenGLRenderAPI::Init()
	{
#ifdef BUILD_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRenderAPI::SetClearColor(glm::vec4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::SetPolygonModel(Polygon pol)
	{
		switch (pol) {
			case Polygon::OFF: 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case Polygon::LINE:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
			case Polygon::POINT: 
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				break;
		}
	}

	void OpenGLRenderAPI::EnableDepthTesting(bool enabled)
	{
		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);


	}

	bool OpenGLRenderAPI::IsDepthTestingEnabled()
	{
		return depthTestingEnabled;
	}

	void OpenGLRenderAPI::DrawElements(Shr<VertexArray>& vertexArray, uint32_t elementCount)
	{
		vertexArray->Bind();
		uint32_t count = elementCount ? elementCount : vertexArray->GetElementBuffer()->GetElementCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		vertexArray->Unbind();
	}
	
	void OpenGLRenderAPI::DrawLines(Shr<VertexArray>& vertexArray, uint32_t vertexCount, float thickness)
	{
		vertexArray->Bind();
		SetLineWidth(thickness);
		glDrawArrays(GL_LINES, 0, vertexCount);
	}
	
	void OpenGLRenderAPI::SetLineWidth(float thickness)
	{
		glLineWidth(thickness);
	}
}
