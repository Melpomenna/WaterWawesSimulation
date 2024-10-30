#pragma once

#include <memory>

#include <Classes/Render/Render.h>

class Application
{
public:
	Application();
	~Application() = default;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	[[nodiscard]] int Exec() const noexcept;

private:
	void Init();

private:
	std::unique_ptr<Render::Render> render_;
};

