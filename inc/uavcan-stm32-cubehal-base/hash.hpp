#pragma once

#include <cstdint>

namespace bus_node_base
{
    static constexpr uint64_t HASH_SEED = 0xcbf29ce484222325;
    uint64_t fnvWithSeed(uint64_t hash, const uint8_t *data, const uint8_t *const dataEnd);
    uint64_t computeFirmwareHash();
} // namespace bus_node_base
