//#include "Recommendation_Controller.h"
//#include "structs.h"
#include "stdafx.h"

#ifndef MEMBER_MANAGER_H
#define MEMBER_MANAGER_H

class Rec_Controller;
class Rec_Model_Manager;

class Member_Manager{
public:
	friend Rec_Controller;
	friend Rec_Model_Manager;

	Member_Manager(sql::Connection* con,int num_user,int num_item);
	~Member_Manager();

	void Make_UserList();
	void Make_ItemList();
	void add_User(int User_id);
	void add_Item(int Item_id);
	void learning_U_Vector(int user_id,int item_id,float user_pref);
	void learning_I_Vector(int user_id,int item_id,int item_prior);

private:
	MemberVector* U_Vectorlist;
	MemberVector* I_Vectorlist;

	int num_of_user;
	int num_of_item;

	sql::Connection* _con;

};

#endif MEMBER_MANAGER_H