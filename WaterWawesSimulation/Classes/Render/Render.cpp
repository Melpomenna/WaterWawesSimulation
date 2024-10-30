#include <Classes/Render/RenderHelper.h>

#include "Render.h"

#include <functional>
#include <stdexcept>

namespace Render
{
	class Render::RenderSingletoneContext final
	{
	public:
		explicit RenderSingletoneContext(const Render* render);


		[[maybe_unused]] [[nodiscard]] const Render* ResolveRender() const noexcept
		{
			return render_;
		}

	private:
		const Render* render_;
	};

	Render::RenderSingletoneContext* CTX = nullptr;

	namespace
	{
		void OnClick(GLFWwindow*, int, int, int, int)
		{
		}
	}

	Render::Render() : window_(nullptr)
	{
		if (!CTX)
		{
			CTX = new RenderSingletoneContext(this);
		}
		Init();
	}

	Render::~Render()
	{
		if (!window_)
		{
			return;
		}
		delete CTX;
		CTX = nullptr;
		Destroy();
		window_ = nullptr;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void Render::Update()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}

	bool Render::IsRunning() const noexcept
	{
		return !glfwWindowShouldClose(window_);
	}


	void Render::Destroy() const noexcept
	{
		glfwDestroyWindow(window_);
		glfwTerminate();
	}


	void Render::Init()
	{
		if (glfwInit() == GLFW_FALSE)
		{
			throw std::runtime_error("Cannot Init GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);


		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window_ = glfwCreateWindow(static_cast<int_fast64_t>(RenderView::Width),
		                           static_cast<int_fast64_t>(RenderView::Height), TITLE, nullptr, nullptr);

		if (!GL_CALL(glfwMakeContextCurrent, window_))
		{
			throw std::runtime_error("Something went wrong!");
		}

		if (!GL_CALL(glEnable, GL_BLEND))
		{
			throw std::runtime_error("Cannot enable GL_BLEND");
		}

		if (!GL_CALL(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
		{
			throw std::runtime_error("Cannot set blend func");
		}

		if (!GL_CALL(glEnable, GL_DEPTH_TEST))
		{
			throw std::runtime_error("Cannot enable GL_DEPTH_TEST");
		}

		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("Cannot init GLEW");
		}

		SetUpViewPort();

		glfwSetKeyCallback(window_, OnClick);

		glfwShowWindow(window_);
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	void Render::SetUpViewPort() const noexcept
	{
		constexpr double w = static_cast<double>(RenderView::Width);
		constexpr double h = static_cast<double>(RenderView::Height);
		constexpr double d = static_cast<double>(RenderView::Depth);

		glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-w, w, -h, h, -d, d);
	}

	Render::RenderSingletoneContext::RenderSingletoneContext(const Render* render) : render_(render)
	{
	}
}
