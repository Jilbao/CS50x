# TODO
from cs50 import get_float



def main():
    while True:
        money = get_float("Change owed: ")
        try:
            if money < 0:
                print("enter positive number")
                continue
            money = money * 100
            q = 0
            d = 0
            n = 0
            p = 0
            if money >= 25:
                q = int(calculate_quarters(money))
                money = money - q * 25
            if money >= 10:
                d = int(calculate_dimes(money))
                money = money - d * 10
            if money >= 5:
                n = int(calculate_nickels(money))
                money = money - n * 5
            if money >= 1:
                p = int(calculate_pennies(money))
                money = money - p * 1
            coins = q + d + n + p
            print(coins)
            break
        except ValueError:
            # doesn't work, probably because get_float function
            print("invalid input")


def calculate_quarters(money):
    q = int(money / 25)
    return q


def calculate_dimes(money):
    d = int(money / 10)
    return d


def calculate_nickels(money):
    n = int(money / 5)
    return n


def calculate_pennies(money):
    p = int(money / 1)
    return p


if __name__ == "__main__":
    main()