#ifndef Point_H
#define Point_H
#endif
#include <iostream>
#include "javaneh.grpc.pb.h"
#ifndef  Database_H
#include <database.h>
#endif
#include <nabo.h>

using namespace std;
using namespace Tiro;

using namespace Nabo;
using namespace Eigen;

class Points 
{
	public:
		Point *points;
		int point_slice = 1000;
		MatrixXd points_matrix;
		int count;
		Database *_db;
		Points();
		Points(Database *db);
		void readPoints(int mission, int iteration);
		void neighbors(int* neighbor_points, const int k, double lat, double lon);
		long double radians(double n); 
		double degrees(double n); 
		double radius(double degree); 
		double distance(double lat1, double lon1, double lat2, double lon2);
		double lineDistance(double start_end_distance, double start_current_distance, double end_current_distance);
		double heading(double lat1, double lon1, double lat2, double lon2);
		void cleanup();
};
