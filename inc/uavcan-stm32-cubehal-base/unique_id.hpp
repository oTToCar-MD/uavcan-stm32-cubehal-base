#pragma once

#include "uavcan/uavcan.hpp"
#include <cstdint>

namespace bus_node_base
{
void fillUniqueIdField(uavcan::protocol::HardwareVersion &hwVersion, uint32_t uidWord0,
                       uint32_t uidWord1, uint32_t uidWord2);
}
