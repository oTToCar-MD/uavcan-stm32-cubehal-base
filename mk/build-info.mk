$(shell git diff --quiet)

ifeq ($(.SHELLSTATUS),0)
REPO_IS_DIRTY := false
else
REPO_IS_DIRTY := true
endif

DEFS += BUILD_INFO_COMMIT_LONG=\""$(shell git rev-parse HEAD)"\"
DEFS += BUILD_INFO_COMMIT_SHORT=0x$(shell git rev-parse HEAD | cut -c 1-8)
DEFS += BUILD_INFO_IS_DIRTY=$(REPO_IS_DIRTY)
DEFS += BUILD_INFO_BRANCH=\""$(shell git rev-parse --abbrev-ref HEAD)"\"
CPPFLAGS += -DBUILD_INFO_TIME=\""$(shell date -u +"%Y/%m/%d %H:%M:%S") UTC"\"