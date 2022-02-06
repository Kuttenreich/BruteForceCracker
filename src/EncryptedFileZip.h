#ifndef ENCRYPTEDFILEZIP_H_
#define ENCRYPTEDFILEZIP_H_
#include "EncryptedFile.h"
#include <zip.h>
#include <iostream>
#include <utility>
#include <map>
#include "SystemCallValidation.h"

namespace bfcracker
{
    class EncryptedFileZip: public EncryptedFile
    {
        public:
            EncryptedFileZip(const std::string& file):_zipArchive(nullptr),_zipFile(nullptr),_zipSource(nullptr),_encryptedEntityIdx(-1)
            {
                _file = file;
                loadFile();
                findEncryptedFile();
            };

            int loadFile() override
            {
                zip_error_t err;
                zip_error_init(&err);
                _zipSource = zip_source_file_create(_file.c_str(),0,-1,&err);
                _zipArchive = zip_open_from_source(_zipSource,0,&err);
                if (nullptr == _zipArchive) 
                {
                    std::cout<<"Error loading file \""<<_file<<"\": "<<  zip_error_strerror(&err)<<std::endl;

                    return 1;
                }
                return 0;
            }

            // returns true if correct pw found
            bool checkPassword(const std::string& password)
            {
                if(_encryptedEntityIdx!=-1)
                {   
                    zip_file_t* f= zip_fopen_index_encrypted(_zipArchive,_encryptedEntityIdx,0,password.c_str());

                    if(f)
                    {
                        // I tried various things to further check the validity of the password, but couldn't find a proper way with libzip --> hack to use console command
                        return _unzipValidator.checkPw(password);
                    }
                }
                return false;
            }

            void findEncryptedFile() 
            {
                if(_zipArchive)
                {
                    zip_int64_t numEntries = zip_get_num_entries(_zipArchive,0);
                    zip_stat_t entityStats;
                    zip_stat_init(&entityStats);
                    for(zip_uint64_t i=0;i<numEntries;++i)
                    {
                        zip_stat_index(_zipArchive,i,0,&entityStats);
                        
                        if(entityStats.encryption_method)
                        {
                            //std::cout<<"Info: "<<std::endl;
                            //std::cout<<"===> file \""<< entityStats.name<< "\" is encrypted. ";
                            if(!zip_encryption_method_supported(entityStats.encryption_method,1))
                            {
                                std::cout<<" Unknownd encryption. We can not use this file for password checks."<<std::endl;
                            }
                            else
                            {
                                //std::cout<<"We are going to use this file for password checks."<<std::endl;
                                _encryptedEntityIdx = i;
                                _nameEncryptedEntity = entityStats.name;
                                _unzipValidator = scv::SystemCallValidator_unzip(_file,_nameEncryptedEntity);
                                return;
                            }                            
                        }
                    }
                }
                std::cout<<"No encrypted file found"<<std::endl;
            }

            ~EncryptedFileZip()
            {
                if(_zipSource)
                {
                    zip_source_close(_zipSource);
                }
                if(_zipArchive)
                {
                    zip_discard(_zipArchive);
                }
            }
        
            zip_stat_t getZipStatEncrypted()
            {
                zip_stat_t st;
                zip_stat_init(&st);
                if(_encryptedEntityIdx!=-1)
                    zip_stat_index(_zipArchive, _encryptedEntityIdx, 0, &st);
                return st;
            }
        private:
            struct zip_source* _zipSource; 
            struct zip *_zipArchive;
            struct zip_file *_zipFile;
            zip_uint64_t _encryptedEntityIdx;
            std::string _nameEncryptedEntity;
            scv::SystemCallValidator_unzip _unzipValidator;
    };
}
#endif