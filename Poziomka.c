/**
 * @file Symulator gry w poziomkę.
 * @brief Program wczytuje ruchy Gracza Lewego, następnie oblicza ruchy Prawego gracza.
 * Program wczytuje ruchy Gracza Lewego, następnie oblicza ruchy Prawego, które optymalizują parametr "ocena planszy",
 * wypisuje ruch Prawego uzależniony od zmiennej WYBOR.
 * Znak "." oznacza poddanie się (dowolnego gracza);
 * Znak "-" oznacza przekazanie prawa do rozpoczęcia gry Prawemu graczowi;
 * Grę definiują cztery stałe: POLA, WIERSZE, KOLUMNY, WYBÓR;
 * 
 * @author Jan Roguwski <jroguwski@gmail.com>
 * @date 2021
 * **/
#include<stdio.h>
#include<limits.h>

/**

Definicje stałych;
Definicje typów;

**/

#ifndef POLA
# define POLA 5 //możliwe wartości od 1 do 26;
#endif

#ifndef WIERSZE
# define WIERSZE 26 //możliwe wartości od 1 do 26;
#endif

#ifndef KOLUMNY
# define KOLUMNY 26 //możliwe wartości od 1 do 26;
#endif

#ifndef WYBOR
# define WYBOR 1000 //dodatnia liczba mieszcząca się w typie int;
#endif
/*
Typ do przechowywania wszystkich możliwych ruchów maksymalizujacych ocene planszy dla Prawego;
Wszystkich możliwych ruchów jest <= WIERSZE * KOLUMNY;
Ocena Planszy dla Prawego jest zawsze z przedziału [-WIERSZE * KOLUMNY, WIERSZE * KOLUMNY];
Sposób zapisu ruchu: liczba czterocyfrowa abcd, gdzie liczba dwucyfrowa ab to nr
wiersza, liczba dwucyfrowa cd to nr kolumny;
*/
typedef int Ruchy[WIERSZE * KOLUMNY];

/**

Wczytywanie ruchów Lewego gracza;
znak '-' oznacza, że gracz Prawy zaczyna grę;
znak '.' oznacza poddanie się Lewego gracza;
Format linii wejścia: Xx\n, gdzie X - nazwa kolumny (z perspektywy Lewego), x - nazwa wiersza (z perspektywy Lewego);
Na przykład, dla poniższych poleceń gracza Lewego:
Fc
Ce
Da
Ga
I odpowiadającym im poleceniom gracza Prawego:
cH
eI
cA
fG
Otrzymamy poniższy stan planszy:

      A B C D E F G H I J K|
    a . . . # . . # . . . .|
    b . . . # . . # . . . .|
    c # # # # . # # # # # .|
    d . . . . . # . . . . .|
    e . . # . . # . . # # #|
    f . . # . . . # # # . .|
    g . . # . . . . . . . .|
    -----------------------+

**/

/*
wczytuje z wejścia zapis ruchu Lewego;
ruch Lewego jest postaci Xx, gdzie X - nazwa kolumny, x - nazwa wiersza;
jeśli gracz Lewy oddaje pierwszy ruch Prawemu, *nic==1;
jeśli gracz Lewy się poddaje, *poddanie==1;
*/
void czytaj_ruch(int *w, int *k, int *nic, int *poddanie)
{
    *poddanie=0;
    *nic=0;
    *w=(-1);
    *k=(-1);
    char c=(char)getchar();
    switch(c)
    {
        case '.':
        {
            *poddanie=1;
            break;
        }
        case '-':
        {
            *nic=1;
            break;
        }
        default:
        {
            *k=c-'A';
            c=(char)getchar();
            *w=c-'a';
            break;
        }
    }
    /*
    wczytanie ostatniego znaku wiersza: '\n';
    */
    c=(char)getchar();
    return;
}

/**

Ocena planszy;
Stan planszy z perspektywy gracza Prawego to liczba P-L, gdzie:
P - liczba bloków, które Prawy może naraz umieścić na planszy;
L - liczba bloków, które Lewy może naraz umieścić na planszy;

**/

