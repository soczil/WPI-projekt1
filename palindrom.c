#include <stdio.h>
#include <stdbool.h>

#define ROZMIAR_PLANSZY 8
#define DLUGOSC_PALINDROMU 5

/* Maksymalnie tyle roznych piecio-elementowych wyrazow znajduje sie w jednej linii (wierszu, kolumnie, przekatnej) na planszy. */
#define MAX_PALINDROMOW_W_LINII 4

/* Funkcja ustawia wszystkie wartosci w planszy na znak '-'. */
void start_planszy(char t[][ROZMIAR_PLANSZY]) {
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
            t[i][j] = '-';
        }
    }
}

/*Funkcja wypisuje plansze. */
void wypisz_plansze(char t[][ROZMIAR_PLANSZY]) {
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
            printf(" %c", t[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        printf(" %c", 'a' + i);
    }
    printf("\n");
}

/* Funckja wczytuje ruch gracza i sprawdza czy jest on poprawny. Jesli ruch jest poprawny to wynikiem funkcji jest ten ruch.
   Jesli ruch jest niepoprawny to wynikiem jest 1. Jesli zostal wczytany koniec pliku to wynikiem jest 0. */
char wczytaj_ruch() {
    char c = getchar();
    if (c != EOF) {
        if (c == '\n') {
            c = 1;
        } else {
            char d = getchar();
            if (d != '\n' || c > 'h' || (c < 'a' && c != '=')) {
                c = 1;
                while (d != '\n') {
                    d = getchar();
                }
            }
        }
    } else {
        c = 0;
    }
    return c;
}

/* Funkcja przyjmuje ruch gracza i w wyniku daje numer wiersza, na ktorym mozna postawic 
   pion w danej kolumnie. */
int gdzie_postawic(char t[][ROZMIAR_PLANSZY], char c) {
    for (int i = ROZMIAR_PLANSZY - 1; i >= 0; i--) {
        if (t[i][c - 'a'] == '-') {
            return i;
        }
    }
    return -1;
}

/* Funkcja sprawdza czy w wierszu, w ktorym postawiono pion, zostal utworzony 
   palindrom z pieciu pionow. */
bool czy_palindrom_wiersz(char t[][ROZMIAR_PLANSZY], int wiersz) {
    bool palindrom = false;
    for (int licznik = 0; licznik < MAX_PALINDROMOW_W_LINII && !palindrom; licznik++) {
        palindrom = true;
        /* Sprawdza czy na srodkowym elemencie slowa jest jakis pion. */
        if (t[wiersz][licznik + (DLUGOSC_PALINDROMU / 2)] == '-') {
            palindrom = false;
        } else {
            for (int i = licznik, j = i + DLUGOSC_PALINDROMU - 1; i < j; i++, j--) { 
                if (t[wiersz][i] != t[wiersz][j] || t[wiersz][i] == '-' || t[wiersz][j] == '-') {
                    palindrom = false;
                }
            }
        }
    }
    return palindrom;
}

/* Funkcja sprawdza czy w kolumnie, w ktorej postawiono pion, zostal utworzony
   palindrom z pieciu pionow. */
bool czy_palindrom_kolumna(char t[][ROZMIAR_PLANSZY], int kolumna) { 
    bool palindrom = false;
    for (int licznik = 0; licznik < MAX_PALINDROMOW_W_LINII && !palindrom; licznik++) {
        palindrom = true;
        /* Sprawdza czy na srodkowym elemencie slowa jest jakis pion. */
        if (t[licznik + (DLUGOSC_PALINDROMU / 2)][kolumna] == '-') {
            palindrom = false;
        } else {
            for (int i = licznik, j = i + DLUGOSC_PALINDROMU - 1; i < j; i++, j--) {
                if (t[i][kolumna] != t[j][kolumna] || t[i][kolumna] == '-' || t[j][kolumna] == '-') {
                    palindrom = false;
                }
            }
        } 
    }
    return palindrom;
}

/* Funkcja sprawdza czy na przekatnej ,przechodzacej przez punkt, na ktorym
   postawiono pion, zostal utworzony palindrom z pieciu pionow. */
bool czy_palindrom_przekatna1(char t[][ROZMIAR_PLANSZY], int wiersz, int kolumna) {
    bool palindrom = false;
    int krok, i, j;
    /* Ustawia wspolrzedne punktu na poczatku przekatnej. */
    while (wiersz != ROZMIAR_PLANSZY - 1 && kolumna != 0) {
        wiersz++;
        kolumna--;
    }
    for (int licznik = 0; licznik < DLUGOSC_PALINDROMU - 1 && !palindrom; licznik++) { 
        palindrom = true; 
        if ((wiersz - (DLUGOSC_PALINDROMU / 2) >= 0)
            && (kolumna + (DLUGOSC_PALINDROMU / 2) < ROZMIAR_PLANSZY)) {
            if (t[wiersz - (DLUGOSC_PALINDROMU / 2)][kolumna + (DLUGOSC_PALINDROMU / 2)] == '-') {
                palindrom = false;
            } else {
                krok = 0;
                /* Ustawia wspolrzedne na przeciwnej stronie badanego odcinka przekatnej. */
                i = wiersz - DLUGOSC_PALINDROMU + 1;
                j = kolumna + DLUGOSC_PALINDROMU - 1;
                while (krok <= (MAX_PALINDROMOW_W_LINII / 2) && palindrom) {
                    if ((wiersz - krok >= 0) && (kolumna + krok < ROZMIAR_PLANSZY)
                        && (i + krok < ROZMIAR_PLANSZY) && (j - krok >= 0)) {
                        if (t[wiersz - krok][kolumna + krok] != t[i + krok][j - krok]
                            || t[wiersz - krok][kolumna + krok] == '-' || t[i + krok][j - krok] == '-') {
                            palindrom = false;
                        }
                    }
                    krok++;
                }
            }
        } else {
            palindrom = false;
        }
        wiersz--;
        kolumna++;   
    }
    return palindrom;
}

/* Funkcja sprawdza czy na drugiej przekatnej ,przechodzacej przez punkt, 
   na ktorym postawiono pion, zostal utworzony palindrom z pieciu pionow. */
bool czy_palindrom_przekatna2(char t[][ROZMIAR_PLANSZY], int wiersz, int kolumna) {
    bool palindrom = false;
    int krok, i, j;
    /* Ustawia wspolrzedne punktu na poczatku przekatnej. */
    while (wiersz != ROZMIAR_PLANSZY - 1 && kolumna != ROZMIAR_PLANSZY - 1) {
        wiersz++;
        kolumna++;
    }
    for (int licznik = 0; licznik < DLUGOSC_PALINDROMU - 1 && !palindrom; licznik++) { 
        palindrom = true; 
        if ((wiersz - (DLUGOSC_PALINDROMU / 2)) >= 0 && (kolumna - (DLUGOSC_PALINDROMU / 2) >= 0)) {
            if (t[wiersz - (DLUGOSC_PALINDROMU / 2)][kolumna - (DLUGOSC_PALINDROMU / 2)] == '-') {
                palindrom = false;
            } else {
                krok = 0;
                /* Ustawia wspolrzedne na przeciwnej stronie badanego odcinka przekatnej. */
                i = wiersz - DLUGOSC_PALINDROMU + 1;
                j = kolumna - DLUGOSC_PALINDROMU + 1;
                while (krok < (MAX_PALINDROMOW_W_LINII / 2) && palindrom) {
                    if ((wiersz - krok >= 0) && (kolumna - krok >= 0) && (i + krok < ROZMIAR_PLANSZY)
                        && (j + krok < ROZMIAR_PLANSZY)) {
                        if (t[wiersz - krok][kolumna - krok] != t[i + krok][j + krok]
                            || t[wiersz - krok][kolumna - krok] == '-' || t[i + krok][j + krok] == '-') {
                            palindrom = false;
                        }
                    }
                    krok++;
                }
            }
        } else {
            palindrom = false;
        }
        wiersz--;
        kolumna--;
    }
    return palindrom;
}

/* Funkcja, ktora prowadzi rozgrywke miedzy graczami. */
void graj(char plansza[][ROZMIAR_PLANSZY]) {
    int wiersz = 0, kolumna = 0;
    char gracz = '1';
    bool koniec = false;
    char c;
    while (!koniec) {
        wypisz_plansze(plansza);
        printf("%c:\n", gracz);
        c = wczytaj_ruch();
        /* Koniec gry jesli zostal wczytany EOF lub znak '='. */
        if (c == 0 || c == '=') {
            koniec = true;
        } else if (c != 1) { /* Jesli zostal wczytany poprawny ruch, to funkcja wykonuje ten ruch. */
            wiersz = gdzie_postawic(plansza, c);
            if (wiersz != -1) {
                kolumna = c - 'a';
                plansza[wiersz][kolumna] = gracz;
                if (czy_palindrom_wiersz(plansza, wiersz) || czy_palindrom_kolumna(plansza, kolumna)
                    || czy_palindrom_przekatna1(plansza, wiersz, kolumna)
                    || czy_palindrom_przekatna2(plansza, wiersz, kolumna)) {
                    wypisz_plansze(plansza);
                    printf("%c.\n", gracz);
                    koniec = true;
                }
                gracz = (gracz == '1') ? '2' : '1'; /* Zmiana gracza. */
            }
        }
    }
}

int main(void) {
    char plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY];

    start_planszy(plansza);

    graj(plansza);

    return 0;
}
