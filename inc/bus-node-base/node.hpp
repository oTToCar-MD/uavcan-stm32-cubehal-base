#pragma once

#include <uavcan/uavcan.hpp>
#include <uavcan_stm32/uavcan_stm32.hpp>

namespace bus_node_base
{
static constexpr unsigned NodeMemoryPoolSize = UAVCAN_MEMPOOL_SIZE;
using Node = uavcan::Node<NodeMemoryPoolSize>;

class NodeLocker : public uavcan_stm32::MutexLocker
{
public:
    NodeLocker();
};

/**
 * Returns a reference to a UAVCAN node object. The node will be constructed the first time
 * this method is called.
 */
Node &getNode();
} // namespace bus_node_base
