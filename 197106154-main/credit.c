#include <stdio.h>
#include <cs50.h>

bool check_luhn(long number);
void check_card_type(long number);

int main(void)
{
    long number = get_long("Number: ");

    if (check_luhn(number))
    {
        check_card_type(number);
    }
    else
    {
        printf("INVALID\n");
    }
}

bool check_luhn(long number)
{
    int sum = 0;
    int count = 0;

    while (number > 0)
    {
        int digit = number % 10;

        if (count % 2 == 0)
        {
            sum += digit;
        }
        else
        {
            int doubled = digit * 2;
            sum += (doubled / 10) + (doubled % 10);
        }

        number /= 10;
        count++;
    }

    return (sum % 10 == 0);
}

void check_card_type(long number)
{
    if ((number >= 340000000000000 && number < 350000000000000) || (number >= 370000000000000 && number < 380000000000000))
    {
        printf("AMEX\n");
    }
    else if (number >= 5100000000000000 && number < 5600000000000000)
    {
        printf("MASTERCARD\n");
    }
    else if ((number >= 4000000000000 && number < 5000000000000) || (number >= 4000000000000000 && number < 5000000000000000))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
