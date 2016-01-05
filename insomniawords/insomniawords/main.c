#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define s_size 256



void show_menu();
void mem_alloc(char**, char**, unsigned int*);
void save_file(char**, char**, unsigned int*);

int main()
{
    unsigned int Sum_of_Terms = 0;
    char **En, **Gr;
    char ch, save;

    // Memory Initialization.
    En = (char**) malloc(sizeof(char*));
    Gr = (char**) malloc(sizeof(char*));


    do{
        show_menu();
        printf("\nInsert option: ");
        //scanf("%d",&ch);
        ch = _getch();
        printf("\n");

        if (ch == '6')
        {
            do{
                printf("Would you like to save changes?(y/n): ");
                save = _getch();
                printf("\n");

            } while (save != 'y' && save != 'n');
           

        }

        if (ch>'6' || ch<'1') printf("Invalid choice!\n");
        switch (ch)
        {
        case '1': mem_alloc(En, Gr, &Sum_of_Terms);
            break;
        }
        printf("\n Sum: %d", Sum_of_Terms);


    } while (ch != '6');


    return 0;
}

void show_menu()
{
    printf("\n\n********************************************");
    printf("\n***************** MENU *********************\n");
    printf("********************************************\n\n");

    printf("1. Insert new word. \n");
    printf("2. Modify an existing entry. \n");
    printf("3. Delete an entry. \n");
    printf("4. Translate a word. \n");
    printf("5. Translate a sentence. \n");
    printf("6. EXIT.\n");

}


void mem_alloc(char **En, char **Gr, unsigned int *w_count) // w_count = word count.
{
    int i;
    char s[s_size];

    (*w_count)++;

    En = (char**) malloc(sizeof(char*)*(*w_count));
    Gr = (char**) malloc(sizeof(char*)*(*w_count));

    if (En == NULL || Gr == NULL)
        printf("Could Not Allocate Memory (1).\n");
    else
    {

        //English Word.
        En[*w_count] = (char*) malloc(s_size*sizeof(char));
        if (En[*w_count] == NULL)
            printf("Could not allocate memory (2).\n");
        else
        {
            printf("Insert English Word: ");
            scanf("%s", s);
            //gets(s);
            En[*w_count] = (char*) malloc((strlen(s) + 1)*sizeof(char)); // !**!
            strcpy(En[*w_count], s);
        }
        //Greek Word.
        Gr[*w_count] = (char*) malloc(s_size*sizeof(char));
        if (Gr[*w_count] == NULL)
            printf("Could not allocate memory (2).\n");
        else
        {
            printf("Insert Greek Word: ");
            scanf("%s", s);
            //gets(s);
            Gr[*w_count] = (char*) malloc((strlen(s) + 1)*sizeof(char)); // !**!
            strcpy(Gr[*w_count], s);

        }

      

    }//else (1)

}