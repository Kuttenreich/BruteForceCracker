#ifndef BRUTEFORCECRACKER_H_
#define BRUTEFORCECRACKER_H_
#include "PwProviderManager.h"
#include "EncryptedFile.h"
#include "PwProviderList.h"
#include "PwProviderManagerList.h"
#include "EncryptedFileZip.h"
#include <string>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>
namespace bfcracker
{
    class BruteForceCracker_Factory{

        template<class T_manager, class T_file>
        class BruteForceCracker
        {
            public:

                const std::vector<std::string>& crack()
                {
                    // start threads that use reference to PwProvider and copy of Encrypted File to test PW 
                    const std::vector<PwProvider*> providerList = _pwProviderManager->getPwProviderList();
                    int i=0;

                    for(PwProvider* provPtr: providerList)
                    {
                        std::vector<std::unique_ptr<T_file>>* v = _encryptedFileHandles.get();
                        T_file* filePtr = v->at(i).get();
                        _threadList.emplace_back(std::thread(&BruteForceCracker::testPasswords,this,provPtr,filePtr));
                        ++i;
                    }
                    for(int t=0;t<providerList.size();++t)
                    {
                        _threadList[t].join();
                    }
                    std::cout<<std::endl<<"Finished. All threads joined"<<std::endl;
                    return _passwords;
                }
                
            private:

                std::unique_ptr<T_manager> _pwProviderManager;
                std::unique_ptr<std::vector<std::unique_ptr<T_file>>> _encryptedFileHandles;
                friend class BruteForceCracker_Factory;
                std::vector<std::thread> _threadList;
                std::vector<std::string> _passwords;
                std::mutex _m;

                BruteForceCracker(T_manager* manager,std::vector<std::unique_ptr<T_file>>* fileHandles)
                {
                    _pwProviderManager = std::unique_ptr<T_manager>( manager);
                    _encryptedFileHandles = std::unique_ptr<std::vector<std::unique_ptr<T_file>>>(fileHandles);
                }

                void addPassword(std::string&& pw)
                {
                    std::unique_lock<std::mutex> myLock(_m);
                    _passwords.emplace_back(pw);
                }

                void testPasswords(PwProvider* pwProvider,T_file* file)
                {
                    std::string pwCandidate;
                    while(pwProvider->taskCompletionPrc()<1)
                    {
                        pwCandidate = pwProvider->getPW();
                        if(file->checkPassword(pwCandidate))
                        {
                            addPassword(std::move(pwCandidate));
                        }
                    }
                }

                const std::vector<std::string>& getPasswords()
                {
                    return _passwords;
                }
        };

        public:
            // I use a factory pattern here, as I don't want the user to worry about classes to use. Meaning it's not trivial to create an object of type BruteForceCracker
            static BruteForceCracker<PwProviderManagerList,EncryptedFileZip> createZipListCracker(const std::string& pwListPath, const std::string& zipFile, int numThreads)
            {
                // create cracker with a password list for pw source for breaking a zip file

                PwProviderManagerList* pwProviderManagerListPtr = new PwProviderManagerList(pwListPath,numThreads);
                std::vector<std::unique_ptr<EncryptedFileZip>>* encryptedFileHandlesPtr = new std::vector<std::unique_ptr<EncryptedFileZip>>();
                for(int i=0;i<numThreads;++i)
                {
                    encryptedFileHandlesPtr->emplace_back(std::make_unique<EncryptedFileZip>(zipFile));
                }
                return BruteForceCracker<PwProviderManagerList,EncryptedFileZip>(pwProviderManagerListPtr,encryptedFileHandlesPtr);
            }
    };
}
#endif