#ifndef error_h
#define error_h

// Error handling macros
#ifdef __GNUC__
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#else
    #define FUNCTION_NAME __func__
#endif

#define FatalError std::cerr << "\n\n*** FatalError ***\n" \
    << "    Function: " << FUNCTION_NAME << "\n" \
    << "    Location: " << __FILE__ << ":" << __LINE__ << "\n\n"

#endif
