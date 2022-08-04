import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py DB_NAME SEQUENCE_NAME")
    # TODO: Read database file into a variable
    dnas = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for dna in reader:
            dnas.append(dna)

    # TODO: Read DNA sequence file into a variable

    with open(sys.argv[2]) as f:
        sq = f.read()
    # TODO: Find longest match of each STR in DNA sequence

    # Creating a list and add the name of the person for each match
    count = []
    for i in dnas:
        for j in i:
            if not j == "name":
                longmatch1 = longest_match(sq, j)
                if longmatch1 == int(i[j]):
                    count.append(i["name"])

    # TODO: Check database for matching profiles
    # Finding STR count inside dna database headers(-1 because first datacell is names )
    STR_count = len(dnas[0]) - 1

    # Small database
    if STR_count == 3:
        # matched is the person who have most matched DNA (name and frequency)
        matched_name = max(set(count), key=count.count)
        matched_freq = count.count(matched_name)
        # matched must have match all of the STR types
        if matched_freq == STR_count:
            print(matched_name)
        else:
            print("No match")
    # Large database
    elif STR_count == 8:
        # matched is the person who have most matched DNA (name) and (frequency)
        matched_name = max(set(count), key=count.count)
        matched_freq = count.count(matched_name)
        # matched must have match all of the STR types
        if matched_freq == STR_count:
            print(matched_name)
        else:
            print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
