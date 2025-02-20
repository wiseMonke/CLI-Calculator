#ifndef AST_FUNCTIONS_H
#define AST_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "MathFunctions.h"

//  AST node structure
typedef struct ASTNode
{
    char *value;           // Can be a number, operator, or function name (e.g., sin, cos, tan)
    struct ASTNode *left;  // Left child in AST
    struct ASTNode *right; // Right child in AST
} ASTNode;

// Parser structure
typedef struct Parser
{
    char **tokens; // Tokenized input
    int pos;       // Tracks current position in tokens
    int numTokens; // Number of tokens
} Parser;

// Function declarations
ASTNode *createNode(const char *value, ASTNode *left, ASTNode *right);
void freeNode(ASTNode *node);
ASTNode *parseExpression(Parser *parser);
ASTNode *parseTerm(Parser *parser);
ASTNode *parseExponentiation(Parser *parser);
ASTNode *parseFactor(Parser *parser);
int match(Parser *parser, char *expected);
char *previous(Parser *parser);
void consume(Parser *parser, char *expected);
char *consumeNumber(Parser *parser);
int implicitMultiplication(Parser *parser);
double evaluate(ASTNode *node);
char **tokenise(const char *expression, int *numTokens);
void freeTokens(char **tokens, int numTokens);

// Create a new AST node
ASTNode *createNode(const char *value, ASTNode *left, ASTNode *right)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->value = strdup(value);
    node->left = left;
    node->right = right;
    return node;
}

// Free an AST node
void freeNode(ASTNode *node)
{
    if (node == NULL)
        return;
    free(node->value);
    freeNode(node->left);
    freeNode(node->right);
    free(node);
}

// Parse an expression (handles + and -)
ASTNode *parseExpression(Parser *parser)
{
    ASTNode *node = parseTerm(parser); // Start with higher precedence
    while (match(parser, "+") || match(parser, "-"))
    {
        char *op = previous(parser);        // Store operator
        ASTNode *right = parseTerm(parser); // Parse right-hand side
        node = createNode(op, node, right);
    }
    return node;
}

// Parse a term (handles *, /, and implicit multiplication)
ASTNode *parseTerm(Parser *parser)
{
    ASTNode *node = parseExponentiation(parser); // Higher precedence than + and -
    while (match(parser, "*") || match(parser, "/") || implicitMultiplication(parser))
    {
        const char *op = implicitMultiplication(parser) ? "*" : previous(parser);
        ASTNode *right = parseExponentiation(parser);
        node = createNode(op, node, right);
    }
    return node;
}

// Parse exponentiation (handles ^, right-associative)
ASTNode *parseExponentiation(Parser *parser)
{
    ASTNode *node = parseFactor(parser);
    while (match(parser, "^"))
    {
        char *op = previous(parser);
        ASTNode *right = parseFactor(parser);
        node = createNode(op, node, right);
    }
    return node;
}

// Parse a factor (handles numbers, functions, and parentheses)
ASTNode *parseFactor(Parser *parser)
{
    // Check for function identifiers: sin, cos, tan, etc.
    if (parser->pos < parser->numTokens)
    {
        char *token = parser->tokens[parser->pos];
        if (strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tan") == 0 ||
            strcmp(token, "ln") == 0 || strcmp(token, "exp") == 0 || strcmp(token, "sinh") == 0 ||
            strcmp(token, "cosh") == 0 || strcmp(token, "tanh") == 0 || strcmp(token, "asin") == 0 ||
            strcmp(token, "acos") == 0 || strcmp(token, "atan") == 0 || strcmp(token, "asinh") == 0 ||
            strcmp(token, "acosh") == 0 || strcmp(token, "atanh") == 0 || strcmp(token, "pow") == 0 ||
            strcmp(token, "log_base") == 0)
        {
            parser->pos++;        // Consume the function name
            consume(parser, "("); // Search for '('
            ASTNode *arg = parseExpression(parser);
            if (strcmp(token, "pow") == 0 || strcmp(token, "log_base") == 0)
            {
                consume(parser, ","); // Expect ',' for pow and log_base
                ASTNode *arg2 = parseExpression(parser);
                consume(parser, ")");                // Expect ')'
                return createNode(token, arg, arg2); // Two arguments for pow and log_base
            }
            consume(parser, ")");                // Expect ')'
            return createNode(token, arg, NULL); // Single argument for other functions
        }
    }

    if (match(parser, "("))
    {
        ASTNode *node = parseExpression(parser);
        consume(parser, ")"); // Ensure closing brackets
        return node;
    }
    return createNode(consumeNumber(parser), NULL, NULL); // Create node for number
}

