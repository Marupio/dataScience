#include <Csprng.h>


// ****** Constructors ****** //

ds::Csprng::Csprng(EntropyInterface& entropy, std::string randomiser):
    entropy_(entropy)
{
    // CSPRNG initialisation
    std::unique_lock<std::mutex> entropyLock(entropy_.entropyMutex(), std::defer_lock);
    entropyLock.lock();

    mbedtls_entropy_context& entropyData(entropy_.entropy());
    mbedtls_ctr_drbg_init(&ctr_drbg_);

    int ret = mbedtls_ctr_drbg_seed(
            &ctr_drbg_,
            mbedtls_entropy_func,
            &entropyData,
            (const unsigned char *) randomiser.c_str(),
            randomiser.length()
        );
    entropyLock.unlock();
    if( ret != 0 ) {
        FatalError << "CSPRNG failed to initialise, ret = " << ret << std::endl;
        abort();
    }
}


// ****** Public member functions ****** //

void ds::Csprng::getRandomBytes(unsigned char * output, size_t output_len) const {
    int ret = mbedtls_ctr_drbg_random(
            (void *) &ctr_drbg_,
            output,
            output_len
        );
    switch(ret) {
    case MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED:
        FatalError << "MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED" << std::endl;
        abort();
    case MBEDTLS_ERR_CTR_DRBG_REQUEST_TOO_BIG:
        FatalError << "MBEDTLS_ERR_CTR_DRBG_REQUEST_TOO_BIG" << std::endl;
        abort();
    default:
        FatalError << "Unknown return: " << ret << std::endl;
        abort();
    case 0: {
        break;
    } // end case 0
    } // end switch
}


void ds::Csprng::writeRandBytes(size_t output_len) const {
    unsigned char* str = new unsigned char[output_len];
    getRandomBytes(str, output_len);
    std::cout << "str = [";
    if (output_len > 0) {
        std::cout << short(str[0]);
    }
    for (size_t i = 1; i < output_len; ++i) {
        std::cout << " " << short(str[i]);
    }
    std::cout << "]" << std::endl;
}


// ****** END ****** //