//
// Created by Vova on 04.09.2020.
//

#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <algorithm>
#include <ostream>

#include <execution>
#include <cassert>
#include <iomanip>

using namespace std::string_literals;

/// Console encoding stuff:
#ifdef WIN32
#define IS_WINDOWS
#include "windows.h"
#undef max
#undef min
#endif

inline void switch_console_to_utf8() {
#ifdef IS_WINDOWS
	SetConsoleOutputCP(CP_UTF8);
	// system(("chcp "s + std::to_string(CP_UTF8)).c_str());
#else // Assume, it`s Linux => the console already uses utf8 by default:
	// NOP
#endif
}

namespace detail
{
	template <
			typename T,
			typename std::enable_if<std::is_floating_point_v<T>
			                        || (std::is_integral_v<T> && !std::is_same_v<T, bool>), int> * = nullptr
	>
	struct maximize_type_impl
	{
		using type = typename std::conditional<std::is_floating_point_v<T>, double, long long>::type;
	};



// || (std::is_integral_v<T> && !std::is_same_v<T, bool>)>* = nullptr

	template <>
	struct maximize_type_impl<bool>
	{
		// static_assert(false, "Can`t maximize type 'bool' ");
		using type = long long int;
	};

}


template<class T>
using maximize_type = typename detail::maximize_type_impl<T>::type;

