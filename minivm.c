/*
  VM specifications:
  - 256 byte memory (8-bit addressing)
  - program loaded to start at byte 16
  - the program terminates with a jump to 0

  Instruction set:
  |------+-------------+----------+-----------------------------|
  | Code | Instruction | Argument | Meaning                     |
  |------+-------------+----------+-----------------------------|
  |    0 | GET         | addr     | [0] <- [[addr]]             |
  |    1 | SET         | addr     | [addr] <- [0]               |
  |    2 | LIT         | data     | [0] <- data                 |
  |    3 | SUB         | addr     | [0] <- [addr] - [0]         |
  |    4 | CGT         | addr     | [0] <- 0 when [0] > [addr]  |
  |    5 | JNZ         | addr     | Jumps to `addr` if [0] != 0 |
  |    6 | INP         | -        | [0] <- input character      |
  |    7 | OUT         | -        | print [0]                   |
  |    8 | END         | -        | Terminates the program      |
  |------+-------------+----------+-----------------------------|

  Example: bytes 2,65,7,8 write the 'A' character.

 */

#include <stdio.h>

int main(int argc, char **argv) {
  unsigned char mem[256];
  FILE *f;
  if (argc != 2) {
    printf("Usage: %s <program>\n", argv[0]);
    return 1;
  }
  f = fopen(argv[1], "rb");
  if (!f) {
    printf("Cannot open file: %s\n", argv[1]);
    return 2;
  }
  fread(mem + 16, 1, 240, f);
  fclose(f);

  unsigned char ip = 16;
  while (mem[ip] != 8) {
    // printf("[%03d] %03d %03d\t[0]: %03d\n", ip, mem[ip], mem[ip+1], *mem);
    switch (mem[ip]) {
    case 0: mem[0] = mem[mem[mem[ip+1]]];        ip += 2; break;
    case 1: mem[mem[ip+1]] = mem[0];             ip += 2; break;
    case 2: mem[0] = mem[ip+1];                  ip += 2; break;
    case 3: mem[0] = mem[mem[ip+1]] - mem[0];    ip += 2; break;
    case 4: if (*mem > mem[mem[ip+1]]) *mem = 0; ip += 2; break;
    case 5: if (*mem) ip = mem[ip+1];    else    ip += 2; break;
    case 6: scanf("%c", mem);                    ip += 1; break;
    case 7: printf("%c", *mem);                  ip += 1; break;
    default:
      printf("Invalid opcode (%d) at address %d\n", mem[ip], ip);
    }
  }
}
