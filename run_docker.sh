sudo docker run -it --gpus 0 --rm --name ai_service \
-e WEBSOCKET_ADDRESS=wss://wss.eflyyzh.com:19511?token=18a2f1038bfac08eaa2ad3cff21cfda7  \
-e AI_NO=AI_000191010 \
-e STREAM_OUTPUT_WIDTH=1280 \
-e STREAM-OUTPUT-HEIGHT=720 \
-e STREAM_OUTPUT_BITRATE=4000000 \
-e SERVICE_GPU_CONFIG=0:4 \
-e KEYFRAMES_DUTION_SECONDS=5 \
-v $(pwd)/ai_service_logs:/ai_service_logs \
-v /home/my/workflow2024/01_project/AI_deployment/AIService/yzh_ai_service:/yzh_ai_service/ \
yzh_ai_service:A4000 \
/bin/bash

# CMD uvicorn service:app --ws-ping-interval 300 --ws-ping-timeout 300 --host 0.0.0.0 --port 8000
# -e WEBSOCKET_ADDRESS=ws://172.17.0.1:8765  \
