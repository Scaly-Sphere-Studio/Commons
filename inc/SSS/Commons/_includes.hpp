#pragma once

    // --- Defines ---

#define SSS_BEGIN__ namespace SSS {
#define SSS_END__ }

#define INTERNAL_BEGIN__ namespace _internal {
#define INTERNAL_END__ }

#define NO_RETURN__ __declspec(noreturn)

    // --- Includes ---

// STL
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <chrono>

// CLib
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>