// Check if the current token matches an expected token
int match(Parser *parser, char *expected)
{
    if (parser->pos < parser->numTokens && strcmp(parser->tokens[parser->pos], expected) == 0)
    {
        parser->pos++;
        return 1;
    }
    return 0;
}

// Get the previous token
char *previous(Parser *parser)
{
    return parser->tokens[parser->pos - 1];
}

// Consume a specific token
void consume(Parser *parser, char *expected)
{
    if (!match(parser, expected))
    {
        fprintf(stderr, "Syntax error: Expected '%s', got '%s'\n", expected, parser->tokens[parser->pos]);
        exit(1);
    }
}

// Consume a number token
char *consumeNumber(Parser *parser)
{
    char *num = parser->tokens[parser->pos];
    parser->pos++;
    return num;
}

// Detect implicit multiplication (e.g., between a number and a parenthesis or function)
int implicitMultiplication(Parser *parser)
{
    if (parser->pos < parser->numTokens)
    {
        char *prev = previous(parser);            // Last token
        char *curr = parser->tokens[parser->pos]; // Current token
        return (isdigit(prev[0]) || strcmp(prev, ")") == 0) &&
               (isdigit(curr[0]) || strcmp(curr, "(") == 0 ||
                strcmp(curr, "sin") == 0 || strcmp(curr, "cos") == 0 ||
                strcmp(curr, "tan") == 0 || strcmp(curr, "ln") == 0 || strcmp(curr, "exp") == 0 ||
                strcmp(curr, "sinh") == 0 || strcmp(curr, "cosh") == 0 || strcmp(curr, "tanh") == 0 ||
                strcmp(curr, "asin") == 0 || strcmp(curr, "acos") == 0 || strcmp(curr, "atan") == 0 ||
                strcmp(curr, "asinh") == 0 || strcmp(curr, "acosh") == 0 || strcmp(curr, "atanh") == 0 ||
                strcmp(curr, "pow") == 0 || strcmp(curr, "log_base") == 0);
    }
    return 0;
}

// Evaluate the AST
double evaluate(ASTNode *node)
{
    // If leaf node (a number), convert it to a double
    if (node->left == NULL && node->right == NULL)
    {
        return atof(node->value);
    }

    // Check for function nodes first
    if (strcmp(node->value, "sin") == 0)
    {
        double arg = evaluate(node->left);
        return customSIN(arg);
    }
    if (strcmp(node->value, "cos") == 0)
    {
        double arg = evaluate(node->left);
        return customCOS(arg);
    }
    if (strcmp(node->value, "tan") == 0)
    {
        double arg = evaluate(node->left);
        return customTAN(arg);
    }
    if (strcmp(node->value, "ln") == 0)
    {
        double arg = evaluate(node->left);
        return customLN(arg);
    }
    if (strcmp(node->value, "exp") == 0)
    {
        double arg = evaluate(node->left);
        return customEXP(arg);
    }
    if (strcmp(node->value, "sinh") == 0)
    {
        double arg = evaluate(node->left);
        return customSINH(arg);
    }
    if (strcmp(node->value, "cosh") == 0)
    {
        double arg = evaluate(node->left);
        return customCOSH(arg);
    }
    if (strcmp(node->value, "tanh") == 0)
    {
        double arg = evaluate(node->left);
        return customTANH(arg);
    }
    if (strcmp(node->value, "asin") == 0)
    {
        double arg = evaluate(node->left);
        return customASIN(arg);
    }
    if (strcmp(node->value, "acos") == 0)
    {
        double arg = evaluate(node->left);
        return customACOS(arg);
    }
    if (strcmp(node->value, "atan") == 0)
    {
        double arg = evaluate(node->left);
        return customATAN(arg);
    }
    if (strcmp(node->value, "asinh") == 0)
    {
        double arg = evaluate(node->left);
        return customASINH(arg);
    }
    if (strcmp(node->value, "acosh") == 0)
    {
        double arg = evaluate(node->left);
        return customACOSH(arg);
    }
    if (strcmp(node->value, "atanh") == 0)
    {
        double arg = evaluate(node->left);
        return customATANH(arg);
    }
    if (strcmp(node->value, "pow") == 0)
    {
        double base = evaluate(node->left);
        double exponent = evaluate(node->right);
        return customPOW(base, exponent);
    }
    if (strcmp(node->value, "log_base") == 0)
    {
        double a = evaluate(node->left);
        double b = evaluate(node->right);
        return customLogBase(a, b);
    }

    // Otherwise, it is an operator.
    double left_val = evaluate(node->left);
    double right_val = evaluate(node->right);
    double result = 0.0;

    if (strcmp(node->value, "+") == 0)
    {
        result = left_val + right_val;
    }
    else if (strcmp(node->value, "-") == 0)
    {
        result = left_val - right_val;
    }
    else if (strcmp(node->value, "*") == 0)
    {
        result = left_val * right_val;
    }
    else if (strcmp(node->value, "/") == 0)
    {
        result = left_val / right_val;
    }
    else if (strcmp(node->value, "^") == 0)
    {
        result = customPOW(left_val, right_val);
    }

    // Correct negative zero if needed
    if (result == 0 && signbit(result))
    {
        result = 0;
    }

    return result;
}

