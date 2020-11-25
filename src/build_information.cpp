#include "uavcan-stm32-cubehal-base/build_information.hpp"

#ifdef DEBUG
#define BUILD_INFO_IS_DEBUG true
#else
#define BUILD_INFO_IS_DEBUG false
#endif

namespace build_info
{
const uint32_t CommitHashShort = BUILD_INFO_COMMIT_SHORT;
const bool IsDirty = BUILD_INFO_IS_DIRTY;
const bool IsDebugBuild = BUILD_INFO_IS_DEBUG;
const char *const BuildTime = BUILD_INFO_TIME;
const char *const CommitHashLong = BUILD_INFO_COMMIT_LONG;
const char *const BranchName = BUILD_INFO_BRANCH;
} // namespace build_info