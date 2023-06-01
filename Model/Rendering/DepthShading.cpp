#include "DepthShading.hh"

vec3 DepthShading::shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom) {
    float d = glm::distance(lookFrom, info.p)/2;
    return vec3(d, d, d);
}
