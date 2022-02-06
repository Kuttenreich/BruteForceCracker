#ifndef PASSWORDPROVIDERMANAGER_H_
#define PASSWORDPROVIDERMANAGER_H_

#include "PwProvider.h"
#include <vector>
namespace bfcracker
{
    class PwProviderManager
    {
        public:
            virtual const PwProvider* getPwProvider(int idx)=0;
            virtual std::vector<PwProvider*>& getPwProviderList()=0; 
    };
}


#endif