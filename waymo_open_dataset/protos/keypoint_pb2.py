# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: waymo_open_dataset/protos/keypoint.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n(waymo_open_dataset/protos/keypoint.proto\x12\x1cwaymo.open_dataset.keypoints\"\x1d\n\x05Vec2d\x12\t\n\x01x\x18\x01 \x01(\x01\x12\t\n\x01y\x18\x02 \x01(\x01\"(\n\x05Vec3d\x12\t\n\x01x\x18\x01 \x01(\x01\x12\t\n\x01y\x18\x02 \x01(\x01\x12\t\n\x01z\x18\x03 \x01(\x01\")\n\x12KeypointVisibility\x12\x13\n\x0bis_occluded\x18\x01 \x01(\x08\"\x97\x01\n\nKeypoint2d\x12\x38\n\x0blocation_px\x18\x01 \x01(\x0b\x32#.waymo.open_dataset.keypoints.Vec2d\x12\x44\n\nvisibility\x18\x02 \x01(\x0b\x32\x30.waymo.open_dataset.keypoints.KeypointVisibility*\t\x08\xe8\x07\x10\x80\x80\x80\x80\x02\"\x96\x01\n\nKeypoint3d\x12\x37\n\nlocation_m\x18\x01 \x01(\x0b\x32#.waymo.open_dataset.keypoints.Vec3d\x12\x44\n\nvisibility\x18\x02 \x01(\x0b\x32\x30.waymo.open_dataset.keypoints.KeypointVisibility*\t\x08\xe8\x07\x10\x80\x80\x80\x80\x02\"\xc8\x01\n\x0e\x43\x61meraKeypoint\x12\x38\n\x04type\x18\x01 \x01(\x0e\x32*.waymo.open_dataset.keypoints.KeypointType\x12=\n\x0bkeypoint_2d\x18\x02 \x01(\x0b\x32(.waymo.open_dataset.keypoints.Keypoint2d\x12=\n\x0bkeypoint_3d\x18\x03 \x01(\x0b\x32(.waymo.open_dataset.keypoints.Keypoint3d\"Q\n\x0f\x43\x61meraKeypoints\x12>\n\x08keypoint\x18\x01 \x03(\x0b\x32,.waymo.open_dataset.keypoints.CameraKeypoint\"\x88\x01\n\rLaserKeypoint\x12\x38\n\x04type\x18\x01 \x01(\x0e\x32*.waymo.open_dataset.keypoints.KeypointType\x12=\n\x0bkeypoint_3d\x18\x02 \x01(\x0b\x32(.waymo.open_dataset.keypoints.Keypoint3d\"O\n\x0eLaserKeypoints\x12=\n\x08keypoint\x18\x01 \x03(\x0b\x32+.waymo.open_dataset.keypoints.LaserKeypoint*\xee\x03\n\x0cKeypointType\x12\x1d\n\x19KEYPOINT_TYPE_UNSPECIFIED\x10\x00\x12\x16\n\x12KEYPOINT_TYPE_NOSE\x10\x01\x12\x1f\n\x1bKEYPOINT_TYPE_LEFT_SHOULDER\x10\x05\x12\x1c\n\x18KEYPOINT_TYPE_LEFT_ELBOW\x10\x06\x12\x1c\n\x18KEYPOINT_TYPE_LEFT_WRIST\x10\x07\x12\x1a\n\x16KEYPOINT_TYPE_LEFT_HIP\x10\x08\x12\x1b\n\x17KEYPOINT_TYPE_LEFT_KNEE\x10\t\x12\x1c\n\x18KEYPOINT_TYPE_LEFT_ANKLE\x10\n\x12 \n\x1cKEYPOINT_TYPE_RIGHT_SHOULDER\x10\r\x12\x1d\n\x19KEYPOINT_TYPE_RIGHT_ELBOW\x10\x0e\x12\x1d\n\x19KEYPOINT_TYPE_RIGHT_WRIST\x10\x0f\x12\x1b\n\x17KEYPOINT_TYPE_RIGHT_HIP\x10\x10\x12\x1c\n\x18KEYPOINT_TYPE_RIGHT_KNEE\x10\x11\x12\x1d\n\x19KEYPOINT_TYPE_RIGHT_ANKLE\x10\x12\x12\x1a\n\x16KEYPOINT_TYPE_FOREHEAD\x10\x13\x12\x1d\n\x19KEYPOINT_TYPE_HEAD_CENTER\x10\x14')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'waymo_open_dataset.protos.keypoint_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _KEYPOINTTYPE._serialized_start=1004
  _KEYPOINTTYPE._serialized_end=1498
  _VEC2D._serialized_start=74
  _VEC2D._serialized_end=103
  _VEC3D._serialized_start=105
  _VEC3D._serialized_end=145
  _KEYPOINTVISIBILITY._serialized_start=147
  _KEYPOINTVISIBILITY._serialized_end=188
  _KEYPOINT2D._serialized_start=191
  _KEYPOINT2D._serialized_end=342
  _KEYPOINT3D._serialized_start=345
  _KEYPOINT3D._serialized_end=495
  _CAMERAKEYPOINT._serialized_start=498
  _CAMERAKEYPOINT._serialized_end=698
  _CAMERAKEYPOINTS._serialized_start=700
  _CAMERAKEYPOINTS._serialized_end=781
  _LASERKEYPOINT._serialized_start=784
  _LASERKEYPOINT._serialized_end=920
  _LASERKEYPOINTS._serialized_start=922
  _LASERKEYPOINTS._serialized_end=1001
# @@protoc_insertion_point(module_scope)
