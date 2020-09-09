//
// Created by Vova on 09.09.2020.
//

#pragma once

#include <cassert>
#include <iostream>
#include <vector>

inline void test_NDEBUG () {
#if defined(NDEBUG)
	std::cout << "NDEBUG is defined" << std::endl;
#else
	std::cout << "NDEBUG is NOT defined" << std::endl;
#endif
}


inline void vector_size_test() {
	std::vector<int> iv = {1, 1, 1, 1, 1, 1, 1};
	std::cout << iv.capacity() << std::endl;
	iv.clear();
	std::cout << iv.capacity() << std::endl;
	std::vector<int>().swap(iv);
	std::cout << iv.capacity() << std::endl;

}
