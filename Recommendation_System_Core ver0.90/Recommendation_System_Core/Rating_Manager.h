//#include "Recommendation_Controller.h"
//#include "structs.h"
//#include "stdafx.h"

#ifndef RATING_MANAGER_H
#define RATING_MANAGER_H

class Rec_Controller;
class Rec_Model_Manager;

class Rating_Manager{
public:
	friend Rec_Controller;
	friend Rec_Model_Manager;

	Rating_Manager(sql::Connection* con,int num_user,int num_item);
	~Rating_Manager();

	void Make_RatingTable();
	void Input_Rating(int user_id,int item_id,int user_pref);
	float Rating_Normalizer(float dev,float user_pref,float m);
	float Rating_Prediction(int User_id,int Item_id,int* User_Cluster_Table,int* Item_Cluster_Table);
	void add_User(int User_id);
	void add_Item(int Item_id);
	float distance(bool type,int target_id,int client_id);
	float _mean(int user_id);
	float deviation(int user_id,float m);
	void Input_Rating(int user_id,int item_id,int user_pref,float dev,float m);

private:
	float** Rating_Table;

	float* m;
	float* d;

	int num_of_user;
	int num_of_item;

	sql::Connection* _con;

};

#endif RATING_MANAGER_H