# Task 3: Humanizer
## Objective
Your mission is to humanize an AI generated piece of text. You are going to accomplish this by replacing every word in the text file with their synonyms.

## Scope of the task
`main()` function calls an empty `humanize_file()` function from the [humanize.h](humanize.h) module to achieve the objective of the task.

You must implement a working `humanize_file()` function for the [humanize.h](humanize.h) module. You are only allowed to call either `get_synonym()` function by including [synonym.h](synonym.h) or standard C library functions.

Notes:
1. Possible return codes of `get_synonym()` are explained in a comment inside of [synonym.h](synonym.h). You need to take care of all possible outputs for that function and increment `words_humanized` if necessary.
2. You might want to change the `FILE_PATH` macro inside of [main.c](main.c) so it matches your system's actual directory.
3. The `get_synonym()` function already ignores primitive words with no lexical meaning so you are safe to feed any word into that function.

## Example humanizing
- `ai.txt`: "Science helps us understand how the world works, from tiny atoms to distant galaxies. New discoveries can change what we know and lead to technologies that improve everyday life."
- `human.txt`: "skill helps america infer however the existence plant, from diminutive atoms to aloof galaxies. radical discoveries stern shift what we bang and conduct to technologies that ameliorate mundane spirit."