/*
Dwie funkcje do zliczania liczby możliwych bloków, które mogą umieścić na planszy gracze naraz;
*/
int zlicz_bloki_prawy(int p[WIERSZE][KOLUMNY])
{
    int wynik=0;
    for(int i=0; i<WIERSZE; ++i)
    {
        int j=0;
        int dl_seg=0;//dlugość spójnego segmentu wolnych pól w wierszu i;
        while(j<KOLUMNY)
        {
            dl_seg=0;
            while(j<KOLUMNY && p[i][j]==0)
            {
                dl_seg++;
                j++;
            }
            wynik+=dl_seg/POLA;
            while(j<KOLUMNY && p[i][j]==1) j++;
        }
    }
    return wynik;
}
int zlicz_bloki_lewy(int p[WIERSZE][KOLUMNY])
{
    int wynik=0;
    for(int i=0; i<KOLUMNY; ++i)
    {
        int j=0;
        int dl_seg=0;//dlugość spójnego segmentu wolnych pól w kolumnie i;
        while(j<WIERSZE)
        {
            dl_seg=0;
            while(j<WIERSZE && p[j][i]==0)
            {
                dl_seg++;
                j++;
            }
            wynik+=dl_seg/POLA;
            while(j<WIERSZE && p[j][i]==1) j++;
        }
    }
    return wynik;
}
/*
Liczy ocenę podanej planszy z perspektywy Prawego;
*/
void ocen_plansze(int p[WIERSZE][KOLUMNY], int *ocena)
{
    /*
    ocena=rożnica możliwych do ułożenia na raz bloków;
    */
    *ocena=zlicz_bloki_prawy(p)-zlicz_bloki_lewy(p);
    return;
}

/**

Obsługa Planszy;

**/
/*
Inicjalizacja planszy;
*/
void inicjalizujplansze(int p[WIERSZE][KOLUMNY])
{
    for(int i=0; i<WIERSZE; ++i)
    {
        for(int j=0; j<KOLUMNY; ++j) p[i][j]=0;
    }
}
/*
zaznacza na planszy blok pionowy długości POLA;
blok zaczyna się w wierszu w i kolumnie k, z perspektywy Prawego;
jeśli nie można zaznaczyć, ustawia *blad na 1;
w przeciwnymm wypadku *blad==0;
*/
void zaznacz_pionowo(int w, int k, int p[WIERSZE][KOLUMNY], int *blad)
{
    *blad=0;
    for(int i=0; i<POLA && !(*blad); ++i)
    {
        if(w+i>=WIERSZE || p[w+i][k]) *blad=1;
    }
    if(!(*blad)) for(int i=0; i<POLA; ++i) p[w+i][k]=1;
    return;
}
/*
zaznacza na planszy blok poziomy długości POLA;
blok zaczyna się w wierszu w i kolumnie k, z perspektywy Prawego;
jeśli nie można zaznaczyć, ustawia *blad na 1;
w przeciwnymm wypadku *blad==0;
*/
void zaznacz_poziomo(int w, int k, int p[WIERSZE][KOLUMNY], int *blad)
{
    *blad=0;
    for(int i=0; i<POLA && !(*blad); ++i)
    {
        if(k+i>=KOLUMNY || p[w][k+i]) *blad=1;
    }
    if(!(*blad)) for(int i=0; i<POLA; ++i) p[w][k+i]=1;
    return;
}
/*
wykonuje ruch, zaznaczając odpowiednie pola na podanej planszy;
jeśli ruchu nie można wykonać, ustawia *blad na 1;
jeśli ruch można wykonać, *blad==0;
zwraca plansze z wykonanym na niej ruchem;
w, k - nr wiersza i kolumny z perspektywy gracza wykonującego ruch;
lewy - liczba ze zbioru {0,1}, lewy==1 wtw gdy ruch wykonuje gracz Lewy;
*/
void wykonajruch(int w, int k, int lewy, int p[WIERSZE][KOLUMNY], int *blad)
{
    *blad=0;
    /*
    jeśli graczem jest Lewy, to zamieniamy kolumny z wierszami;
    */
    if(lewy) zaznacz_pionowo(w, k, p, blad);
    else zaznacz_poziomo(w, k, p, blad);
    return;
}
/*
Podczas wyznaczania optymalnego ruchu Prawego, będziemy wykonywać różne ruchy i obliczać stan planszy po nich;
po każdym takim próbnym ruchu, należy go cofnąć;
do tego służy ta funkcja;
cofa ona jedynie ruchy wykonywane przez Prawego;
*/
void cofnijruch(int w, int k, int p[WIERSZE][KOLUMNY])
{
    for(int i=0; i<POLA; ++i)
    {
        p[w][k+i]=0;
    }
    return;
}

