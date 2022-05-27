/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UBERZ_READABLE_ARCHIVE_H
#define UBERZ_READABLE_ARCHIVE_H

#include <stdint.h>

#include <uberz/ArchiveEnums.h>

#include <filament/MaterialEnums.h>

namespace filament::uberz {

// ArchiveSpec is a parse-free binary format. The client simply casts a word-aligned content blob
// into a ReadableArchive struct pointer, then calls the following function to convert all the
// offset fields into pointers.
void convertOffsetsToPointers(struct ReadableArchive* archive);

#pragma clang diagnostic push
#pragma clang diagnostic warning "-Wpadded"

// Precompiled set of materials bundled with a list of features flags that each material supports.
// This is the readable counterpart to WriteableArchive.
// Used by gltfio; users do not need to access this class directly.
struct ReadableArchive {
    uint32_t magic;
    uint32_t version;
    uint64_t specsCount;
    union {
        struct ArchiveSpec* specs;
        uint64_t specsOffset;
    };
};

static constexpr filament::Shading INVALID_SHADING_MODEL = (filament::Shading) 0xff;
static constexpr filament::BlendingMode INVALID_BLENDING = (filament::BlendingMode) 0xff;

struct ArchiveSpec {
    union {
        filament::Shading shadingModel;
        uint32_t padding0;
    };
    union {
        filament::BlendingMode blendingMode;
        uint32_t padding1;
    };
    uint64_t flagsCount;
    union {
        struct ArchiveFlag* flags;
        uint64_t flagsOffset;
    };
    uint64_t packageByteCount;
    union {
        uint8_t* package;
        uint64_t packageOffset;
    };
};

struct ArchiveFlag {
    union {
        const char* name;
        uint64_t nameOffset;
    };
    ArchiveFeature value;
};

#pragma clang diagnostic pop

} // namespace filament::uberz

#endif // UBERZ_READABLE_ARCHIVE_H
