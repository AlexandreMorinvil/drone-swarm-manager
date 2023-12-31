FROM ubuntu:18.04
LABEL argos-example.version="0.1"

# Install common dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    dpkg \ 
    git \
    pkg-config \
    python \
    python-dev \
    python-numpy \
    sudo \
    software-properties-common \
    && rm -rf /var/lib/apt/lists/*

# Install ARGoS dependencies
RUN apt-get update && apt-get install -y \
    wget \
    freeglut3-dev \
    qt5-default \
    libxi-dev \
    libxmu-dev \
    libfreeimage-dev \
    libfreeimageplus-dev \
    liblua5.2-dev \
    lua5.2 \
    liblua5.3-dev \
    lua5.3 \
    libboost-filesystem-dev \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Add dummy argument to force rebuild starting from that point
ARG UPDATE_ARGOS=unknown

# Install Argos from source
RUN cd /root/ &&\
    git clone https://github.com/MISTLab/argos3.git &&\
    cd argos3 &&\
    git checkout inf3995 &&\
    mkdir build_simulator &&\
    cd build_simulator &&\
    cmake ../src -DCMAKE_BUILD_TYPE=Debug \
     -DARGOS_BUILD_FOR=simulator \
     -DARGOS_THREADSAFE_LOG=ON \
     -DARGOS_DYNAMIC_LOADING=ON &&\
    make -j $(nproc)
RUN touch /root/argos3/build_simulator/argos3.1.gz &&\
    touch /root/argos3/build_simulator/README.html &&\
    cd /root/argos3/build_simulator &&\
    make install
RUN chmod +x /root/argos3/build_simulator/argos_post_install.sh &&\
    ./root/argos3/build_simulator/argos_post_install.sh &&\
    echo "\nsource /root/argos3/build_simulator/setup_env.sh\n" >> /.bashrc

#################################
#          YOUR CODE            #
#################################

# Add dummy argument to force rebuild starting from that point
ARG UPDATE_CODE=unknown

# Copy repository
RUN cd /root &&\
    mkdir /root/simulation &&\
    cd /root/simulation

WORKDIR /root/simulation
COPY . .

# Build code
RUN cd /root/simulation &&\
    mkdir build &&\
    cd build &&\
    cmake -DCMAKE_BUILD_TYPE=Debug .. &&\
    make -j $(nproc) &&\
    cp /root/simulation/test/controllers/sim-alfred/launch_all_tests.sh /root/simulation/build &&\
    chmod +x launch_all_tests.sh