// Tokenize the input expression with support for numbers, decimals, negatives, and identifiers (sin, cos, tan, log_base, etc.)
char **tokenise(const char *expression, int *numTokens)
{
    char **tokens = NULL;
    int capacity = 10;
    int count = 0;
    tokens = (char **)malloc(capacity * sizeof(char *));

    int i = 0;
    while (expression[i] != '\0')
    {
        // Skip whitespace
        if (isspace(expression[i]))
        {
            i++;
            continue;
        }

        // If the token starts with a digit, a decimal point, or a '-' sign in a valid context, it is a number.
        if (isdigit(expression[i]) ||
            expression[i] == '.' ||
            (expression[i] == '-' &&
             ((i == 0) || (expression[i - 1] == '(') || strchr("+-*/^", expression[i - 1]) != NULL)))
        {
            int start = i;
            int dotCount = 0;
            if (expression[i] == '-')
            {
                i++;
            }
            while (isdigit(expression[i]) || expression[i] == '.')
            {
                if (expression[i] == '.')
                {
                    dotCount++;
                    if (dotCount > 1)
                    {
                        fprintf(stderr, "Invalid number: multiple decimal points in token\n");
                        exit(1);
                    }
                }
                i++;
            }
            int len = i - start;
            char *num = (char *)malloc((len + 1) * sizeof(char));
            strncpy(num, expression + start, len);
            num[len] = '\0';
            tokens[count++] = num;
        }
        // If the token starts with an alphabetic character or underscore, it is an identifier (e.g., sin, cos, tan, log_base).
        else if (isalpha(expression[i]) || expression[i] == '_')
        {
            int start = i;
            while (isalpha(expression[i]) || isdigit(expression[i]) || expression[i] == '_')
                i++;
            int len = i - start;
            char *id = (char *)malloc((len + 1) * sizeof(char));
            strncpy(id, expression + start, len);
            id[len] = '\0';
            tokens[count++] = id;
        }
        // Otherwise, it must be an operator, parenthesis, or comma.
        else if (strchr("+-*/^(),", expression[i]) != NULL)
        {
            char *op = (char *)malloc(2 * sizeof(char));
            op[0] = expression[i];
            op[1] = '\0';
            tokens[count++] = op;
            i++;
        }
        else
        {
            fprintf(stderr, "Unexpected character: %c\n", expression[i]);
            exit(1);
        }

        if (count >= capacity)
        {
            capacity *= 2;
            tokens = (char **)realloc(tokens, capacity * sizeof(char *));
        }
    }

    *numTokens = count;
    return tokens;
}

// Free the tokens array
void freeTokens(char **tokens, int numTokens)
{
    for (int i = 0; i < numTokens; i++)
    {
        free(tokens[i]);
    }
    free(tokens);
}

#endif