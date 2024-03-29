
#include "tools.h"
#include <stdlib.h>
#include <ctype.h>

const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const double letter_freq[] = {
    0.08167,  // Frequency of 'a'
    0.01492,  // Frequency of 'b'
    0.02782,  // Frequency of 'c'
    0.04253,  // Frequency of 'd'
    0.12702,  // Frequency of 'e'
    0.02228,  // Frequency of 'f'
    0.02015,  // Frequency of 'g'
    0.06094,  // Frequency of 'h'
    0.06966,  // Frequency of 'i'
    0.00153,  // Frequency of 'j'
    0.00772,  // Frequency of 'k'
    0.04025,  // Frequency of 'l'
    0.02406,  // Frequency of 'm'
    0.06749,  // Frequency of 'n'
    0.07507,  // Frequency of 'o'
    0.01929,  // Frequency of 'p'
    0.00095,  // Frequency of 'q'
    0.05987,  // Frequency of 'r'
    0.06327,  // Frequency of 's'
    0.09056,  // Frequency of 't'
    0.02758,  // Frequency of 'u'
    0.00978,  // Frequency of 'v'
    0.02360,  // Frequency of 'w'
    0.00150,  // Frequency of 'x'
    0.01974,  // Frequency of 'y'
    0.00074   // Frequency of 'z'
};
double score_text(char *text, int len){
    double score = 0;
    int letter_index = 0; // used to get frequency from the above array, a = 0, b = 1 ... 
    for(int i = 0; i < len; i++){
        letter_index = text[i] - 97; // a's ASCII value is 97
        if(letter_index < 0 || letter_index > 25){ 
            continue;
        }
        score += letter_freq[letter_index];
    }
    return score;
}

int hex_to_bytes_conv(const char *input, int input_size, char *output){
    // if(IS_ODD(input_size)){
    //     // first byte
    //     int i = 0;
    //     output[i] = 0; 
    //     output[i] |= (input[i] - '0');

    //     for(i = 1; i < input_size; i += 2){
    //         sscanf(&input[i], "%2hhx", &output[i / 2 + 1]);
    //     }
    //     return (input_size / 2 + 1);
    // }
    if(IS_ODD(input_size)){
        int i = 0;
        for(; i < input_size - 1; i += 2){
            sscanf(&input[i], "%2hhx", &output[i / 2]);
        }
        sscanf(&input[i], "%2hhx", &output[i / 2]);
        return (input_size / 2 + 1);
    }
    
    for(int i = 0; i < input_size; i += 2)
        sscanf(&input[i], "%2hhx", &output[i / 2]);

    return input_size / 2;
}

int init_int24(uint8_t *a, int24 *num){
    int counter  = 0;
    for(int i = 0; i < 3; i++){
        num->data |= (a[i] << (i * 8));
        printf("nums = %d\n", num->data);
        counter++;
    }
    return 1;
}

int three_bytes_to_base64(int24 *num, char *output){
    for(int i = 0; i < 4; i++){
        output[i] = (num->data >> (6 * i)) & 0b00111111;
    }
    return strlen(output); 
}

int read_b64(const char *bytes, int len, char *output){
    unsigned char input_buffer, output_buffer, leftover_bits;
    input_buffer = bytes[0];
    leftover_bits = 8; // before processing the first byte
    int i = 0;
    
    int output_counter = 0;
    
    while(i < len){
        output_buffer = 0;
        // take the first 6 bits and OR it onto output_buffer, then left shift output_buffer
        // by 2 bits so output_buffer <= 63, 2 lower bits left in input_buffer after this point
        // not incrementing index due to the 2 remaining bits in the byte
        if(leftover_bits == 8){
            output_buffer = input_buffer & 0xFC; // 0xFC = 0b11111100
            output_buffer >>= 2;
            leftover_bits = 2;
        }
        // take the 2 remaining bits, then move the index by 1, and then grab 4 bits and OR
        // them onto output_buffer and place them right after the 2 bits, 4 bits remaining
        else if(leftover_bits == 2){
            output_buffer = input_buffer << 6;
            i++;
            if(i >= len) break;
            input_buffer = bytes[i];
            output_buffer |= input_buffer >> 2 & 0x3C; // 0x3C = 00111100
            output_buffer >>= 2;
            leftover_bits = 4;
        }
        // take the 4 remaining bits, then move the index by 1, then grab 2 bits and OR
        // them onto output_buffer, same as above
        else if(leftover_bits == 4){
            output_buffer = input_buffer << 4;
            i++;
            if(i >= len) break;
            input_buffer = bytes[i];
            output_buffer |= input_buffer >> 4 & 0x0C; // 0x0C = 0b00001100
            output_buffer >>= 2;
            leftover_bits = 6;
        }
        // take 6 remaining bits, not moving index because one of the above condition will
        // increment it
        else if(leftover_bits == 6){
            output_buffer = input_buffer << 2;
            output_buffer >>= 2;
            leftover_bits = 0;
        }
        else if(leftover_bits == 0){
            i++;
            if(i >= len) break;
            input_buffer = bytes[i];
            output_buffer = bytes[i] & 0xFC;
            output_buffer >>= 2;
            leftover_bits = 2;
        }
        output[output_counter] = base64_chars[output_buffer];
        output_counter++;
    }
    return output_counter;
}

