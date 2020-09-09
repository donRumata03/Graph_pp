//
// Created by Vova on 09.09.2020.
//

#pragma once

#include <cassert>
#include <iostream>

inline void test_NDEBUG () {
#if defined(NDEBUG)
	std::cout << "NDEBUG is defined" << std::endl;
#else
	std::cout << "NDEBUG is NOT defined" << std::endl;
#endif
}
