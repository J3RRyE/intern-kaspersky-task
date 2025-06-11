FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y build-essential cmake clang

WORKDIR /app
COPY . /app

RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make