void print_hex(char *hex_str, int len){
    for(int i = 0; i < len; i++)
        printf("%02x", hex_str[i]);
    printf("\n");
}

void printBinary(int number) {
    unsigned int mask = 1 << (sizeof(int) * 8 - 1); // Create a mask with a 1 in the leftmost position

    for (size_t i = 0; i < sizeof(int) * 8; ++i) {
        putchar((number & mask) ? '1' : '0'); // Check if the current bit is 1 or 0
        mask >>= 1; // Move the mask one position to the right
    }
    printf("\n");
}

void buffers_xor(const char *a, const char *b, int len, char *output){
    for(int i = 0; i < len; i++){
        output[i] = a[i] ^ b[i];
        // printf("a[%d] = %c b[%d] = %c, output[%d] = %c\n", i, a[i], i, b[i], i, output[i]);
    }
}

// void print_bytes_array(const unsigned char *bytes, unsigned int size_byte, char *str){
//     printf("%s\n", str);
//     uint16_t bytes_counter = 0;
//     int num_of_lines = 0;
//     int num_bytes_to_print = 0;
//     int remainder = size_byte % 32;
//     if(remainder == 0){
//         num_of_lines = size_byte / 32;
//     } else{
//         num_of_lines = size_byte / 32 + 1;
//     }
//     for(int i = 0; i < num_of_lines; i++){
//         printf("%04u     ", bytes_counter);
//         if(i == num_of_lines - 1 && remainder != 0){ 
//             num_bytes_to_print = remainder;
//         } else{
//             num_bytes_to_print = 32;
//         }
//         // printing the hex value 
//         for(int j = 0; j < num_bytes_to_print; j++){
//             printf("%02X ", bytes[(i * num_bytes_to_print) + j]);
//             if(j == 15){
//                 printf(" ");
//             }
//         }
//         printf(" ");
//         // upon reaching last line, add padding to hex values so that all characters will align
//         if(i == num_of_lines - 1){
//             int padding = 32 - num_bytes_to_print;
//             for (int j = 0; j < padding; j++) {
//                 printf("   ");
//             }
//         }
//         printf(" ");
//         // printing characters
//         for (int j = 0; j < num_bytes_to_print; j++) {
//             uint8_t value = bytes[i * 32 + j];
//             if (isprint(value)) {
//                 printf("%c", value);
//             } else {
//                 printf(".");
//             }
//         }
//         printf("\n");
//         bytes_counter += num_bytes_to_print;
//     }
// }

void print_bytes_array(const unsigned char *bytes, unsigned int size_byte, char *str) {
    printf("%s\n", str);
    uint16_t bytes_counter = 0;
    int num_of_lines = (size_byte + 31) / 32;  // Simplified computation for number of lines

    for(int i = 0; i < num_of_lines; i++) {
        printf("%04u     ", bytes_counter);
        int line_limit = (i == num_of_lines - 1 && size_byte % 32 != 0) ? size_byte % 32 : 32;

        // printing the hex value 
        for(int j = 0; j < line_limit; j++) {
            printf("%02X ", bytes[i * 32 + j]);
            if(j == 15) {
                printf("  "); // Double space for visual separation
            }
        }

        // Add padding to align the characters if this is the last line and it's not full
        if(line_limit < 32) {
            for(int j = line_limit; j < 32; j++) {
                printf("   ");
                if(j == 15) {
                    printf("  "); // Double space for visual separation
                }
            }
        }

        printf("  "); // Space before characters

        // printing characters
        for(int j = 0; j < line_limit; j++) {
            uint8_t value = bytes[i * 32 + j];
            printf("%c", isprint(value) ? value : '.');
        }

        printf("\n");
        bytes_counter += line_limit;
    }
}


int crypt_gen_rand(unsigned char *buffer, unsigned int bits) {
    if (!buffer) {
        return -1; // Buffer is null
    }

    FILE *f = fopen("/dev/urandom", "rb"); // Open /dev/urandom for reading
    if (f == NULL) {
        perror("Error opening /dev/urandom");
        return -1;
    }

    size_t bytesRead = fread(buffer, 1, bits, f); // Read 32 bytes (256 bits)
    fclose(f); // Close the file

    if (bytesRead != bits) {
        fprintf(stderr, "Failed to read enough random data\n");
        return -1;
    }

    return 0; 
}



