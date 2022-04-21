#pragma once

/** @file
 *  Base header including resources and defining macros used by other headers.
 */

// STL
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <future>
#include <mutex>
#include <atomic>

// CLib
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>

/** Declares the SSS namespace.
 *  Further code will be nested in the SSS namespace.\n
 *  Should be used in pair with with #__SSS_END.
 */
#define __SSS_BEGIN namespace SSS {
/** Closes the SSS namespace declaration.
 *  Further code will no longer be nested in the SSS namespace.\n
 *  Should be used in pair with with #__SSS_BEGIN.
 */
#define __SSS_END }

/** Declares an \c _internal namespace.
 *  Further code will be nested in an \c _internal namespace.\n
 *  Should be used in pair with with #__INTERNAL_END.
 */
#define __INTERNAL_BEGIN namespace _internal {
/** Closes an \c _internal namespace declaration.
 *  Further code will no longer be nested in an \c _internal namespace.\n
 *  Should be used in pair with with #__INTERNAL_BEGIN.
 */
#define __INTERNAL_END }

/** The following function does not return.
 *  @sa SSS::throw_exc()
 */
#define __NO_RETURN __declspec(noreturn)
