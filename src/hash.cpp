#include "uavcan-stm32-cubehal-base/hash.hpp"

extern char _sidata;
extern char _sdata;
extern char _edata;

namespace
{
constexpr uint32_t FlashStartAddress = 0x08000000;

uint32_t getBinarySize()
{
    return (&_sidata - reinterpret_cast<char *>(FlashStartAddress)) + (&_edata - &_sdata);
}

uint64_t fnv(const uint8_t *data, const uint8_t *const dataEnd)
{
    return bus_node_base::fnvWithSeed(bus_node_base::HASH_SEED, data, dataEnd);
}
} // namespace

namespace bus_node_base
{
uint64_t computeFirmwareHash()
{
    return fnv(reinterpret_cast<uint8_t *>(FlashStartAddress),
               reinterpret_cast<uint8_t *>(FlashStartAddress) + getBinarySize());
}

uint64_t fnvWithSeed(uint64_t hash, const uint8_t *data, const uint8_t *const dataEnd)
{
    constexpr uint64_t MagicPrime = 0x00000100000001b3;

    for (; data < dataEnd; data++)
    {
        hash = (hash ^ *data) * MagicPrime;
    }

    return hash;
}
} // namespace bus_node_base