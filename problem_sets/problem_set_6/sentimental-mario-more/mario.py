def main():
    while True:
        height = input("Height: ")
        if check_height(height):
            break
    print_pyramid(int(height))


def print_pyramid(height):
    for i in range(height):
        for _ in range(height - i - 1):
            print(" ", end="")
        for _ in range(i + 1):
            print("#", end="")
        for _ in range(2):
            print(" ", end="")
        for _ in range(i + 1):
            print("#", end="")
        print()


def check_height(height):
    try:
        height = int(height)
        if 1 <= height <= 8:
            return True
        else:
            return False
    except ValueError:
        return False


if __name__ == "__main__":
    main()
