#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

// Define a standard chunk size (4KB is highly optimized for OS disk sectors)
#define BUFFER_SIZE 4096

// Hashes an open file stream from its current position to EOF
uint64_t hash_file(FILE *file) {
    // 1. Initialize State (64-bit FNV constants)
    uint64_t hash_state = 0xCBF29CE484222325ULL;
    const uint64_t magic_prime = 0x00000100000001B3ULL;
    
    uint8_t buffer[BUFFER_SIZE];
    size_t bytes_read;

    // 2. The Streaming Loop
    // fread reads up to BUFFER_SIZE bytes at a time into our array
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            hash_state = hash_state ^ buffer[i];
            hash_state = hash_state * magic_prime;
        }
    }

    // 3. Finalizer Shift
    hash_state ^= (hash_state >> 32);
    
    return hash_state;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // "rb" is critical. On some OS layouts, "r" alters newline characters (\r\n -> \n),
    // which completely breaks binary integrity hashes.
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    uint64_t file_hash = hash_file(file);
    fclose(file);

    // Print out the final file hash exactly like a checksum utility
    printf("%016llX  %s\n", (unsigned long long)file_hash, argv[1]);

    return 0;
}
