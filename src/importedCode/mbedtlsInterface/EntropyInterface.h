
#ifndef EntropyInterface_h
#define EntropyInterface_h

#include <mutex>

#include <mbedtls/entropy.h>

namespace ds {

class EntropyInterface
{
public:

    //- Constructor null
    EntropyInterface() {
        mbedtls_entropy_init(&entropy_);
    }

    //- Destructor
    ~EntropyInterface() {}


    // Public Member Functions

        //- Access entropyMutex
        std::mutex& entropyMutex() const {
            return entropyMutex_;
        }

        //- Access entropy itself
        mbedtls_entropy_context& entropy() {
            return entropy_;
        }

    // Private Member Data

        //- Entropy collector
        mbedtls_entropy_context entropy_;

        //- Entropy collector mutex
        mutable std::mutex entropyMutex_;
};

} // end namespace

#endif