#include <vector>

#include "Point.hpp"
#include "Cluster.hpp"
#include "ClustersBuilder.hpp"


ClustersBuilder::ClustersBuilder(const std::vector<Point> &originalPoints, float clusterEps)
{
    this->originalPoints = originalPoints;
    shiftedPoints = originalPoints;
    // vector of booleans such that the element in position i is false if the i-th point
    // has stopped to shift
    shifting = std::vector<bool>(originalPoints.size(), true);
    numPointsStoppedShifting = 0;
    this->clusterEps = clusterEps;
}


Point &ClustersBuilder::getShiftedPoint(long index)
{
    return shiftedPoints[index];
}


void ClustersBuilder::shiftPoint(const long index, const Point &newPosition)
{
    if (newPosition.euclideanDistance(shiftedPoints[index]) <= clusterEps/10) {
        shifting[index] = false;
        ++numPointsStoppedShifting;
    } else
        shiftedPoints[index] = newPosition;
}


bool ClustersBuilder::stopShifting(long index)
{
    return !shifting[index];
}


bool ClustersBuilder::stopShiftingAll()
{
    return numPointsStoppedShifting == originalPoints.size();
}


std::vector<Point>::iterator ClustersBuilder::begin()
{
    return shiftedPoints.begin();
}


std::vector<Point>::iterator ClustersBuilder::end()
{
    return shiftedPoints.end();
}


std::vector<Cluster> ClustersBuilder::buildClusters()
{
    std::vector<Cluster> clusters;

    // shifted points with distance minor or equal than clusterEps will go in the same cluster
    for (int i = 0; i < shiftedPoints.size(); ++i) {
        Point shiftedPoint = shiftedPoints[i];
        auto it = clusters.begin();
        auto itEnd = clusters.end();
        while (it != itEnd) {
            if (it->getCentroid().euclideanDistance(shiftedPoint) <= clusterEps) {
                // the point belongs to a cluster already created
                it->addPoint(originalPoints[i]);
                break;
            }
            ++it;
        }
        if (it == itEnd) {
            // create a new cluster
            Cluster cluster(shiftedPoint);
            cluster.addPoint(originalPoints[i]);
            clusters.emplace_back(cluster);
        }
    }
    return clusters;
}