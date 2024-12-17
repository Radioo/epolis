#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <wx/wx.h>
#include <deque>

constexpr size_t MAX_BUFFER_SIZE = 20;

namespace epolis::utility {
    class operations {
    public:
        void clean_borders(cv::Mat& input_image, bool convert_grayscale = true);
        void fill_holes(cv::Mat& input_image);
        void clear_buffer();

        bool animate_marker_reconstruction(bool reset = false, bool animate = true);

        cv::Mat get_marker_animation_frame();
        cv::Mat get_input_image_binary();
        cv::Mat get_destination();
        cv::Mat get_result();
        cv::Mat get_animation_frame();
        cv::Mat get_inverted_image();
        cv::Mat get_previous_image();

    private:
        cv::Mat input_image_binary;
        cv::Mat marker_animation_frame;
        cv::Mat marker_next_frame;
        cv::Mat destination;
        cv::Mat animation_frame;
        cv::Mat inverted_image;
        cv::Mat result;
        cv::Mat operation_image;
        cv::Mat red_markers_image;

        std::deque<cv::Mat> imageBuffer;

        int iteration = 0;
        int red_border = 0;

        bool is_pixel_diff(bool reset = false);
        void convert_to_grayscale(cv::Mat& input_image);

        cv::Mat convert_to_binary(cv::Mat& input_image);
        cv::Mat apply_mask(const cv::Mat& input_image);
    };
}

