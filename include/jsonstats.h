#ifndef JSONSTATS_H
    #define JSONSTATS_H
    #include "padkit/jsonparser.h"

    void atArrayEnd_jstat   ([[maybe_unused]] JSONParser jp[static const 1]);
    void atArrayStart_jstat ([[maybe_unused]] JSONParser jp[static const 1]);
    void atFalse_jstat      ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNameEnd_jstat    ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNameStart_jstat  ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNull_jstat       ([[maybe_unused]] JSONParser jp[static const 1]);
    void atNumber_jstat     ([[maybe_unused]] JSONParser jp[static const 1], [[maybe_unused]] double const number);
    void atObjectEnd_jstat  ([[maybe_unused]] JSONParser jp[static const 1]);
    void atObjectStart_jstat([[maybe_unused]] JSONParser jp[static const 1]);
    void atString_jstat     (
        [[maybe_unused]] JSONParser jp[static const 1],
        [[maybe_unused]] char const str[static const 1],
        [[maybe_unused]] size_t const len
    );
    void atTrue_jstat       ([[maybe_unused]] JSONParser jp[static const 1]);
    void atValueEnd_jstat   ([[maybe_unused]] JSONParser jp[static const 1]);
    void atValueStart_jstat ([[maybe_unused]] JSONParser jp[static const 1]);

    long compute_jstat(void);

    void dump_jstat(void);
#endif
