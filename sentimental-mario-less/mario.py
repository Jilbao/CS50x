# TODO

while True:
    try:
        # Getting User Input
        height = int(input("Height: "))
        # Control input between 0-9 if not skip to top
        if height < 1 or height > 8:
            continue
        # variable for creating empty-spaces works reversed
        x = height
        for i in range(height):
            x = x - 1
            # empty-spaces
            for dot in range(x):
                print(" ", end="")
            # blocks
            for j in range(i+1):
                print("#", end="")
            # lines
            print("")
        break
    except ValueError:
        print("Invalid Input!")
