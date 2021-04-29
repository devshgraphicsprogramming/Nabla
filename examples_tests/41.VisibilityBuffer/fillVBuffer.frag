#version 430 core

layout(location = 0) flat in uint drawID;

#include "common.glsl"

// TODO: investigate using snorm16 for the derivatives
layout(location = 0) out uvec4 triangleIDdrawID_unorm16Bary_dBarydScreenHalf2x2; // 32bit triangleID, 2x16bit barycentrics, 4x16bit barycentric derivatives 
#include <nbl/builtin/glsl/barycentric/frag.glsl>

uint nbl_glsl_barycentric_frag_getDrawID() {return drawID;}
vec3 nbl_glsl_barycentric_frag_getVertexPos(in uint drawID, in uint primID, in uint primsVx) {return vec3(0.0/0.0);} // TODO

void main()
{
    vec2 bary = nbl_glsl_barycentric_frag_get();

    triangleIDdrawID_unorm16Bary_dBarydScreenHalf2x2[0] = bitfieldInsert(gl_PrimitiveID,drawID,MAX_TRIANGLES_IN_BATCH,32-MAX_TRIANGLES_IN_BATCH);
    triangleIDdrawID_unorm16Bary_dBarydScreenHalf2x2[1] = packUnorm2x16(bary);
    triangleIDdrawID_unorm16Bary_dBarydScreenHalf2x2[2] = packHalf2x16(dFdx(bary));
    triangleIDdrawID_unorm16Bary_dBarydScreenHalf2x2[3] = packHalf2x16(dFdy(bary));
}