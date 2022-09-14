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

#ifndef WAYMO_OPEN_DATASET_METRICS_IOU_H_
#define WAYMO_OPEN_DATASET_METRICS_IOU_H_

#include <functional>

#include "waymo_open_dataset/label.pb.h"
#include "waymo_open_dataset/math/polygon2d.h"
#include "waymo_open_dataset/protos/metrics.pb.h"

namespace waymo {
namespace open_dataset {

// Discretizes IoU from float: [0.0, 1.0] to int: [0, kMaxIoU] as some matching
// algorithms (e.g. Hungarian) require integer version IoU.
extern const int kMaxIoU;

// The function type for customized IOU computation function.
using ComputeIoUFunc =
    std::function<double(const Label::Box&, const Label::Box&)>;

// Computes the intersection of union of two boxes.
// box_type is not specified in b1 and b2 as that is usually the same for all
// boxes in one problem setup.
//
// REQUIRES: The input boxes' length/width/height are >= 0.
double ComputeIoU(const Label::Box& b1, const Label::Box& b2,
                  Label::Box::Type box_type);

// Computes the longitudinal affinity between a precition bounding box and a
// ground truth bounding box.
//
// The localization error (e_loc) is decomposed into longitudinal error (e_lon)
// and lateral error (e_lat), both in world units. We only consider the
// longitudinal error (e_lon).
//
// Given the longitudinal error tolerance (tol_lon_percentage), where
//     tol_lon = max(r_gt * tol_lon_percentage, min_tol_lon_meters)
// is derived from the ground truth range, the longitudinal affinity can be
// computed as min(1-(e_lon/tol_r), 0.0).
double ComputeLongitudinalAffinity(
    const Label::Box& prediction_box, const Label::Box& ground_truth_box,
    const Config::LongitudinalErrorTolerantConfig& let_metric_config);

// The function type for customized Longitudinal Affinity computation function.
using ComputeLongitudinalAffinityFunc =
    std::function<double(const Label::Box&, const Label::Box&)>;

// Returns a function to calculate longitudinal affinity based on the given LET
// metrics config.
ComputeLongitudinalAffinityFunc GetComputeLongitudinalAffinityFunc(
    const Config::LongitudinalErrorTolerantConfig& let_metric_config);

// Aligns the prediction box with respect to a ground truth box so that the
// longitudinal error is minimized.
// This function assumes that the boxes are calibrated with the sensor
// origin, i.e., the sensor is located at (0, 0, 0).
//
// NOTE: this function will correct any degree of the longitudinal error.
// It is meant to be applied to box pairs that pass a certain longitudinal
// affinity threshold.
Label::Box AlignedPredictionBox(
    const Label::Box& prediction_box, const Label::Box& ground_truth_box,
    Config::LongitudinalErrorTolerantConfig::AlignType align_type);

// Computes the longitudinal error tolerant IoU (LET-IoU).
// The boxes will be first transformed into the sensor coordinate system,
// then the IoU will be computed between the aligned prediction box and the
// ground truth box. Box type will be used to determine if we compute the 2D IoU
// or 3D IoU. It has the same effect as the one in `ComputeIoU`.
//
// NOTE:
// 1. This function will correct any degree of the longitudinal error.
//    It is meant to be applied at box pairs which pass a certain longitudinal
//    affinity threshold.
// 2. This function currently only supports TYPE_3D bounding boxes and TYPE_2D
//    bounding boxes representing the top-down view objects.
double ComputeLetIoU(
    const Label::Box& prediction_box, const Label::Box& ground_truth_box,
    const Config::LongitudinalErrorTolerantConfig::Location3D& sensor_location,
    Config::LongitudinalErrorTolerantConfig::AlignType align_type,
    Label::Box::Type box_type);

// Returns a function to calculate LET-IoU based on the given LET metrics
// config.
ComputeIoUFunc GetComputeLetIoUFunc(
    const Config::LongitudinalErrorTolerantConfig::Location3D& sensor_location,
    Config::LongitudinalErrorTolerantConfig::AlignType align_type,
    Label::Box::Type box_type);

// Converts a box proto to a polygon.
Polygon2d ToPolygon2d(const open_dataset::Label::Box& box);

}  // namespace open_dataset
}  // namespace waymo

#endif  // WAYMO_OPEN_DATASET_METRICS_IOU_H_
