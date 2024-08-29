/**
 * @file jsonstats.h
 * @brief Defines events to parse JSON stats.
 * @author Yavuz Koroglu
 */
#ifndef JSONSTATS_H
    #define JSONSTATS_H
    #include "padkit/jsonparser.h"

    void atArrayEnd_jstat   ([[maybe_unused]] JSONParser jp[static const 1]);
    void atArrayStart_jstat ([[maybe_unused]] JSONParser jp[static const 1]);
    void atFalse_jstat      ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNameEnd_jstat    ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNameStart_jstat  ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNull_jstat       ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNumber_jstat     (
        [[maybe_unused]] JSONParser jp[static const 1],
        double const number
    );
    void atObjectEnd_jstat  ([[maybe_unused]] JSONParser jp[static const 1]);
    void atObjectStart_jstat([[maybe_unused]] JSONParser jp[static const 1]);
    void atRootStart_jstat  ([[maybe_unused]] JSONParser jp[static const 1]);
    void atString_jstat     (
        [[maybe_unused]] JSONParser jp[static const 1],
        [[maybe_unused]] char const str[static const 1],
        size_t const len
    );
    void atTrue_jstat       ([[maybe_unused]] JSONParser jp[static const 1]);
    void atValueEnd_jstat   ([[maybe_unused]] JSONParser jp[static const 1]);
    void atValueStart_jstat ([[maybe_unused]] JSONParser jp[static const 1]);

    /**
     * @brief Computes the JSON statistics.
     *
     * @return A long integer (errorcode).
     */
    long compute_jstat(void);

    /**
     * @brief Dumps the JSON statistics to stdout.
     */
    void dump_jstat(void);
#endif
