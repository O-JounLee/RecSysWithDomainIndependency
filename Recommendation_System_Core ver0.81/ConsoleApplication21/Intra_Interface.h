//#include "structs.h"

#ifndef INTRA_INTERFACE_H
#define INTRA_INTERFACE_H

//함수 임시 정의


int get_User_num();

int get_Item_num();

MemberVector* get_MemberVector(bool type,int User_id);

UserProfile* get_UserProfile(int User_id);

ItemProperty* get_ItemProperty(int Item_id);

int get_Log_num();

Rating_Point get_RatingPoint(int log_num);


#endif INTRA_INTERFACE_H