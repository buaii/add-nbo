#include <stdint.h>
#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstdlib>

void my_error(const char *errmsg) {
    fprintf(stderr, "ERROR: Incorrect param / %s \n", errmsg);
}

void input_check(char *argv[], uint32_t inputs[]) {
    for (int i = 0; i < 2; i++) {  
        FILE *fp = fopen(argv[i], "r");

        if (fp == NULL) {
            throw "Cannot open file!";
        }

        fseek(fp, 0, SEEK_END);
        int len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        if (len < 4) {
            fclose(fp);
            throw "File is too small!";
        } else if (len > 4) {
            fclose(fp);
            throw "File is too big!";
        }

        size_t result = fread(&inputs[i], sizeof(uint32_t), 1, fp);
        if (result != 1) {
            fclose(fp);
            throw "Cannot read 4bytes from file!";
        }
    
        fclose(fp);
    }
}

u_int32_t my_ntohl(u_int32_t input) {
    uint32_t temp1 = input & 0xff000000;
    uint32_t temp2 = input & 0x00ff0000;
    uint32_t temp3 = input & 0x0000ff00;
    uint32_t temp4 = input & 0x000000ff;

    return temp1 >> 24 | temp2 >> 8 | temp3 << 8 | temp4 << 24;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        my_error("Usage: add-nbo <file1> <file2>");
        return 0;
    }

    try {
        uint32_t inputs[2]; 
        input_check(argv+1, inputs);

        uint32_t num1 = my_ntohl(inputs[0]);
        uint32_t num2 = my_ntohl(inputs[1]);
        uint32_t sum = num1 + num2;

        printf("%u(0x%x) + %u(0x%x) = %u(0x%x)", num1, num1, num2, num2, sum, sum);
    } catch (const char *s) {
        std::cout << "ERROR: " << s << std::endl;
    }
    
    return 0;
}