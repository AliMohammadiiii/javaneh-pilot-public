#include <iostream>
#include "javaneh.grpc.pb.h"
#include <points.h>
#include <math.h>

using namespace std;
using namespace Tiro;

Points::Points(){}
Points::Points(Database *db){
	_db = db;
	int count = 1000;
}


void Points::readPoints(int mission, int iteration){
	char cmd[64]; 
	sprintf(cmd, "select count(id) as c from point where iteration=%d;", iteration);
	count  = _db->count(cmd); 
	if(count < point_slice){
		point_slice = count;
	}
	
	points = new Point[count];
	_db->readPoints(points, iteration);
	points_matrix.resize(2, point_slice);
	// P.resize(2, point_slice);
	// for(int i=0; i< count; i++)
	// {
	// 	points_matrix(0, i) = (double)points[i].lat()/10000000.0;
	// 	points_matrix(1, i) = (double)points[i].lon()/10000000.0;
	// }
}

void Points::neighbors(int* neighbor_points, const int k, double lat, double lon){

	

	VectorXd q(2, 1);
	q(0, 0) = lat;
	q(1, 0) = lon;
	
	int start = neighbor_points[0]-((int)(point_slice/2));
	// int start = 0;


	if(start < 0){
		start = 0;
	}else if(start > count-point_slice){
		start = count - point_slice;
	}
	// printf("start : %i , neighbor_points : %i , count : %i \n", start, neighbor_points[0], count);
	
	for(int i=0; i< point_slice; i++)
	{
		points_matrix(0, i) = (double)points[i+start].lat();
		points_matrix(1, i) = (double)points[i+start ].lon();
	}

	NNSearchD* nns = NNSearchD::createKDTreeLinearHeap(points_matrix);

	// look for the K nearest neighbour of a the single-point query
	
	VectorXi indices(k);
	VectorXd dists2(k);

	nns->knn(q, indices, dists2, k);
	for(int i =0; i< k; i++){
		neighbor_points[i] = start + indices[i];
	}
	

	// cleanup kd-tree
	delete nns;

}
long double Points::radians(double n) {
  return n * (M_PI / 180);
}
double Points::degrees(double n){
	return n * (180.0/M_PI);
}

double Points::radius(double degree) {
    return degree/180 * M_PI;
}

double Points::distance(double lat1, double lon1, double lat2, double lon2) { 
	lat1 = radians(lat1);
    lon1 = radians(lon1);
    lat2 = radians(lat2);
    lon2 = radians(lon2);
	double dlong = lon2 - lon1;
    double dlat = lat2 - lat1;

	double distance = pow(sin(dlat / 2), 2) + 
                          cos(lat1) * cos(lat2) * 
                          pow(sin(dlong / 2), 2);
	distance = 2 * asin(sqrt(distance));
	return distance * 6371;	
}

double Points::lineDistance(double start_end_distance, double start_current_distance, double end_current_distance) {
	double s = (start_current_distance + start_end_distance + end_current_distance) / 2;
	double d = 2*(sqrt(s*(s-start_end_distance)*(s-start_current_distance)*(s-end_current_distance)))/start_end_distance;
	return d;
}

double Points::heading(double lat1, double lon1, double lat2, double lon2){
    double teta1 = radians(lat1);
    double teta2 = radians(lat2);
    double delta1 = radians(lat2-lat1);
    double delta2 = radians(lon2-lon1);

    //==================Heading Formula Calculation================//

    double y = sin(delta2) * cos(teta2);
    double x = cos(teta1)*sin(teta2) - sin(teta1)*cos(teta2)*cos(delta2);
    double brng = atan2(y,x);
    brng = degrees(brng);// radians to degrees
    brng = fmod((brng + 360) , 360 );
    return brng;
}

void Points::cleanup(){
	delete[] points;
}