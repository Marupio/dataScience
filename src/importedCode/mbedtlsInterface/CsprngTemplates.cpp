template<class T>
T ds::Csprng::operator()(T maxValue) const {
    size_t output_len = sizeof(T);
    unsigned char* str = new unsigned char[output_len];
    int ret = mbedtls_ctr_drbg_random(
            (void *) &ctr_drbg_,
            str,
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
    case 0:
        break;
    }

    T retValue = 0;
    for (size_t i = 0; i < output_len; ++i) {
        retValue |= str[i] << (output_len - i - 1)*CHAR_BIT;
    }
    if (retValue < 0) {
        retValue = (-retValue)%maxValue;
    } else {
        retValue = retValue%maxValue;
    }
    return retValue;
}
