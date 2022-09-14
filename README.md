# Waymo Dataset Convert

> No need install waymo-open-dataset, so can use on macOS or Windows

## Prepare Dataset

> Download any Waymo(*.tfrecord) as you need

## Convert to KITTI Format

> Install tf, numpy, matplotlib, tqdm... used in converter.py

```bash
python converter.py --dataset_path ../waymo_dataset [--out_path ../waymo_kitti --workers 16 --is_test false]
```

for more info, see:

https://xixioba.github.io/2022/09/14/Waymo-Dataset-Parse/

## More on the converted data

The converted data should have the following file structure:
```
save_dir
├── calib
├── image_0
├── image_1
├── image_2
├── image_3
├── image_4
├── label_0
├── label_1
├── label_2
├── label_3
├── label_4
├── label_all
├── pose
├── velodyne
```
Important Notes: 
- KITTI only annotates 3D bounding boxes visible in the front camera,
whereas WOD annotates 3D bounding boxes both within and out of the field of views of all 5 cameras
- KITTI's front camera has a index 2, whereas the WOD's front camera has a index 0
- The calibration files are for the front camera
- label_x only contains 3D bounding boxes visible in camera idx x.
- label_all contains all 3D bounding boxes, even those out of the field of views of all cameras.
- calib contains calibration files for the front camera only at the moment
- pose file is not included in the regular KITTI set. It contains self driving car's pose, 
in the form of a transformation matrix (4x4) from the vehicle frame to the global frame

To read a pose file:
```python3
import numpy as np
pose = np.loadtxt(<pathname>, dtype=np.float32)
```

The label files in label_x (x in {0,1,2,3}) are similar to the regular KITTI label:

```
#Values    Name      Description
----------------------------------------------------------------------------
   1    type         Describes the type of object: 'Car', 'Pedestrian', 'Cyclist'
   1    truncated    Always 0
   1    occluded     Always 0
   1    alpha        Always -10
   4    bbox         2D bounding box of object in the image (0-based index):
                     contains left, top, right, bottom pixel coordinates
   3    dimensions   3D object dimensions: height, width, length (in meters)
   3    location     3D object location x,y,z in camera coordinates (in meters)
   1    rotation_y   Rotation ry around Y-axis in camera coordinates [-pi..pi]
```

The label files in label_all have the following format:
```
#Values    Name      Description
----------------------------------------------------------------------------
   1    type         Describes the type of object: 'Car', 'Pedestrian', 'Cyclist'
   1    truncated    Always 0
   1    occluded     Always 0
   1    alpha        Always -10
   4    bbox         2D bounding box of object in the image (0-based index):
                     contains left, top, right, bottom pixel coordinates
   3    dimensions   3D object dimensions: height, width, length (in meters)
   3    location     3D object location x,y,z in camera coordinates (in meters)
   1    rotation_y   Rotation ry around Y-axis in camera coordinates [-pi..pi]
   1    cam_idx	     The index of the camera in which the 3D bounding box is visible. 
                     Set to 0 if not visible in any cameras   
```
