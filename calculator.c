#include "ASTFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 200

void printWelcomeMessage()
{
    system("figlet Calculator");
    printf("\033[1;3mWelcome to My Calculator!\033[0m\n");
    printf("\033[1;32mEnter an expression (or 'q' to quit): \033[0m\n");
}

const char *getResultColour(int count)
{
    const char *colours[] = {
        "\033[1;32m", // Green
        "\033[1;34m", // Blue
        "\033[1;36m", // Cyan
        "\033[1;35m", // Magenta
        "\033[1;33m"  // Yellow
    };
    int numColours = sizeof(colours) / sizeof(colours[0]);
    return colours[count % numColours];
}

int main()
{
    char expression[MAX_SIZE];
    int colourCount = 0;
    printWelcomeMessage();

    while (1)
    {
        printf(">> ");
        fgets(expression, sizeof(expression), stdin);
        expression[strcspn(expression, "\n")] = '\0'; // Remove newline character

        if (strcmp(expression, "q") == 0)
        {
            printf("Exiting calculator. Goodbye!\n");
            break;
        }

        int numTokens;
        char **tokens = tokenise(expression, &numTokens);
        Parser parser = {tokens, 0, numTokens};
        ASTNode *ast = parseExpression(&parser);

        if (ast)
        {
            double result = evaluate(ast);
            printf("%sResult: %.6f\033[0m\n", getResultColour(colourCount), result);
            freeNode(ast);
            colourCount++;
        }
        else
        {
            printf("\033[1;31mError: Invalid expression.\033[0m\n");
        }

        freeTokens(tokens, numTokens);
    }

    return 0;
}