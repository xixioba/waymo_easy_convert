# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: waymo_open_dataset/protos/segmentation_metrics.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from waymo_open_dataset import dataset_pb2 as waymo__open__dataset_dot_dataset__pb2
from waymo_open_dataset.protos import segmentation_pb2 as waymo__open__dataset_dot_protos_dot_segmentation__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n4waymo_open_dataset/protos/segmentation_metrics.proto\x12\x12waymo.open_dataset\x1a waymo_open_dataset/dataset.proto\x1a,waymo_open_dataset/protos/segmentation.proto\"\x81\x01\n\x11SegmentationFrame\x12\x36\n\x13segmentation_labels\x18\x01 \x03(\x0b\x32\x19.waymo.open_dataset.Laser\x12\x14\n\x0c\x63ontext_name\x18\x02 \x01(\t\x12\x1e\n\x16\x66rame_timestamp_micros\x18\x03 \x01(\x03\"N\n\x15SegmentationFrameList\x12\x35\n\x06\x66rames\x18\x01 \x03(\x0b\x32%.waymo.open_dataset.SegmentationFrame\"b\n\x19SegmentationMetricsConfig\x12\x45\n\x12segmentation_types\x18\x01 \x03(\x0e\x32%.waymo.open_dataset.Segmentation.TypeB\x02\x10\x01\"A\n\x18SegmentationMeasurements\x12\x15\n\rintersections\x18\x01 \x03(\x03\x12\x0e\n\x06unions\x18\x02 \x03(\x03\"\xf9\x01\n\x13SegmentationMetrics\x12O\n\rper_class_iou\x18\x01 \x03(\x0b\x32\x38.waymo.open_dataset.SegmentationMetrics.PerClassIouEntry\x12\x0c\n\x04miou\x18\x02 \x01(\x02\x12O\n\x19segmentation_measurements\x18\x03 \x01(\x0b\x32,.waymo.open_dataset.SegmentationMeasurements\x1a\x32\n\x10PerClassIouEntry\x12\x0b\n\x03key\x18\x01 \x01(\x05\x12\r\n\x05value\x18\x02 \x01(\x02:\x02\x38\x01')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'waymo_open_dataset.protos.segmentation_metrics_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _SEGMENTATIONMETRICSCONFIG.fields_by_name['segmentation_types']._options = None
  _SEGMENTATIONMETRICSCONFIG.fields_by_name['segmentation_types']._serialized_options = b'\020\001'
  _SEGMENTATIONMETRICS_PERCLASSIOUENTRY._options = None
  _SEGMENTATIONMETRICS_PERCLASSIOUENTRY._serialized_options = b'8\001'
  _SEGMENTATIONFRAME._serialized_start=157
  _SEGMENTATIONFRAME._serialized_end=286
  _SEGMENTATIONFRAMELIST._serialized_start=288
  _SEGMENTATIONFRAMELIST._serialized_end=366
  _SEGMENTATIONMETRICSCONFIG._serialized_start=368
  _SEGMENTATIONMETRICSCONFIG._serialized_end=466
  _SEGMENTATIONMEASUREMENTS._serialized_start=468
  _SEGMENTATIONMEASUREMENTS._serialized_end=533
  _SEGMENTATIONMETRICS._serialized_start=536
  _SEGMENTATIONMETRICS._serialized_end=785
  _SEGMENTATIONMETRICS_PERCLASSIOUENTRY._serialized_start=735
  _SEGMENTATIONMETRICS_PERCLASSIOUENTRY._serialized_end=785
# @@protoc_insertion_point(module_scope)