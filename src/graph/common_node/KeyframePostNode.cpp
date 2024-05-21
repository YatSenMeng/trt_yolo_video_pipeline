#pragma once
#include "KeyframePostNode.hpp"
#include "utils/base64.h"
#include "utils/json11.hpp"

namespace Node {

KeyframePostNode::KeyframePostNode(const std::string &name,
                                   const std::string &client_url,
                                   const std::string &post_url,
                                   const std::string &task_no,
                                   int64              post_cycle)
    : m_client(client_url),
      Node(name, GraphCore::NODE_TYPE::DES_NODE),
      m_client_url(client_url),
      m_post_url(post_url),
      m_post_cycle(post_cycle),
      m_task_no(task_no) {
    m_client.set_connection_timeout(3, 0);  // 设置连接超时时间
    m_client.set_read_timeout(3, 0);        // 设置读取超时时间
    m_client.set_write_timeout(3, 0);       // 设置写入超时时间
    m_client.set_keep_alive(true);          // 设置长连接
}

// keyframe post node
Data::BaseData::ptr KeyframePostNode::handle_data(Data::BaseData::ptr data) {
    // 判断是否达到上传周期

    if (m_post_cycle > 0) {
        auto now = time(0);        
        if (m_last_post_time == 0)
        {
            m_last_post_time = time(0);
        }

        if (now - m_last_post_time < m_post_cycle) {
            return nullptr;
        }
        m_last_post_time = now;

    }
    
    // 获取检测结果
    auto status =
        data->Get<DETECTBOX_FUTURE_TYPE>(DETECTBOX_FUTURE).wait_for(std::chrono::milliseconds(120));
    if (status == std::future_status::timeout) {
        WarnL << "推理结果等待超时";
        return nullptr;
    }
    auto box_array = data->Get<DETECTBOX_FUTURE_TYPE>(DETECTBOX_FUTURE).get();
    if (box_array.empty())
    {
        return nullptr;
    }
    // 生成json数据
    auto               image = data->Get<MAT_IMAGE_TYPE>(MAT_IMAGE);
    std::vector<uchar> img_data;
    cv::imencode(".jpg", image, img_data);
    std::string img_base64 = encodeBase64(std::string(img_data.begin(), img_data.end()));


    auto detect_boxes = json11::Json::array();
    // for (auto &box : box_array) {
    //     detect_boxes.push_back(json11::Json::object{{"class_label", box.class_label},
    //                                                 {"confidence", box.confidence},
    //                                                 {"left", box.left},
    //                                                 {"top", box.top},
    //                                                 {"right", box.right},
    //                                                 {"bottom", box.bottom}});
    // }
    // json11::Json json_data = json11::Json::object{{"img_base64", img_base64},
    //                                               {"img_width", image.cols},
    //                                               {"img_height", image.rows},
    //                                               {"frame_id", data->Get<int>(FRAME_INDEX)},
    //                                               {"post_time", Time2Str(time(nullptr))},
    //                                               {"detection_boxes", detect_boxes}};  
    int idx = 0;

    
    for (auto &box : box_array) 
    {
        detect_boxes.push_back(json11::Json::object{{"index", idx},
                                                    {"width", int(box.right - box.left)},
                                                    {"height", int(box.bottom - box.top)},
                                                    {"left", int(box.left + (box.right - box.left)/2)},
                                                    {"top", int(box.top + (box.bottom - box.top)/2)},
                                                    {"type", box.class_name}});
        idx ++;
    }
    json11::Json json_data = json11::Json::object{{"keyframe",  img_base64},
                                                  {"task_no", m_task_no},
                                                  {"date", Time2Str(time(nullptr))},
                                                  {"targets", detect_boxes}};
    // // 发送数据    
    auto res = m_client.Post(m_post_url.c_str(), json_data.dump(), "application/json");
    if (NULL == res)
    {
        ErrorL << "keyframe服务器已关闭";
    }
    else if (res->status != 200) {
        ErrorL << "上传数据失败";
        if (error_cb && res == NULL) {
            error_cb("上传数据", res->status, res->body);
        }
    }
    return nullptr;
}

}  // namespace Node