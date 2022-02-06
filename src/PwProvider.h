#ifndef PASSWORDPROVIDER_H
#define PASSWORDPROVIDER_H

#include <string>
namespace bfcracker
{
    class PwProvider
    {
        public:
            virtual const std::string& getPW()=0;
            virtual float taskCompletionPrc()=0;
    };
}
#endif