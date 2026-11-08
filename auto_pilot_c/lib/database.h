#ifndef Database_H
#define Database_H
#endif
#include <iostream>
#include "sqlite3.h"
#include "javaneh.grpc.pb.h"

using namespace std;
using namespace Tiro;

class Database
{
	private:
		sqlite3 *db;
		//void createTables(sqlite3_stmt **stmt);
		void createTables();
	public:
		Database(){}
		~Database();
		void connect(const char* filename, int readonly);
		void close();
		void read(const string command, sqlite3_stmt **stmt);
		int count(char* command);
		void readMission(Mission* mission, int mission_id);
		int readIteration(int mission_id, int line);
		int addIteration(int mission_id, int line);
		int newPoint(Point* point);
		void setLastPoint(int last_point_id);
		void readPoints(Point *point, const int iteration);		
		void normalize(int iteration, Point start_point, Point end_point);	
};
