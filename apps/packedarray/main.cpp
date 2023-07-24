#include <iostream>
#include <vector>
#include <packedarray/ContainerUnpacked.h>
#include <packedarray/ContainerPacked.h>
#include <packedarray/ContainerNativePacked.h>
#include <zserio/FileUtil.h>
#include <zserio/SerializeUtil.h>

#include "RolledArray.h"

template <typename T, typename D>
auto makeObj(D &&data)
{
    T result;
    result.setData(std::forward<D>(data));
    result.setSize(result.getData().size());
    return result;
}

template <typename A, typename PA, typename D>
auto makeArraysPair(D &&data)
{
    std::pair<A, PA> result;
    result.first.setData(data);
    result.first.setSize(result.first.getData().size());
    result.second.setData(std::forward<D>(data));
    result.second.setSize(result.second.getData().size());
    return result;
}

template<typename T>
void testCase(const std::string patternName, const T& data)
{
    std::cout << "Pattern (" << patternName << "): " << std::endl;
    std::cout << "====================" << std::endl;

    auto objs = makeArraysPair<packedarray::ArrayUint8, packedarray::PackedArrayUint8>(data);
    ::zserio::serializeToFile(objs.first, patternName + "-rArray.bin"); // raw
    ::zserio::serializeToFile(objs.second, patternName + "-pArray.bin"); // packed

    std::cout << "elements: " << data.size() << std::endl;
    std::cout << "bit size:" << std::endl
              << "        ARRAY: " << objs.first.bitSizeOf() << std::endl
              << " PACKED_ARRAY: " << objs.second.bitSizeOf() << std::endl;
    std::cout << "bits per element:" << std::endl
              << "        ARRAY: " << (double(objs.first.bitSizeOf()) / data.size()) << std::endl
              << " PACKED_ARRAY: " << (double(objs.second.bitSizeOf()) / data.size()) << std::endl;
    std::cout << "size diff: " << (100.0 * (double(objs.second.bitSizeOf()) - double(objs.first.bitSizeOf())) / double(objs.first.bitSizeOf())) << "%" << std::endl;
    if (objs.first.bitSizeOf() < objs.second.bitSizeOf())
    {
        std::cout << " !!!WARNING!!! not effective packing" << std::endl;
    }
    std::cout << std::endl;
}

void uint8_test()
{
    std::cout << std::endl;
    std::cout << "####################" << std::endl;
    std::cout << "#### uint8_test ####" << std::endl;
    std::cout << std::endl;

    testCase("0-255", []()
             {
            std::vector<uint8_t> result;
            std::generate_n(std::back_inserter(result), 256, [n = 0]() mutable -> uint8_t {
                return n++;
            });
            return result; }());

    std::cout << "single zero completely destroys effectivity." << std::endl;
    testCase("0-255+0", []()
             {
            std::vector<uint8_t> result;
            std::generate_n(std::back_inserter(result), 256, [n = 0]() mutable -> uint8_t {
                return n++;
            });
            result.push_back(0);
            return result; }());

    testCase("0+1+0+1", []()
             {
            std::vector<uint8_t> result;
            std::generate_n(std::back_inserter(result), 256, [n = 0]() mutable -> uint8_t {
                return (n++ & 1) ? 0 : 1;
            });
            return result; }());

    testCase("0+32+0+32", []()
             {
            std::vector<uint8_t> result;
            std::generate_n(std::back_inserter(result), 256, [n = 0]() mutable -> uint8_t {
                return (n++ & 1) ? 0 : 32;
            });
            return result; }());

    std::cout << "too big delta completely destroys effectivity." << std::endl;
    testCase("0+64+0+64", []()
             {
            std::vector<uint8_t> result;
            std::generate_n(std::back_inserter(result), 256, [n = 0]() mutable -> uint8_t {
                return (n++ & 1) ? 0 : 64;
            });
            return result; }());

    std::cout << "too big delta completely destroys effectivity." << std::endl;
    testCase("0+128+0+128", []()
             {
            std::vector<uint8_t> result;
            std::generate_n(std::back_inserter(result), 256, [n = 0]() mutable -> uint8_t {
                return (n++ & 1) ? 0 : 128;
            });
            return result; }());

    std::cout << "32791 0(zero) bits and only single 1(one) bit completely destroys effectivity." << std::endl;
    testCase("0-0+64", []()
             {
            std::vector<uint8_t> result;
            std::generate_n(std::back_inserter(result), 4 * 1024, [n = 0]() mutable -> uint8_t {
                return 0;
            });
            result.push_back(64);
            return result; }());
}

int main()
{
    // create containers for unpacked, packed, and native packed arrays
    packedarray::ContainerUnpacked containerUnpacked;
    packedarray::ContainerPacked containerPacked;
    packedarray::ContainerPacked containerPacked2;
    packedarray::ContainerNativePacked containerNativePacked;

    // create a vector from the rolled array
    std::vector<uint8_t> rolledVector(rolledArray.begin(), rolledArray.end());
    std::vector<uint8_t> rolledVector2(rolledArray2.begin(), rolledArray2.end());

    // create an unpacked array and set it in the unpacked container
    packedarray::ArrayUint8 array(ARRAY_SIZE, rolledVector);
    containerUnpacked.setArray(array);

    // print the size of the unpacked container
    std::cout << "Unpacked Array: " << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "size: " << containerUnpacked.bitSizeOf() << std::endl;
    std::cout << "bits per element: " << (containerUnpacked.bitSizeOf() / ARRAY_SIZE) << std::endl;
    std::cout <<  std::endl;

    // create a packed array and set it in the packed container
    packedarray::PackedArrayUint8 packedArray(ARRAY_SIZE, rolledVector);
    containerPacked.setArray(packedArray);

    // print the size of the packed container
    std::cout << "Packed Array: " << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "size: " << containerPacked.bitSizeOf() << std::endl;
    std::cout << "bits per element: " << (containerPacked.bitSizeOf() / ARRAY_SIZE) << std::endl;
    std::cout <<  std::endl;
    
    // create a native array and set it in the native packed container
    containerNativePacked.setData(rolledVector);

    // print the size of the native packed container
    std::cout << "Native Packed Array: " << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "size: " << containerNativePacked.bitSizeOf() << std::endl;
    std::cout << "bits per element: " << (containerNativePacked.bitSizeOf() / ARRAY_SIZE) << std::endl;
    std::cout <<  std::endl;
    
    // Demonstrate a non-optimal packed array by wrapping the content around, i.e. restart at
    // 0 after reaching 255.
    packedarray::PackedArrayUint8 packedArray2(ARRAY_SIZE_2, rolledVector2);
    containerPacked2.setArray(packedArray2);

    // print the size of the packed container
    std::cout << "Packed Array (3 times 0..255): " << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "size: " << containerPacked2.bitSizeOf() << std::endl;
    std::cout << "bits per element: " << (containerPacked2.bitSizeOf() / ARRAY_SIZE_2) << std::endl;
    std::cout <<  std::endl;

    // write the containers to disk
    zserio::serializeToFile(containerUnpacked, "unpacked.bin");
    zserio::serializeToFile(containerPacked, "packed.bin");
    zserio::serializeToFile(containerNativePacked, "nativepacked.bin");
    zserio::serializeToFile(containerPacked2, "packed2.bin");

    uint8_test();

    return 0;
}