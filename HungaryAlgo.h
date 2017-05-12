//
// Created by dilin on 5/11/17.
//

#ifndef PEOPLETRACKER_DEMO_HUNGARYALGO_H
#define PEOPLETRACKER_DEMO_HUNGARYALGO_H
#include <opencv2/opencv.hpp>
#include<cfloat>
#include<cstdint>

class HungaryAlgo
{
public:
    HungaryAlgo(cv::Mat costMat);
    void run();
    virtual ~HungaryAlgo();
    cv::Mat mask;

private:
    cv::Mat costMat;
    cv::Mat rowCover;
    cv::Mat colCover;
    cv::Mat path;
    int path_count;
    int path_row_0;
    int path_col_0;
    void step_one(int &step);
    void step_two(int &step);
    void step_three(int &step);
    void find_a_zero(int &row, int &col);
    bool star_in_row(int row);
    void find_star_in_row(int row, int &col);
    void step_four(int &step);
    void find_star_in_col(int &row, int col);
    void find_prime_in_row(int r, int &c);
    void augment_path();
    void clear_covers();
    void erase_primes();
    void step_five(int &step);
    void find_smallest(double &minval);
    void step_six(int &step);
};


#endif //PEOPLETRACKER_DEMO_HUNGARYALGO_H
