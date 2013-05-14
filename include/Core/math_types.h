#pragma once

// On MSVC, disable "warning C4201: nonstandard extension used : nameless struct/union" warning (level 4).
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201)   // Conditional expression is constant
#endif

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace Bolts {

	//Math Constants
#ifndef __ANDROID__
#define M_E         2.71828182845904523536028747135266250f   /* e */
#define M_LOG2E     1.44269504088896340735992468100189214f   /* log 2e */
#define M_LOG10E    0.434294481903251827651128918916605082f  /* log 10e */
#define M_LN2       0.693147180559945309417232121458176568f  /* log e2 */
#define M_LN10      2.30258509299404568401799145468436421f   /* log e10 */
#define M_PI        3.14159265358979323846264338327950288f  /* pi */
#define M_PI_2      1.57079632679489661923132169163975144f   /* pi/2 */
#define M_PI_4      0.785398163397448309615660845819875721f  /* pi/4 */
#define M_1_PI      0.318309886183790671537767526745028724f  /* 1/pi */
#define M_2_PI      0.636619772367581343075535053490057448f  /* 2/pi */
#define M_2_SQRTPI  1.12837916709551257389615890312154517f   /* 2/sqrt(pi) */
#define M_SQRT2     1.41421356237309504880168872420969808f  /* sqrt(2) */
#define M_SQRT1_2   0.707106781186547524400844362104849039f  /* 1/sqrt(2) */
#endif

	//Math Objects
	typedef glm::vec2 vec2;
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	typedef glm::quat quat;
	typedef glm::mat4 mat4;
	typedef glm::mat3 mat3;

};