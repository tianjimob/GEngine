struct PrimitiveSceneData {
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
    int ptimitiveId;
    float _padding;
};

struct PrimitiveSceneDatafloat {
    mat4  localToWorld;
    mat4  worldToLocal;
    vec3  actorWorldPosition;
    float worldBoundsExtentX;
    vec3  worldBoundsOrigin;
    float worldBoundsRadius;
    vec3  localBoundsMin;
    float worldBoundsExtentY;
    vec3  localBoundsMax;
    float worldBoundsExtentZ;
    float instanceSceneDataOffset;
    float numInstanceSceneDataEntries;
    float ptimitiveId;
    float _padding;
};

struct PrimitiveInstanceSceneData {
    mat4 localToWorld;
    mat4 worldToLocal;
    vec3 localBoundsOrigin;
    int primitiveId;
    vec3 localBoundsExtent;
    float _padding;
};