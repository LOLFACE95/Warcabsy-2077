#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define R 8
#define G 2

struct plansza
{
    char pola[R][R];
    int tura, wolne[R][R], dalej;
};

struct lista
{
    int w1, k1, w2, k2;
    struct lista *nast;
};

void element_listy(struct lista *glowa, int w1, int k1, int w2, int k2)
{
    if(glowa)
    {
        struct lista *ptr = calloc(1, sizeof(struct lista));

        ptr->w1 = w1;
        ptr->k1 = k1;
        ptr->w2 = w2;
        ptr->k2 = k2;
        ptr->nast = NULL;

        while(glowa->nast)
        {
            glowa = glowa->nast;
        }

        glowa->nast = ptr;
    }
}

void wypisz_liste(struct lista *glowa)
{
    int x1 = glowa->w1, y1 = glowa->k1;

    if(glowa)
    {
        printf("\n%c%c:", (char*)(glowa->k1 + 65), (char*)(glowa->w1 + 49));

        while(glowa)
        {
            if(x1 == glowa->w1 && y1 == glowa->k1)
            {
                printf(" %c%c", (char*)(glowa->k2 + 65), (char*)(glowa->w2 + 49));
            }
            else
            {
                x1 = glowa->w1;
                y1 = glowa->k1;
                printf("\n%c%c: %c%c", (char*)(y1 + 65), (char*)(x1 + 49), (char*)(glowa->k2 + 65), (char*)(glowa->w2 + 49));
            }

            glowa = glowa->nast;
        }

        printf("\n");
    }
}

void zniszcz_liste(struct lista *glowa)
{
    while(glowa)
    {
        struct lista *ptr = glowa;
        glowa = glowa->nast;
        free(ptr);
    }
}

struct plansza uzupelnij(struct plansza T)
{
    for(int i = 0; i < R; i++)
    {
        for(int j = 0; j < R; j++)
        {
            if(i < 3 && (i + j) % 2 == 0)
            {
                T.pola[i][j] = 'b';
                T.wolne[i][j] = 0;
            }
            else if(i >= R - 3 && (i + j) % 2 == 0)
            {
                T.pola[i][j] = 'c';
                T.wolne[i][j] = 0;
            }
            else
            {
                T.pola[i][j] = ' ';
                T.wolne[i][j] = 1;
            }
        }
    }

    return T;
}

void wypisz(struct plansza T)
{
    printf("\n   ");

    for(int i = 0; i < R; i++)
    {
        printf("%4c", (char*)(i + 65));
    }

    printf("\n");

    for(int i = 0; i < R; i++)
    {
        printf("    ");

        for(int j = 0; j < R; j++)
        {
            printf("+---");
        }

        printf("+\n%3d ", i + 1);

        for(int j = 0; j < R; j++)
        {
            printf("| %c ", T.pola[i][j]);
        }

        printf("|%3d \n", i + 1);
    }

    printf("    ");

    for(int i = 0; i < R; i++)
    {
        printf("+---");
    }

    printf("+\n   ");

    for(int i = 0; i < R; i++)
    {
        printf("%4c", (char*)(i + 65));
    }

    printf("\n\n");
}

int ocena_gry(struct plansza T)
{
    int b = 0, c = 0;

    for(int i = 0; i < R; i++)
    {
        for(int j = 0; j < R; j++)
        {
            if(T.pola[i][j] == 'b')
            {
                b++;
            }

            if(T.pola[i][j] == 'B')
            {
                b += 3;
            }

            if(T.pola[i][j] == 'c')
            {
                c--;
            }

            if(T.pola[i][j] == 'C')
            {
                c -= 3;
            }
        }
    }

    return b + c;
}

