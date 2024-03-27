//
// Created by elaydin on 07.07.2023.
//

#ifndef CLAP_B7_DRIVER_HPP
#define CLAP_B7_DRIVER_HPP
#include <termios.h>

#include "clap_b7_driver/clap_binary_parser.h"
#include <clap_b7_driver/clap_msg_wrapper.h>
#include <clap_b7_driver/clap_structs.h>
#include <clap_b7_driver/clap_publisher.h>
#include <clap_b7_driver/ll_to_utm_transform.h>

#include <rclcpp/rclcpp.hpp>

#include <mavros_msgs/msg/rtcm.hpp>




namespace clap_b7
{
    class ClapB7Driver : public rclcpp::Node {
    public:
        ClapB7Driver();
        ~ClapB7Driver(){
            close(file_descriptor_);
        }
        int try_serial_connection(const std::basic_string<char>& port, unsigned int baud);
        void Update();

    private:
        int file_descriptor_;
        struct termios tty_;
        clap_b7::BinaryParser parser_{};
        void clap_read_callback(const uint8_t *data, uint16_t id);
        rclcpp::TimerBase::SharedPtr timer_;
        ClapMsgWrapper msg_wrapper_{};
        Publishers publishers_{};
        LlToUtmTransform ll_to_utm_transform_{};

        /*
         * Messages
         */
        RawImu raw_imu_{};
        InsPvax ins_pvax_{};
        BestGnssPos gnss_pos_{};
        BestGnssVel gnss_vel_{};
        UniHeading heading_{};
        ECEF ecef_{};
        TimeDWheelData wheel_data_{};

        /*
         * Config
         */
        ConfigParams params_;

        /*
         * Subscribers
         */

        rclcpp::Subscription<mavros_msgs::msg::RTCM>::SharedPtr rtcm_sub_;
        void load_parameters();

        void rtcm_callback(const mavros_msgs::msg::RTCM::SharedPtr msg);
    };
} // namespace clap_b7


#endif //CLAP_B7_DRIVER_HPP
