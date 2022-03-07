FROM ubuntu:20.04

ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update -y
RUN apt-get install -y --no-install-recommends git
RUN apt-get install -y --no-install-recommends curl 
RUN apt-get install -y --no-install-recommends wget 
RUN apt-get install -y --no-install-recommends vim 
RUN apt-get install -y --no-install-recommends tree 
RUN apt-get install -y --no-install-recommends zip 
RUN apt-get install -y --no-install-recommends unzip 
RUN apt-get install -y --no-install-recommends libtool 
RUN apt-get install -y --no-install-recommends m4 
RUN apt-get install -y --no-install-recommends automake 
RUN apt-get install -y --no-install-recommends cmake 
RUN apt-get install -y --no-install-recommends make 
RUN apt-get install -y --no-install-recommends gnutls-bin 
RUN apt-get install -y --no-install-recommends clang 
RUN apt-get install -y --no-install-recommends gdb 
RUN apt-get install -y --no-install-recommends cscope 
RUN apt-get install -y --no-install-recommends htop 
RUN apt-get install -y --no-install-recommends iotop 
RUN apt-get install -y --no-install-recommends iftop 
RUN apt-get install -y --no-install-recommends zsh 
RUN apt-get install -y --no-install-recommends nano 
RUN apt-get install -y --no-install-recommends xterm 
RUN apt-get install -y --no-install-recommends autoconf 
RUN apt-get install -y --no-install-recommends ssh 
RUN apt-get install -y --no-install-recommends build-essential 
RUN apt-get install -y --no-install-recommends pkg-config 
RUN apt-get install -y --no-install-recommends libgtest-dev 
RUN apt-get install -y --no-install-recommends libmicrohttpd-dev 
RUN apt-get install -y --no-install-recommends ca-certificates 
RUN apt-get install -y --no-install-recommends g++ 
RUN apt-get install -y --no-install-recommends python-dev 
RUN apt-get install -y --no-install-recommends autotools-dev 
RUN apt-get install -y --no-install-recommends libicu-dev 
RUN apt-get install -y --no-install-recommends libbz2-dev 
RUN apt-get install -y --no-install-recommends libboost-all-dev 
RUN apt-get install -y --no-install-recommends iproute2
RUN apt-get install -y --no-install-recommends python3-pip
RUN apt-get install -y --no-install-recommends libcurl4-openssl-dev
                                           
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

COPY --chown=webserver certs $HOME/certs

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