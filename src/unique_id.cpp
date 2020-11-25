#include "uavcan-stm32-cubehal-base/unique_id.hpp"
#include <algorithm>
#include <array>

namespace bus_node_base
{
void fillUniqueIdField(uavcan::protocol::HardwareVersion &hwVersion, uint32_t uidWord0,
                       uint32_t uidWord1, uint32_t uidWord2)
{
    std::fill(std::begin(hwVersion.unique_id), std::end(hwVersion.unique_id), 0x0);
    hwVersion.unique_id[0] = 0x6;

    auto toByteArray = [](uint32_t value) {
        std::array<uint8_t, 4> result;
        result.at(0) = static_cast<uint8_t>((value & 0xff000000) >> 24);
        result.at(1) = static_cast<uint8_t>((value & 0xff0000) >> 16);
        result.at(2) = static_cast<uint8_t>((value & 0xff000) >> 8);
        result.at(3) = static_cast<uint8_t>(value & 0xff);

        return result;
    };

    auto uidWord0Array = toByteArray(uidWord0);
    auto uidWord1Array = toByteArray(uidWord1);
    auto uidWord2Array = toByteArray(uidWord2);

    std::move(begin(uidWord0Array), end(uidWord0Array), std::begin(hwVersion.unique_id) + 4);
    std::move(begin(uidWord1Array), end(uidWord1Array), std::begin(hwVersion.unique_id) + 8);
    std::move(begin(uidWord2Array), end(uidWord2Array), std::begin(hwVersion.unique_id) + 12);
}
} // namespace bus_node_base