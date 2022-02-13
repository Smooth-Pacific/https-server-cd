# Builds a Docker Image with the supplied Dockerfile

export DOCKERFILE=$1

if [ $# -lt 1 ]; then # checks if number of arguments is less than 1
    echo "Less than 1 arguments were supplied"
    echo "Usage: 'sh build_img.sh <path_to_Dockerfile>'"
else
    if [ -f "./${DOCKERFILE}" ]; then # checks if Dockerfile exists
        # copy over certs to source dir
        if [ -d "./certs" ] 
        then
            echo "./certs exists, do nothing"
            cp -r ~/certs ./
        else
            echo "./certs doesn't exist, creating and entering directory"
            mkdir ./certs
            cp -r ~/certs ./
        fi

        # build docker image
        docker build -f $DOCKERFILE -t dockerdev .
    else
        echo "Dockerfile supplied does not exist"
    fi
fi
