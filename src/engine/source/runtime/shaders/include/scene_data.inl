layout(std430) buffer PrimitiveSceneData {
    mat4 localToWorld;
    mat4 worldToLocal;
    int instanceSceneDataOffset;
};