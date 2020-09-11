//
// Created by vova on 11.09.2020.
//

#pragma once

#include <iostream>
#include "algorithms/path_restorer.h"

inline void test_simulated_restoring() {
    size_t n = 7;
    std::vector<size_t> parents = {
        size_t(-1), 2, 4, 4, 0, 0, 1,
    };

    auto path = restore_path(parents, 0, 6);

    for(auto& elem : path) {
        std::cout << elem << "\n";
    } std::cout << std::endl;
}
