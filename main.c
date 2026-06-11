#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

// 64-bit Lightweight Hash Function
uint64_t lightweight_hash64(const void *data, size_t length) {
    // 1. Initialize State: Official 64-bit FNV offset basis
    uint64_t hash_state = 0xCBF29CE484222325ULL;
    
    // Official 64-bit magic prime constant for bit dispersion
    const uint64_t magic_prime = 0x00000100000001B3ULL;
    
    const uint8_t *byte_ptr = (const uint8_t *)data;

    // 2. The Mixing Loop
    for (size_t i = 0; i < length; i++) {
        // XOR the input byte into the lower bits of the 64-bit state
        hash_state = hash_state ^ byte_ptr[i];
        
        // Multiply by the 64-bit prime to overflow and distribute across all 64 bits
        hash_state = hash_state * magic_prime;
    }

    // 3. Finalizer: A wider shift cascade to blend the high and low halves
    hash_state ^= (hash_state >> 32);
    
    return hash_state;
}

int main() {
    const char *str1 = "block_type_grass";
    
    uint64_t hash = lightweight_hash64(str1, 16);

    // Print with %llX or %lX depending on platform, PRIu64 is standard, 
    // but 0x%016llX forces a clean 16-character hex print.
    printf("String: '%s'\nHash: 0x%016llX\n", str1, (unsigned long long)hash);

    return 0;
}
