FROM gcc:latest
COPY server/udp_server.c /
RUN gcc -o udp_server udp_server.c
CMD ["./udp_server"]