CURRENT_DIR=$(cd $(dirname $0); pwd)
echo $CURRENT_DIR
export PYTHONPATH=$PYTHONPATH:$CURRENT_DIR

python $CURRENT_DIR/run_accuracy_checker.py -c unet-camvid-onnx-0001.yml --csv_result=unet_nb_vpu4_inference_results.csv
python $CURRENT_DIR/run_accuracy_checker.py -c tiny-yolo-v4.yml --csv_result=yolo_nb_vpu4_inference_results.csv
