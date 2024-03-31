#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

float calc_hours(int hours[], int weeks, char output);

int main(void)
{
    int weeks = get_int("Number of weeks taking CS50: ");
    int hours[weeks];

    for (int i = 0; i < weeks; i++)
    {
        hours[i] = get_int("Week %i HW Hours: ", i);
    }

    char output;
    do
    {
        output = toupper(get_char("Enter T for total hours, A for average hours per week: "));
    }
    while (output != 'T' && output != 'A');

    printf("%.1f hours\n", calc_hours(hours, weeks, output));
}

// Calculates the average or total of hours
float calc_hours(int hours[], int weeks, char output)
{
    float hours_total = 0;
    for (int i = 0, n = weeks; i < n; i++)
    {
        hours_total += hours[i];
    }
    if (output == 'T')
    {
        return hours_total;
    }
    else
    {
        float hours_average = hours_total / weeks;
        return hours_average;
    }
}