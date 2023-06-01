#include "NormalShading.hh"

vec3 NormalShading::shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom) {
    return info.normal;
}
