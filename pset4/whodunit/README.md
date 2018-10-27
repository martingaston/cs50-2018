# Questions

## What's `stdint.h`?

A header file introduced in C99 which provides a set of typedefs for integer types.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

It helps specify clear data intentions (e.g. signed or unsigned) to a developer and ensure the definitions are equal across all platforms where they are defined.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

-   BYTE is an 8-bit (1 byte) unsigned value
-   DWORD is a 32-bit (4 byte) unsigned value
-   LONG is a 32-bit (4 byte) signed value in twos-complement format
-   WORD is a 16-bit (2 byte) unsigned value

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The magic numbers for a bitmap are `0x4d42` (or `BM` in ANSI)

## What's the difference between `bfSize` and `biSize`?

`biSize` is the size of the `BITMAPINFOHEADER` struct and equals 40 bytes. `bfSize` is the size of the whole bitmap file, including `BITMAPFILEHEADER` and `BITMAPINFOHEADER` (which total 54 bytes).

## What does it mean if `biHeight` is negative?

A positive `biHeight` means the bitmap is a bottom-up DIB (device independent bitmap) and its origin begins in the lower-left corner. A negative `biHeight` is a top-down DIB And its origin is the upper left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

`fopen` returns a FILE pointer upon success, and a `NULL` in case of any error. Essentially, a `NULL` return means that the file was unable to be opened.

## Why is the third argument to `fread` always `1` in our code?

The third argument of `fread` is the amount (`nmemb`) of elements to read. So we are reading `1` of each of our structs - `BITMAPFILEHEADER`, `BITMAPINFOHEADER` and `TRIPLE`

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

`padding` would equal `3`: 3 (biWidth) \* 3 (TRIPLE) + 3 (bytes of padding) to make the required multiple of 4. A good way to see the padding in action is to add options to xxd: `xxd -c 12 -g 3 -s 54 source.bmp`

## What does `fseek` do?

`fseek` sets the position of the `FILE` stream (1st arg) to a new offset (2nd arg) from the whence (3rd arg) position.

## What is `SEEK_CUR`?

`SEEK_CUR` is the current position of the pointer in the `FILE` stream

## Whodunit?

It was Professor Plum with the candlestick in the library.
