./build/test_yolo_detect rtmp://video-pull.eflyyzh.com/0507/1 rtmp://video-push.eflyyzh.com/ai/test http://127.0.0.1:8899/item/ 10000 ./resource/weights/yolov5s.engine ./resource/labels/yzh_car.labels
#./build/test_yolo_detect_multi rtmp://video-pull.eflyyzh.com/ai/src rtmp://video-push.eflyyzh.com/ai/dst ./resource/weights/yolov5s.engine ./resource/labels/yzh_car.labels
# rtmp://video-pull.eflyyzh.com/0507/1
# rtmp://video-push.eflyyzh.com/ai/test
# rtmp://video-pull.eflyyzh.com/ai/src
# rtmp://video-push.eflyyzh.com/ai/dst
# rtmp://172.17.0.1:1935/live/source
# sudo docker exec -it ai_service /bin/bash
