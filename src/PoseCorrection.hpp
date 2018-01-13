#pragma once

#include "ga_slam/TypeDefs.hpp"

#include <opencv2/core/core.hpp>

#include <mutex>

namespace ga_slam {

class PoseCorrection {
  public:
    PoseCorrection(void)
        : globalMap_(),
          lastCorrectedPose_(Pose::Identity()) {}

    PoseCorrection(const PoseCorrection&) = delete;
    PoseCorrection& operator=(const PoseCorrection&) = delete;
    PoseCorrection(PoseCorrection&&) = delete;
    PoseCorrection& operator=(PoseCorrection&&) = delete;

    const Map& getGlobalMap(void) const { return globalMap_; }

    std::mutex& getGlobalMapMutex(void) { return globalMapMutex_; }

    void setParameters(
            double traversedDistanceThreshold,
            double slopeSumThreshold);

    void createGlobalMap(const Cloud::ConstPtr& cloud);

    bool distanceCriterionFulfilled(const Pose& pose) const;

    bool featureCriterionFulfilled(const Map& localMap) const;

    Pose matchMaps(const Pose& pose, const Map& localMap);

    static cv::Mat convertMapToImage(const Map& map);

    static cv::Mat calculateGradientMagnitudeImage(const cv::Mat& image);

    static cv::Mat calculateApproximateGradientMagnitudeImage(
            const cv::Mat& image);

    static cv::Mat calculateLaplacianImage(
            const cv::Mat& image);

    static void displayImage(
            const cv::Mat& image,
            const std::string& windowName = "Image Display",
            int width = 500,
            int height = 500);

  protected:
    Map globalMap_;
    mutable std::mutex globalMapMutex_;

    Pose lastCorrectedPose_;

    double traversedDistanceThreshold_;
    double slopeSumThreshold_;
};

}  // namespace ga_slam

