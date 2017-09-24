FROM ubuntu:zesty

MAINTAINER Murray Butler

WORKDIR /root/

# add build dependencies & get the code.
RUN \
apt-get update && \
apt-get -y install apt-utils rapidjson-dev git libstdc++6 gcc g++ make cmake libpthread-stubs0-dev

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

# Install
RUN make install

# clone and install the project
WORKDIR /root/
RUN git clone https://github.com/murraybutler/RESTServ.git
WORKDIR /root/RESTServ
RUN make all
ENV LD_LIBRARY_PATH /root/RESTServ/lib/

