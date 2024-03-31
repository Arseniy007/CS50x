#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Getting a the user's name
    string name = get_string("What's your name: ");
    // Printing "hello" and the name
    printf("hello, %s\n", name);
}