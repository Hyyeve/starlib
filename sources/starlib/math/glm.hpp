#pragma once
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_INLINE
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_LEFT_HANDED
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtx/norm.hpp>

#define MAKE_PRINTABLE(type, func) inline std::ostream& operator << (std::ostream& lhs, const type& rhs) { \
lhs << func; \
return lhs; }

MAKE_PRINTABLE(glm::vec2, '[' << rhs.x << ", " << rhs.y << ']');
MAKE_PRINTABLE(glm::ivec2, '[' << rhs.x << ", " << rhs.y << ']');
MAKE_PRINTABLE(glm::dvec2, '[' << rhs.x << ", " << rhs.y << ']');
MAKE_PRINTABLE(glm::vec3, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ']');
MAKE_PRINTABLE(glm::ivec3, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ']');
MAKE_PRINTABLE(glm::dvec3, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ']');
MAKE_PRINTABLE(glm::vec4, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ']');
MAKE_PRINTABLE(glm::ivec4, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ']');
MAKE_PRINTABLE(glm::dvec4, '[' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ']');
MAKE_PRINTABLE(glm::quat, '[' << glm::pitch(rhs) << ", " << glm::yaw(rhs) << ", " << glm::roll(rhs) << ']');

#undef MAKE_PRINTABLE
