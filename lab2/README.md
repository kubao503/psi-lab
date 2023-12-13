# Laboratorium 2
## Zespół: Z43
- Maciej Kozłowski - lider
- Adam Jeż
- Jakub Stacherski
- Michał Satała

## 2.1
Klient napisany w C przesyła do serwera, napisanego w Pythonie, drzewo binarne, którego węzły zawierają napisy zmiennej długości. Struktura węzła drzewa wygląda następująco:
```c
struct Node {
    struct Node *child_left;
    struct Node *child_right;
    char *text;
};
```
Funkcja `get_tree_example()` zwraca korzeń przykładowego drzewa, które następnie jest "spłaszczane", zapisywane do bufora i wysyłane do serwera. Serwer odbudowuje otrzymane drzewo używając następującej struktury reprezentującej węzeł:

```python
class Node:
    def __init__(self, text) -> None:
        self.child_left = None
        self.child_right = None
        self.text = text
```

W celu weryfikacji poprawności działania, zarówno klient jak i serwer wypisują wszystkie ścieżki od korzenia do wszystkich liści. Każda ścieżka wypisywana jest w nowej linii. Przykładowe dane imitują strukturę serwera DNS:
```
.google.com.               \
.studia.elka.pw.edu.pl.     } Oznacza, że drzewo zawiera 3 liście
.mion.elka.pw.edu.pl.      /
```
