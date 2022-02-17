FROM ubuntu:20.04

ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update -y
RUN apt-get install -y --no-install-recommends git \
                                           curl \
                                           wget \
                                           vim \
                                           tree \
                                           zip \
                                           unzip \
                                           libtool \
                                           m4 \
                                           automake \
                                           cmake \
                                           make \
                                           gnutls-bin \
                                           clang \
                                           gdb \
                                           cscope \
                                           htop \
                                           iotop \
                                           iftop \
                                           zsh \
                                           nano \
                                           xterm \
                                           autoconf \
                                           ssh \
                                           build-essential \
                                           pkg-config \
                                           libgtest-dev \
                                           libmicrohttpd-dev \
                                           ca-certificates \
                                           g++ \
                                           python-dev \
                                           autotools-dev \
                                           libicu-dev \
                                           libbz2-dev \
                                           libboost-all-dev
                                           
RUN apt-get install -y --no-install-recommends libboost-dev 

#RUN wget -O boost_1_78_0.tar.gz https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz && \
#    tar xzvf boost_1_78_0.tar.gz && \
#    cd boost_1_78_0/ && \
#    ./bootstrap.sh --prefix=/usr/ && \
#    ./b2 --with=all -j 8 install && \
#    ldconfig -v

# install libhttpserver
RUN cd /tmp && \
    git clone https://github.com/etr/libhttpserver.git && \
    cd libhttpserver && \
    ./bootstrap && \
    mkdir build && \
    cd build && \
    ../configure && \
    make && \
    make install && \
    ldconfig -v

# set env variables otherwise libhttpserver doesn't work
ENV LD_LIBRARY_PATH=/usr/local/lib

# set user; everything below is under user webserver
ENV HOME=/home/webserver
RUN useradd -ms /bin/bash webserver
USER webserver
WORKDIR /home/webserver

# switch to root to install CA certificate and switch back to webserver
USER root
# copy over CA-certificate to local container;
COPY ./certs/root_ca/certs/smoothstack_root.crt /usr/local/share/ca-certificates/smoothstack_client.crt
RUN update-ca-certificates
USER webserver

# you can comment out everything below if you don't want oh-my-zsh/make the container more lightweight

# installs oh-my-zsh
RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.1.2/zsh-in-docker.sh)" -- \
    -p git \
    -p https://github.com/zsh-users/zsh-autosuggestions \
    -p https://github.com/zsh-users/zsh-completions \
    -p https://github.com/marlonrichert/zsh-autocomplete \
    -p https://github.com/zsh-users/zsh-syntax-highlighting

# set environmental variables for zsh
RUN echo "TERM=xterm-256color" >> $HOME/.zshrc
ENV COLORTERM=truecolor
RUN echo "autoload -U colors && colors" >> $HOME/.zshrc
RUN echo "PS1='%{$fg[red]%}%n%{$reset_color%}@%{$fg[blue]%}%m %{$fg[yellow]%}%~ %{$reset_color%}%% '" >> $HOME/.zshrc
RUN echo "source ~/.oh-my-zsh/custom/plugins/zsh-autocomplete/zsh-autocomplete.plugin.zsh" >> $HOME/.zshrc
RUN echo "source ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh" >> $HOME/.zshrc