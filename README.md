# Huffman Coding in C

This project implements the **Huffman Coding** algorithm in the C programming language. Huffman coding is a popular algorithm used for **lossless data compression**.

The program:
- Builds a Huffman tree based on character frequencies
- Generates binary Huffman codes
- Encodes a given input text using these codes
- Decodes the encoded binary string back into the original text
- Outputs relevant files with results

---

## File Descriptions

| File Name        | Description                                    |
|------------------|------------------------------------------------|
| `main.c`         | Source code implementing Huffman coding        |
| `input.txt`      | Input file containing text to compress         |
| `output.txt`     | Output file with Huffman codes and results     |
| `preorder.txt`   | Tree nodes listed in preorder traversal        |

---

##  How to Compile and Run

```bash
gcc main.c -o huffman
./huffman
```
## Algorithm Features
Frequency analysis of characters

Priority queue (min-heap) based tree construction

Encoding using binary strings

Recursive decoding of Huffman-coded string

Handles input length up to 5000 characters

Includes edge case handling for single-character input

## Sample Input (input.txt)
hello huffman
## Sample Output (output.txt)
Huffman kodlari :
h -> 00
u -> 010
f -> 011
m -> 100
a -> 101
n -> 110
e -> 1110
l -> 1111
...

Sifrelenen metin: 00 1110 1111 1111 100 ...

Sifresi cozulen metin: hello huffman

## Sample Preorder (preorder.txt)
 -> 14
h -> 2
f -> 3
u -> 2
...

## Notes
The program currently supports ASCII characters.

Does not support multibyte UTF-8 characters (e.g., 'ç', 'ğ', 'ş', etc.)

Memory is managed manually; all dynamic allocations are checked.
