FROM centos:latest

RUN sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-Linux-*
RUN sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-Linux-*

RUN yum update -y
RUN yum -y install dnf
RUN dnf -y --enablerepo=powertools install gtest-devel
RUN dnf -y --enablerepo=powertools install libmicrohttpd-devel
RUN dnf -y group install "Development Tools"
RUN dnf -y install ca-certificates
RUN yum -y install git
RUN yum -y install curl
RUN yum -y install wget
RUN yum -y install vim
RUN yum -y install tree
RUN yum -y install zip
RUN yum -y install unzip
RUN yum -y install libtool
RUN yum -y install m4
RUN yum -y install automake
RUN yum -y install cmake
RUN yum -y install make
RUN yum -y install clang
RUN yum -y install gdb
RUN yum -y install cscope
RUN yum -y install epel-release
RUN yum -y install htop
RUN yum -y install iotop
RUN yum -y install iftop
RUN yum -y install zsh
RUN yum -y install nano
RUN yum -y install xterm
RUN yum -y install autoconf

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
COPY ./certs/root_ca/certs/smoothstack_root.crt /etc/pki/ca-trust/source/anchors/smoothstack_root.crt
RUN update-ca-trust enable
RUN update-ca-trust trust
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