/**
 * @file jsonstats.c
 * @brief Implements the functions defined in jsonstats.h
 * @author Yavuz Koroglu
 */
#include "jsonstats.h"
#include "padkit/debug.h"

static FILE* jsonFile       = nullptr;

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
static size_t nVars         = 0;

static size_t depth         = 0;
static size_t maxDepth      = 0;

static size_t maxStrLen     = 0;

static double runningAvg    = 0.00;

static void atArrayEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    /* ']' is a token. */
    nTokens++;

    /* The next token may be preceded with ',' or ':'. */
    isFirstElement = 0;

    depth--;
}

static void atArrayStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    /* Count the preceding ',' or ':' token, if available. */
    nTokens += !isFirstElement;

    /* '[' is a token. */
    nTokens++;

    /* The next token will NOT be preceded with ',' or ':'. */
    isFirstElement = 1;

    nArrays++;

    depth++;
    if (maxDepth < depth) maxDepth = depth;
}

static void atFalse_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nBooleans++;
    nFalses++;
}

static void atNameEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    /* The next token will be preceded by a ':'. */
    isFirstElement = 0;
}

static void atNameStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    /* Count the preceding ',' or ':' token, if available. */
    nTokens += !isFirstElement;

    /* A name is a token. */
    nTokens++;

    nVars++;
    nNames++;
}

static void atNull_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nNulls++;
}

static void atNumber_jstat(
    [[maybe_unused]] JSONParser jp[static const 1],
    double const number
) {
    runningAvg += (number - runningAvg) / (double)(++nNumbers);
}

static void atObjectEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    /* '}' is a token. */
    nTokens++;

    /* The next token may be preceded with ',' or ':'. */
    isFirstElement = 0;

    depth--;
}

static void atObjectStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    /* Count the preceding ',' or ':' token, if available. */
    nTokens += !isFirstElement;

    /* '{' is a token. */
    nTokens++;

    /* The next token will NOT be preceded with ',' or ':'. */
    isFirstElement = 1;

    nObjects++;

    depth++;
    if (maxDepth < depth) maxDepth = depth;
}

static void atRootStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    isFirstElement  = 1;
    parsingValue    = 0;

    nArrays         = 0;
    nBooleans       = 0;
    nFalses         = 0;
    nNames          = 0;
    nNulls          = 0;
    nNumbers        = 0;
    nObjects        = 0;
    nStrings        = 0;
    nStringValues   = 0;
    nTokens         = 0;
    nTrues          = 0;
    nValues         = 0;
    nVars           = 0;

    depth           = 0;
    maxDepth        = 0;

    maxStrLen       = 0;

    runningAvg      = 0.00;
}

static void atString_jstat(
    [[maybe_unused]] JSONParser jp[static const 1],
    [[maybe_unused]] char const str[static const 1],
    size_t const len
) {
    nStrings++;
    nStringValues += parsingValue;
    if (maxStrLen < len) maxStrLen = len;
}

static void atTrue_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    nBooleans++;
    nTrues++;
}

static void atValueEnd_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    parsingValue    = 0;

    /* The next token will be preceded by a ':'. */
    isFirstElement = 0;
}

static void atValueStart_jstat([[maybe_unused]] JSONParser jp[static const 1]) {
    /* Count the preceding ',' or ':' token, if available. */
    nTokens += !isFirstElement;

    /* Every value is a token. */
    nTokens++;

    nVars++;
    nValues++;
    parsingValue = 1;
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
    printf("     # Variables = %zu\n", nVars);
    puts("");
    printf("       Max Depth = %zu\n", maxDepth);
    printf("  Max String Len = %zu\n", maxStrLen);
    puts("");
    printf("Avg. All Numbers = %.2lf\n", runningAvg);
}

int main(int argc, char* argv[]) {
    puts("");

    if (argc < 2) {
        printf("  Usage: %s <JSON-file>\n\n", argv[0]);
        return EXIT_FAILURE;
    }

    jsonFile = fopen(argv[1], "r");
    if (jsonFile == nullptr) {
        printf("ERROR: Could NOT open '%s'\n\n", argv[1]);
        return EXIT_FAILURE;
    } else {
        JSONParser jp[1] = { NOT_A_JSON_PARSER };
        construct_jsonp(
            jp,
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
            atRootStart_jstat,
            atString_jstat,
            atTrue_jstat,
            atValueEnd_jstat,
            atValueStart_jstat
        );

        switch (parseStream_jsonp(jp)) {
            case JSON_PARSER_OK:
                free_jsonp(jp);

                DEBUG_ERROR_IF(fclose(jsonFile) == EOF)
                NDEBUG_EXECUTE(fclose(jsonFile))

                dump_jstat();
                puts("");

                return EXIT_SUCCESS;
            case JSON_PARSER_MEMORY_ERROR:
                free_jsonp(jp);

                DEBUG_ERROR_IF(fclose(jsonFile) == EOF)
                NDEBUG_EXECUTE(fclose(jsonFile))

                puts("ERROR @ JSONParser: Memory Error\n");

                return EXIT_FAILURE;
            case JSON_PARSER_STACK_ERROR:
                free_jsonp(jp);

                DEBUG_ERROR_IF(fclose(jsonFile) == EOF)
                NDEBUG_EXECUTE(fclose(jsonFile))

                puts("ERROR @ JSONParser: Stack Error\n");

                return EXIT_FAILURE;
            case JSON_PARSER_STREAM_ERROR:
                free_jsonp(jp);

                puts("ERROR @ JSONParser: I/O Error\n");

                return EXIT_FAILURE;
            case JSON_PARSER_STRING_ERROR:
                free_jsonp(jp);

                DEBUG_ERROR_IF(fclose(jsonFile) == EOF)
                NDEBUG_EXECUTE(fclose(jsonFile))

                puts("ERROR @ JSONParser: String Error\n");

                return EXIT_FAILURE;
            case JSON_PARSER_SYNTAX_ERROR:
                free_jsonp(jp);

                DEBUG_ERROR_IF(fclose(jsonFile) == EOF)
                NDEBUG_EXECUTE(fclose(jsonFile))

                puts("ERROR @ JSONParser: Syntax Error\n");

                return EXIT_FAILURE;
            case JSON_PARSER_INVALID:
                DEBUG_ERROR_IF(fclose(jsonFile) == EOF)
                NDEBUG_EXECUTE(fclose(jsonFile))

                puts("ERROR @ JSONParser: Invalid Parser\n");

                return EXIT_FAILURE;
            default:
                puts("ERROR @ JSONParser: Unknown Error\n");

                return EXIT_FAILURE;
        }
    }
}
