def main():
    with open("Games.txt", "r") as file:
        lines = file.readlines()

    lines.sort()

    with open("Games_sorted.txt", "w") as file:
        file.write("".join(lines))


if __name__ == "__main__":
    main()
