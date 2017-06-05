#ifndef error_h
#define error_h

#include <iostream>
#include <sstream>
#include <string>

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

namespace ds {

class dsException: public std::exception {

public:
    dsException(): what_("Unknown exception") {}
    dsException(const char* what): what_(what) {}
    dsException(std::string what): what_(what) {}
    virtual const char* what() const throw() {
        return what_.c_str();
    }

private:
    std::string what_;
};

} // end namespace

#endif

// ****** END ****** //