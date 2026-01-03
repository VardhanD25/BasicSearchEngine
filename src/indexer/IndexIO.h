//IndexIO.h
#pragma once
#include <cstdint>

struct Posting {
    uint32_t docId;
    uint32_t tf;
};

struct LexiconEntry {
    uint64_t offset;
    uint32_t count;
};
