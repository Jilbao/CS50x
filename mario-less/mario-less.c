#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    string dot = " ";
    int dot_count;
    do
    {
        n = get_int("Height :");
    }
    while (n < 1 || n > 8);
    {
        //rows
        for (int i = 1; i <= n; i++)
        {
            //dots or spaces. Iteration equals to input
            for (int j = n; j > i; j--)
            {
                printf("%s", dot);
            }
            //blocks
            for (int j = 0; j < i; j++)
            {
                printf("#");
            }
            printf("\n");
        }
    }




}