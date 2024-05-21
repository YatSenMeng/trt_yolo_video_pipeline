//
// Created by lijin on 2023/12/22.
//

#ifndef VIDEOPIPELINE_YOLODETECTPIPELINE_H
#define VIDEOPIPELINE_YOLODETECTPIPELINE_H

#include "ffmpeg/record/Mp4RecordControlData.h"
#include "graph/pipeline/Pipeline.h"
#include "infer/Infer.h"


namespace pipeline {

class YoloDetectPipeline : public Pipeline {
public:
    typedef std::shared_ptr<YoloDetectPipeline> ptr;

    explicit YoloDetectPipeline(std::string              task_name,
                                std::string              input_url,
                                std::string              output_url,
                                std::string              keyframe_url, 
                                std::string              task_no,
                                const infer::Infer::ptr &trt_instance,
                                int                      output_width   = 1280,
                                int                      output_height  = 720,
                                int                      output_fps     = 25,
                                int                      output_bitrate = 1024 * 1024 * 2);

public:
    void add_record_task(const Data::Mp4RecordControlData::ptr &record_task);

private:
    bool Init() override;

private:
    infer::Infer::ptr m_trt_instance;
    std::string       m_input_url;      // rtsp url or video file path
    std::string       m_output_url;     // rtmp url
    std::string       m_keyframe_url;   // keyframe_url
    std::string       m_task_no;        // task_no
    int               m_output_width   = 1280;
    int               m_output_height  = 720;
    int               m_output_fps     = 25;
    int               m_output_bitrate = 1024 * 1024 * 2;
};

}  // namespace pipeline

#endif  // VIDEOPIPELINE_YOLODETECTPIPELINE_H
