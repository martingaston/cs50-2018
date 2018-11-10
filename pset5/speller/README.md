# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

It's a type of pneumoconiosis - lung disease - known as silicosis, caused by inhaling crystalline silica dust. It's also the longest word in the Oxford English Dictionary.

## According to its man page, what does `getrusage` do?

`getrusage` tracks system usage, such as memory, swaps and the CPU time used.

## Per that same man page, how many members are in a variable of type `struct rusage`?

The rusage struct returns 16 members, fourteen `long` members and two `timeeval` structs.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

Passing additional struct values around creates additional calls to the stack, increasing memory usage.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

After checking for correct arguments, loading the dictionary into memory and opening the text file to check, `main` creates a `for` loop that iterates over the text file character by character until it reaches the end of the file or reaches an error - the man page for `fgetc` details that it will return `EOF` in both of these instances.

`main` then checks three conditionals to parse the characters.

- The first `if` checks that the character being parsed is either an alphabetic character or an apostrophe, appending the character to the `word` array if true, iterating the `index` variable by 1. A nested `if` conditional then checks to see if `index` is longer than our allowed `LENGTH`, reading the rest of the word into the stream and setting `index` back to `0` if true.
- The loop then passes to the next `else if` statement, which checks to see if the character being pased is a number. If so, it continues until the end of the word and then resets `index` back to `0`
- If the loop reaches the next `else if` statement and the `index` variable is greater than `0`, `word` array is considered to contain a valid word. The word is terminated with a `\0`, the `words` integer is incremented by `1` and the program will then `check()` to see if the word is correctly spelt, printing it to `stdout` if it is not.

Once the EOF has been reached, the program checks the FILE for errors with `ferror` and exits if this is the case. If no errors are found, file is closed, the `size()` and `unload()` functions are executed and the benchmarks are displayed before a `0` is returned.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

Using `fscanf` to read in type `%s` will return a value that's only terminated by whitespace characters (blank, newline and tab) - which means all the punctuation, which we've said is not valid, would be read as valid.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

Because we don't want our `const` parameters to be modified :)
