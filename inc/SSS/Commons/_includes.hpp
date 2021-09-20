#pragma once

    // --- Defines ---

#define __SSS_BEGIN namespace SSS {
#define __SSS_END }

#define __INTERNAL_BEGIN namespace _internal {
#define __INTERNAL_END }

#define __NO_RETURN __declspec(noreturn)

    // --- Includes ---

// STL
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

// CLib
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>