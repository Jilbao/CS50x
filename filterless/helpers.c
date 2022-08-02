#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int averageRGB = round(((image[i][j].rgbtBlue) + (image[i][j].rgbtGreen) + (image[i][j].rgbtRed)) / 3.0);
            image[i][j].rgbtBlue = averageRGB;
            image[i][j].rgbtGreen = averageRGB;
            image[i][j].rgbtRed = averageRGB;

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
            int sepiaRed = round(((image[i][j].rgbtBlue) * 0.189) + ((image[i][j].rgbtGreen) * 0.769) + ((image[i][j].rgbtRed) * 0.393));
            int sepiaGreen = round(((image[i][j].rgbtBlue) * 0.168) + ((image[i][j].rgbtGreen) * 0.686) + ((image[i][j].rgbtRed) * 0.349));
            int sepiaBlue = round(((image[i][j].rgbtBlue) * 0.131) + ((image[i][j].rgbtGreen) * 0.534) + ((image[i][j].rgbtRed) * 0.272));
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = sepiaRed - (sepiaRed % 255);
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }
            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = sepiaGreen - (sepiaGreen % 255);
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = sepiaBlue - (sepiaBlue % 255) ;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    int a[height][width][3];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            a[i][j][2] = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][(width - 1) - j].rgbtBlue;
            image[i][(width - 1) - j].rgbtBlue = a[i][j][2];

            a[i][j][1] = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][(width - 1) - j].rgbtGreen;
            image[i][(width - 1) - j].rgbtGreen = a[i][j][1];

            a[i][j][0] = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][(width - 1) - j].rgbtRed;
            image[i][(width - 1) - j].rgbtRed = a[i][j][0];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // CORNERS (4 Pixels of Grid)
            // Top-Left
            if (i == 0 && j == 0)
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) /
                                           4.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j +
                                              1].rgbtGreen) / 4.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j +
                                             1].rgbtBlue) / 4.0);

            }
            // Top-Right
            else if (i == 0 && j == (width - 1))
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed) /
                                           4.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j -
                                              1].rgbtGreen) / 4.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j -
                                             1].rgbtBlue) / 4.0);
            }
            // Bottom-Left
            else if (i == (height - 1) && j == 0)
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed) /
                                           4.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j +
                                              1].rgbtGreen) / 4.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j +
                                             1].rgbtBlue) / 4.0);
            }
            //Bottom-Right
            else if (i == (height - 1) && (j == width - 1))
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j - 1].rgbtRed) /
                                           4.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j -
                                              1].rgbtGreen) / 4.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j -
                                             1].rgbtBlue) / 4.0);
            }
            // SIDES (6 Pixels of Grid)
            // Top
            else if ((i == 0 && j > 0) && (i == 0 && j < (width - 1)))
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed +
                                            image[i + 1][j - 1].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i +
                                              1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue +
                                             image[i + 1][j - 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6.0);
            }
            // Left
            else if ((j == 0 && i > 0) && (j == 0 && i < (height - 1)))
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j + 1].rgbtRed +
                                            image[i + 1][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed) / 6.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j +
                                              1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen) / 6.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                             image[i + 1][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue) / 6.0);
            }
            // Right
            else if ((j == (width - 1) && i > 0) && (j == (width - 1) && i < (height - 1)))
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                                            image[i + 1][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed) / 6.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j -
                                              1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen) / 6.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                             image[i + 1][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue) / 6.0);
            }
            // Bottom
            else if ((i == (height - 1) && j > 0) && (i == (height - 1) && j < (width - 1)))
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j + 1].rgbtRed + image[i - 1][j].rgbtRed +
                                            image[i - 1][j - 1].rgbtRed + image[i - 1][j + 1].rgbtRed) / 6.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i -
                                              1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen) / 6.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i - 1][j].rgbtBlue +
                                             image[i - 1][j - 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue) / 6.0);
            }
            // Rest of the pixels (9 Pixels of Grid)
            else
            {
                copy[i][j].rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j + 1].rgbtRed + image[i - 1][j].rgbtRed +
                                            image[i - 1][j - 1].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j +
                                                    1].rgbtRed) / 9.0);
                copy[i][j].rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i -
                                              1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j -
                                                      1].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9.0);
                copy[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i - 1][j].rgbtBlue +
                                             image[i - 1][j - 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i +
                                                     1][j + 1].rgbtBlue) / 9.0);
            }

        }
    }
    // Copy "copy" image's values to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
        }
    }
    return;
}
