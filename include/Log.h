#pragma once

#include <cstdio>

inline void log_info(const char* const msg)
{
	std::printf("[I] %s \n", msg);
}

template <typename T>
inline void log_info(const char* const msg, const T* const ptr)
{
	std::printf("[I] %s %p \n", msg, ptr);
}
