# https-server-cd
# 1. Setup Environment
## 1.1 Clone PKI repo and create CA certificates
First we will need to clone the PKI repo
```
git clone https://github.com/Smooth-Pacific/https-server-pki-cd.git
```
We will then move into the repository and run the `create_ca.sh` script which will create the necessary CA certificates in `~/certs`.
```
cd https-server-pki-cd
sh create_ca.sh
```

## 1.2 Clone HTTPS server repo
We can now clone and enter the https server repository
```
git clone https://github.com/Smooth-Pacific/https-server-cd.git
cd https-server-cd
```

## 1.3 Build docker image
We can now build the docker image with the following script:
```
sh build_img.sh Dockerfile
```
The script will automatically copy the CA certificates from the `~/certs` (created in step 1.1) directory to `./certs` in the current repo directory. This will allow docker to build the image with the correct certificate, as docker cannot access any other parent folders.

## 1.4 Spinup and attach to docker container
Once the docker image is created, we can spinup and attach to the container as user webserver.
```
sh spinup.sh && sh attach.sh
```
The `attach.sh` script can be used separately to attach to the container in a new terminal

## 1.5 Check that the Root Certificate was installed successfully
Once we are attached to the container, we now need to check that the root certificate was installed successfully. If the root certificate has been validated we can exit the container via the command `exit`
```
awk -v cmd='openssl x509 -noout -subject' '/BEGIN/{close(cmd)};{print | cmd}' < /etc/pki/ca-trust/source/anchors/smoothstack_root.crt | grep smoothstack

exit
```

# 2. Build and Run the webserver
## 2.1 Build project inside docker container
We can now build the webserver, but first we attach to the container.
```
sh attach.sh
```
Once attached, we can now navigate to the project folder and build.
```
cd ~/source_directory
mkdir build && cd build
cmake ..
make
```
These commands will create a binary `./server` in the `~/source_directory/build` directory

## 2.2 Copy the CA certificates
We now need to copy the CA certificates directory into the container. We can accomplish by following the commands:
```
cd ~/source_directory
cp -r .certs ~/certs
```
This will copy the CA certificates to the `~certs` directory inside the container

## 2.3 Customizing server options
Before we run the server we can modify server settings in the `run_server.sh` script. The server options should look like:
```
# START SERVER OPTIONS
export PORT=8081
export MAX_CONNECTIONS=16
export TIMEOUT=120
export MEMORY_LIMIT=65536
export HTTPS_MEM_KEY_PATH=~/certs/server_ca/private/smoothstack_server.key
export HTTPS_MEM_CERT_PATH=~/certs/server_ca/certs/smoothstack_server.crt
# END SERVER OPTIONS
```
The script is setting environmental variables which the server can then access. We can modify these options if necessary.

## 2.4 Run server binary
After the project is built and all options set, we can run the webserver by running the `run_server.sh` script in the project root directory.
```
cd ~/source_directory
. ./run_server.sh
```

# 3. Testing
## 3.1 Test the server inside the docker container
We can test the server by opening a new terminal, attaching to the container, and running `curl 'https://172.17.0.2:8081/helloworld'`
```
# in a new terminal
sh attach.sh
curl -v 'https://172.17.0.2:8081/helloworld'
# or
curl -v 'https://localhost:8081/helloworld'
```
The command should output something like this in the very last line:
```
Hello, World!%
```