//
// Created by dilin on 5/11/17.
//

#include "HungaryAlgo.h"

HungaryAlgo::HungaryAlgo(cv::Mat costMat)
{
    this->costMat = costMat;
    mask = cv::Mat(costMat.rows,costMat.cols,CV_64F,0.0);
    rowCover = cv::Mat(costMat.rows,1,CV_8U,0.0);
    colCover = cv::Mat(costMat.cols,1,CV_8U,0.0);
    path = cv::Mat(costMat.rows+costMat.cols+1,1,CV_8U,0.0);
}

HungaryAlgo::~HungaryAlgo()
{
    costMat.release();
    mask.release();
    rowCover.release();
    colCover.release();
    path.release();
}


void HungaryAlgo::step_one(int &step)
{
    double min_in_row;

    for(int i=0;i<costMat.rows;i++)
    {
        min_in_row = costMat.at<double>(i,0);
        for(int j=0;j<costMat.cols;j++)
        {
            if(costMat.at<double>(i,j)<min_in_row)
                min_in_row = costMat.at<double>(i,j);
        }
        for(int j=0;j<costMat.cols;j++)
        {
            costMat.at<double>(i,j) -= min_in_row;
        }
    }

    step = 2;
}

void HungaryAlgo::step_two(int &step)
{
    for(int i=0;i<costMat.rows;i++)
    {
        for(int j=0;j<costMat.cols;j++)
        {
            if(costMat.at<double>(i,j)==0 && rowCover.at<uint8_t >(i)==0 && colCover.at<uint8_t>(j)==0)
            {
                mask.at<uint8_t>(i,j) = 1;
                rowCover.at<uint8_t>(i) = 1;
                colCover.at<uint8_t>(j) = 1;
            }
        }
    }
    for(int r=0;r<costMat.rows;r++)
        rowCover.at<uint8_t>(r) = 0;
    for(int c=0;c<costMat.cols;c++)
        colCover.at<uint8_t>(c) = 0;
    step = 3;
}

void HungaryAlgo::step_three(int &step)
{
    int colCount;
    for(int i=0;i<mask.rows;i++)
    {
        for(int j=0;j<mask.cols;j++)
        {
            if(mask.at<uint8_t>(i,j)==1)
                colCover.at<uint8_t>(j) = 1;

        }
    }

    colCount = 0;

    for(int j=0;j<mask.cols;j++)
    {
        if(colCover.at<uint8_t>(j)==1)
            colCount += 1;
    }
    if(colCount>=mask.cols || colCount>=mask.rows)
        step = 7;
    else
        step = 4;
}

void HungaryAlgo::find_a_zero(int &row, int &col)
{
    int r=0;
    int c;
    bool done;
    row = -1;
    col = -1;
    done = false;
    while(!done)
    {
        c = 0;
        while(true)
        {
            if(costMat.at<double>(r,c)==0 && rowCover.at<uint8_t>(r)==0 && colCover.at<uint8_t>(c)==0)
            {
                row = r;
                col = c;
                done = true;
            }
            c += 1;
            if(c >= costMat.cols || done)
                break;
        }
        r += 1;
        if(r>=costMat.rows)
            done = true;
    }
}

bool HungaryAlgo::star_in_row(int row)
{
    bool tmp = false;
    for(int c=0;c<mask.cols;c++)
    {
        if(mask.at<uint8_t>(row,c)==1)
            tmp = true;
    }
    return tmp;
}

void HungaryAlgo::find_star_in_row(int row, int &col)
{
    col = -1;
    for(int c=0;c<mask.cols;c++)
    {
        if(mask.at<uint8_t>(row,c)==1)
            col = c;
    }
}

void HungaryAlgo::step_four(int &step)
{
    int row = -1;
    int col = -1;
    bool done = false;

    while(!done)
    {
        find_a_zero(row,col);
        if(row == -1)
        {
            done = true;
            step = 6;
        }
        else
        {
            mask.at<uint8_t>(row,col) = 2;
            if(star_in_row(row))
            {
                find_star_in_row(row,col);
                rowCover.at<uint8_t>(row) = 1;
                colCover.at<uint8_t>(col) = 0;
            }
            else
            {
                done = true;
                step = 5;
                path_row_0 = row;
                path_col_0 = col;
            }
        }
    }
}

void HungaryAlgo::find_star_in_col(int &r, int c)
{
    r = -1;
    for(int i=0;i<mask.rows;i++)
    {
        if(mask.at<uint8_t>(i,c)==1)
            r = i;
    }
}

void HungaryAlgo::find_prime_in_row(int r, int &c)
{
    for(int j=0;j<mask.cols;j++)
    {
        if(mask.at<uint8_t>(r,j)==2)
            c = j;
    }
}

