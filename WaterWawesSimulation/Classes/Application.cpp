#include "Application.h"

Application::Application()
{
    Init();
}


int Application::Exec() const noexcept
{
    while (render_->IsRunning())
    {
        render_->Update();
    }

    return 0;
}


void Application::Init()
{
    render_ = std::make_unique<Render::Render>();
}
