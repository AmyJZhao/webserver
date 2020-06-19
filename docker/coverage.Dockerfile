FROM aaa-p2:base as builder

COPY . /usr/src/projects/aaa-p2
WORKDIR /usr/src/projects/aaa-p2/build
# RUN cd build_coverage
RUN pwd
RUN cmake -DCMAKE_BUILD_TYPE=Coverage ..
RUN make coverage