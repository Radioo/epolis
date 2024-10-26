#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <wx/wx.h>



namespace epolis::utility {

class operations {
    public:
    void clean_borders(cv::Mat& input_image);
    bool animate_marker_reconstruction(bool reset = false);
    cv::Mat get_marker_animation_frame();
    cv::Mat get_input_image_binary();
    cv::Mat get_clean_borders_result();
    cv::Mat get_animation_frame();

    private:
    cv::Mat input_image_binary;
    cv::Mat marker_animation_frame;
    cv::Mat marker_next_frame;
    cv::Mat clean_borders_result;
    cv::Mat animation_frame;

};

}

