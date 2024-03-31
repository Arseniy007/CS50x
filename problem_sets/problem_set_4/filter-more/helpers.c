#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];

            // Calculate the average of all the colors and assign it to each color
            int average = round((pixel.rgbtBlue + pixel.rgbtRed + pixel.rgbtGreen) / 3.0);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];

            // Calculate using formula the "sepia" version of each color
            int sepia_red = round(.393 * pixel.rgbtRed + .769 * pixel.rgbtGreen + .189 * pixel.rgbtBlue);
            int sepia_green = round(.349 * pixel.rgbtRed + .686 * pixel.rgbtGreen + .168 * pixel.rgbtBlue);
            int sepia_blue = round(.272 * pixel.rgbtRed + .534 * pixel.rgbtGreen + .131 * pixel.rgbtBlue);

            int *colors[] = {&sepia_red, &sepia_green, &sepia_blue};
            int number_of_colors = 3;

            // Check if the the each result integer is not greater than 255
            for (int k = 0; k < number_of_colors; k++)
            {
                if (*colors[k] > 255)
                {
                    *colors[k] = 255;
                }
            }

            image[i][j].rgbtRed = sepia_red;
            image[i][j].rgbtGreen = sepia_green;
            image[i][j].rgbtBlue = sepia_blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width / 2; j < n; j++)
        {
            RGBTRIPLE *pixel_1 = &image[i][j];
            RGBTRIPLE *pixel_2 = &image[i][width - j - 1];

            RGBTRIPLE pixel_temp = *pixel_1;
            *pixel_1 = *pixel_2;
            *pixel_2 = pixel_temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Blur each pixel in the copy image and then change the pixel in the originale image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = copy[i][j];

            int sum_of_neighbors_red = 0;
            int sum_of_neighbors_green = 0;
            int sum_of_neighbors_blue = 0;
            int number_of_neighbors = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int m = -1; m < 2; m++)
                {
                    // Check if the pixel is on the edge of the image and therefore does not have a neighbor
                    if (i + k >= 0 && i + k <= height - 1 && j + m >= 0 && j + m <= width - 1)
                    {
                        number_of_neighbors++;

                        sum_of_neighbors_red += copy[i + k][j + m].rgbtRed;
                        sum_of_neighbors_green += copy[i + k][j + m].rgbtGreen;
                        sum_of_neighbors_blue += copy[i + k][j + m].rgbtBlue;
                    }
                }
            }

            image[i][j].rgbtRed = round(sum_of_neighbors_red / (float) number_of_neighbors);
            image[i][j].rgbtGreen = round(sum_of_neighbors_green / (float) number_of_neighbors);
            image[i][j].rgbtBlue = round(sum_of_neighbors_blue / (float) number_of_neighbors);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Iterate through each pixel in the "copy" of the image and calculate using "Sobel operator" its new color value
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = copy[i][j];

            int sum_of_neighbors_red_Gx = 0;
            int sum_of_neighbors_green_Gx = 0;
            int sum_of_neighbors_blue_Gx = 0;

            int sum_of_neighbors_red_Gy = 0;
            int sum_of_neighbors_green_Gy = 0;
            int sum_of_neighbors_blue_Gy = 0;

            // Iterate through each neighbor of the pixel and calculate its Gx and Gy color value
            for (int k = -1; k < 2; k++)
            {
                for (int m = -1; m < 2; m++)
                {
                    if (i + k >= 0 && i + k <= height - 1 && j + m >= 0 && j + m <= width - 1)
                    {
                        int value_of_red = copy[i + k][j + m].rgbtRed;
                        int value_of_green = copy[i + k][j + m].rgbtGreen;
                        int value_of_blue = copy[i + k][j + m].rgbtBlue;

                        sum_of_neighbors_red_Gx += value_of_red * Gx[k + 1][m + 1];
                        sum_of_neighbors_green_Gx += value_of_green * Gx[k + 1][m + 1];
                        sum_of_neighbors_blue_Gx += value_of_blue * Gx[k + 1][m + 1];

                        sum_of_neighbors_red_Gy += value_of_red * Gy[k + 1][m + 1];
                        sum_of_neighbors_green_Gy += value_of_green * Gy[k + 1][m + 1];
                        sum_of_neighbors_blue_Gy += value_of_blue * Gy[k + 1][m + 1];
                    }
                }
            }

            // Calculate the result value for each color
            int result_red = round(sqrt(pow(sum_of_neighbors_red_Gx, 2) + pow(sum_of_neighbors_red_Gy, 2)));
            int result_green = round(sqrt(pow(sum_of_neighbors_green_Gx, 2) + pow(sum_of_neighbors_green_Gy, 2)));
            int result_blue = round(sqrt(pow(sum_of_neighbors_blue_Gx, 2) + pow(sum_of_neighbors_blue_Gy, 2)));

            // Check if the each result integer is not greater than 255
            int *result_colors[] = {&result_red, &result_green, &result_blue};
            int number_of_colors = 3;

            for (int color = 0; color < number_of_colors; color++)
            {
                if (*result_colors[color] > 255)
                {
                    *result_colors[color] = 255;
                }
            }

            // Assigning result color  values to our pixel
            image[i][j].rgbtRed = result_red;
            image[i][j].rgbtGreen = result_green;
            image[i][j].rgbtBlue = result_blue;
        }
    }
    return;
}
