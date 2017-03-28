#ifndef error_h
#define error_h

#include<iostream>

// Error handling macros
#ifdef __GNUC__
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#else
    #define FUNCTION_NAME __func__
#endif

#define FatalError std::cerr << "\n\n*** FatalError ***\n" \
    << "    Function: " << FUNCTION_NAME << "\n" \
    << "    Location: " << __FILE__ << ":" << __LINE__ << "\n\n"

#define Warning std::cerr << "\n\n*** Warning ***\n" \
    << "    Function: " << FUNCTION_NAME << "\n" \
    << "    Location: " << __FILE__ << ":" << __LINE__ << "\n\n"

#endif
