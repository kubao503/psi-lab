# Laboratorium 1
## Zespół: Z43
- Maciej Kozłowski - lider
- Adam Jeż
- Jakub Stacherski
- Michał Satała


## 1.1
Stworzyliśmy programy dla serwera i klienta. Klient wysyła na serwer listę par klucz-wartość w formie tekstowej. Na początku wiadomości umieszczane są dwie liczby. Pierwsza z nich określa ilość par a druga długość pojedynczego klucza bądź wartości. Serwer, po otrzymaniu wiadomości, odsyła potwierdzenie do klienta z liczbą odebranych bajtów. Klient weryfikuje, czy liczba ta jest równa liczbie bajtów, które sam wysłał. Zarówno program klienta jak i serwera napisane są w C i Pythonie.

## 1.2
Uruchamiamy za pomocą komendy "docker-compose up".
Klient oczekuje na potwierdzenie od serwera i sprawdza poprawność potwierdzenia. W przypadku przekroczenia timeout-u powtarza próbę przesłania pakietu, aż do otrzymania potwierdzenia.
Za pomocą komendy "docker exec client tc qdisc add dev eth0 root netem delay 1000ms 500ms loss 50%", można zasymulować opóźnienie w przesyłaniu pakietu 1000 ms z rozrzutem (jitter) 500 ms i prawdopodobieństwem „zagubienia” pakietu równym 50%. (należy uruchomić komendę w trakcie działania kontenerów)

## 1.3a
Wykorzystując program z zadania 1.1 próbowaliśmy wysłać kolejne datagramy o przyrastającej wielkości. Największe dane jakie udało nam się wysłać miały rozmiar 65506B, przy próbie wysłania danych o rozmiarze 65508B wystąpił następujący błąd:
![image](https://github.com/kubao503/psi-lab/assets/67715928/18d1b40e-6ea4-4cce-9fa1-18567337d554)

Wynika to z tego, iż największy rozmiar pojedynczego datagramu protokołu IPv4 wynosi 65535B, z czego 8B zajmuje nagłówek UDP, a 20B - nagłówek IP. Na dane zostaje więc 65507B (Maximum Transmission Unit - (TCP header + IP header) = Maximum Segment Size).

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
