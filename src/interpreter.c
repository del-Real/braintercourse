#include "../include/interpreter.h"
#include "../include/renderer.h"

int main(int argc, char **argv) {

    // Initialize context struct values
    InterpreterContext ctx;
    memset(ctx.memArray, 0, sizeof(ctx.memArray));
    memset(ctx.output, 0, sizeof(ctx.output));
    ctx.input = NULL;
    ctx.instPtr = 0;
    ctx.dataPtr = 0;
    ctx.maxDataPtr = 0;

    // Get input program
    if (argc == 2) {
        ctx.input = argv[1];
    } else if (argc > 2) {
        fprintf(stderr, "Usage:\n\t./braintercourse\n\t./braintercourse <brainfuck program>\n");
    } else {
        static char buffer[BUFFER_SIZE];
        printf("Enter program:\n"); // request input
        fgets(buffer, sizeof(buffer), stdin);
        ctx.input = buffer;
    }

    // Execute interpreter algorithm
    ExecuteInterpreter(&ctx);

    // Print output
    printf("%s", ctx.output);

    // Initialize and run (raylib) renderer
    InitRenderer();
    RunRenderer(&ctx);

    return 0;
}

void ExecuteInterpreter(InterpreterContext *ctx) {
    /* ----------------------------------------------------------------------------------
     * MAIN INTERPRETER LOOP
     * ----------------------------------------------------------------------------------
     * Operation rules:
     * '>' - Move data pointer 1 block to the right
     * '<' - Move data pointer 1 block to the left
     * '+' - Increment value stored in current cell
     * '-' - Decrement value stored in current cell
     * '[' - Jump forward past matching ']' if current cell == 0
     * ']' - Jump back after matching '[' if current cell != 0
     * '.' - Output current cell as ASCII (similar to putchar() in C)
     * ',' - Read input byte into current cell (similar to getchar() in C)
     */

    while (ctx->input[ctx->instPtr] != '\0') {
        // Evaluate instruction
        switch (ctx->input[ctx->instPtr]) {
            case '>':
                if (ctx->dataPtr < ARRAY_SIZE - 1) {
                    ctx->dataPtr++;
                    if (ctx->dataPtr > ctx->maxDataPtr) {
                        ctx->maxDataPtr = ctx->dataPtr; // Track highest position
                    }
                } else {
                    fprintf(stderr, "Memory overflow error caused by '>'\n");
                    exit(1);
                }
                break;
            case '<':
                if (ctx->dataPtr > 0) {
                    ctx->dataPtr--;
                } else {
                    fprintf(stderr, "Memory underflow error caused by '<'\n");
                    exit(1);
                }
                break;
            case '+':
                ctx->memArray[ctx->dataPtr]++;
                break;
            case '-':
                ctx->memArray[ctx->dataPtr]--;
                break;
            case '[':
                if (ctx->memArray[ctx->dataPtr] == 0) {
                    // Process nested loops
                    int bracketCount = 1;
                    while (bracketCount > 0) {
                        ctx->instPtr++;
                        if (ctx->input[ctx->instPtr] == '[') {
                            bracketCount++;
                        } else if (ctx->input[ctx->instPtr] == ']') {
                            bracketCount--;
                        }
                    }
                }
                break;
            case ']':
                if (ctx->memArray[ctx->dataPtr] != 0) {
                    // Process nested loops
                    int bracketCount = 1;
                    while (bracketCount > 0) {
                        ctx->instPtr--;
                        if (ctx->input[ctx->instPtr] == ']') {
                            bracketCount++;
                        } else if (ctx->input[ctx->instPtr] == '[') {
                            bracketCount--;
                        }
                    }
                }
                break;
            case '.': {
                // Assign value to output
                int len;
                len = strlen(ctx->output);
                ctx->output[len] = ctx->memArray[ctx->dataPtr];
                ctx->output[len + 1] = '\0';
                break;
            }
            case ',': {
                int ch = getchar();
                if (ch != EOF) {
                    ctx->memArray[ctx->dataPtr] = ch;
                }
                break;
            }
        }

        // Next instruction
        ctx->instPtr++;
    }
}