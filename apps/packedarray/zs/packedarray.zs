package packedarray;

struct Array<T> {
  varsize size;
  T data[size];
};

struct PackedArray<T> {
  varsize size;
  packed T data[size];
};

instantiate Array<uint8> ArrayUint8;
instantiate PackedArray<uint8> PackedArrayUint8;


struct ContainerUnpacked
{
  ArrayUint8 array;
};

struct ContainerPacked
{
  PackedArrayUint8 array;
};

struct ContainerNativePacked
{
  packed uint8 data[];
};