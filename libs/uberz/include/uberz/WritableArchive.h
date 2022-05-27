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

#ifndef UBERZ_WRITABLE_ARCHIVE_H
#define UBERZ_WRITABLE_ARCHIVE_H

#include <filament/MaterialEnums.h>

#include <uberz/ArchiveEnums.h>
#include <utils/FixedCapacityVector.h>
#include <utils/CString.h>

#include <tsl/robin_map.h>

namespace filament::uberz {

// Precompiled set of materials bundled with a list of features flags that each material supports.
// This is the writeable counterpart to ReadableArchive.
// Used by gltfio; users do not need to access this class directly.
class WritableArchive {
public:
    WritableArchive(size_t materialCount) : mMaterials(materialCount) {}
    void addMaterial(const char* name, const uint8_t* package, size_t packageSize);
    void addSpecLine(const char* line);
    utils::FixedCapacityVector<uint8_t> serialize() const;

private:
    size_t mLineNumber = 1;
    ssize_t mMaterialIndex = -1;

    struct Material {
        utils::CString name;
        utils::FixedCapacityVector<uint8_t> package;
        filament::Shading shadingModel;
        filament::BlendingMode blendingMode;
        tsl::robin_map<utils::CString, ArchiveFeature> flags;
    };

    utils::FixedCapacityVector<Material> mMaterials;
};

} // namespace filament::uberz

#endif // UBERZ_WRITABLE_ARCHIVE_H
