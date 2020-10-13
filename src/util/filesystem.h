#pragma once

#include <filesystem>

// For some reason, the STL library on MacOS has filesystem under 
// std::__fs, not std. This is a work around that.
#if defined(_WIN32) || defined(_WIN64)

namespace fs = std::filesystem;

#elif defined(__APPLE__)

namespace fs = std::__fs::filesystem;

#endif
