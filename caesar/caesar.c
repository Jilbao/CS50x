#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    //Declarations
    char rotate(char c, int n);
    bool only_digits(string s);

    //Get key lenght
    int keyLen = argc;

    //lack of argv[1]
    if (keyLen != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    //Get key as string
    string key = argv[1];

    //Convert key to int
    int keyInt = atoi(key);



    //Only digits check
    if (only_digits(key) != true)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Get user input as plaintext
    string plaintxt = get_string("plaintext: ");

    //Cipher text loop
    printf("ciphertext: ");
    for (int i = 0; i < strlen(plaintxt); i++)
    {
        printf("%c", rotate(plaintxt[i], keyInt));
    }
    printf("\n");

}

bool only_digits(string s)
{
    bool r = false;
    for (int i = 0; i <  strlen(s); i++)
    {
        r = isdigit(s[i]);
    }
    return r;
}
char rotate(char c, int n)
{
    char new = 0;
    int wrap = n % 26;

    //For alphabetic chars
    if (isalpha(c) != 0)
    {
        // For lower chars
        if (islower(c))
        {
            int a = c - 97;
            new = ((a + wrap) % 26) + 97;
        }
        // For upper chars
        if (isupper(c))
        {
            int a = c - 65;
            new = ((a + wrap) % 26) + 65;
        }

    }
    //For non-alphabetic chars
    if (isalpha(c) == 0)
    {
        new = c;
    }
    return new;
}