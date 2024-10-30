#pragma once

#include <GLFW/glfw3.h>

enum class RenderView : int_fast64_t
{
	Width = 800,
	Height = 640,
	Depth = 800
};

#define TITLE "Water Wawes Simulation"

namespace Render
{
	class Render final
	{
	public:
		class RenderSingletoneContext;

	private:
		friend class RenderSingletoneContext;

	public:
		Render();
		~Render();

		Render(const Render&) = delete;
		Render& operator=(const Render&) = delete;

		[[nodiscard]] bool IsRunning() const noexcept;

		void Update();

	private:
		void Init();
		void Destroy() const noexcept;
		void SetUpViewPort() const noexcept;

	private:
		GLFWwindow* window_;
	};
}
