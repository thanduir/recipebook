#pragma once

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <podofo/podofo.h>

#ifdef _MSC_VER
#pragma warning( pop )
#elif __GNUC__
#pragma GCC diagnostic pop
#endif
