#include <cs50.h>
#include <stdio.h>


int get_start_size(void);
int get_end_size(int start);
int calculate_years(int start_size, int end_size);


int main(void)
{
    int start_size = get_start_size();
    int end_size = get_end_size(start_size);
    int years = calculate_years(start_size, end_size);
    // Printing the number of years
    printf("Years: %i\n", years);
}


// Promps user for start_size
int get_start_size(void)
{
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);
    return start;
}


// Promps user for end_size
int get_end_size(int start)
{
    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);
    return end;
}


// Calculate the number of years
int calculate_years(int start_size, int end_size)
{
    int year = 0;
    while (start_size < end_size)
    {
        start_size = start_size + start_size / 3 - start_size / 4;
        year++;
    }
    return year;
}