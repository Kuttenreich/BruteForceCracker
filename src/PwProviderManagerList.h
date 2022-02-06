#ifndef PASSWORDPROVIDERMANAGER_LIST_H
#define PASSWORDPROVIDERMANAGER_LIST_H

#include "PwProviderManager.h"
#include "PwProviderList.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
namespace bfcracker
{
    class PwProviderManagerList:public PwProviderManager
    {
        public:
            PwProviderManagerList(const std::string& pwListPath, const int numPwProvider=1):_pwListPath(pwListPath),_numPwProvider(std::max<int>(1,numPwProvider))
            {
                if(!loadFile())
                {
                    if(_numPwProvider>_pwList.size())
                    {
                        _numPwProvider=_pwList.size();
                        std::cout<<"Reduced PwProvider/Threads to number of passwords = "<<_numPwProvider<<std::endl;
                    }
                    if(!createPwProvider())
                        std::cout<<"Successfully created PwProvider"<<std::endl;
                }
            }

            const PwProvider* getPwProvider(int idx) override
            {
                return _providerList[idx];
            }

            std::vector<PwProvider*>& getPwProviderList() override
            {
                return _providerList;
            }

        private:
            std::string _pwListPath;
            std::vector<std::string> _pwList;
            int _numPwProvider;
            std::vector<PwProvider*> _providerList;

            int loadFile()
            {
                std::fstream pwFile(_pwListPath,std::ios::in);
                if(pwFile.is_open())
                {
                    std::string nextLine;
                    while(getline(pwFile,nextLine))
                    {
                        nextLine.erase (std::remove (nextLine.begin(), nextLine.end(), ' '), nextLine.end());
                        _pwList.push_back(nextLine);
                    }
                    std::cout<<"PwProviderManagerList: Read "<<_pwList.size()<<" elements in list"<<std::endl;
                    return 0;
                }
                else
                {
                    std::cout<<"PwProviderManagerList: Could not open \""<<_pwListPath<<"\""<<std::endl;
                }
                return 1;
            }

            int createPwProvider()
            {
                // split the number of the passwords in the password list equaly over the PwProviders
                // Attention: end iterator is per definition pointing to the first element just not contained!
                int numPwPerProvider = _pwList.size()/_numPwProvider;
                for(int i=0;i<_numPwProvider;++i)
                {
                    auto start=_pwList.begin()+i*numPwPerProvider;
                    size_t rangeEnd = static_cast<size_t>((i+1)*numPwPerProvider); //fix
                    if ((i+1) == _numPwProvider)
                    {
                        rangeEnd = _pwList.size();   // necessary because we floored numPwPerProvider, so there may be some remainig
                    }
                    auto end=_pwList.begin()+rangeEnd;
                    PwProvider* pwProv = new PwProviderList(std::move(start),std::move(end));
                    _providerList.emplace_back(std::move(pwProv));
                }

                return 0;
            }

    };
}
#endif
