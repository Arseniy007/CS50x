#include <cs50.h>
#include <stdio.h>

int get_the_height(void);
void print_pyramid(int height);


int main(void)
{
    int height = get_the_height();
    print_pyramid(height);
}


// Prompting the user for height
int get_the_height(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    return height;
}


// Printing the pyramid
void print_pyramid(int height)
{
    for (int i = 1; i <= height; i++)
    {
        for (int j = i; j < height; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}