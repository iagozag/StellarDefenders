#include "norm_greater.hpp"

glm::vec2 norm_greater(glm::vec2 v){
    float ma = (v.x > v.y ? v.x : v.y);
    return v / ma;
}