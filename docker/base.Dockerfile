### Base environment container ###
# Get the base Ubuntu image from Docker Hub
FROM ubuntu:bionic as base

# Update the base image and install build environment
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    curl \
    httpie \
    libboost-log-dev \
    libboost-signals-dev \
    libboost-system-dev \
    libgtest-dev \
    libboost-regex-dev \
    netcat \
    gcovr
