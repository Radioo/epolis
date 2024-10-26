#include "epolis\Utility\operations.h"

namespace epolis::utility {
    void operations::clean_borders(cv::Mat& input_image) {
        cv::Mat gray,marker;

        cv::cvtColor(input_image, gray, cv::COLOR_BGR2GRAY);

        cv::threshold(gray, input_image_binary, 128, 255, cv::THRESH_OTSU);

        cv::Rect roi(1, 1, input_image_binary.cols - 2, input_image_binary.rows - 2);

        cv::Mat cropped_image = input_image_binary(roi).clone();
        cv::Mat padded_image;

        cv::copyMakeBorder(cropped_image,padded_image, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(0));

        cv::bitwise_xor(input_image_binary, padded_image, marker_animation_frame);
    }

    bool operations::animate_marker_reconstruction(bool reset) {
        static int changed_pixels = 0;
        static int count = 0;
        static bool flag = true;
        if (reset) {
            changed_pixels = 0;
            count = 0;
            flag = true;
            return false;
        }
        constexpr auto dilation_type = cv::MORPH_RECT;
        constexpr auto dilation_size = 1;
        cv::Mat difference;
        const cv::Mat element = getStructuringElement(
            dilation_type,
            cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
            cv::Point(dilation_size, dilation_size)
        );

        if(flag) {
            cv::dilate(marker_animation_frame, marker_next_frame, element);
            animation_frame = marker_next_frame;
        }
        else {
            cv::bitwise_and(marker_next_frame, input_image_binary, marker_animation_frame);
            animation_frame = marker_animation_frame;
            cv::absdiff(marker_next_frame, marker_animation_frame, difference);
            int non_zero_count = cv::countNonZero(difference);
            if (changed_pixels == non_zero_count) {
                count++;
            }
            else {
                count = 0;
            }
            if (changed_pixels == non_zero_count && count == 3) {
                cv::bitwise_xor(marker_animation_frame, input_image_binary, clean_borders_result);
                changed_pixels = 0;
                count = 0;
                flag = true;
                return true;

            }
            changed_pixels = non_zero_count;
        }
        flag = !flag;

        return false;
    }

    cv::Mat operations::get_marker_animation_frame() {
        return marker_animation_frame;
    }

    cv::Mat operations::get_input_image_binary() {
        return input_image_binary;
    }

    cv::Mat operations::get_clean_borders_result() {
        return clean_borders_result;
    }

    cv::Mat operations::get_animation_frame() {
        return animation_frame;
    }
}
