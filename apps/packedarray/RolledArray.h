// RolledArray.h
#ifndef ROLLEDARRAY_H
#define ROLLEDARRAY_H

#include <array>

constexpr size_t ARRAY_SIZE = 255;
constexpr size_t REPEAT = 3;
constexpr size_t ARRAY_SIZE_2 = ARRAY_SIZE * REPEAT;

template <std::size_t... Is>
constexpr auto generate_array(std::index_sequence<Is...>) {
    return std::array<uint8_t, ARRAY_SIZE>{{(uint8_t)Is...}};
}

template <std::size_t... I1, std::size_t... I2, std::size_t... I3>
constexpr auto generate_array_repeated(std::index_sequence<I1...>, std::index_sequence<I2...>, std::index_sequence<I3...>) {
    return std::array<uint8_t, ARRAY_SIZE_2>{{(uint8_t)I1..., (uint8_t)I2..., (uint8_t)I3...}};
}

constexpr auto rolledArray = generate_array(std::make_index_sequence<ARRAY_SIZE>{});
constexpr auto rolledArray2 = generate_array_repeated(std::make_index_sequence<ARRAY_SIZE>{}, std::make_index_sequence<ARRAY_SIZE>{}, std::make_index_sequence<ARRAY_SIZE>{});


#endif /* ROLLEDARRAY_H */
