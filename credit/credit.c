#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>



int main(void)
{
    long cc_number = get_long("Number: ");
    //Getting Digits
    int d1 = cc_number % 10;
    int d2 = (cc_number / 10) % 10;
    int d3 = (cc_number / 100) % 10;
    int d4 = (cc_number / 1000) % 10;
    int d5 = (cc_number / 10000) % 10;
    int d6 = (cc_number / 100000) % 10;
    int d7 = (cc_number / 1000000) % 10;
    int d8 = (cc_number / 10000000) % 10;
    int d9 = (cc_number / 100000000) % 10;
    int d10 = (cc_number / 1000000000) % 10;
    int d11 = (cc_number / 10000000000) % 10;
    int d12 = (cc_number / 100000000000) % 10;
    int d13 = (cc_number / 1000000000000) % 10;
    int d14 = (cc_number / 10000000000000) % 10;
    int d15 = (cc_number / 100000000000000) % 10;
    int d16 = (cc_number / 1000000000000000) % 10;
    //Finding lenght of number
    int lenght = 0;
    do
    {
        cc_number = cc_number / 10;
        ++lenght;
    }
    while (cc_number != 0);
    {

    }
    //Early lenght control for Invalid inputs
    if (lenght < 12 || lenght > 16)
    {
        printf("INVALID\n");
        exit(0);
    }

    //Multiply even digits and seperate them to their digits again to sum
    int multi2sum = ((d2 * 2 ) % 10 + ((d2*2)/10) % 10) + ((d4*2) % 10 + ((d4*2)/10) % 10) + ((d6*2) % 10 + ((d6*2)/10) % 10) + ((d8*2) % 10 + ((d8*2)/10) % 10) + ((d10*2) % 10 + ((d10*2)/10) % 10) + ((d12*2) % 10 + ((d12*2)/10) % 10) + ((d14*2) % 10 + ((d14*2)/10) % 10) + ((d16*2) % 10 + ((d16*2)/10) % 10);

    //Sum odd digits with sum of prior process
    int oddpluseven = multi2sum + d1 + d3 + d5 + d7 + d9 + d11 + d13 + d15;


    //Control valid or invalid via checking last digit
    if ((oddpluseven % 10) == 0)
    {

        //Classify Card type after confirming valid
        if (d16 == 4 || d13 == 4)
        {
            printf("VISA\n");
        }
        else if (d15 == 3 && (d14 == 4 || d14 == 7))
        {
            printf("AMEX\n");
        }
        else if (d16 == 5 && (d15 == 1 || d15 == 2 || d15 == 3 || d15 == 4 || d15 == 5))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}