# How to setup Mosquitto MQTT Broker using docker 
These instructions will work on any Debian based OS including Ubuntu, RaspberryPi, WSL2 etc...  
(For non-Debian distros, commands for installation need to be tweaked)  
_By default the config allows only to use local connections for security reasons._

## Install docker

Latest instructions are [here](https://docs.docker.com/engine/install/ubuntu/) on docker website.

## Create base folder for mqtt configuration

```bash

mkdir mqtt5
cd mqtt5

```

## Create Mosquitto config file - mosquitto.conf
```bash
nano mosquitto.conf
```

Basic configuration file content below
```
allow_anonymous false
listener 1883
persistence true
password_file /mosquitto/config/pwfile
```

## Create Mosquitto password file - pwfile

```bash
touch pwfile
```

## Create docker-compose file called 'mqtt5.yml'

```yml

version: "3.7"
services:
  # mqtt5 eclipse-mosquitto
  mqtt5:
    image: eclipse-mosquitto
    container_name: mqtt5
    ports:
      - "1883:1883" #default mqtt port (plain MQTT) / 8883 for TLS
    volumes:
      - ./mosquitto.conf:/mosquitto/config/mosquitto.conf:rw #mapped config file
      - ./pwfile:/mosquitto/config/pwfile:rw # mapped password file
      - mqtt5_data:/mosquitto/data:rw # volume mapped for data
      - mqtt5_log:/mosquitto/log:rw # volume mapped for log

# volumes for mapping data and log
volumes:
  mqtt5_data:
  mqtt5_log:

networks:
  default:
    name: mqtt5-network

```

## Create and run docker container for MQTT

```bash

sudo docker-compose -f mqtt5.yml -p mqtt5 up -d

```

### Check if the container is up and working (note down container-id)

```bash

sudo docker ps

```

## Create a user/password in the pwfile

```bash

# login interactively into the mqtt container
sudo docker exec -it <container-id> sh

# add user and it will prompt for password
mosquitto_passwd -c /mosquitto/config/pwfile user1

# delete user command format
mosquitto_passwd -D /mosquitto/config/pwfile <user-name-to-delete>

```

## Time to test !!!

### Install mosquitto client tools for testing
```bash

sudo apt install mosquitto-clients

```

### Let us start Subscriber now - topic name => 'hello/topic'

```bash

# Without authentication
mosquitto_sub -v -t 'hello/topic'

# With authentication
mosquitto_sub -v -t 'hello/topic' -u user1 -P <password>

```

### Let us start Publising to that topic

```bash

# Without authentication
mosquitto_pub -t 'hello/topic' -m 'hello MQTT'

# With authentication
mosquitto_pub -t 'hello/topic' -m 'hello MQTT' -u user1 -P <password>

```
