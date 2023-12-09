FROM gcc:latest
WORKDIR /client
COPY *.h *.c ./
RUN gcc *.c -o tcp_client
ENTRYPOINT ["./tcp_client"]
