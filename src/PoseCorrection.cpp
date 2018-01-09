#include "ga_slam/PoseCorrection.hpp"

namespace ga_slam {

void PoseCorrection::createGlobalMap(const Cloud::ConstPtr& cloud) {
    globalMap_.setParameters(100., 100., 1., -1000., 1000.);

    auto& meanData = globalMap_.getMeanZ();
    auto& varianceData = globalMap_.getVarianceZ();

    size_t cloudIndex = 0;
    size_t mapIndex;

    for (const auto& point : cloud->points) {
        cloudIndex++;

        if (!globalMap_.getIndexFromPosition(point.x, point.y, mapIndex))
            continue;

        float& mean = meanData(mapIndex);
        float& variance = varianceData(mapIndex);
        const float& pointVariance = 1.;

        if (!std::isfinite(mean)) {
            mean = point.z;
            variance = pointVariance;
        } else {
            const double innovation = point.z - mean;
            const double gain = variance / (variance + pointVariance);
            mean = mean + (gain * innovation);
            variance = variance * (1. - gain);
        }
    }

    globalMap_.setValid(true);
    globalMap_.setTimestamp(cloud->header.stamp);
}

}  // namespace ga_slam

