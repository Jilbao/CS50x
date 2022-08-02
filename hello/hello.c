#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //User Input
    string name = get_string("Enter your name: ");
    //Print
    printf("hello, %s\n", name);
}