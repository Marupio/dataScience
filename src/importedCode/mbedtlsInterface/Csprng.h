/*
    An interface for the cryptographically secure random generator included in mbedtls
*/

#ifndef Csprng_h
#define Csprng_h

#include <EntropyInterface.h>
#include <mbedtls/ctr_drbg.h>

#include <types.h>

namespace ds {


class Csprng
{
public:

    // Constructors

        //- Construct with entropy reference
        //  Randomise - when true, resets the standard randomisation seed
        //  to the clock time
        Csprng(EntropyInterface& entropy, std::string randomiser = std::string());


    //- Destructor
    ~Csprng() {}


    // Member functions

        //- Returns a set of random bytes to preallocated memory
        void getRandomBytes(unsigned char * output, size_t output_len) const;

        //- Writes out a set of random bytes
        void writeRandBytes(size_t output_len) const;


    // Member Operators

        template<class T>
        T operator()(T maxValue) const;


private:

    // Private member data

        //- Entropy generator for CSPRNG
        //  There should only be one instance in multi-threaded applications
        EntropyInterface& entropy_;

        //- CSPRNG CTR_DRBG
        mutable mbedtls_ctr_drbg_context ctr_drbg_;


    // Private member functions

};

} // end namespace ds

#include <CsprngTemplates.cpp>

#endif
