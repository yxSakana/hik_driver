#include <iostream>

#include <QApplication>

#include <HikDriver/HikDriver.h>
#include <google_logger/google_logger.h>
#include <HikDriver/HikUi.h>

int main(int argc, char* argv[]) {
    armor_auto_aim::google_log::initGoogleLogger(argc, argv);
    QApplication q_app(argc, argv);

//    HikDriver hik_driver(0, TriggerSource::Software);
    HikDriver hik_driver;
    HikFrame frame;
    std::unique_ptr<HikUi> hik_ui;
    float ex_time = 20000;
    if (hik_driver.isConnected()) {
        hik_driver.setExposureTime(ex_time);
        hik_driver.setGain(13);
        hik_driver.showParamInfo();
        hik_ui = std::make_unique<HikUi>(hik_driver);
        hik_ui->show();
        hik_driver.startReadThread();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::thread t([&hik_driver, &ex_time]()->void {
            bool mode = false;
            while (1) {
                hik_driver.setExposureTime(ex_time);
                hik_driver.setTriggerMode(mode);
                mode = !mode;
                ex_time += 1;
                LOG(INFO) << fmt::format("mode: {}; sources: {}",
                                         hik_driver.getTriggerMode(),
                                         hik_driver.getTriggerSources());
            }
        });
        t.detach();

        std::thread t1([&hik_driver, &frame](){
            while (1) {
//            hik_driver.getFrame(frame, 0);
                hik_driver.getFrame(frame);
//            hik_driver.triggerImageData(frame);
//            std::this_thread::sleep_for(std::chrono::milliseconds(200));
                if (!frame.empty()) {
                    cv::putText(*frame.getRgbFrame(), std::to_string(frame.getTimestamp()),
                                cv::Point(40, 40), cv::FONT_HERSHEY_SIMPLEX,
                                0.75, cv::Scalar(255, 255, 255), 2);
                    cv::imshow("frame", *frame.getRgbFrame());
                    cv::waitKey(1);
                } else {
                    LOG_EVERY_T(ERROR, 10) << "frame is empty";
                }
            }
        });
        t1.detach();
    }

    return QApplication::exec();
}
