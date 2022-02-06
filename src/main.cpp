#include <iostream>
#include "EncryptedFileZip.h"
#include "PwProviderManagerList.h"
#include "BruteForceCracker.h"
#include <chrono>
#include <stdlib.h>
using namespace bfcracker;

int main(int argc, char* argv[])
{

    std::cout<<std::endl<<"=========== Init ==========="<<std::endl;
    std::string zipFile="../testData/testZip.zip";
    std::string pwCanidatesFile="../testData/testPWList.txt";
    int numThreads=1;
    if(argc>1)
        zipFile = std::string(argv[1]);
    if(argc>2)
        pwCanidatesFile = std::string(argv[2]);
    if(argc>3)
        numThreads=atoi(argv[3]);

    std::cout<<"File to crack: "<<zipFile<<std::endl;
    std::cout<<"Threads to use: "<<numThreads<<std::endl;
    std::cout<<"PW list to use: "<<pwCanidatesFile<<std::endl;


    auto cracker = BruteForceCracker_Factory::createZipListCracker(pwCanidatesFile,zipFile,numThreads); 
    std::cout<<std::endl<<"=========== Cracking ==========="<<std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    const std::vector<std::string>& passwords = cracker.crack();    //here the actual cracking takes place
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout<<std::endl<<"=========== Found passwords ==========="<<std::endl;
    for(const std::string& s : passwords)
    {
        std::cout<<s<<std::endl;
    }

    std::cout <<std::endl<<std::endl<< "Time passed = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    return 0;
}