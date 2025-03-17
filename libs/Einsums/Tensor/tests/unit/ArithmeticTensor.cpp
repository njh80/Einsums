//--------------------------------------------------------------------------------------------
// Copyright (c) The Einsums Developers. All rights reserved.
// Licensed under the MIT License. See LICENSE.txt in the project root for license information.
//--------------------------------------------------------------------------------------------

#include <Einsums/Tensor.hpp>
#include <Einsums/TensorUtilities.hpp>

#include <Einsums/Testing.hpp>

TEST_CASE("Arithmetic Tensor") {
    using namespace einsums;
    size_t size = 10;
    auto   A    = create_random_tensor("A", size, size);
    auto   B    = create_random_tensor("B", size, size);
    auto   C    = create_tensor_like(A);

    C = A + B;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs(A(i, j) + B(i, j), 1e-10));
        }
    }

    C = A - B;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs(A(i, j) - B(i, j), 1e-10));
        }
    }

    C = A * B;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs(A(i, j) * B(i, j), 1e-10));
        }
    }

    C = A / B;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs(A(i, j) / B(i, j), 1e-10));
        }
    }

    C = -A;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs(-A(i, j), 1e-10));
        }
    }

    C = 2.0 * A;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs(2.0 * A(i, j), 1e-10));
        }
    }

    C = (A + B) / (A * B);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs((A(i, j) + B(i, j)) / (A(i, j) * B(i, j)), 1e-10));
        }
    }

    C = 2.0 * A + B;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            CHECK_THAT(C(i, j), Catch::Matchers::WithinAbs(2.0 * A(i, j) + B(i, j), 1e-10));
        }
    }
}