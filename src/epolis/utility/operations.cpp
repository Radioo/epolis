#include "epolis\utility\operations.h"

namespace epolis::utility {
    void operations::clean_borders(cv::Mat& input_image, bool convert_grayscale) {
        if (convert_grayscale) {
            convert_to_grayscale(input_image);
            operation_image = input_image_binary.clone();
        }
        else {
            operation_image = input_image.clone();
        }

        cv::Rect roi(1, 1, operation_image.cols - 2, operation_image.rows - 2);

        cv::Mat cropped_image = operation_image(roi).clone();
        cv::Mat padded_image;

        cv::copyMakeBorder(cropped_image,padded_image, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(0));

        cv::bitwise_xor(operation_image, padded_image, marker_animation_frame);
        cv::Mat color_image = marker_animation_frame.clone();
        cv::cvtColor(color_image, red_markers_image, cv::COLOR_GRAY2BGR);

        for (int y = 0; y < color_image.rows; y++) {
            for (int x = 0; x < color_image.cols; x++) {
                if (color_image.at<uchar>(y, x) == 255) {
                    red_markers_image.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0);
                }
            }
        }

        is_pixel_diff(true);
    }

    bool operations::animate_marker_reconstruction(bool reset, bool animate) {
        static bool flag = true;
        if (reset) {
            flag = true;
            return false;
        }
        constexpr auto dilation_type = cv::MORPH_CROSS;
        constexpr auto dilation_size = 1;
        const cv::Mat element = getStructuringElement(
            dilation_type,
            cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
            cv::Point(dilation_size, dilation_size)
        );

        if (animate) {
            if(flag) {
                cv::dilate(marker_animation_frame, marker_next_frame, element);
                animation_frame = apply_mask(marker_next_frame);
            }
            else {
                cv::bitwise_and(marker_next_frame, input_image_binary, marker_animation_frame);
                animation_frame = apply_mask(marker_animation_frame);
                if (!is_pixel_diff()) {
                    cv::bitwise_xor(marker_animation_frame, input_image_binary, destination);
                    flag = true;
                    return true;
                }
            }
        }
        else {
            //brak red markerów dla wersji bez animacji
            do {
                cv::dilate(marker_animation_frame, marker_next_frame, element);
                cv::bitwise_and(marker_next_frame, operation_image, marker_animation_frame);
            }
            while (is_pixel_diff());
            cv::bitwise_xor(marker_animation_frame, operation_image, destination);
            animation_frame = marker_animation_frame;
            flag = true;
            return true;
        }
        flag = !flag;

        return false;
    }

    void operations::fill_holes(cv::Mat &input_image) {
        convert_to_grayscale(input_image);

        bitwise_not(input_image_binary, inverted_image);

        is_pixel_diff(true);

        clean_borders(inverted_image, false);
        animate_marker_reconstruction(false, false);

        cv::bitwise_or(input_image_binary, destination, result);

    }

    cv::Mat operations::get_marker_animation_frame() {
        return marker_animation_frame;
    }

    cv::Mat operations::get_input_image_binary() {
        return input_image_binary;
    }

    cv::Mat operations::get_destination() {
        return destination;
    }

    cv::Mat operations::get_result() {
        return result;
    }

    cv::Mat operations::get_animation_frame() {
        return animation_frame;
    }

    cv::Mat operations::get_inverted_image() {
        return inverted_image;
    }

    bool operations::is_pixel_diff(bool reset) {
        static int changed_pixels = 0;
        static int count = 0;
        cv::Mat difference;

        if (reset) {
            changed_pixels = 0;
            count = 0;
            return true;
        }

        cv::absdiff(marker_next_frame, marker_animation_frame, difference);
        int non_zero_count = cv::countNonZero(difference);
        if (changed_pixels == non_zero_count) {
            count++;
        }
        else {
            count = 0;
        }
        if (changed_pixels == non_zero_count && count == 3) {
            changed_pixels = 0;
            count = 0;
            return false;
        }
        changed_pixels = non_zero_count;
        return true;
    }

    void operations::convert_to_grayscale(cv::Mat &input_image) {
        cv::Mat gray;

        cvtColor(input_image, gray, cv::COLOR_BGR2GRAY);

        cv::threshold(gray,input_image_binary, 128, 255, cv::THRESH_OTSU);
    }

    cv::Mat operations::apply_mask(const cv::Mat& input_image) {
        cv::Mat combined_image;
        cv::cvtColor(input_image, combined_image, cv::COLOR_GRAY2BGR);

        cv::Mat red_mask;
        cv::inRange(red_markers_image, cv::Scalar(200, 0, 0), cv::Scalar(255, 50, 50), red_mask);
        combined_image.setTo(cv::Scalar(255, 0, 0), red_mask);
        return combined_image;
    }
}
