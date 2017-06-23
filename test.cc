#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

// to compile..
//   gcc test.cc sqlite3.c -lpthread -ldl

static int callback (void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf ("%s = %s\n",azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf ("\n");
	return 0;
}

int print_db (const char *db_file) {
	sqlite3 *db;
	const char *sql = "select * from test";
	sqlite3_stmt *prepared_stmt;
	int nrecs;
	char *errmsg;
	int i;
	int row = 0;

	int rc;
	rc = sqlite3_open_v2 (db_file, &db, SQLITE_OPEN_READONLY, NULL);
	if (rc) {
		fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close (db);
		return 1;
	}
	int retcode = sqlite3_prepare_v2 (db, sql, -1, &prepared_stmt, NULL);
	if (retcode == SQLITE_OK) {
		int numcols = sqlite3_column_count (prepared_stmt);
		for (int i = 0; i < numcols; i++) {
			printf (" COL[%d]: %s \n", i, sqlite3_column_name (prepared_stmt, i));
		}
		while (1) {
			int s = sqlite3_step (prepared_stmt);
			if (s == SQLITE_ROW) {
				int bytes;
				const unsigned char *text;
				bytes = sqlite3_column_bytes (prepared_stmt, 0);
				text = sqlite3_column_text (prepared_stmt, 0);
				printf ("%d: %s\n", row, text);
				//printf ("%d: \n", row);
				row++;
			}
			else if (s == SQLITE_DONE) {
				break;
			}
			else {
				fprintf (stderr, "Unknown error: sqlite3_step %d \n", s);
				exit (1);
			}
		}
	}
	else {
		fprintf (stderr, "error from sqlite3_prepare_v2 %d\n", retcode);
	}
	rc = sqlite3_finalize (prepared_stmt);
	if (rc != SQLITE_OK) {
		printf ("sqlite3_finalize errcode %d \n", rc);
	}
	return 0;
}

//api
//// sqlite3_column_name (sqlite3_stmt *prepared_stmt, int index);  
//  sqlite3_column_count (sqlite3_stmt *prepared_stmt); 
//  sqlite3_data_count (sqlite3_stmt *prepared_stmt); 

int test_db (int argc, char **argv) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	if (argc != 3) {
		fprintf (stderr, "Usage: %s DATABASE SQL-STMT\n", argv[0]);
		return 1;
	}
	rc = sqlite3_open (argv[1], &db);
	if (rc) {
		fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close (db);
		return 1;
	}
	rc = sqlite3_exec (db, argv[2], callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf (stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free (zErrMsg);
	}
	sqlite3_close (db);
	return 0;
}

int main (int argc, char **argv) {
	//test_db (argc, argv);
	print_db ("toy.db");
}
