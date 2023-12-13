## 2.2

Stworzyliśmy sieć która akceptuje połączenia ipv4 i ipv6:

```command
docker network create --ipv6 --subnet=fd00:1::/64 --subnet=172.20.0.0/16 psi_v6
```

Zmodyfikowaliśmy kod serwera z punktu 2.1, aby przyjmował połączenia ipv4 i ipv6.

serwer przyjmuje połączenie ipv4:

![ipv4 test](ipv4.png)

serwer przyjmuje połączenie ipv6:

![ipv6 test](ipv6.png)
