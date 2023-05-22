
########################################################################################################################
# dut build stage
########################################################################################################################

# base image
FROM public.ecr.aws/lts/ubuntu:latest AS build
#ENV DEBIAN_FRONTEND=noninteractive
# build as root
# USER 0
# install required build tools via packet manager
#RUN apt-get update -y && apt-get install -y ca-certificates curl build-essential git cmake libz-dev libssl-dev libcurl4-openssl-dev
RUN apt-get update -y && apt-get install -y build-essential cmake

WORKDIR /dut

COPY CMakeLists.txt .
COPY dut.cpp .
COPY testSet.cpp .
COPY messages.hpp .

WORKDIR /dut/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 8

RUN pwd
RUN ls -lha
# 


########################################################################################################################
# testSet final stage
########################################################################################################################

FROM public.ecr.aws/lts/ubuntu:latest

COPY --from=build \
    /dut/build/testSet \
    /app/

RUN mkdir -p /app/results

ENTRYPOINT [ "/app/testSet" ]
