#include "Parallel_extractCSER.h"    

Parallel_extractCSER::Parallel_extractCSER(vector<Mat> &_channels, vector< vector<ERStat> > &_regions,
                         vector<Ptr<ERFilter> >_er_filter1, vector<Ptr<ERFilter> >_er_filter2)
        : channels(_channels),regions(_regions),er_filter1(_er_filter1),er_filter2(_er_filter2){}

    void Parallel_extractCSER::operator()( const cv::Range &r ) const
    {
        for (int c=r.start; c < r.end; c++)
        {
            er_filter1[c]->run(channels[c], regions[c]);
            er_filter2[c]->run(channels[c], regions[c]);
        }
    }
