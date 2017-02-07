#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "/usr/include/mysql/mysql.h"

#include <artik_module.h>
#include <artik_platform.h>
#include <artik_pwm.h>
#include <artik_gpio.h>

#define DB_HOST "host_name"
#define DB_USER "root"
#define DB_PASS "root"
#define DB_NAME "db_name"
#define CHOP(x) x[strlen(x) - 1] = ' '

#define CHECK_RET(x) { if(x != S_OK) goto exit; }
#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 0

#define PWM_pin 25
#define DIRECTION_pin 0
#define BRAKE_pin 26

bool digitalWrite(int pin, int val){
		FILE * fd;

		char fName[128];

		sprintf(fName, "/sys/class/gpio/gpio%d/value",pin);
		if ((fd=fopen(fName, "w")) == NULL){
				printf("Error : can't open pin\n");
				return false;
		}
		if(val == HIGH)
				fprintf(fd, "1\n");
		else fprintf(fd, "0\n");

		fclose(fd);
		return true;
}

bool digitalPinMode(int pin, int dir){
		FILE * fd;
		char fName[128];

		if((fd=fopen("/sys/class/gpio/export", "w")) == NULL){
				printf("Error: unable to export pin\n");
				return false;
		}
		fprintf(fd, "%d\n", pin);
		fclose(fd);

		sprintf(fName, "/sys/class/gpio/gpio%d/direction", pin);
		if((fd = fopen(fName, "w")) == NULL){
				printf("Error: can't open pin direction\n");
				return false;
		}
		if(dir == OUTPUT) fprintf(fd, "out\n");
		else fprintf(fd, "in\n");
		fclose(fd);

		return true;
}

int main() {
/*****************mysql************/
		MYSQL       *connection=NULL, conn;
		MYSQL_RES   *sql_result;
		MYSQL_ROW   sql_row;
		int       query_stat; 
		char query[255];

		char buffer[20];
		long longval;
		unsigned long* lengths;

		mysql_init(&conn);
		connection = mysql_real_connect(&conn, DB_HOST,
						                        DB_USER, DB_PASS,
						                        DB_NAME, 3306,
 																		(char *)NULL, 0);

		if (connection == NULL)
		{
				fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
				return 1;
		}

		/*
		 * ra?
		 */
		artik_error ret = S_OK;
		int platid = artik_get_platform();

		while(1)
		{
			
			if((platid == ARTIK520) || (platid == ARTIK1020) || (platid == ARTIK710)){
//				ret = pwm_test_frequency(platid);
		//		CHECK_RET(ret);
		
					query_stat = mysql_query(connection, "select * from alarm");
					if (query_stat != 0)
					{
						fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
						return 1;
					}


					sql_result = mysql_store_result(connection);

					if( (sql_row = mysql_fetch_row(sql_result)) != NULL )
					{
						printf("idx:%s go:%s\n", sql_row[0], sql_row[1]);
					}

					lengths = mysql_fetch_lengths(sql_result);
			
					if(lengths[1]!=0)
					{
						memcpy(buffer, sql_row[1], lengths[1]);
						buffer[lengths[1]]=0;
						sscanf(sql_row[1], "%d", &longval);
					}
					printf("longval: %d\n", longval);
					mysql_free_result(sql_result);

				if(longval == 1) // alarm on
				{
					printf("alarm on\n");
					digitalWrite(PWM_pin, HIGH);
					sleep(1);
					digitalWrite(DIRECTION_pin, HIGH);
					sleep(1);
					digitalWrite(BRAKE_pin, LOW);
					sleep(1);
				}
				else //alarm off
				{
					printf("alarm off\n");
					digitalWrite(PWM_pin, HIGH);
					sleep(1);
					digitalWrite(DIRECTION_pin, HIGH);
					sleep(1);
					digitalWrite(BRAKE_pin, HIGH);
					sleep(1);
				}

		}
	}

exit:
		mysql_close(connection);
		return (ret == S_OK) ? 0 : -1;

}