/**

Wybór ruchu Prawego gracza;
Jeśli więcej niż jeden ruch optymalizuje pozycję, należy:
    - posortować możliwe ruchy względem wierszy a następnie względem kolumn;
    - wybrać ruch o numerze (liczonym od 0): WYBOR % n, gdzie n to liczba możliwych ruchów;

**/

/*
inicjalizuje tablice ruchów R;
*/
void zeruj_ruchy(Ruchy R)
{
    for(int i=0; i<KOLUMNY * WIERSZE; ++i) R[i]=-1;
    return;
}
/*
Zwraca indeks ruchu zapisanego w tablicy R;
zwraca -1, jeśli Prawy się poddaje;
*/
int wybierz_ruch(Ruchy R)
{
    int n=0;
    int i=0;
    while(R[i]>(-1)&&i<KOLUMNY * WIERSZE)
    {
        n++;
        i++;
    }
    if(n>0) return WYBOR % n;
    return -1;
}
/*
Sprawdza każdy możliwy ruch po kolei;
dla każdego ruchu liczy ocene planszy powstałej po jego wykonaniu;
tworzy ciąg ruchów optymalizujących stan planszy i zapisuje go do tablicy p;
*/
void sprawdz_ruchy(Ruchy R, int p[WIERSZE][KOLUMNY])
{
    zeruj_ruchy(R);
    int blad=0;
    int ocena_max=INT_MIN;
    int ocena_akt;
    int wsk=0;
    for(int i=0; i<WIERSZE; ++i)
    {
        for(int j=0; j<=KOLUMNY - POLA; ++j)
        {
            wykonajruch(i, j, 0, p, &blad);
            
            if(blad==0)
            {
                ocen_plansze(p, &ocena_akt);
                if(ocena_akt>ocena_max)
                {
                    wsk=0;
                    zeruj_ruchy(R);
                    ocena_max=ocena_akt;
                    R[wsk]=i*100+j;
                    wsk++;
                }
                else if(ocena_akt==ocena_max)
                {
                    R[wsk]=(i*100)+j;
                    wsk++;
                }
                cofnijruch(i, j, p);
            }
        }
    }
}

/**

Wypisywanie wyników;

**/

/*
Pisze jeden wiersz wyjścia;
wyjście postaci xX\n, gdzie x-nazwa wiersza, X-nazwa kolumny;
jeśli Prawy się poddaje (nie ma ruchu), wypisujemy .\n;
zmienna poddanie==1 wtw Prawy się poddaje (w przeciwnym wypadku poddanie==0);
*/
void pisz_ruch(int w, int k)
{
    char c=(char)w+'a';
    printf("%c", c);
    c=(char)k+'A';
    printf("%c\n", c);
    return;
}

int main()
{
    int plansza[WIERSZE][KOLUMNY];
    inicjalizujplansze(plansza);
    int koniec=0;
    int w, k, nic, poddanie, blad, ruch;
    Ruchy R;
    zeruj_ruchy(R);
    while(!koniec)
    {
        czytaj_ruch(&w,&k,&nic,&poddanie);
        koniec=(poddanie==1);
        if(!koniec)
        {
            if(!nic)
            {
                wykonajruch(w,k,1,plansza,&blad);
            }
            sprawdz_ruchy(R,plansza);
            ruch=wybierz_ruch(R);
            if(ruch==(-1))
            {
                koniec=1;
                printf(".\n");
            }
            else
            {
                wykonajruch(R[ruch]/100, R[ruch]%100, 0, plansza, &blad);
                pisz_ruch(R[ruch]/100,R[ruch]%100);
            }
        }
    }
    return 0;
}