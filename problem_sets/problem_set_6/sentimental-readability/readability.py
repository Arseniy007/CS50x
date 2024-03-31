def main():
    text = input("Text: ")

    # Calculate number of letters, words and sentences in text
    number_of_letters = count_letters(text)
    number_of_words = count_words(text)
    number_of_sentences = count_sentences(text)

    # Calculate number of letters and sentences per 100 words
    number_of_letters_per_100_words = float(number_of_letters / number_of_words * 100)
    number_of_sentences_per_100_words = float(number_of_sentences / number_of_words * 100)

    # Calculate the final index
    index = calculate_level(number_of_letters_per_100_words, number_of_sentences_per_100_words)

    # Print the result
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print("Grade", index)


def count_letters(text):
    number_of_letters = 0
    for letter in text:
        if letter.isalpha():
            number_of_letters += 1
    return number_of_letters


def count_words(text):
    words = text.split(" ")
    number_of_words = len(words)
    return number_of_words


def count_sentences(text):
    number_of_sentences = 0
    for letter in text:
        if letter in [".", "?", "!"]:
            number_of_sentences += 1
    return number_of_sentences


def calculate_level(number_of_letters_per_100_words, number_of_sentences_per_100_words):
    index = round(0.0588 * number_of_letters_per_100_words - 0.296 * number_of_sentences_per_100_words - 15.8)
    return index


if __name__ == "__main__":
    main()
