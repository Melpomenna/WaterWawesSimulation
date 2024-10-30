#pragma once

#include <string>
#include <type_traits>

#include <GL/glew.h>

#include <iostream>

#include <sstream>
#include <thread>

#include <cassert>

#define GL_CALL(function, ...) GlCallImpl(__FILE__, __LINE__, #function, #__VA_ARGS__, function, __VA_ARGS__)


template <typename FuncPtrType>
bool CheckGlErrors(const char* functionName, const char* params, const FuncPtrType& lastFunc,
                   const std::string& filename,
                   const std::uint_fast32_t line)
{
	if (GLenum error = glGetError(); error != GL_NO_ERROR)
	{
		std::stringstream os;
		os << " ***ERROR*** in (" << filename << ": " << std::to_string(line) << ") " << functionName << "(" << params
			<< ")";
		do
		{
			std::stringstream oss;
			std::string errorStr = "Unknown Error";
			switch (error)
			{
			case GL_INVALID_ENUM:
				errorStr = "Invalid Enumerator";
				break;
			case GL_INVALID_VALUE:
				errorStr = "Invalid Value";
				break;
			case GL_INVALID_OPERATION:
				errorStr = "Invalid Operation";
				break;
			case GL_STACK_OVERFLOW:
				errorStr = "Stack Overflow";
				break;
			case GL_STACK_UNDERFLOW:
				errorStr = "Stack Underflow";
				break;
			case GL_OUT_OF_MEMORY:
				errorStr = "Out of Memory";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorStr = "Invalid Frame Buffer Operation";
				break;
			case GL_CONTEXT_LOST:
				errorStr = "Context Lost";
				break;
			default: break;
			}
			oss << errorStr << os.str();
			std::cerr << "\n\t--" << oss.str() << "--" << "\n";
			assert(error == GL_NO_ERROR);
		}
		while ((error = glGetError()) != GL_NO_ERROR);

		return false;
	}
	return true;
}

template <typename glFunction, typename... Args>
bool GlCallImpl(const char* filename, const std::uint_fast32_t line, const char* functionName, const char* paramsNames,
                const glFunction& function,
                Args&&... args) requires (std::is_same_v<void, decltype(function(args...))>)
{
	if (function == nullptr)
	{
		return false;
	}
	function(std::forward<Args>(args)...);
	return CheckGlErrors(functionName, paramsNames, function, filename, line);
}
