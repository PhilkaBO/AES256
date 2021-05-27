#ifndef AES_ENGINE_H
#define AES_ENGINE_H

#include <vector>


typedef std::vector<unsigned char> ByteArray;

#define bs 16

class AES_Engine {

    public:
        AES_Engine(const ByteArray& key);
        ~AES_Engine();

        static ByteArray::size_type encrypt(const ByteArray& key, const ByteArray& plain, ByteArray& encrypted);
        static ByteArray::size_type decrypt(const ByteArray& key, const ByteArray& encrypted,ByteArray& plain);

        //Стадии кодирования
        ByteArray::size_type encryptStart(const ByteArray::size_type plain_length, ByteArray& encrypted);

        ByteArray::size_type encryptContinue(const ByteArray& plain, ByteArray& encrypted);

        ByteArray::size_type encryptEnd(ByteArray& encrypted);

        ByteArray::size_type decryptStart(const ByteArray::size_type encrypted_length);

        ByteArray::size_type decryptContinue(const ByteArray& encrypted, ByteArray& plain);

        ByteArray::size_type decryptEnd(ByteArray& plain);

    private:
        ByteArray mKey;
        ByteArray mSalt;
        ByteArray mRkey;

        unsigned char mBuffer[3 * bs];
        unsigned char mBufferPos;
        ByteArray::size_type mRemainingLength;

        bool mDecryptInitialized;

        void checkAndEncryptBuffer(ByteArray& encrypted);
        void checkAndDecryptBuffer(ByteArray& plain);

        void encrypt(unsigned char *buffer);
        void decrypt(unsigned char *buffer);

        void expandEncKey(unsigned char *rc);
        void expandDecKey(unsigned char *rc);

        void subBytes(unsigned char *buffer);
        void subBytesInv(unsigned char *buffer);

        void copyKey();

        void addRoundKey(unsigned char *buffer, const unsigned char round);

        void shiftRows(unsigned char *buffer);
        void shiftRowsInv(unsigned char *buffer);

        void mixColumns(unsigned char *buffer);
        void mixColumnsInv(unsigned char *buffer);
};

#endif // AES_ENGINE_H
