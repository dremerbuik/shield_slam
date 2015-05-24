#ifndef __shield_slam__Initializer__
#define __shield_slam__Initializer__

#include <opencv2/opencv.hpp>

#include "ORB.hpp"
#include "Common.hpp"
#include "MapPoint.hpp"
#include "KeyFrame.hpp"

using namespace cv;
using namespace std;

#define SYMMETRIC_ERROR_SIGMA 1.0
#define SYMMETRIC_ERROR_TH 5.991

#define FUNDAMENTAL_ERROR_TH_SCORE 5.991
#define FUNDAMENTAL_ERROR_TH 3.841

#define HOMOGRAPHY_SELECTION_THRESHOLD 0.45

#define REPROJECTION_ERROR_TH 4.0
#define PARALLAX_MIN_DEGREES 1.0

#define TRIANGULATION_MIN_POINTS 50
#define TRIANGULATION_GOOD_POINTS_RATIO 0.9

#define TRIANGULATION_NORM_SCORE_H_TH 0.45
#define TRIANGULATION_NORM_SCORE_F_TH 0.7

#define TRIANGULATION_LS_ITERATIONS 10
#define TRIANGULATION_LS_EPSILON 0.0001

namespace vslam
{
    
    class Initializer
    {
    public:
        
        Initializer();
        virtual ~Initializer() = default;
        
        bool InitializeMap(Mat& img_ref, Mat& img_tar, KeyFrame& kf, vector<MapPoint>& global_map);
        
        Mat FindHomography(PointArray& ref_keypoints, PointArray& tar_keypoints, float& score, vector<bool>& match_inliers, int& num_inliers);
        Mat FindFundamental(PointArray& ref_keypoints, PointArray& tar_keypoints, float& score, vector<bool>& match_inliers, int& num_inliers);
        
        float CheckHomography(PointArray& ref_keypoints, PointArray& tar_keypoints, Mat& H_ref2tar, vector<bool>& match_inliers, int& num_inliers);
        float CheckFundamental(PointArray& ref_keypoints, PointArray& tar_keypoints, Mat& F, vector<bool>& match_inliers, int& num_inliers);
        
        void CameraPoseHomography(Mat& H, Mat& pose);
        void CameraPoseFundamental(Mat& F, Mat& pose);
        
        bool ReconstructHomography(PointArray& ref_keypoints, PointArray& tar_keypoints, vector<DMatch>& matches, vector<bool>& inliers, int& num_inliers, Mat& H, Mat& R, Mat& t, vector<Point3f>& points, vector<bool>& triangulated_state);
        bool ReconstructFundamental(PointArray& ref_keypoints, PointArray& tar_keypoints, vector<DMatch>& matches, vector<bool>& inliers, int& num_inliers, Mat& F, Mat& R, Mat& t, vector<Point3f>& points, vector<bool>& triangulated_state);
        
        int CheckRt(Mat& R, Mat& t, const PointArray& ref_keypoints, const PointArray& tar_keypoints, const vector<bool>& inliers, const vector<DMatch>& matches, vector<Point3f>& point_cloud, float& max_parallax, vector<bool>& triangulated_state);
        float ScoreRt(vector<Mat>& p_R, vector<Mat>& p_t, const PointArray& ref_keypoints, const PointArray& tar_keypoints, const vector<bool>& inliers, const vector<DMatch>& matches, vector<Point3f>& best_point_cloud, float& best_parallax, vector<bool>& best_triangulated_state, int& best_trans_idx);
        
        void Normalize(const PointArray& in_points, PointArray& norm_points, Mat& T);
        void Triangulate(const KeyPoint& ref_keypoint, const KeyPoint& tar_keypoint, const Mat& P1, const Mat& P2, Mat& point_3D);
        Mat_<double> LinearLSTriangulation(const Point3d& u1, const Point3d& u2, const Mat& P1, const Mat& P2);
        Matx31d IterativeLinearLSTriangulation(const Point3d& u1, const Point3d& u2, const Mat& P1, const Mat& P2);
        
        void FilterInliers(PointArray& ref_keypoints, PointArray& tar_keypoints, vector<bool>& inliers, PointArray& ref_inliers, PointArray& tar_inliers);
        
    private:
        
        Ptr<ORB> orb_handler;
        
    protected:
        Mat R, t;
        vector<bool> triangulated_state;
        vector<Point3f> point_cloud_3D;
    };
    
}

#endif /* defined(__shield_slam__Initializer__) */
