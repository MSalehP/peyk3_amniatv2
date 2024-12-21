#include "encryption.h"
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <QElapsedTimer>

using namespace CryptoPP;

qint64 measureAESTime(const std::string &message, bool isEncryption) {
    AutoSeededRandomPool prng;

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    prng.GenerateBlock(key, key.size());

    SecByteBlock iv(AES::BLOCKSIZE);
    prng.GenerateBlock(iv, iv.size());

    std::string result;
    QElapsedTimer timer;
    timer.start();

    try {
        if (isEncryption) {
            CBC_Mode<AES>::Encryption encryption;
            encryption.SetKeyWithIV(key, key.size(), iv);

            StringSource ss(message, true,
                            new StreamTransformationFilter(encryption,
                                                           new StringSink(result)));
        } else {
            CBC_Mode<AES>::Decryption decryption;
            decryption.SetKeyWithIV(key, key.size(), iv);

            StringSource ss(message, true,
                            new StreamTransformationFilter(decryption,
                                                           new StringSink(result)));
        }
    } catch (...) {
        return -1;
    }

    return timer.nsecsElapsed();
}

qint64 measureRSATime(const std::string &message, bool isEncryption) {
    AutoSeededRandomPool prng;

    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(prng, 3072);

    RSA::PublicKey publicKey(params);
    RSA::PrivateKey privateKey(params);

    std::string result;
    QElapsedTimer timer;
    timer.start();

    try {
        if (isEncryption) {
            RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
            StringSource ss(message, true,
                            new PK_EncryptorFilter(prng, encryptor,
                                                   new StringSink(result)));
        } else {
            RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
            StringSource ss(message, true,
                            new PK_DecryptorFilter(prng, decryptor,
                                                   new StringSink(result)));
        }
    } catch (...) {
        return -1;
    }

    return timer.nsecsElapsed();
}
