//
// Created by Vova on 09.09.2020.
//

#pragma once

#include <cassert>
#include <iostream>
#include <vector>

#include <data_structures/graph_base.h>

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

inline void test_maxing_types()
{
	using t1 = maximize_type<int>;
	using t2 = maximize_type<size_t>;

	using t3 = maximize_type<float>;
	using t4 = maximize_type<double>;

	using t5 = maximize_type<bool>;




	std::cout << typeid(t1).name() << std::endl;
	std::cout << typeid(t2).name() << std::endl;
	std::cout << typeid(t3).name() << std::endl;
	std::cout << typeid(t4).name() << std::endl;
	std::cout << typeid(t5).name() << std::endl;
}

inline void test_std_numeric_limits() {

#define tp double

#define make_tester(name) \
	auto name = []() { \
		std::cout \
				<< std::numeric_limits<tp>::infinity() << " " \
				<< std::numeric_limits<tp>::max() << " " \
				<< std::boolalpha << (std::numeric_limits<tp>::infinity() == std::numeric_limits<tp>::max()) \
				<< std::endl; \
	};

	make_tester(limit_tester)

	limit_tester();

#undef tp
#define tp long long

	make_tester(limit_tester2)
	limit_tester2();

	std::cout << std::numeric_limits<long long>::infinity() << " " << std::numeric_limits<long long>::epsilon() << std::endl;
}


inline void test_string_npos_and_size_t_minus1 () {
	std::cout << std::boolalpha << (std::numeric_limits<size_t>::max() == size_t(-1) && size_t(-1) == std::string::npos) << std::endl;
}