int koniec(struct plansza T)
{
    int b = 0, B = 0, suma_b = 0, c = 0, C = 0, suma_c = 0;

    for(int i = 0; i < R; i++)
    {
        for(int j = 0; j < R; j++)
        {
            if(T.pola[i][j] == 'b')
            {
                b++;
                suma_b++;
            }
            else if(T.pola[i][j] == 'B')
            {
                B++;
                suma_b++;
            }
            else if(T.pola[i][j] == 'c')
            {
                c++;
                suma_c++;
            }
            else if(T.pola[i][j] == 'C')
            {
                C++;
                suma_c++;
            }
        }
    }

    if(!c && !C)
    {
        return 0;
    }
    else if(!b && !B)
    {
        return 0;
    }
    else if(suma_b == 1 && suma_c == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void bicie(struct plansza T, struct lista *glowa, int i, int j, char pionek, char dama)
{
    if(i - 1 >= 0 && j - 1 >= 0)
    {
        if(T.pola[i - 1][j - 1] != pionek && T.pola[i - 1][j - 1] != dama && T.pola[i - 1][j - 1] != ' ')
        {
            if(i - 2 >= 0 && j - 2 >= 0)
            {
                if(T.wolne[i - 2][j - 2])
                {
                    element_listy(glowa, i, j, i - 2, j - 2);
                }
            }
        }
    }

    if(i - 1 >= 0 && j + 1 < R)
    {
        if(T.pola[i - 1][j + 1] != pionek && T.pola[i - 1][j + 1] != dama && T.pola[i - 1][j + 1] != ' ')
        {
            if(i - 2 >= 0 && j + 2 < R)
            {
                if(T.wolne[i - 2][j + 2])
                {
                    element_listy(glowa, i, j, i - 2, j + 2);
                }
            }
        }
    }

    if(i + 1 < R && j - 1 >= 0)
    {
        if(T.pola[i + 1][j - 1] != pionek && T.pola[i + 1][j - 1] != dama && T.pola[i + 1][j - 1] != ' ')
        {
            if(i + 2 < R && j - 2 >= 0)
            {
                if(T.wolne[i + 2][j - 2])
                {
                    element_listy(glowa, i, j, i + 2, j - 2);
                }
            }
        }
    }

    if(i + 1 < R && j + 1 < R)
    {
        if(T.pola[i + 1][j + 1] != pionek && T.pola[i + 1][j + 1] != dama && T.pola[i + 1][j + 1] != ' ')
        {
            if(i + 2 < R && j + 2 < R)
            {
                if(T.wolne[i + 2][j + 2])
                {
                    element_listy(glowa, i, j, i + 2, j + 2);
                }
            }
        }
    }
}

void dodawanie_ruchow(struct plansza T, struct lista *glowa, int i, int j, char pionek, char dama)
{
    if(T.pola[i][j] == 'c' || T.pola[i][j] == 'C')
    {
        if(i - 1 >= 0 && j - 1 >= 0)
        {
            if(T.wolne[i - 1][j - 1])
            {
                element_listy(glowa, i, j, i - 1, j - 1);
            }
        }

        if(i - 1 >= 0 && j + 1 < R)
        {
            if(T.wolne[i - 1][j + 1])
            {
                element_listy(glowa, i, j, i - 1, j + 1);
            }
        }

        if(T.pola[i][j] == 'C')
        {
            if(i + 1 < R && j - 1 >= 0)
            {
                if(T.wolne[i + 1][j - 1])
                {
                    element_listy(glowa, i, j, i + 1, j - 1);
                }
            }

            if(i + 1 < R && j + 1 < R)
            {
                if(T.wolne[i + 1][j + 1])
                {
                    element_listy(glowa, i, j, i + 1, j + 1);
                }
            }
        }

        if((i - 1 >= 0 && j - 1 < R) || (i - 1 >= 0 && j + 1 < R) || (i + 1 < R && j - 1 >= 0) || (i + 1 < R && j + 1 < R))
        {
            if(T.pola[i - 1][j - 1] == 'b' || T.pola[i - 1][j + 1] == 'b' || T.pola[i + 1][j - 1] == 'b' || T.pola[i + 1][j + 1] == 'b' || T.pola[i - 1][j - 1] == 'B' || T.pola[i - 1][j + 1] == 'B' || T.pola[i + 1][j - 1] == 'B' || T.pola[i + 1][j + 1] == 'B')
            {
                bicie(T, glowa, i, j, 'c', 'C');
            }
        }
    }

    if(T.pola[i][j] == 'b' || T.pola[i][j] == 'B')
    {
        if(i + 1 < R && j - 1 >= 0)
        {
            if(T.wolne[i + 1][j - 1])
            {
                element_listy(glowa, i, j, i + 1, j - 1);
            }
        }

        if(i + 1 < R && j + 1 < R)
        {
            if(T.wolne[i + 1][j + 1])
            {
                element_listy(glowa, i, j, i + 1, j + 1);
            }
        }

        if(T.pola[i][j] == 'B')
        {
            if(i - 1 >= 0 && j - 1 >= 0)
            {
                if(T.wolne[i - 1][j - 1])
                {
                    element_listy(glowa, i, j, i - 1, j - 1);
                }
            }

            if(i - 1 >= 0 && j + 1 < R)
            {
                if(T.wolne[i - 1][j + 1])
                {
                    element_listy(glowa, i, j, i - 1, j + 1);
                }
            }
        }

        if((i - 1 >= 0 && j - 1 < R) || (i - 1 >= 0 && j + 1 < R) || (i + 1 < R && j - 1 >= 0) || (i + 1 < R && j + 1 < R))
        {
            if(T.pola[i - 1][j - 1] == 'c' || T.pola[i - 1][j + 1] == 'c' || T.pola[i + 1][j - 1] == 'c' || T.pola[i + 1][j + 1] == 'c' || T.pola[i - 1][j - 1] == 'C' || T.pola[i - 1][j + 1] == 'C' || T.pola[i + 1][j - 1] == 'C' || T.pola[i + 1][j + 1] == 'C')
            {
                bicie(T, glowa, i, j, 'b', 'B');
            }
        }
    }
}

struct lista *ocena(struct plansza T)
{
    struct lista *glowa = calloc(1, sizeof(struct lista));

    glowa->w1 = -1;
    glowa->nast = NULL;

    for(int i = 0; i < R; i++)
    {
        for(int j = 0; j < R; j++)
        {
            if(T.tura == -1 && (T.pola[i][j] == 'c' || T.pola[i][j] == 'C'))
            {
                dodawanie_ruchow(T, glowa, i, j, 'c', 'C');
            }

            if(T.tura == 1 && (T.pola[i][j] == 'b' || T.pola[i][j] == 'B'))
            {
                dodawanie_ruchow(T, glowa, i, j, 'b', 'B');
            }
        }
    }

    return glowa;
}

struct plansza ruch(struct plansza T, struct lista *glowa, char y1, char x1, char y2, char x2)
{
    int w1, k1, w2, k2;

    w1 = (int*)(x1 - 49);
    k1 = (int*)(y1 - 65);
    w2 = (int*)(x2 - 49);
    k2 = (int*)(y2 - 65);

    int x = 0;

    while(glowa)
    {
        if(w1 == glowa->w1 && k1 == glowa->k1 && w2 == glowa->w2 && k2 == glowa->k2)
        {
            x = 1;
        }

        glowa = glowa->nast;
    }

    if(x)
    {
        if(T.tura == -1 && (T.pola[w1][k1] == 'c' || T.pola[w1][k1] == 'C'))
        {
            if(T.pola[w1][k1] == 'c')
            {
                if(w2 != 0)
                {
                    T.pola[w2][k2] = 'c';
                }
                else if(w2 == 0)
                {
                    T.pola[w2][k2] = 'C';
                }
            }
            else
            {
                T.pola[w2][k2] = 'C';
            }

            T.pola[w1][k1] = ' ';
            T.wolne[w1][k1] = 1;
            T.wolne[w2][k2] = 0;
            T.dalej = 0;

            if(abs(k2 - k1) == 2)
            {
                if(k1 > k2)
                {
                    if(w1 > w2)
                    {
                        T.pola[w1 - 1][k1 - 1] = ' ';
                        T.wolne[w1 - 1][k1 - 1] = 1;
                    }
                    else if(w1 < w2)
                    {
                        T.pola[w1 + 1][k1 - 1] = ' ';
                        T.wolne[w1 + 1][k1 - 1] = 1;
                    }
                }
                if(k1 < k2)
                {
                    if(w1 > w2)
                    {
                        T.pola[w1 - 1][k1 + 1] = ' ';
                        T.wolne[w1 - 1][k1 + 1] = 1;
                    }
                    else if(w1 < w2)
                    {
                        T.pola[w1 + 1][k1 + 1] = ' ';
                        T.wolne[w1 + 1][k1 + 1] = 1;
                    }
                }
            }
        }

        if(T.tura == 1 && (T.pola[w1][k1] == 'b' || T.pola[w1][k1] == 'B'))
        {
            if(T.pola[w1][k1] == 'b')
            {
                if(w2 != 7)
                {
                    T.pola[w2][k2] = 'b';
                }
                else if(w2 == 7)
                {
                    T.pola[w2][k2] = 'B';
                }
            }
            else
            {
                T.pola[w2][k2] = 'B';
            }

            T.pola[w1][k1] = ' ';
            T.wolne[w1][k1] = 1;
            T.wolne[w2][k2] = 0;
            T.dalej = 0;

            if(abs(k2 - k1) == 2)
            {
                if(k1 > k2)
                {
                    if(w1 > w2)
                    {
                        T.pola[w1 - 1][k1 - 1] = ' ';
                        T.wolne[w1 - 1][k1 - 1] = 1;
                    }
                    else if(w1 < w2)
                    {
                        T.pola[w1 + 1][k1 - 1] = ' ';
                        T.wolne[w1 + 1][k1 - 1] = 1;
                    }
                }
                if(k1 < k2)
                {
                    if(w1 > w2)
                    {
                        T.pola[w1 - 1][k1 + 1] = ' ';
                        T.wolne[w1 - 1][k1 + 1] = 1;
                    }
                    else if(w1 < w2)
                    {
                        T.pola[w1 + 1][k1 + 1] = ' ';
                        T.wolne[w1 + 1][k1 + 1] = 1;
                    }
                }
            }
        }
    }

    return T;
}

int minmax(struct plansza T, int glebokosc, int tura, struct lista *zmienna)
{
    struct lista *glowa = ocena(T);

    if(glowa->w1 == -1)
    {
        struct lista *ptr = glowa->nast;

        free(glowa);
        glowa = ptr;
    }

    if(!koniec(T) || glebokosc == 0)
    {
        int x = ocena_gry(T);

        zniszcz_liste(glowa);

        return x;
    }

    if(tura > 0)
    {
        int max = -100, stan;

        while(glowa)
        {
            struct plansza kopia = T;
            kopia = ruch(kopia, glowa, (char*)(glowa->k1 + 65), (char*)(glowa->w1 + 49), (char*)(glowa->k2 + 65), (char*)(glowa->w2 + 49));

            if(abs(glowa->k1 - glowa->k2) == 2)
            {
                stan = minmax(kopia, glebokosc - 1, kopia.tura, zmienna);
            }
            else
            {
                kopia.tura *= -1;
                stan = minmax(kopia, glebokosc - 1, kopia.tura, zmienna);
            }

            if(max < stan)
            {
                if(glebokosc == G)
                {
                    zmienna->w1 = glowa->w1;
                    zmienna->k1 = glowa->k1;
                    zmienna->w2 = glowa->w2;
                    zmienna->k2 = glowa->k2;
                }
                max = stan;
            }

            glowa = glowa->nast;
        }

        return max;
    }
    else if(tura < 0)
    {
        int min = 100, stan;

        while(glowa)
        {
            struct plansza kopia = T;
            kopia = ruch(kopia, glowa, (char*)(glowa->k1 + 65), (char*)(glowa->w1 + 49), (char*)(glowa->k2 + 65), (char*)(glowa->w2 + 49));

            if(abs(glowa->k1 - glowa->k2) == 2)
            {
                stan = minmax(kopia, glebokosc - 1, kopia.tura, zmienna);
            }
            else
            {
                kopia.tura *= -1;
                stan = minmax(kopia, glebokosc - 1, kopia.tura, zmienna);
            }

            if(min > stan)
            {
                if(glebokosc == G)
                {
                    zmienna->w1 = glowa->w1;
                    zmienna->k1 = glowa->k1;
                    zmienna->w2 = glowa->w2;
                    zmienna->k2 = glowa->k2;
                }

                min = stan;
            }

            glowa = glowa->nast;
        }

        return min;
    }

    zniszcz_liste(glowa);
}

struct lista *menu(struct lista *glowa)
{
    struct lista *polozenie = calloc(1, sizeof(struct lista));
    char x1, y1, x2, y2;

    printf("\nDostepne ruchy: ");
    wypisz_liste(glowa);
    printf("\nRuch czarnych\nPodaj ruch: ");

    scanf(" %c%c%c%c", &y1, &x1, &y2, &x2);

    if(y1 > 96 && y1 < 123)
    {
        y1 -= 32;
    }

    if(y2 > 96 && y2 < 123)
    {
        y2 -= 32;
    }

    polozenie->w1 = (int*)(x1 - 49);
    polozenie->k1 = (int*)(y1 - 65);
    polozenie->w2 = (int*)(x2 - 49);
    polozenie->k2 = (int*)(y2 - 65);

    return polozenie;
}

void podsumowanie(struct plansza T)
{
    int b = 0, B = 0, suma_b = 0, c = 0, C = 0, suma_c = 0;

    for(int i = 0; i < R; i++)
    {
        for(int j = 0; j < R; j++)
        {
            if(T.pola[i][j] == 'b')
            {
                b++;
                suma_b++;
            }
            else if(T.pola[i][j] == 'B')
            {
                B++;
                suma_b++;
            }
            else if(T.pola[i][j] == 'c')
            {
                c++;
                suma_c++;
            }
            else if(T.pola[i][j] == 'C')
            {
                C++;
                suma_c++;
            }
        }
    }

    if(suma_b > suma_c)
    {
        printf("\nBiale wygraly!\n");
    }
    else if(suma_b < suma_c)
    {
        printf("\nCzarne wygraly!\n");
    }
    else if(suma_b == 1 && suma_c == 1)
    {
        printf("\nRemis!\n");
    }
}

int main(void)
{
    struct plansza T = uzupelnij(T);

    T.tura = 1;

    do
    {
        T.dalej = 1;

        while(T.dalej)
        {
            char w1, k1, w2, k2;
            struct lista *glowa = ocena(T);

            if(!glowa->nast)
            {
                wypisz(T);
                printf("SIEMA\n");
                podsumowanie(T);

                return 0;
            }

            ocena_gry(T);

            if(glowa->w1 == -1)
            {
                struct lista *ptr = glowa->nast;

                free(glowa);
                glowa = ptr;
            }

            if(T.tura == -1)
            {
                struct lista *polozenie = menu(glowa);

                w1 = (char*)(polozenie->w1 + 49);
                k1 = (char*)(polozenie->k1 + 65);
                w2 = (char*)(polozenie->w2 + 49);
                k2 = (char*)(polozenie->k2 + 65);
                T = ruch(T, glowa, k1, w1, k2, w2);
                zniszcz_liste(polozenie);

                if(abs(k1 - k2) == 2)
                {
                    do
                    {
                        struct lista *ptr = calloc(1, sizeof(struct lista));

                        ptr->w1 = -1;
                        ptr->nast = NULL;
                        bicie(T, ptr, (int*)(w2 - 49), (int*)(k2 - 65), 'c', 'C');
                        wypisz(T);

                        if(!ptr->nast)
                        {
                            break;
                        }

                        struct lista *schowek = ptr->nast;

                        free(ptr);
                        ptr = schowek;

                        struct lista *polozenie = menu(ptr);
                        w1 = (char*)(polozenie->w1 + 49);
                        k1 = (char*)(polozenie->k1 + 65);
                        w2 = (char*)(polozenie->w2 + 49);
                        k2 = (char*)(polozenie->k2 + 65);

                        T = ruch(T, ptr, k1, w1, k2, w2);
                        zniszcz_liste(ptr);
                        zniszcz_liste(polozenie);

                    }
                    while(abs(k1 - k2) == 2);
                }
            }
            else if(T.tura == 1)
            {
                struct lista *zmienna = calloc(1, sizeof(struct lista));

                minmax(T, G, T.tura, zmienna);
                wypisz(T);
                T = ruch(T, glowa, (char*)(zmienna->k1 + 65), (char*)(zmienna->w1 + 49), (char*)(zmienna->k2 + 65), (char*)(zmienna->w2 + 49));
                wypisz(T);
                printf("Ruch bialych: %c%d% c%d\n", (char*)(zmienna->k1 + 65), zmienna->w1 + 1, (char*)(zmienna->k2 + 65), zmienna->w2 + 1);

                if(abs(zmienna->k1 - zmienna->k2) == 2)
                {
                    T.dalej = 1;
                }

                char schowek_w2 = zmienna->w2;

                while(T.dalej)
                {
                    T.dalej = 0;
                    zniszcz_liste(glowa);
                    glowa = ocena(T);
                    minmax(T, G, T.tura, zmienna);

                    if(schowek_w2 == zmienna->w1 && abs(zmienna->k1 - zmienna->k2) == 2)
                    {
                        schowek_w2 = zmienna->w2;
                        T = ruch(T, glowa, (char*)(zmienna->k1 + 65), (char*)(zmienna->w1 + 49), (char*)(zmienna->k2 + 65), (char*)(zmienna->w2 + 49));
                        wypisz(T);
                        printf("Ruch bialych: %c%d% c%d\n", (char*)(zmienna->k1 + 65), zmienna->w1 + 1, (char*)(zmienna->k2 + 65), zmienna->w2 + 1);

                        if(abs(zmienna->k1 - zmienna->k2) == 2)
                        {
                            T.dalej = 1;
                        }
                    }
                }

                zniszcz_liste(zmienna);
            }

            zniszcz_liste(glowa);
        }

        T.tura *= -1;
    }
    while(koniec(T));

    podsumowanie(T);
}
