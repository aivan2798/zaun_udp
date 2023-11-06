#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>

std::string base64_encode(const unsigned char* input, size_t length) {
    BIO *bio, *b64;
    char* buffer = nullptr;
    size_t length64 = 0;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, input, length);
    BIO_flush(bio);

    // Get the length of the encoded data
    length64 = BIO_get_mem_data(bio, &buffer);

    std::string result(buffer, length64);

    BIO_free_all(bio);
    return result;
}

std::vector<unsigned char> base64_decode(const std::string &encoded) {
    BIO *bio, *b64;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new_mem_buf(encoded.c_str(), static_cast<int>(encoded.length()));
    bio = BIO_push(b64, bio);

    // Calculate the size of the decoded data
    size_t max_decoded_length = encoded.length() * 3 / 4;
    std::vector<unsigned char> decoded(max_decoded_length);

    int decoded_length = BIO_read(bio, decoded.data(), static_cast<int>(decoded.size()));

    BIO_free_all(bio);
    decoded.resize(decoded_length);  // Resize to the actual decoded size

    return decoded;
}