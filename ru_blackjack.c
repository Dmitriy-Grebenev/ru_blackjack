#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CARDS 52

int balance;
int pot;
int deck[52];
int sdeck[52];

void play();
void turn();
int hit(int *val);
char *cface(int v);
char *csuit(int v);
int cval(int v);
void bet();
void dealer();
void result();
void initdeck();
void shuffle();
void checkAce(int v1, int v2, int *a1);
void quit();

int main()
{
    printf("Добро пожаловать в BlackJack Simulator! Основные правила:\n");
    printf("\n- Побейте расклад дилера, не превышая 21.");
    printf("\n- Картинки стоят 10, тузы стоят 1 или 11, в зависимости от того, какой расклад лучше.");
    printf("\n- Каждый игрок начинает с двумя картами, а одна из карт дилера скрыта до конца.");
    printf("\n- Наберите \"hit\", чтобы попросить еще одну карту. Введите \"stand\", чтобы удержать свой итог и закончить ход.");
    printf("\n- Если у вас больше 21, вы проиграли, и дилер выигрывает независимо от своего расклада.");
    printf("\n- Если вам сдали 21 с самого начала (туз и 10), у вас блэкджек. ");
    printf("\n- Если вам выпал блэкджек, вы автоматически выигрываете в 1,5 раза больше суммы вашей ставки,");
    printf("\n- если только дилер также не получит блэкджек, в этом случае это будет пуш.");
    printf("\n- Помните: Наберите \"hit\", чтобы получить еще одну карту, и \"stand\", чтобы остаться. В начале раунда введите \"bet\", а затем количество, которое вы хотите поставить (например, \"bet 50\").");
    printf("\nНаберите \"play\", чтобы начать. В любой момент вы можете набрать 'help', чтобы получить список допустимых команд.\n");

    char input[6];
    scanf("%5s", input);

    while (strcmp(input, "quit") != 0)
    {
        while (strcmp(input, "play") != 0)
        {
            if (strcmp(input, "help") == 0)
                printf("Напишите 'play' для начала игры.\n");
            else if (strcmp(input, "quit") == 0)
            {
                quit();
                printf("Введите 'play' для начала игры.\n");
            }
            else
                printf("Неверная команда, введите 'play' для начала.\n");
            scanf("%s", input);
        }
        play();
        scanf("%5s", input);
    }
}

void play()
{
    printf("Начнем с того, что у вас есть 500 денег.\n");
    balance = 500;
    while (balance > 0)
    {
        bet();
        initdeck();
        shuffle();
        turn();
    }
    printf("Извините, похоже, вы проиграли! Введите \"play\", чтобы сыграть в другую игру.\n");
}

void turn()
{
    int inc = 0;

    int d1 = hit(&inc);
    char *cn1 = csuit(d1);
    char *cf1 = cface(d1);
    int cv1 = cval(d1);

    printf("Вы получили следующие карты: %s %s стоимостью %d и", cf1, cn1, cv1);

    int d2 = hit(&inc);
    char *cn2 = csuit(d2);
    char *cf2 = cface(d2);
    int cv2 = cval(d2);

    printf(" %s %s стоимостью %d.\n", cf2, cn2, cv2);

    int d3 = hit(&inc);
    char *cn3 = csuit(d3);
    char *cf3 = cface(d3);
    int cv3 = cval(d3);

    printf("Показанная карта дилера: %s %s стоимостью %d.\n", cf3, cn3, cv3);

    int d4 = hit(&inc);
    char *cn4 = csuit(d4);
    char *cf4 = cface(d4);
    int cv4 = cval(d4);

    int ptotal = cv1 + cv2;
    int dtotal = cv3;

    checkAce(cv1, cv2, &ptotal);

    printf("В сумме вы имеете %d очков, а дилер имеет %d очков.\n", ptotal, dtotal);

    if (ptotal == 21)
    {
        printf("Поздравляем! У вас блэкджек! Ваш выигрыш 2 к 1, %d.\n", (int)(pot * 2));
        balance += (pot * 2);
    }
    else
    {
        char input[6];
        scanf("%5s", input);
        while (strcmp(input, "stand") != 0)
        {
            if (strcmp(input, "hit") == 0)
            {
                int val = hit(&inc);
                char *ns = csuit(val);
                char *nf = cface(val);
                int nv = cval(val);
                ptotal += nv;
                printf("Вы получили %s %s стоимостью %d.\n", nf, ns, nv);
                if (ptotal < 21)
                {
                    if (nv == 1)
                    {
                        if (ptotal + 10 == 21)
                        {
                            ptotal += 10;
                            printf("Поздравляем! У вас 21! Ваш выигрыш 2 к 1.\n");
                            balance += (pot * 2);
                            break;
                        }
                        else
                        {
                            int input;
                            printf("У вас туз. Введите число: он должен стоит 1 или 11?.\n");
                            scanf("%d", &input);
                            if (input == 11)
                                ptotal += 10;
                        }
                    }
                }
                else if (ptotal == 21)
                {
                    printf("Поздравляем! У вас 21!\n");
                    break;
                }
                else
                {
                    printf("О нет, вы проиграли с %d очками. Попробуйте еще раз!\n", ptotal);
                    break;
                }
            }
            else if (strcmp(input, "help") == 0)
                printf("Введите 'hit', чтобы получить еще одну карту. Введите 'stand', чтобы оставить как есть.\n");
            else if (strcmp(input, "quit") == 0)
            {
                quit();
                printf("Введите 'hit' или 'stand'.\n");
            }
            else
                printf("Неизвестная команда, попробуйте снова.\n");
            printf("Your new total is %d.\n", ptotal);
            scanf("%5s", input);
        }
    }

    if (ptotal < 21)
    {

        printf("Скрытой картой дилера оказалась %s %s стоимостью %d.\n", cf4, cn4, cv4);
        dtotal += cv4;
        if (cv4 == 1)
        {
            if (dtotal + 11 < 21)
                dtotal += 10;
        }

        if (dtotal >= 16)
        {
            printf("У дилера %d.\n", dtotal);
        }

        while (dtotal < 16)
        {
            int val = hit(&inc);
            char *ns = csuit(val);
            char *nf = cface(val);
            int nv = cval(val);
            dtotal += nv;
            printf("Дилер получил %s %s стоимостью %d.\n", nf, ns, nv);
            if (dtotal < 16)
            {
                if (nv == 1)
                {
                    if (dtotal + 11 < 21)
                        dtotal += 10;
                }
            }
            else if (dtotal == 21)
            {
                printf("Дилер получил 21.\n");
                break;
            }
            else if (dtotal > 21)
            {
                printf("Дилер получил %d! Вы выиграли!\n", dtotal);
                break;
            }
            else
            {
                printf("У дилера %d.\n", dtotal);
                break;
            }
            printf("Сумма очков дилера: %d.\n", dtotal);
        }
        if (dtotal < ptotal)
        {
            printf("Вы победили дилера! Ваша выплата: %d.\n", (int)(pot * 1.5));
            balance += (pot * 1.5);
        }
        else if (dtotal == ptotal)
        {
            printf("Ничья! Push pot, выплата 1 к 1: %d.\n", pot);
            balance += pot;
        }
        else if (dtotal > ptotal && dtotal <= 21)
        {
            printf("О нет! Кажется, дилер выиграл. Попробуйте снова!\n");
        }
        else
        {
            printf("Вы выиграли дилера! Ваша выплата %d.\n", (int)(pot * 1.5));
            balance += (pot * 1.5);
        }
    }
    if (balance > 0)
        printf("Ваш баланс: %d.\n", balance);
}

