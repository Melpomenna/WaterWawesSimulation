#include <Classes/Render/glad.h>
#include "Render.h"
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>

namespace Render
{
    class Render::RenderSingletoneContext final
    {
    public:
        explicit RenderSingletoneContext(Render* render);


        [[maybe_unused]] [[nodiscard]] const Render* ResolveRender() const noexcept
        {
            return render_;
        }

        void Resize(const int width, const int height) const noexcept
        {
            render_->width_ = width;
            render_->height_ = height;
        }

        [[nodiscard]] bool IsExtensionSupported(const GLuint extension) const noexcept
        {
            return render_->supportedExtensions_.size() > extension;
        }

    private:
        Render* render_;
    };

    Render::RenderSingletoneContext* CTX = nullptr;

    namespace
    {
        void OnClick(GLFWwindow*, int, int, int, int)
        {
        }

        void OnClickPos(GLFWwindow*, double, double)
        {
        }

        void OnScroll(GLFWwindow*, double, double)
        {
        }

        void OnResize(GLFWwindow*, const int width, const int height)
        {
            CTX->Resize(width, height);
        }

        void DebugGLLogOn(const GLenum source, const GLenum type, const GLuint id, const GLenum severity,
                          [[maybe_unused]] const GLsizei length, const GLchar* message,
                          [[maybe_unused]] const void* param)
        {
            const std::map<GLenum, std::string> mSources{
                {GL_DEBUG_SOURCE_API, "\033[32mGL_DEBUG_SOURCE_API: function from OpenGL API\033[0m"},
                {
                    GL_DEBUG_SOURCE_WINDOW_SYSTEM,
                    "\033[32mGL_DEBUG_SOURCE_WINDOW_SYSTEM: function from Window system\033[0m"
                },
                {
                    GL_DEBUG_SOURCE_THIRD_PARTY,
                    "\033[32mGL_DEBUG_SOURCE_THIRD_PARTY: function from third party OpenGL\033[0m"
                },
                {GL_DEBUG_SOURCE_APPLICATION, "\033[32mGL_DEBUG_SOURCE_APPLICATION: function from application\033[0m"},
                {GL_DEBUG_SOURCE_OTHER, "\033[32mGL_DEBUG_SOURCE_OTHER: function from other\033[0m"},
                {
                    GL_DEBUG_SOURCE_SHADER_COMPILER,
                    "\033[32mGL_DEBUG_SOURCE_SHADER_COMPILER: shader compiler function\033[0m"
                }
            };

            const std::map<GLenum, std::string> mTypes{
                {GL_DEBUG_TYPE_ERROR, "GL_DEBUG_TYPE_ERROR: error in OpenGL",},
                {
                    GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
                    "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: use deprecated OpenGL function",
                },
                {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: undefined behavior",},
                {GL_DEBUG_TYPE_PORTABILITY, "GL_DEBUG_TYPE_PORTABILITY: use unportable function",},
                {GL_DEBUG_TYPE_PERFORMANCE, "GL_DEBUG_TYPE_PERFORMANCE: low performance",},
                {GL_DEBUG_TYPE_MARKER, "GL_DEBUG_TYPE_MARKER: annotation",},
                {GL_DEBUG_TYPE_PUSH_GROUP, "GL_DEBUG_TYPE_PUSH_GROUP: adding a debugging group to the stack",},
                {GL_DEBUG_TYPE_POP_GROUP, "GL_DEBUG_TYPE_POP_GROUP: removing a debugging group from the stack",},
                {GL_DEBUG_TYPE_OTHER, "GL_DEBUG_TYPE_OTHER: other"},
            };

            const std::map<GLenum, std::string> mSeverity{
                {GL_DEBUG_SEVERITY_HIGH, "\033[31mGL_DEBUG_SEVERITY_HIGH: critical error\033[0m"},
                {GL_DEBUG_SEVERITY_MEDIUM, "\033[35mGL_DEBUG_SEVERITY_MEDIUM: important notification\033[0m"},
                {GL_DEBUG_SEVERITY_LOW, "\033[32mGL_DEBUG_SEVERITY_LOW: maybe unused notification\033[0m"},
                {GL_DEBUG_SEVERITY_NOTIFICATION, "\033[34mGL_DEBUG_SEVERITY_NOTIFICATION: notification\033[0m"}
            };

            static std::stringstream oss;

            std::stringstream toss;
            toss  << '[' << mSeverity.at(severity) << "]:" << mSources.at(source) << ",[" << mTypes.at(type) <<
                "] Id:" << id
                << ", Message:" << message << '\n';
            bool isSame = false;
            if (oss.str() != toss.str())
            {
                oss << toss.str();
            }
            else {
                isSame = true;
            }

            if (!isSame)
            {
                std::cerr << oss.str();
            }
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

        glEnable(GL_COLOR_BUFFER_BIT);

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

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        window_ = glfwCreateWindow(static_cast<int_fast64_t>(RenderView::Width),
                                   static_cast<int_fast64_t>(RenderView::Height), TITLE, nullptr, nullptr);

        glfwMakeContextCurrent(window_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD\n");
        }

        GLint supportedExtensionsCount;
        glGetIntegerv(GL_NUM_EXTENSIONS, &supportedExtensionsCount);

        supportedExtensions_ = std::pmr::vector<GLint>(supportedExtensionsCount);

        for (GLint i = 0; i < supportedExtensionsCount; ++i)
        {
            supportedExtensions_[i] = i;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH);

        width_ = static_cast<int>(RenderView::Width);
        height_ = static_cast<int>(RenderView::Height);
        depth_ = static_cast<int>(RenderView::Depth);

        SetUpViewPort();

        glfwSetKeyCallback(window_, OnClick);
        glfwSetCursorPosCallback(window_, OnClickPos);
        glfwSetScrollCallback(window_, OnScroll);
        glfwSetWindowSizeCallback(window_, OnResize);
        glDebugMessageCallback(DebugGLLogOn, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,GL_DONT_CARE, 0, nullptr, GL_TRUE);

        glfwShowWindow(window_);
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void Render::SetUpViewPort() const noexcept
    {
        const double w = width_;
        const double h = height_;

        glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    }

    Render::RenderSingletoneContext::RenderSingletoneContext(Render* const render) : render_(render)
    {
    }
}