void HungaryAlgo::augment_path()
{
    for(int p=0;p<path_count;p++)
    {
        if(mask.at<uint8_t>(path.at<uint8_t>(p,0),path.at<uint8_t>(p,1))==1)
            mask.at<uint8_t>(path.at<uint8_t>(p,0),path.at<uint8_t>(p,1)) = 0;
        else
            mask.at<uint8_t>(path.at<uint8_t>(p,0),path.at<uint8_t>(p,1)) = 1;
    }
}

void HungaryAlgo::clear_covers()
{
    for(int r=0;r<costMat.rows;r++)
        rowCover.at<uint8_t>(r) = 0;
    for(int c=0;c<costMat.cols;c++)
        colCover.at<uint8_t>(c) = 0;
}


void HungaryAlgo::erase_primes()
{
    for(int r=0;r<costMat.rows;r++)
    {
        for(int c=0;c<costMat.cols;c++)
        {
            if(mask.at<uint8_t>(r,c)==2)
                mask.at<uint8_t>(r,c) = 0;
        }
    }
}

void HungaryAlgo::step_five(int &step)
{
    bool done;
    int r = -1;
    int c = -1;

    path_count = 1;
    path.at<uint8_t>(path_count-1,0) = (uint8_t) path_row_0;
    path.at<uint8_t>(path_count-1,1) = (uint8_t) path_col_0;
    done = false;
    while(!done)
    {
        find_star_in_col(r,(int)path.at<uint8_t>(path_count-1,1));
        if(r>-1)
        {
            path_count += 1;
            path.at<uint8_t>(path_count-1,0) = (uint8_t)r;
            path.at<uint8_t>(path_count-1,1) = path.at<uint8_t>(path_count-2,1);
        }
        else
            done = true;
        if(!done)
        {
            find_prime_in_row(path.at<uint8_t>(path_count-1,0),c);
            path_count += 1;
            path.at<uint8_t>(path_count-1,0) = path.at<uint8_t>(path_count-2,0);
            path.at<uint8_t>(path_count-1,1) = (uint8_t)c;
        }
    }
    augment_path();
    clear_covers();
    erase_primes();
    step = 3;
}

void HungaryAlgo::find_smallest(double &minval)
{
    for(int r=0;r<costMat.rows;r++)
    {
        for(int c=0;c<costMat.cols;c++)
        {
            if(rowCover.at<uint8_t>(r)==0 && colCover.at<uint8_t>(c)==0)
            {
                if(minval>costMat.at<double>(r,c))
                    minval = costMat.at<double>(r,c);
            }
        }
    }
}

void HungaryAlgo::step_six(int &step)
{
    double minVal = DBL_MAX;
    find_smallest(minVal);
    for(int r=0;r<costMat.rows;r++)
    {
        for(int c=0;c<costMat.cols;c++)
        {
            if(rowCover.at<uint8_t>(r)==1)
                costMat.at<double>(r,c) += minVal;
            if(colCover.at<uint8_t>(c)==0)
                costMat.at<double>(r,c) -= minVal;
        }
    }
    step = 4;
}

void HungaryAlgo::run()
{
    bool done = false;
    int step=1;
    while(!done)
    {
        std::cout << "-------Step" << step << "--------" << std::endl << std::endl;
        for ( int row = 0 ; row < costMat.rows ; row++ )
        {
            for ( int col = 0 ; col < costMat.cols ; col++ )
            {
                std::cout << costMat.at<double>(row,col) << ",";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        for ( int row = 0 ; row < costMat.rows ; row++ )
        {
            for ( int col = 0 ; col < costMat.cols ; col++ )
            {
                std::cout << unsigned(mask.at<uint8_t>(row,col)) << ",";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
        for ( int row= 0 ; row < costMat.rows ; row++ )
        {
            std::cout << unsigned(rowCover.at<uint8_t>(row)) << ",";
        }
        std::cout << std::endl;

        std::cout << std::endl;
        for ( int col= 0 ; col < costMat.cols ; col++ )
        {
            std::cout << unsigned(colCover.at<uint8_t>(col)) << ",";
        }
        std::cout << std::endl;

        std::cout << std::endl <<"-----------------------------" <<std::endl;

        switch(step)
        {
            case 1:
                step_one(step);
                break;
            case 2:
                step_two(step);
                break;
            case 3:
                step_three(step);
                break;
            case 4:
                step_four(step);
                break;
            case 5:
                step_five(step);
                break;
            case 6:
                step_six(step);
                break;
            case 7:
                done = true;
                break;
        }
    }
}