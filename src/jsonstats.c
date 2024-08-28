/**
 * @file jsonstats.c
 * @brief Implements the functions defined in jsonstats.h
 * @author Yavuz Koroglu
 */
#include "jsonstats.h"
#include "padkit/debug.h"

static FILE* jsonFile       = nullptr;
static JSONParser jsonp[1]  = { NOT_A_JSON_PARSER };

static bool isFirstElement  = 1;
static bool parsingValue    = 0;

static size_t nArrays       = 0;
static size_t nBooleans     = 0;
static size_t nFalses       = 0;
static size_t nNames        = 0;
static size_t nNulls        = 0;
static size_t nNumbers      = 0;
static size_t nObjects      = 0;
static size_t nStrings      = 0;
static size_t nStringValues = 0;
static size_t nTokens       = 0;
static size_t nTrues        = 0;
static size_t nValues       = 0;

static size_t depth         = 0;
static size_t maxDepth      = 0;

static size_t maxStrLen     = 0;

void atArrayEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nTokens++;
    depth--;
    isFirstElement = 0;
}

void atArrayStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nTokens++;
    nTokens += !isFirstElement;
    nArrays++;
    depth++;
    if (maxDepth < depth) maxDepth = depth;
    isFirstElement = 1;
}

void atFalse_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nBooleans++;
    nFalses++;
}

void atNameEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    isFirstElement = 0;
}

void atNameStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nTokens++;
    nTokens += !isFirstElement;
    nNames++;
}

void atNull_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nNulls++;
}

void atNumber_jstat([[maybe_unused]] JSONParser jp[static const 1], [[maybe_unused]] double const number) {
    nTokens++;
    nNumbers++;
}

void atObjectEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nTokens++;
    depth--;
    isFirstElement = 0;
}

void atObjectStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nTokens++;
    nTokens += !isFirstElement;
    nObjects++;
    depth++;
    if (maxDepth < depth) maxDepth = depth;
    isFirstElement = 1;
}

void atString_jstat(
    [[maybe_unused]] JSONParser jp[static const 1],
    [[maybe_unused]] char const str[static const 1],
    size_t const len
) {
    nStrings++;
    nStringValues += parsingValue;
    if (maxStrLen < len) maxStrLen = len;
}

void atTrue_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nBooleans++;
    nTrues++;
}

void atValueEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    parsingValue    = 0;
    isFirstElement  = 0;
}

void atValueStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nTokens++;
    nTokens += !isFirstElement;
    nValues++;
    parsingValue = 1;
}

long compute_jstat(void) {
    construct_jsonp(
        jsonp,
        jsonFile,
        atArrayEnd_jstat,
        atArrayStart_jstat,
        atFalse_jstat,
        atNameEnd_jstat,
        atNameStart_jstat,
        atNull_jstat,
        atNumber_jstat,
        atObjectEnd_jstat,
        atObjectStart_jstat,
        emptyVoidEvent_jsonp,
        emptyVoidEvent_jsonp,
        atString_jstat,
        atTrue_jstat,
        atValueEnd_jstat,
        atValueStart_jstat
    );

    return parseStream_jsonp(jsonp);
}

void dump_jstat(void) {
    printf("        # Tokens = %zu\n", nTokens);
    printf("       # Objects = %zu\n", nObjects);
    printf("        # Arrays = %zu\n", nArrays);
    printf("       # Strings = %zu\n", nStrings);
    printf("         # Names = %zu\n", nNames);
    printf(" # String Values = %zu\n", nStringValues);
    printf("      # Booleans = %zu (%zu falses, %zu trues)\n", nBooleans, nFalses, nTrues);
    printf("         # Nulls = %zu\n", nNulls);
    printf("       # Numbers = %zu\n", nNumbers);
    printf("        # Values = %zu\n", nValues);
    puts("");
    printf("       Max Depth = %zu\n", maxDepth);
    printf("  Max String Len = %zu\n", maxStrLen);
}

int main(int argc, char* argv[]) {
    puts("");

    if (argc < 2) {
        printf("Usage: %s <JSON-file>\n\n", argv[0]);
        return EXIT_FAILURE;
    }

    jsonFile = fopen(argv[1], "r");
    if (jsonFile == nullptr) {
        printf("ERROR: Could NOT open '%s'\n\n", argv[1]);
        return EXIT_FAILURE;
    }

    switch (compute_jstat()) {
        case JSON_PARSER_OK:
            dump_jstat();
            DEBUG_ERROR_IF(fclose(jsonFile) == EOF)
            NDEBUG_EXECUTE(fclose(jsonFile))

            puts("");
            return EXIT_SUCCESS;
        case JSON_PARSER_MEMORY_ERROR:
            puts("ERROR @ JSONParser: Memory Error\n");
            return EXIT_FAILURE;
        case JSON_PARSER_STACK_ERROR:
            puts("ERROR @ JSONParser: Stack Error\n");
            return EXIT_FAILURE;
        case JSON_PARSER_STREAM_ERROR:
            puts("ERROR @ JSONParser: I/O Error\n");
            return EXIT_FAILURE;
        case JSON_PARSER_STRING_ERROR:
            puts("ERROR @ JSONParser: String Error\n");
            return EXIT_FAILURE;
        case JSON_PARSER_SYNTAX_ERROR:
            puts("ERROR @ JSONParser: Syntax Error\n");
            return EXIT_FAILURE;
        case JSON_PARSER_INVALID:
            puts("ERROR @ JSONParser: Invalid Parser\n");
            return EXIT_FAILURE;
        default:
            puts("ERROR @ JSONParser: Unknown Error\n");
            return EXIT_FAILURE;
    }
}
