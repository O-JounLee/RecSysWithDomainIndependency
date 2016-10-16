//#include "structs.h"
//#include "Recommendation_Model_Manager.h"
//#include "Recommendation_Manager.h"
//#include "Clustering_Manager.h"
//#include "Member_Manager.h"
//#include "Rating_Manager.h"
//#include "stdafx.h"

#ifndef RECOMMENDATION_CONTROLLER_H
#define RECOMMENDATION_CONTROLLER_H

class Rec_Manager;

class Rec_Controller{
public:
	//friend Rec_Model_Manager;

	Rec_Controller(sql::Connection* con);
	~Rec_Controller();

	void Time_Interrupt();
	int* get_Rec_list(int user_id);
	void Input_Rating(int log_num);
	void Input_Member(bool type,int id);
	//void modify_member(bool type, int id);
	void modify_member(bool type, int id);

private:
	Rec_Model_Manager* Rec_Model;

	Rec_Manager* Rec_recommand_list;
	int num_Rec_Request;

	sql::Connection* _con;
};

#endif RECOMMENDATION_CONTROLLER_H