## 1.3a Wyjaśnienia
Wykorzystując program z zadania 1.1 próbowaliśmy wysłać kolejne datagramy o przyrastającej wielkości. Największe dane jakie udało się nam wysłać miały rozmiar 65506B, przy próbie wysłania danych o rozmiarze 65508B wystąpił następujący błąd:
![image](https://github.com/kubao503/psi-lab/assets/67715928/09dced7e-3a7e-495d-a930-c4e0bdba7e78)
Wynika to z tego, iż największy rozmiar pojedynczego datagramu protokołu IPv4 wynosi 65535B, z czego 8B zajmuje nagłówek UDP, a 20B - nagłówek IP. Na dane zostaje więc 65507B.

### Build docker containers while being in the top-directory with:
```console
docker build -t z43_server -f server/dockerfile .
docker build -t z43_client -f client/dockerfile .
```
### Run docker containers with (ensure you psi_network is set):
```console
docker run -it --network z43_network --network-alias server --name server z43_server
docker run -it --network z43_network --name client z43_client
```
