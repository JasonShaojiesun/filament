//------------------------------------------------------------------------------
// Instancing
// ------------------------------------------------------------------------------------------------

PerRenderableData object_uniforms;

void initInstanceUniforms(out PerRenderableData p) {
#if defined(FILAMENT_HAS_FEATURE_INSTANCING)
#if defined(VARIANT_HAS_INSTANCED_STEREO)
    // The lowest bit of the instance index represents the eye.
    highp int index = instance_index >> 1;
#else
    highp int index = instance_index;
#endif
    // We're copying each field separately to workaround an issue in some Adreno drivers
    // that fail on non-const array access in a UBO. Accessing the fields works however.
    // e.g.: this fails `p = objectUniforms.data[instance_index];`
    p.worldFromModelMatrix = objectUniforms.data[index].worldFromModelMatrix;
    p.worldFromModelNormalMatrix = objectUniforms.data[index].worldFromModelNormalMatrix;
    p.morphTargetCount = objectUniforms.data[index].morphTargetCount;
    p.flagsChannels = objectUniforms.data[index].flagsChannels;
    p.objectId = objectUniforms.data[index].objectId;
    p.userData = objectUniforms.data[index].userData;
#else
    p.worldFromModelMatrix = objectUniforms.data[0].worldFromModelMatrix;
    p.worldFromModelNormalMatrix = objectUniforms.data[0].worldFromModelNormalMatrix;
    p.morphTargetCount = objectUniforms.data[0].morphTargetCount;
    p.flagsChannels = objectUniforms.data[0].flagsChannels;
    p.objectId = objectUniforms.data[0].objectId;
    p.userData = objectUniforms.data[0].userData;
#endif
}

void initObjectUniforms(out PerRenderableData p) {
#if defined(FILAMENT_HAS_FEATURE_INSTANCING) && defined(MATERIAL_HAS_INSTANCES)
    if ((objectUniforms.data[0].flagsChannels & FILAMENT_OBJECT_INSTANCE_BUFFER_BIT) != 0) {
        // the object has an instance buffer
        initInstanceUniforms(p);
        return;
    }
    // the material manages instancing, all instances share the same uniform block.
    p = objectUniforms.data[0];
#else
    // automatic instancing was used, each instance has its own uniform block.
    initInstanceUniforms(p);
#endif
}

//------------------------------------------------------------------------------
// Instance access
//------------------------------------------------------------------------------

#if defined(FILAMENT_HAS_FEATURE_INSTANCING)
#if defined(MATERIAL_HAS_INSTANCES)
/** @public-api */
int getInstanceIndex() {
    return instance_index;
}
#endif
#endif
