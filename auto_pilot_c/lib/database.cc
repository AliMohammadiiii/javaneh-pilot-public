#include <iostream>
#include <string>
#include <list>
#include "sqlite3.h"
#include "database.h"
#include "javaneh.grpc.pb.h"

using namespace std;
using namespace Tiro;


void Database::connect(const char* filename, int readonly)
{	
	if(readonly == 0){
		sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	}else
	{		
		sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL);
	}
	
	createTables();
}

Database::~Database()
{ 
	close();
}

void Database::close()
{ 
	sqlite3_close_v2(db);
}

void Database::createTables()
{
	// mission table
	string sql = "CREATE TABLE IF NOT EXISTS mission(\
		id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
		name VARCHAR(32)\
	);";
	sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
	
	// iteration table
	sql = "CREATE TABLE IF NOT EXISTS iteration(\
            id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            mission INTEGER NOT NULL,\
			line INTEGER NOT NULL\
	);";
	sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);

	// point table
	// each mission has 3 line, 
	// 0  = rightmost range of motion
	// 1  = middle range of motion
	// 2  = leftmost range of motion
	sql = "CREATE TABLE IF NOT EXISTS point (\
             id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
             mission INTEGER NOT NULL,\
			 line INTEGER NOT NULL,\
             iteration INTEGER NOT NULL,\
             lat INTEGER NOT NULL,\
             lon INTEGER NOT NULL,\
             safe_mode INTEGER NOT NULL,\
             turn_in_place INTEGER NOT NULL,\
             speed INTEGER NOT NULL,\
             speed_direction INTEGER NOT NULL,\
             angle INTEGER NOT NULL,\
             steering_angle INTEGER NOT NULL,\
             steering_angle_direction INTEGER NOT NULL,\
             is_last INTEGER NOT NULL,\
             start_lat INTEGER,\
             start_lon INTEGER,\
             end_lat INTEGER,\
             end_lon INTEGER\
	);";
	sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
}

void Database::read(const string command, sqlite3_stmt **stmt){
	string sql(command);
	sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, NULL);
}

int Database::count(char* command){
	sqlite3_stmt* stmt;
	read((string)command, &stmt);
	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		int _count = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		return _count;
	}
	sqlite3_finalize(stmt);
	return -1;
}

// get mission , create new one if not exists
void Database::readMission(Mission* mission, int mission_id){
	sqlite3_stmt* stmt;
	char cmd[64];
	sprintf(cmd, "select * from mission where id=%d", mission_id);
	read((string)cmd, &stmt);
	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		mission->set_id(sqlite3_column_int(stmt, 0));
		// mission->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
	}
	else{
		sprintf(cmd, "insert into mission (id, name) values ('%d', 'test %i')", mission_id, mission_id);
		sqlite3_exec(db, ((string)cmd).c_str(), NULL, 0, NULL);
		mission->set_id(sqlite3_column_int(stmt, 0));
	}	
	sqlite3_finalize(stmt);
}

int Database::readIteration(int mission_id, int line){
	sqlite3_stmt* stmt;
	char cmd[64];
	
	sprintf(cmd, "select id from iteration where mission=%d and line = %d", mission_id, line);
	read((string)cmd, &stmt);
	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		int iteration = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		return iteration;
		// mission->set_name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
	}
	sqlite3_finalize(stmt);
	return -1;
}
void Database::readPoints(Point *points, const int iteration)
{

	int ret_code = 0;
	char cmd[64]; 
	sqlite3_stmt* stmt;
	sprintf(cmd, "select * from point where iteration=%i", iteration);
	read((string)cmd, &stmt);
	int i = 0;
	while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
		int lat = sqlite3_column_int(stmt, 4);
		points[i] = Point();
		points[i].set_id(sqlite3_column_int(stmt, 0)); 
		points[i].set_mission(sqlite3_column_int(stmt, 1)); 
		points[i].set_line(sqlite3_column_int(stmt, 2)); 
		points[i].set_iteration(sqlite3_column_int(stmt, 3)); 
		points[i].set_lat(sqlite3_column_double(stmt, 4)); 
		points[i].set_lon(sqlite3_column_double(stmt, 5)); 
		points[i].set_safe_mode(sqlite3_column_int(stmt, 6)); 
		points[i].set_turn_in_place(sqlite3_column_int(stmt, 7)); 
		points[i].set_speed(sqlite3_column_int(stmt, 8)); 
		points[i].set_speed_direction(sqlite3_column_int(stmt, 9)); 
		points[i].set_angle(sqlite3_column_int(stmt, 10)); 
		points[i].set_steering_angle(sqlite3_column_int(stmt, 11)); 
		points[i].set_steering_angle_direction(sqlite3_column_int(stmt, 12)); 
		points[i].set_is_last(sqlite3_column_int(stmt, 13)); 
		points[i].set_start_lat(sqlite3_column_double(stmt, 4)); 
		points[i].set_start_lon(sqlite3_column_double(stmt, 5)); 
		points[i].set_end_lat(sqlite3_column_double(stmt, 4)); 
		points[i].set_end_lon(sqlite3_column_double(stmt, 5)); 
		i++; 
	}
	sqlite3_finalize(stmt);
}

int Database::addIteration(int mission_id, int line){
	char cmd[64];
	sprintf(cmd, "insert into iteration (mission, line) values (%i, %i)", mission_id, line);
	sqlite3_exec(db, ((string)cmd).c_str(), NULL, 0, NULL);
	return sqlite3_last_insert_rowid(db);
}

// set new point
int Database::newPoint(Point* point){
	char cmd[256];
	sprintf(cmd, "insert into point (mission, \
	line, \
	iteration, \
	lat, \
	lon, \
	safe_mode, \
	turn_in_place, \
	speed, \
	speed_direction, \
	angle, \
	steering_angle, \
	steering_angle_direction, \
	is_last) \
	values \
	(%i, %i, %i, %.8f, %.8f, %i, %i, %i, %i, %i, %i, %i, %i)",
		point->mission(), 
		point->line(),
		point->iteration(),
		point->lat(),
		point->lon(),
		point->safe_mode(),
		point->turn_in_place(),
		point->speed(),
		point->speed_direction(),
		point->angle(),
		point->steering_angle(),
		point->steering_angle_direction(), 
		0);
	sqlite3_exec(db, ((string)cmd).c_str(), NULL, 0, NULL);	
	return sqlite3_last_insert_rowid(db);
	
}

void Database::setLastPoint(int last_point_id){
	char cmd[64];
	sprintf(cmd, "update point set is_last = 1 where id = %i;", last_point_id);
	sqlite3_exec(db, ((string)cmd).c_str(), NULL, 0, NULL);
}

void Database::normalize(int iteration, Point start_point, Point end_point){
	char cmd[256];
	sprintf(cmd, "update point set start_lat = %.8f, start_lon = %.8f, end_lat = %.8f, end_lon = %.8f where id >= %i and id <= %i and iteration = %i;", 
		start_point.lat(), start_point.lon(), end_point.lat(), end_point.lon(), start_point.id(), end_point.id(), iteration);
	sqlite3_exec(db, ((string)cmd).c_str(), NULL, 0, NULL);
}