# BruteForceCracker #

Find out passwords for encrypted data by a brute force approach.  

Currently this project is limited on the **decryption of zip files**, by trying password candidates from a txt file.  
Extending the code on further file types, e.g. PDF, was one of the underlying architecture design principles.

>"...the 25 most common passwords made up more than 10% of the surveyed passwords, with the most common password of 2016, "123456", making up 4%..."
(https://time.com/4639791/worst-passwords-2016/)


## Setup ##
- start docker by using one of the provided start scripts, depending on your OS:
  ```
  ./startDocker_linux.sh
  ./startDocker_windows.bat
  ```
  This mounts your currently checked out folder into the docker

- build the project in the docker:
   ```
  mkdir build && cd build
  cmake .. && make && make install
  cd ../bin 
  ```
  
  After installation the `BruteForceCracker` binary is located in `<folder>/bin`

## Usage ##
- get/create a list of passwords. The passwords must be in a txt file and separated by line break 
- you can specify some parameters for 'BruteForceCracker' binary:
  ```
  ./BruteForceCracker <path-to-zip-file> <path-to-passwordList> <number of threads>
  ```
  example with provided files:
  ```
  ./BruteForceCracker ../testData/testZip.zip ../testData/testPWList.txt 2
  ```
## Extending this project ##
- if you want to extend this project on further datatypes you just have to 
  - provide your own class of type *EncryptedFile*
  - extend the *BruteForceCracker* with a *create*-method using your instance of your type of *EncryptedFile*
- if you want to extend this project with further sources of password candidates (e.g. randomly created pw) you just have to
  - provide your own class of type *PwProvider*
  - provider your own class of type *PwProviderManager*
  - extend the *BruteForceCracker* with a *create*-method using your instance of type of *PwProviderManager*
- you can combine different types of *EncryptedFile* and *PwProviderManager* by extending the *BruteForceCracker* with a *create*-Function using the desired combination

## Good to know ##
- zip decoding is based on `libzip`. [API documentation](https://libzip.org/documentation/)
- [libzip](https://libzip.org/) (tested with V1.8.0) turned out to be unreliable in checking the validity of a given password  
  Therefore an additional system call to `unzip` checks whether the password really matches
- here you can find some [lists of popular passwords](https://github.com/danielmiessler/SecLists/tree/master/Passwords/Common-Credentials)   
