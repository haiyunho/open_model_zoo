CURRENT_DIR=$(cd $(dirname $0); pwd)
echo $CURRENT_DIR
export PYTHONPATH=$PYTHONPATH:$CURRENT_DIR

python $CURRENT_DIR/run_accuracy_checker.py -c unet-camvid-onnx-0001.yml
python $CURRENT_DIR/run_accuracy_checker.py -c tiny-yolo-v4.yml
