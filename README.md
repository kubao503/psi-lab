### Build docker containers while being in the top-directory with:
```console
docker build -t server -f server/dockerfile .
docker build -t client -f client/dockerfile .
```
### Run docker containers with (ensure you psi_network is set):
```console
docker run -it --network psi_network --network-alias server server
docker run -it --network psi_network client
```
