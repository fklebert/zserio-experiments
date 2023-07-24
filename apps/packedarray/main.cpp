#include <iostream>
#include <vector>
#include <packedarray/ContainerUnpacked.h>
#include <packedarray/ContainerPacked.h>
#include <packedarray/ContainerNativePacked.h>
#include <zserio/FileUtil.h>
#include <zserio/SerializeUtil.h>

#include "RolledArray.h"

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


    return 0;
}