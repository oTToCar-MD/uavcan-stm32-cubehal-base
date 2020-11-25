#pragma once

#include <cstdint>

namespace build_info
{
extern const uint32_t CommitHashShort;
extern const bool IsDirty;
extern const bool IsDebugBuild;
extern const char *const BuildTime;
extern const char *const CommitHashLong;
extern const char *const BranchName;
} // namespace build_info
