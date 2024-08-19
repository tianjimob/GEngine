layout(std430) buffer PrimitiveSceneData {
    mat4 localToWorld;
    mat4 worldToLocal;
    vec3 actorWorldPosition;
    float worldBoundsExtentX;
    vec3 worldBoundsOrigin;
    float worldBoundsRadius;
    vec3 localBoundsMin;
    float worldBoundsExtentY;
    vec3 localBoundsMax;
    float worldBoundsExtentZ;
    int instanceSceneDataOffset;
    int numInstanceSceneDataEntries;
};

layout(std430) buffer PrimitiveInstanceSceneData {
    mat4 localToWorld;
    mat4 worldToLocal;
    vec3 localBoundsOrigin;
    float primitiveId;
    vec3 localBoundsExtent;
};