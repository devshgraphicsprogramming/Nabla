#ifndef _IRR_BUILTIN_GLSL_FORMAT_DECODE_INCLUDED_
#define _IRR_BUILTIN_GLSL_FORMAT_DECODE_INCLUDED_

#include <irr/builtin/glsl/format/constants.glsl>

vec3 irr_glsl_decodeRGB19E7(in uvec2 x)
{
	int exp = int(bitfieldExtract(x.y, 3*irr_glsl_RGB19E7_MANTISSA_BITS-32, irr_glsl_RGB19E7_EXPONENT_BITS) - irr_glsl_RGB19E7_EXP_BIAS - irr_glsl_RGB19E7_MANTISSA_BITS);
	float scale = exp2(float(exp));//uintBitsToFloat((uint(exp)+127u)<<23u)
	
	vec3 v;
	v.x = int(bitfieldExtract(x.x, 0, irr_glsl_RGB19E7_MANTISSA_BITS))*scale;
	v.y = int(
		bitfieldExtract(x.x, irr_glsl_RGB19E7_MANTISSA_BITS, 32-irr_glsl_RGB19E7_MANTISSA_BITS) | 
		(bitfieldExtract(x.y, 0, irr_glsl_RGB19E7_MANTISSA_BITS-(32-irr_glsl_RGB19E7_MANTISSA_BITS))<<(32-irr_glsl_RGB19E7_MANTISSA_BITS))
	) * scale;
	v.z = int(bitfieldExtract(x.y, irr_glsl_RGB19E7_MANTISSA_BITS-(32-irr_glsl_RGB19E7_MANTISSA_BITS), irr_glsl_RGB19E7_MANTISSA_BITS)) * scale;
	
	return v;
}

vec4 irr_glsl_decodeRGB10A2(in uint x)
{
	uvec4 shifted = uvec4(x,uvec3(x)>>uvec3(10,20,30));
	const uvec3 rgbMask = uvec3(0x3ffu);
	return vec4(vec3(shifted.rgb&rgbMask),shifted.a)/vec4(vec3(rgbMask),3.0);
}

#endif