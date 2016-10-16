#include "mysql_connection.h"

#ifndef INTRA_INTERFACE_H
#define INTRA_INTERFACE_H

//함수 임시 정의

void encryptor(sql::Connection *con);

int get_User_num(sql::Connection *con);

int get_Item_num(sql::Connection *con);

MemberVector* get_MemberVector(sql::Connection *con, bool type, int id, MemberVector* mv);

UserProfile* get_UserProfile(sql::Connection *con, int id, UserProfile* up);

ItemProperty* get_ItemProperty(sql::Connection *con, int Item_id, ItemProperty* ip);

int get_Log_num(sql::Connection *con);

Rating_Point* get_RatingPoint(sql::Connection *con, int log_num, Rating_Point* rp);


#endif INTRA_INTERFACE_H