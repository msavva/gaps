#!/usr/bin/env bash
#
# gaps/scripts/install-shape2pose.sh
#  Retrieve shape2pose code and dependencies, build,
#  and copy data to data/shape2pose

GAPS_SERVER="tux.cs.princeton.edu:/u/funk/gaps/"
MODELS_SERVER="tux.cs.princeton.edu:/u/funk/models/"

# make sure git state is up-to-date
cd ../; git pull

# get and build shape2pose pkg dependencies
cd pkgs
scp -r ${GAPS_SERVER}/pkgs/CSparse .
scp -r ${GAPS_SERVER}/pkgs/splm .
scp -r ${GAPS_SERVER}/pkgs/minpack .
cd CSparse; make clean; make; cd ..
cd splm; make clean; make; cd ..
cd minpack; make clean; make; cd ..

# get and build shape2pose app
cd ../apps
scp -r ${GAPS_SERVER}/apps/shape2pose .
cd shape2pose; make clean; make; cd ..

# get shape2pose data
cd ../
mkdir -p data
cd data
scp -r ${MODELS_SERVER}/shape2pose.zip .
unzip shape2pose.zip
rm shape2pose.zip
cd shape2pose

# run a test (assumes gaps bin output is in PATH)
shape2pose input/meshes/bikes/bicycle000002.off input/body/regions_skeleton.txt -input_ground_truth_pose input/ground_truth_poses/bikes/bicycle000002.pose -input_predicted_poses input/vovas_predicted_poses/bikes/bicycle000002.pose -v
