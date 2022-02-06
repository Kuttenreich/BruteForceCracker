#ifndef SYSTEMCALLVALIDATION_H_
#define SYSTEMCALLVALIDATION_H_
#include <string>
#include <memory>

namespace scv
{
    class SystemCallValidator_unzip
    {
        public:
            bool checkPw(const std::string& pw)
            {
                const std::string cmd = _prefix + pw + _postfix;
                int ret = system(cmd.c_str());
                if(ret==0)
                {
                    std::cout<<"(Content of decrypted file)"<<std::endl;
                    return true;
                }
                    
                return false;
            }
            
            SystemCallValidator_unzip()=default;
            SystemCallValidator_unzip(const std::string& _zipArchivePath, const std::string& _nameEncryptedFile): _prefix("unzip -qpP "),_postfix(" "+_zipArchivePath+" "+_nameEncryptedFile){}

            SystemCallValidator_unzip(SystemCallValidator_unzip&)=default;

            SystemCallValidator_unzip& operator=(const SystemCallValidator_unzip& other)
            {
                _prefix = other._prefix;
                _postfix = other._postfix;
                return *this;
            }

            SystemCallValidator_unzip& operator=(SystemCallValidator_unzip&& other)
            {
                _prefix = std::move(other._prefix);
                _postfix = std::move(other._postfix);
                return *this;
            }

            ~SystemCallValidator_unzip()=default;        
       
            std::string _prefix;
            std::string _postfix;

    };

}
#endif