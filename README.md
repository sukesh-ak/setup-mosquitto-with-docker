# How to setup Mosquitto MQTT Broker using docker 
These instructions will work on any Debian based OS including Ubuntu, RaspberryPi, WSL2 etc...  
(For non-Debian distros, commands for installation need to be tweaked)  
_By default the config allows only to use local connections for security reasons._

## Install docker
```bash

sudo apt install docker.io

```

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
allow_anonymous true
listener 1883
persistence true
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
      - "1883:1883" #default mqtt port
    volumes:
      - ./mosquitto.conf:/mosquitto/config/mosquitto.conf
      - mqtt5_data:/mosquitto/data
      - mqtt5_log:/mosquitto/log

# volumes for mapping data and log
volumes:
  mqtt5_data:
  mqtt5_log:

networks:
  default:
    name: mqtt5-network

```

## Create docker container for mqtt

```bash

sudo docker-compose -f mqtt5.yml -p mqtt5 up -d

```

## Time to test !!!

### Install mosquitto client tools for testing
```bash

sudo apt install mosquitto-clients

```

### Let us start Subscriber now - topic name => 'hello/topic'

```bash

mosquitto_sub -v -t 'hello/topic'

```

### Let us start Publising to that topic

```bash

mosquitto_pub -t 'hello/topic' -m 'hello MQTT'

```
