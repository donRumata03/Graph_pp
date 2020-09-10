//
// Created by Vova on 04.09.2020.
//

#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <execution>

#include <ostream>
#include <iomanip>


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

