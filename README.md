# Saper-SFML
Obiektowa gra Saper stworzona przy pomocy biblioteki SFML.

## Opis klas i metod
```
class Wynik
```
Zawiera w sobie zmienne opisujące sprawdzające czy nastąpiła wygrana oraz czas pojedyńczej gry.


```
class Board
```
Klasa obsługująca plansze gry, której gracz nie widzi.
```
bool generujPlansze()
```
Metoda odpowiadająca za wypełnienie tablicy plansza wartościami równymi 10, pozwala to wyświetlić zakryte pole.

```
class Bomb
```
Główna klasa odpowiadająca za mechanikę gry.

```
bool ustawMine()
```
Ustawia minę w wylosowanej pozycji, sprawdza czy otoczenie miny nie wychodzi poza zakres.

```
void losujPozycje()
```
Metoda losuje pozycje każdej miny.

```
class Game
```
Obsługuje obiekty biblioteki SFML, generuje okno gry.

```
bool checkWin()
```
Sprawdza po każdym naciśnięciu lewego przycisku myszy nastąpiła wygrana.

```
void show();
```
Odkrywa puste pola.

```
void showResult()
```
Wyświetla wynik gry.

```
void play()
```
Główna funkcja odpowiadająca za gre, zawiera pętlę obsługującą Eventy, rysuje obiekty na ekranie.

```
class MainMenu
```
KLasa generuje i obsługuje menu gry.

```
void draw()
```
Rysuje tekst w oknie.

```
void MoveUp(), void MoveDown()
```
Obsługują przesuwanie się po elementach menu głównego.

```
void showMenu()
```
Metoda rysuje okno menu, tworzy obiekty klasy Game, pozwala na wybranie odpowiedniej opcji z menu.



## Obsługa gry
W menu głównym poruszamy się przy pomocy strzałek, a wybraną opcje zatwierdzamy klawiszem Enter.
W oknie gry wbieramy pole kursorem, odkrywamy pole klikając lewym przyciskiem myszy, natomiast flage ustawiamy przy pomocy prawego przycisku.
Zapisanie aktualnego postępu gry następuje po naciśnięciu klaisza S na klawiaturze.
Z gry wychodzimy przez zamknięcie okna gry.
