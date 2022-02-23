# Spins up a Docker container

# sets image name
export docker_img="dockerdev"

# mounts local current working directory to /home/calvin/ in the container
docker run -dit -e COLORTERM=truecolor --network mynetv6-1 -v "$(pwd)":/home/webserver/source_directory:z $docker_img bash
