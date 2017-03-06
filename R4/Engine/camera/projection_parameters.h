// +--------------------------------------------+
// | File: projection_parameters.h              |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-06.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

namespace engine
{

// This structure stores the perspective projection attributes of a camera.
struct ProjectionParameters
{
    float mFovy   { 60.0f };	    // Field of view in Y [angle in degrees].
    float mFarZ   { 1000.0f };		// Maximum rendering distance.
    float mNearZ  { 0.1f };         // Minimum rendering distance.
    float mAspect { 4.0f / 3.0f };  // Ratio a = W / H  [width/height].
};

}  // namespace engine.