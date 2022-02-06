FROM gcc:11.2.0
#this suppresses any questions of the debian configuration system (--> chose default answer in apt)
RUN apt-get update -y
RUN apt-get -y install cmake libzip-dev 

# Runtime linker complains "/usr/lib/x86_64-linux-gnu/libstdc++.so.6: version GLIBCXX_3.4.29 not found" 
# Fixed by setting path of latest libstdc++ (libstdc++.so.6.0.29) on LD_PATH 
ENV LD_LIBRARY_PATH=/usr/local/lib64:LD_LIBRARY_PATH

WORKDIR /home/host/dev