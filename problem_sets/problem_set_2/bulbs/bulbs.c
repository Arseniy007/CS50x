#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);


int main(void)
{
    // Get input from the user
    string text = get_string("Message: ");
    int length = strlen(text);

    for (int i = 0, l = length; i < l; i++)
    {
        // Change char into its ASCII decimal number
        int decimal_number = (int) text[i];

        // Create a byte array
        int byte[] = {0, 0, 0, 0, 0, 0, 0, 0};

        // Turn decimal into binary number and store it in our array
        for (int j = 7; decimal_number > 0; j--)
        {
            if (decimal_number % 2 == 1)
            {
                byte[j] = 1;
            }
            decimal_number /= 2;
        }

        // Print our byte bit by bit
        for (int k = 0, n = BITS_IN_BYTE; k < n; k++)
        {
            int bit = byte[k];
            print_bulb(bit);
        }
        printf("\n");
    }
}

// Prints the light bulbs
void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}