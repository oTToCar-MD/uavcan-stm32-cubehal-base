#include "node.hpp"
#include <cstdint>

namespace
{
constexpr int RxQueueSize = UAVCAN_RX_QUEUE_SIZE;
constexpr std::uint32_t BitRate = 1'000'000;

uavcan_stm32::Mutex nodeMutex;

uavcan::ISystemClock &getSystemClock()
{
    return uavcan_stm32::SystemClock::instance();
}

uavcan::ICanDriver &getCanDriver()
{
    static uavcan_stm32::CanInitHelper<RxQueueSize> can;
    static bool initialized = false;

    if (!initialized)
    {
        initialized = true;
        int res = can.init(BitRate);

        if (res < 0)
        {
            // Handle the error
        }
    }

    return can.driver;
}
} // namespace

namespace base
{
NodeLocker::NodeLocker() : uavcan_stm32::MutexLocker{nodeMutex}
{
}

/**
 * Node object will be constructed at the time of the first access.
 * Note that most library objects are noncopyable (e.g. publishers, subscribers, servers,
 * callers, timers, ...). Attempt to copy a noncopyable object causes compilation failure.
 */
Node &getNode()
{
    static Node node(getCanDriver(), getSystemClock());
    return node;
}
} // namespace base
