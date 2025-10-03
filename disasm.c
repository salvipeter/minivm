// Disassembler written by Claude Sonnet 4.5 (with some corrections)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 256
#define LOAD_OFFSET 16

int labels[MAX_SIZE];
int label_count = 0;

const char *opcodes[] = {
    "LIT", "GET", "SET", "PUT", "SUB", "CGT", "JNZ", "INP", "OUT", "END"
};

int has_arg[] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0};

void add_label(int addr) {
    for (int i = 0; i < label_count; i++) {
        if (labels[i] == addr)
            return;
    }
    labels[label_count] = addr;
    label_count++;
}

int is_printable_str(unsigned char *data, int start, int len, int max_len) {
    if (start + len > max_len) return 0;
    int printable = 0;
    for (int i = start; i < start + len && i < max_len; i++) {
        if (data[i] == 0) break;
        if (isprint(data[i])) {
            printable++;
        } else {
            return 0;
        }
    }
    return printable >= 3; // At least 3 printable chars
}

char *get_label_name(int addr) {
    static char buf[32];
    for (int i = 0; i < label_count; i++) {
        if (labels[i] == addr) {
            sprintf(buf, "line_%03d", addr);
            return buf;
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    unsigned char data[MAX_SIZE];
    int size = fread(data, 1, MAX_SIZE, f);
    fclose(f);

    // First pass: identify jump targets
    int data_started = 0;
    for (int pc = 0; pc < size; ) {
        int opcode = data[pc];
        if (opcode < 10)
            data_started = 0;
        else {
            if (!data_started)
                add_label(LOAD_OFFSET + pc);
            data_started = 1;
        }
        if (opcode == 6) { // JNZ
            if (pc + 1 < size) {
                int target = data[pc + 1];
                // Only add label if target is within program bounds
                if (target >= LOAD_OFFSET && target < LOAD_OFFSET + size) {
                    add_label(target);
                }
            }
        }
        if (opcode < 10 && has_arg[opcode]) {
            pc += 2;
        } else if (opcode < 10) {
            pc += 1;
        } else {
            pc += 1;
        }
    }

    // Second pass: disassemble
    printf("; Disassembled output\n");
    for (int pc = 0; pc < size; ) {
        int opcode = data[pc];
        // Check if this address has a label
        char *label = get_label_name(pc + LOAD_OFFSET);
        if (label) {
          printf("%s\n", label);
        }
        
        if (opcode < 10) {
            printf("%s", opcodes[opcode]);
            
            if (has_arg[opcode]) {
                if (pc + 1 < size) {
                    int arg = data[pc + 1];
                    
                    // For JNZ, use label if it exists
                    if (opcode == 6) {
                        char *target = get_label_name(arg);
                        if (target) {
                            printf(" %s", target);
                        } else {
                            printf(" %d", arg);
                        }
                    } else {
                        printf(" %d", arg);
                    }
                    
                    // Add comment for LIT with printable characters
                    if (opcode == 0 && isprint(arg)) {
                        printf(" ; '%c'", arg);
                    }
                }
                pc += 2;
            } else {
                pc += 1;
            }
            printf("\n");
        } else {
            // Non-instruction: check if it's a string or data
            int str_len = 0;
            if (is_printable_str(data, pc, 4, size)) {
                // Find string length
                while (pc + str_len < size && data[pc + str_len] != 0 && 
                       isprint(data[pc + str_len])) {
                    str_len++;
                }
                
                printf("STR ");
                for (int i = 0; i < str_len && pc + i < size; i++) {
                    putchar(data[pc + i]);
                }
                printf("\n");
                pc += str_len;
            }
            
            // Output remaining data as DAT (including null terminators, non-printables, etc.)
            if (pc < size && (data[pc] < 10 && has_arg[data[pc]])) {
                // Looks like an instruction, stop here
            } else if (pc < size && data[pc] >= 10) {
                // Output as DAT
                printf("DAT");
                int count = 0;
                while (pc < size && count < 16) {
                    // Stop if we hit what looks like an instruction and has a label
                    if (data[pc] < 10 && get_label_name(pc + LOAD_OFFSET)) break;
                    printf(" %d", data[pc]);
                    pc++;
                    count++;
                }
                printf("\n");
            } else {
                pc++;
            }
        }
    }

    return 0;
}
