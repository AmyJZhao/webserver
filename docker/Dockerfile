### Build/test container ###

# Define builder stage
FROM aaa-p2:base as builder

# Share work directory
COPY . /usr/src/project
WORKDIR /usr/src/project/build

# Build and test
RUN cmake ..
RUN make

                        # RUN pwd
                        # RUN find . -name "server"
                        # RUN ctest --output-on_failure

### Deploy container ###
# Define deploy stage
FROM ubuntu:bionic as deploy

#  Copy server output binary to "."
COPY --from=builder /usr/src/project/build/bin/server .
COPY --from=builder /usr/src/project/config/prod_config .
# If developing locally, comment out line above and uncomment line below
#  COPY --from=builder /usr/src/project/config/local_docker_config .
COPY --from=builder /usr/src/project/tests/integration/files ./files

#  Expose some port(s)
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
# Update with real server name
ENTRYPOINT ["./server"]

# Use CMD to specify arguments to ENTRYPOINT
#  Update with real server args
CMD ["prod_config"]
# If developing locally, comment out line above and uncomment line below
#  CMD ["local_docker_config"]