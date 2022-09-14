/* Copyright 2019 The Waymo Open Dataset Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "waymo_open_dataset/metrics/breakdown_generator.h"

#include <math.h>

#include <cmath>
#include <string>

#include <glog/logging.h>
#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "waymo_open_dataset/dataset.pb.h"
#include "waymo_open_dataset/label.pb.h"
#include "waymo_open_dataset/protos/breakdown.pb.h"
#include "waymo_open_dataset/protos/metrics.pb.h"

namespace waymo {
namespace open_dataset {
namespace {
double Hypot3(double a, double b, double c) {
  return std::sqrt(a * a + b * b + c * c);
}

// Number of cameras in BreakdownGeneratorCamera.
constexpr int kNumCameras = 5;
}  // namespace

// This breakdown generator considers everything as one shard.
class BreakdownGeneratorOneShard : public BreakdownGenerator {
 public:
  ~BreakdownGeneratorOneShard() override {}

  int Shard(const Object& object) const override { return 0; }

  Breakdown::GeneratorId Id() const override { return Breakdown::ONE_SHARD; }

  int NumShards() const override { return 1; }

  std::string ShardName(int shard) const override { return "ONE_SHARD"; }
};

// This breakdown generator considers everything as one shard.
class BreakdownGeneratorAllButSign : public BreakdownGenerator {
 public:
  ~BreakdownGeneratorAllButSign() override {}

  int Shard(const Object& object) const override {
    if (object.object().type() != Label::TYPE_SIGN) {
      return 0;
    }
    return 1;
  }

  Breakdown::GeneratorId Id() const override { return Breakdown::ALL_BUT_SIGN; }

  int NumShards() const override { return 2; }

  std::string ShardName(int shard) const override {
    switch (shard) {
      case 0:
        return "ALL_BUT_SIGN";
      case 1:
        return "SIGN";
      default:
        LOG(FATAL) << "Code should not reach here.";
    }
  }
};

// This breakdown generator breaks down the objects based on its object type.
class BreakdownGeneratorObjectType : public BreakdownGenerator {
 public:
  ~BreakdownGeneratorObjectType() override {}

  int Shard(const Object& object) const override {
    // This returns -1 for TYPE_UNKNOWN. -1 indicates that TYPE_UNKNOWN is not
    // in any shard.
    return static_cast<int>(object.object().type() - 1);
  }

  Breakdown::GeneratorId Id() const override { return Breakdown::OBJECT_TYPE; }

  int NumShards() const override { return static_cast<int>(Label::Type_MAX); }

  std::string ShardName(int shard) const override {
    return absl::StrCat(Breakdown::GeneratorId_Name(Id()), "_",
                        Label::Type_Name(static_cast<Label::Type>(shard + 1)));
  }
};

// This breakdown generator breaks down the objects based on their center
// distance (w.r.t. SDC if the box is in vehicle frame).
class BreakdownGeneratorRange : public BreakdownGenerator {
 public:
  ~BreakdownGeneratorRange() override {}

  int Shard(const Object& object) const override {
    double range = Hypot3(object.object().box().center_x(),
                          object.object().box().center_y(),
                          object.object().box().center_z());
    constexpr float kNearRange = 30.0;
    constexpr float kMidRange = 50.0;
    if (object.object().type() == Label::TYPE_UNKNOWN) {
      return -1;
    }
    const int shard_offset = 3 * (object.object().type() - 1);
    if (shard_offset < 0) {
      return -1;
    }
    if (range < kNearRange) {
      return 0 + shard_offset;
    } else if (range < kMidRange) {
      return 1 + shard_offset;
    } else {
      return 2 + shard_offset;
    }
  }

  Breakdown::GeneratorId Id() const override { return Breakdown::RANGE; }

  int NumShards() const override {
    return 3 * static_cast<int>(Label::Type_MAX);
  }

  std::string ShardName(int shard) const override {
    const Label::Type object_type = static_cast<Label::Type>(shard / 3 + 1);
    CHECK_LE(object_type, Label::Type_MAX) << shard;
    CHECK_GE(object_type, 1) << shard;

    const std::string prefix = absl::StrCat(Breakdown::GeneratorId_Name(Id()),
                                            "_", Label::Type_Name(object_type));
    const int range_shard = shard % 3;
    switch (range_shard) {
      case 0:
        return absl::StrCat(prefix, "_", "[0, 30)");
      case 1:
        return absl::StrCat(prefix, "_", "[30, 50)");
      case 2:
        return absl::StrCat(prefix, "_", "[50, +inf)");
      default:
        LOG(FATAL) << "Code should not reach here.";
    }
  }
};

// This breakdown generator breaks down the objects based on their center
// distance (w.r.t. SDC if the box is in vehicle frame).
class BreakdownGeneratorSize : public BreakdownGenerator {
 public:
  ~BreakdownGeneratorSize() override {}

  int Shard(const Object& object) const override {
    double size = std::max(
        std::max(object.object().box().length(), object.object().box().width()),
        object.object().box().height());
    constexpr float kLarge = 7.0;
    if (object.object().type() == Label::TYPE_UNKNOWN) {
      return -1;
    }
    const int shard_offset = 2 * (object.object().type() - 1);
    if (shard_offset < 0) {
      return -1;
    }
    if (size < kLarge) {
      return 0 + shard_offset;
    } else {
      return 1 + shard_offset;
    }
  }

  Breakdown::GeneratorId Id() const override { return Breakdown::SIZE; }

  int NumShards() const override {
    return 2 * static_cast<int>(Label::Type_MAX);
  }

  std::string ShardName(int shard) const override {
    const Label::Type object_type = static_cast<Label::Type>(shard / 2 + 1);
    CHECK_LE(object_type, Label::Type_MAX) << shard;
    CHECK_GE(object_type, 1) << shard;

    const std::string prefix = absl::StrCat(Breakdown::GeneratorId_Name(Id()),
                                            "_", Label::Type_Name(object_type));
    const int range_shard = shard % 2;
    switch (range_shard) {
      case 0:
        return absl::StrCat(prefix, "_", "small");
      case 1:
        return absl::StrCat(prefix, "_", "large");
      default:
        LOG(FATAL) << "Code should not reach here.";
    }
  }
};

// This generator breaks down the results based on the magnitude of
// the velocity vector.
class BreakdownGeneratorVelocity : public BreakdownGenerator {
 public:
  ~BreakdownGeneratorVelocity() override {}

  int Shard(const Object& object) const override {
    double v_x, v_y;
    if (object.object().metadata().has_speed_x() &&
        object.object().metadata().has_speed_y()) {
      v_x = object.object().metadata().speed_x();
      v_y = object.object().metadata().speed_y();
    } else {
      LOG(WARNING) << "Object does not have speed: " << object.DebugString();
      v_x = 0;
      v_y = 0;
    }
    // In meters per second in world coordinates.
    const double v_mag = sqrt(v_x * v_x + v_y * v_y);

    constexpr float kStationary = 0.2;
    constexpr float kSlow = 1.;
    constexpr float kMedium = 3.;
    constexpr float kFast = 10.;
    if (object.object().type() == Label::TYPE_UNKNOWN) {
      return -1;
    }
    const int shard_offset = 5 * (object.object().type() - 1);

    if (v_mag < kStationary) {
      return 0 + shard_offset;  // Stationary
    } else if (v_mag < kSlow) {
      return 1 + shard_offset;  // Slow
    } else if (v_mag < kMedium) {
      return 2 + shard_offset;  // Medium
    } else if (v_mag < kFast) {
      return 3 + shard_offset;  // Fast
    } else {
      return 4 + shard_offset;  // Very fast
    }
  }

  std::vector<int> ShardsForMatching(const Object& object) const override {
    if (object.object().type() == Label::TYPE_UNKNOWN) {
      return std::vector<int>();
    }
    std::vector<int> shards(5);
    for (int i = 0; i < 5; ++i) {
      shards[i] = 5 * (object.object().type() - 1) + i;
    }
    return shards;
  }

  int NumShards() const override {
    return 5 * static_cast<int>(Label::Type_MAX);
  }

  Breakdown::GeneratorId Id() const override { return Breakdown::VELOCITY; }

  std::string ShardName(int shard) const override {
    const Label::Type object_type = static_cast<Label::Type>(shard / 5 + 1);
    CHECK_LE(object_type, Label::Type_MAX) << shard;
    CHECK_GE(object_type, 1) << shard;

    const std::string prefix = absl::StrCat(Breakdown::GeneratorId_Name(Id()),
                                            "_", Label::Type_Name(object_type));
    const int velocity_shard = shard % 5;
    switch (velocity_shard) {
      case 0:
        return absl::StrCat(prefix, "_", "STATIONARY");
      case 1:
        return absl::StrCat(prefix, "_", "SLOW");
      case 2:
        return absl::StrCat(prefix, "_", "MEDIUM");
      case 3:
        return absl::StrCat(prefix, "_", "FAST");
      case 4:
        return absl::StrCat(prefix, "_", "VERY_FAST");
      default:
        LOG(FATAL) << "Code should not reach here.";
    }
  }

  bool IsGroundTruthOnlyBreakdown() const { return true; }
};

// This generator breaks down the results based on the different camera of
// the detection or ground truth.
class BreakdownGeneratorCamera : public BreakdownGenerator {
 public:
  ~BreakdownGeneratorCamera() override {}

  int Shard(const Object& object) const override {
    if (object.object().type() == Label::TYPE_UNKNOWN) {
      return -1;
    }
    const int shard_offset = kNumCameras * (object.object().type() - 1);
    CameraName::Name camera_name;
    if (Is2DCamera(object)) {
      camera_name = object.camera_name();
    } else {  // 3D detection.
      if (object.object().has_most_visible_camera_name()) {
        const std::string& camera_name_str =
            object.object().most_visible_camera_name();
        if (!CameraName::Name_Parse(camera_name_str, &camera_name)) {
          LOG(WARNING)
              << "Object does not have valid most visible camera name: "
              << object.DebugString();
          return -1;
        }
      } else {
        // If the mostly visible camera is not pre-computed, we estimate the
        // corresponding camera by FOV.
        auto camera_names = EstimateCameraNamesByFov(object);
        if (!camera_names.empty())
          camera_name = camera_names[0];
        else
          return -1;
      }
    }
    const int camera_shard_offset = GetShardOffsetByCameraName(camera_name);
    if (camera_shard_offset < 0) {
      LOG(WARNING) << "Object does not have valid camera: "
                   << object.DebugString();
      return -1;
    }
    return shard_offset + camera_shard_offset;
  }

  std::vector<int> ShardsForMatching(const Object& object) const override {
    if (object.object().type() == Label::TYPE_UNKNOWN) {
      return std::vector<int>();
    }
    if (Is2DCamera(object)) {
      return std::vector<int>({Shard(object)});
    }
    // For 3D detections, we estimate the camera breakdowns by FOV.
    const int shard_offset = kNumCameras * (object.object().type() - 1);
    auto camera_names = EstimateCameraNamesByFov(object);
    std::vector<int> shards(camera_names.size());
    for (int i = 0; i < camera_names.size(); ++i) {
      shards[i] = shard_offset + GetShardOffsetByCameraName(camera_names[i]);
    }
    return shards;
  }

  int NumShards() const override {
    return kNumCameras * static_cast<int>(Label::Type_MAX);
  }

  Breakdown::GeneratorId Id() const override { return Breakdown::CAMERA; }

  std::string ShardName(int shard) const override {
    const Label::Type object_type =
        static_cast<Label::Type>(shard / kNumCameras + 1);
    CHECK_LE(object_type, Label::Type_MAX) << shard;
    CHECK_GE(object_type, 1) << shard;

    const std::string prefix = absl::StrCat(Breakdown::GeneratorId_Name(Id()),
                                            "_", Label::Type_Name(object_type));
    const int camera_shard = shard % kNumCameras;
    switch (camera_shard) {
      case 0:
        return absl::StrCat(prefix, "_", "FRONT");
      case 1:
        return absl::StrCat(prefix, "_", "FRONT-LEFT");
      case 2:
        return absl::StrCat(prefix, "_", "FRONT-RIGHT");
      case 3:
        return absl::StrCat(prefix, "_", "SIDE-LEFT");
      case 4:
        return absl::StrCat(prefix, "_", "SIDE-RIGHT");
      default:
        LOG(FATAL) << "Code should not reach here.";
    }
  }

  bool IsGroundTruthOnlyBreakdown() const { return true; }

 private:
  bool Is2DCamera(const Object& object) const {
    return object.has_camera_name();
  }

  int GetShardOffsetByCameraName(CameraName::Name camera_name) const {
    switch (camera_name) {
      case CameraName::FRONT:
        return 0;
      case CameraName::FRONT_LEFT:
        return 1;
      case CameraName::FRONT_RIGHT:
        return 2;
      case CameraName::SIDE_LEFT:
        return 3;
      case CameraName::SIDE_RIGHT:
        return 4;
      default:
        LOG(FATAL) << "Code should not reach here.";
    }
  }

  std::vector<CameraName::Name> EstimateCameraNamesByFov(
      const Object& object) const {
    std::vector<CameraName::Name> camera_names;
    constexpr double kRadToDegree = 180.0 / M_PI;
    // The field-of-view of the camera images is approximately +-25.2 degrees.
    constexpr double kHalfFovDeg = 25.2;
    // Considering the vehicle coordinate system, where the x-axis represents
    // the front direction, the y-axis represents the left direction, we
    // estimate the angle of an object to the x-axis by calculating arctan(y/x).
    // Objects on the left side yield a positive angle, and objects on the right
    // side yield a negative angle.
    constexpr double kFrontCameraDeg = 0.0;
    constexpr double kFrontLeftCameraDeg = 45.0;
    constexpr double kFrontRightCameraDeg = -45.0;
    constexpr double kSideLeftCameraDeg = 90.0;
    constexpr double kSideRightCameraDeg = -90.0;

    const double c_x = object.object().box().center_x();
    const double c_y = object.object().box().center_y();
    const double c_deg = std::atan2(c_y, c_x) * kRadToDegree;
    if (c_deg > (kFrontCameraDeg - kHalfFovDeg) &&
        c_deg < (kFrontCameraDeg + kHalfFovDeg))
      camera_names.push_back(CameraName::FRONT);
    if (c_deg > (kFrontLeftCameraDeg - kHalfFovDeg) &&
        c_deg < (kFrontLeftCameraDeg + kHalfFovDeg))
      camera_names.push_back(CameraName::FRONT_LEFT);
    if (c_deg > (kFrontRightCameraDeg - kHalfFovDeg) &&
        c_deg < (kFrontRightCameraDeg + kHalfFovDeg))
      camera_names.push_back(CameraName::FRONT_RIGHT);
    if (c_deg > (kSideLeftCameraDeg - kHalfFovDeg) &&
        c_deg < (kSideLeftCameraDeg + kHalfFovDeg))
      camera_names.push_back(CameraName::SIDE_LEFT);
    if (c_deg > (kSideRightCameraDeg - kHalfFovDeg) &&
        c_deg < (kSideRightCameraDeg + kHalfFovDeg))
      camera_names.push_back(CameraName::SIDE_RIGHT);
    return camera_names;
  }
};

std::unique_ptr<BreakdownGenerator> BreakdownGenerator::Create(
    Breakdown::GeneratorId id) {
  switch (id) {
    case Breakdown::OBJECT_TYPE:
      return absl::make_unique<BreakdownGeneratorObjectType>();
    case Breakdown::ONE_SHARD:
      return absl::make_unique<BreakdownGeneratorOneShard>();
    case Breakdown::RANGE:
      return absl::make_unique<BreakdownGeneratorRange>();
    case Breakdown::VELOCITY:
      return absl::make_unique<BreakdownGeneratorVelocity>();
    case Breakdown::ALL_BUT_SIGN:
      return absl::make_unique<BreakdownGeneratorAllButSign>();
    case Breakdown::SIZE:
      return absl::make_unique<BreakdownGeneratorSize>();
    case Breakdown::CAMERA:
      return absl::make_unique<BreakdownGeneratorCamera>();
    default:
      LOG(FATAL) << "Unimplemented breakdown generator "
                 << Breakdown::GeneratorId_Name(id);
  }
}

}  // namespace open_dataset
}  // namespace waymo
