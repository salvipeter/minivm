/*
  VM specifications:
  - 256 byte memory (8-bit addressing)
  - program loaded to start at byte 16
    (bytes 1-15 can be used for communication between programs)

  Instruction set:
  |------+-------------+----------+-----------------------------|
  | Code | Instruction | Argument | Meaning                     |
  |------+-------------+----------+-----------------------------|
  |    0 | LIT         | data     | [0] <- data                 |
  |    1 | GET         | addr     | [0] <- [[addr]]             |
  |    2 | SET         | addr     | [addr] <- [0]               |
  |    3 | PUT         | addr     | [[addr]] <- [0]             |
  |    4 | SUB         | addr     | [0] <- [addr] - [0]         |
  |    5 | CGT         | addr     | [0] <- 0 when [0] > [addr]  |
  |    6 | JNZ         | addr     | Jumps to `addr` if [0] != 0 |
  |    7 | INP         | -        | [0] <- input character      |
  |    8 | OUT         | -        | print [0]                   |
  |    9 | END         | -        | Terminates the program      |
  |------+-------------+----------+-----------------------------|

  Example: bytes 0,65,8,9 write the 'A' character.

 */

#include <stdio.h>

int main(int argc, char **argv) {
  unsigned char mem[256];
  FILE *f;
  if (argc < 2) {
    printf("Usage: %s <program>...\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    f = fopen(argv[i], "rb");
    if (!f) {
      printf("Cannot open file: %s\n", argv[1]);
      return 2;
    }
    fread(mem + 16, 1, 240, f);
    fclose(f);

    unsigned char ip = 16;
    while (mem[ip] != 9) {
      // printf("[%03d] %03d %03d\t[0]: %03d\n", ip, mem[ip], mem[ip+1], *mem);
      switch (mem[ip]) {
      case 0: mem[0] = mem[ip+1];                  ip += 2; break;
      case 1: mem[0] = mem[mem[mem[ip+1]]];        ip += 2; break;
      case 2: mem[mem[ip+1]] = mem[0];             ip += 2; break;
      case 3: mem[mem[mem[ip+1]]] = mem[0];        ip += 2; break;
      case 4: mem[0] = mem[mem[ip+1]] - mem[0];    ip += 2; break;
      case 5: if (*mem > mem[mem[ip+1]]) *mem = 0; ip += 2; break;
      case 6: if (*mem) ip = mem[ip+1];    else    ip += 2; break;
      case 7: scanf("%c", mem);                    ip += 1; break;
      case 8: printf("%c", *mem);                  ip += 1; break;
      default:
        printf("Invalid opcode (%d) at address %d\n", mem[ip], ip);
        return 3;
      }
    }
  }
}
