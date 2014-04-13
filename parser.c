#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct Parser {
    const Symbol *symbols;
    size_t symbol_count;
};

static void get_delimited_tokens(const Parser *parser,
                                 const Token *tokens,
                                 const size_t token_count,
                                 const char *begin_delimeter,
                                 const char *end_delimeter)
{
    if (parser == NULL || tokens == NULL) {
        return;
    }

    unsigned int *indices;
    indices = malloc(token_count * sizeof(unsigned int));

    if (indices == NULL) {
        return;
    }

    {
        unsigned int i;
        for (i = 0; i < token_count; ++i) {
            indices[i] = 0;
        }
    }

    {
        int open_count, close_count;
        open_count = 0;
        close_count = 0;

        unsigned int i;
        for (i = 0; i < token_count; ++i) {
            if (strcmp(tokens[i].value, begin_delimeter) == 0) {
                indices[i] = 1;
                ++open_count;
            }
            else if (strcmp(tokens[i].value, end_delimeter) == 0) {
                indices[i] = 2;
                ++close_count;
            }
        }

        if (open_count != close_count) {
            printf("Invalid input:\n");
            if (close_count > open_count) {
                printf("Expected open:  \"%s\"\n", begin_delimeter);

                if (indices != NULL) {
                    free(indices);
                    indices = NULL;
                }

                return;
            }
            else if (open_count > close_count) {
                printf("Expected close:  \"%s\"\n", end_delimeter);

                if (indices != NULL) {
                    free(indices);
                    indices = NULL;
                }

                return;
            }
        }
    }

    {
        unsigned int i, open_count, close_count;
        open_count = 0;
        close_count = 0;
        for (i = 0; i < token_count; ++i) {
            if (indices[i] == 1) {
                unsigned int j;
                for (j = i; j < token_count; ++j) {
                    printf("%4s%-4u=> \"%s\"\n",
                           "",
                           j,
                           tokens[j].value);

                    if (indices[j] == 1) {
                        ++open_count;
                    }
                    else if (indices[j] == 2) {
                        ++close_count;
                    }

                    if (open_count == close_count) {
                        printf("\n");
                        break;
                    }
                }
            }
        }
    }

    if (indices != NULL) {
        free(indices);
        indices = NULL;
    }
}

static void get_adjacent_tokens(const Parser *parser,
                                const Token *tokens,
                                const size_t token_count,
                                const char *delimeter)
{
    if (parser == NULL || tokens == NULL) {
        return;
    }

    unsigned int *indices;
    indices = malloc(token_count * sizeof(unsigned int));

    if (indices == NULL) {
        return;
    }

    {
        unsigned int i;
        for (i = 0; i < token_count; ++i) {
            indices[i] = 0;
        }
    }

    {
        unsigned int i, found;
        found = 0;
        for (i = 0; i < token_count; ++i) {
            if (found == 1) {
                indices[i] = 2;
                found = 0;
            }
            else if (strcmp(tokens[i].value, delimeter) == 0) {
                indices[i] = 1;
                found = 1;
            }
        }
    }

    {
        unsigned int i;
        for (i = 0; i < token_count; ++i) {
            if (indices[i] == 1 || indices[i] == 2) {
                printf("%4s%-4u=> \"%s\"\n",
                       "",
                       i,
                       tokens[i].value);
            }
        }
    }

    if (indices != NULL) {
        free(indices);
        indices = NULL;
    }
}

Parser *Parser_create(const Symbol *symbols, const size_t symbol_count)
{
    Parser *parser = malloc(sizeof(Parser));

    if (parser != NULL) {
        parser->symbols = symbols;
        parser->symbol_count = symbol_count;
    }

    return parser;
}

void Parser_destroy(Parser *parser)
{
    if (parser != NULL) {
        free(parser);
        parser = NULL;
    }
}

void Parser_match_groups(const Parser *parser,
                         const Token *tokens,
                         const size_t token_count)
{
    printf("Groups:\n{\n");
    get_delimited_tokens(parser,
                         tokens,
                         token_count,
                         parser->symbols[SYMBOL_GROUP_BEG].value,
                         parser->symbols[SYMBOL_GROUP_END].value);
    printf("}\n\n");
}

void Parser_match_ranges(const Parser *parser,
                         const Token *tokens,
                         const size_t token_count)
{
    printf("Ranges:\n{\n");
    get_delimited_tokens(parser,
                         tokens,
                         token_count,
                         parser->symbols[SYMBOL_RANGE_BEG].value,
                         parser->symbols[SYMBOL_RANGE_END].value);
    printf("}\n\n");
}

void Parser_match_escapes(const Parser *parser,
                          const Token *tokens,
                          const size_t token_count)
{
    printf("Escape Characters:\n{\n");
    get_adjacent_tokens(parser,
                        tokens,
                        token_count,
                        parser->symbols[SYMBOL_ESCAPE].value);
    printf("}\n\n");
}