void checkAce(int v1, int v2, int *a1)
{
    if (v1 == 1 || v2 == 1)
    {
        int input;
        if (v1 == 1 && v2 == 1)
        {
            v1 = 11;
            v2 = 1;
            *a1 = 12;
            printf("Поскольку у вас 2 туза, мы устанавливаем, что один из них стоит 11, а другой - 1.\n");
        }
        else if (v1 == 1 || v2 == 1)
        {
            if (v1 + 10 + v2 == 21)
                *a1 == 21;
            else
            {
                printf("У вас есть туз. Выберите, как считать его количество очков: 1 или 11:\n");
                scanf("%d", &input);
                if (input == 11)
                    *a1 += 10;
            }
        }
    }
}

int cval(int v)
{
    int a;
    if (((v % 13) + 1) < 10)
        a = v % 13 + 1;
    else
        a = 10;
    return a;
}

char *csuit(int v)
{
    v = v % 4;
    switch (v)
    {
    case 0:
        return "червей";
    case 1:
        return "треф (крестей)";
    case 2:
        return "бубен";
    case 3:
        return "пик";
    }
}

char *cface(int v)
{
    v = v % 13 + 1;
    char *output;
    switch (v)
    {
    case 1:
        output = "туз";
        break;
    case 2:
        output = "двойка";
        break;
    case 3:
        output = "тройка";
        break;
    case 4:
        output = "четвёрка";
        break;
    case 5:
        output = "пятёрка";
        break;
    case 6:
        output = "шестёрка";
        break;
    case 7:
        output = "семёрка";
        break;
    case 8:
        output = "восьмёрка";
        break;
    case 9:
        output = "девятка";
        break;
    case 10:
        output = "десятка";
        break;
    case 11:
        output = "валет";
        break;
    case 12:
        output = "дама";
        break;
    case 13:
        output = "король";
    }
    return output;
}

void quit()
{
    printf("Вы уверены, что хотите выйти? Введите 'y' или 'n'.\n");
    char input[6];
    scanf("%s", input);
    if (strcmp(input, "y") == 0)
        exit(0);
    else
        printf("Выход отменён.\n");
}

int hit(int *i)
{
    int a = sdeck[*i];
    *i = *i + 1;
    return a;
}

void bet()
{
    char input[6];
    int val;
    printf("Введите величину ставки.\n");
    scanf("%5s", input);
    while (strcmp(input, "bet") != 0)
    {
        if (strcmp(input, "help") == 0)
            printf("Введите 'bet' и величину ставки. Например, 'bet 50'.\n");
        else if (strcmp(input, "quit") == 0)
        {
            quit();
            printf("Введите величину ставки.\n");
        }
        else
            printf("Неверная команда. Введите 'help' для вывода списка команд.\n");
        scanf("%5s", input);
    }
    scanf("%d", &val);
    while (val > balance || (val < 10 && val > 0) || val <= 0)
    {
        if (val > balance)
            printf("Вы не можете сделать ставку, которая больше вашего баланса. Ваш баланс: %d.\n", balance);
        else if (val <= 0)
            printf("Вы должны сделать ставку. Введите корректную величину ставки.\n");
        else
            printf("Ошибка ввода. Попробуйте снова.\n");
        scanf("%d", &val);
    }
    balance -= val;
    printf("Вы сделали ставку величиной %d. Удачи!\n", val);
    pot = val;
}

void initdeck()
{
    for (int i = 0; i < 52; i++)
    {
        deck[i] = i + 1;
    }
}

void shuffle()
{
    srand(time(NULL));
    int i = 0;
    for (i; i < 52; i++)
    {
        sdeck[i] = deck[i];
    }
    i = 0;
    for (i = CARDS - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int n = sdeck[i];
        sdeck[i] = sdeck[j];
        sdeck[j] = n;
    }
}
