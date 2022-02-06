#ifndef PWPROVIDER_H_
#define PWPROVIDER_H_
#include "PwProvider.h"
#include <vector>
namespace bfcracker
{
    class PwProviderList:public PwProvider
    {
        public:
            typedef std::vector<std::string>::iterator pwIterator;
            PwProviderList(pwIterator&& itStart, pwIterator&& itEnd):_endIt(std::move(itEnd)),_startIt(std::move(itStart))
            {
                _currentIt = _startIt;
                _numElems = itEnd - itStart;
            }

            const std::string& getPW() override
            {
                pwIterator tmp = _currentIt;
                if(_currentIt<_endIt)
                    ++_currentIt;
                return *tmp;
            }

            float taskCompletionPrc() override
            {
                return 1.f- (float)(_endIt-_currentIt)/_numElems;
            }

        private:
            pwIterator _startIt;
            pwIterator _currentIt;
            pwIterator _endIt;
            int _numElems;

    };
}
#endif