#ifndef ENCRYPTED_FILE_H
#define ENCRYPTED_FILE_H
#include <string>

namespace bfcracker
{
    class EncryptedFile
    {
        public:
            virtual int loadFile()=0;
            virtual bool checkPassword(const std::string& password)=0;
        protected:
            std::string _file;
    };
}

#endif