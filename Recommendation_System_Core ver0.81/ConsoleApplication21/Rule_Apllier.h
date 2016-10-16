//#include "structs.h"
//#include "stdafx.h"

#ifndef RULE_APPLIER_H
#define RULE_APPLIER_H

class Rec_Manager;

class Rule_Applier{
public:
	friend Rec_Manager;

	Rule_Applier(int num_of_User,int num_of_Item);
	~Rule_Applier();

	int* rule_applier(int client_id,int* pre_Item_list);

private:
	UserProfile U_Profile;
	ItemProperty* Item_list;
	int num_of_cur_item;

	int num_of_user;
	int num_of_item;

};

#endif RULE_APPLIER_H