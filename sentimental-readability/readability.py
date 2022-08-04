from cs50 import get_string


def main():
    # Getting input
    text = get_string("Text: ")
    # Calculating letters,words and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    # Calculating Coleman-Liau index
    L = (letters / (words / 100.0))
    S = (sentences / (words / 100.0))
    index = 0.0588 * L - 0.296 * S - 15.8
    grade = round(index)
    # Posting grade with control
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def count_letters(text):
    count = 0
    for i in text:
        if i.isalpha():
            count += 1
    return count


def count_words(text):
    count = 1
    for i in text:
        if i.endswith(" "):
            count += 1
    return count


def count_sentences(text):
    count = 0
    for i in text:
        if i.endswith(".") or i.endswith("?") or i.endswith("!"):
            count += 1
    return count


if __name__ == "__main__":
    main()