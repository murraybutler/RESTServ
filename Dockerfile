FROM ubuntu:zesty

MAINTAINER Murray Butler

WORKDIR /root/

# add build dependencies & get the code.
RUN \
apt-get update && \
apt-get -y install apt-utils rapidjson-dev git libstdc++6 gcc g++ make cmake libpthread-stubs0-dev autoconf libtool

RUN git clone https://github.com/Tencent/rapidjson.git
WORKDIR /root/rapidjson
RUN git submodule update --init
WORKDIR /root/rapidjson/build
RUN cmake ..
RUN make
RUN make install

#install and build pistache
WORKDIR /root/
RUN git clone https://github.com/rob-h-w/pistache.git
WORKDIR /root/pistache
RUN git checkout Avoid-major-minor-macros
RUN git submodule update --init
# Build Pistache.io.
WORKDIR build
RUN cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
RUN make
RUN make install

#install and build MongoDB C and C++ drivers
#WORKDIR /root/
#RUN git clone https://github.com/mongodb/mongo-c-driver.git
#WORKDIR mongo-c-driver
#RUN git checkout 1.8.0
#RUN ./autogen.sh --with-libbson=bundled
#RUN make
#RUN make install
#WORKDIR /root/
#RUN git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
#WORKDIR /root/mongo-cxx-driver/build
#RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
#RUN make
#RUN make install

# clone and install the project
WORKDIR /root/
RUN git clone https://github.com/murraybutler/RESTServ.git
WORKDIR /root/RESTServ
RUN make all
ENV LD_LIBRARY_PATH /root/RESTServ/lib/:/usr/local/lib

