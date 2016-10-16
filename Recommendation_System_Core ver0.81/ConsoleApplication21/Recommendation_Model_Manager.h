//#include "Recommendation_Controller.h"
//#include "Clustering_Manager.h"
//#include "Member_Manager.h"
//#include "Rating_Manager.h"
//#include "structs.h"
//#include "stdafx.h"

#ifndef RECOMMENDATION_MODEL_MANAGER_H
#define RECOMMENDATION_MODEL_MANAGER_H

class Rec_Controller;

class Rec_Model_Manager{
public:
	friend Rec_Controller;

	//friend Member_Manager;
	//friend Cluster_Manager;
	//friend Rating_Manager;

	Rec_Model_Manager();
	~Rec_Model_Manager();

	void Time_Interrupt();
	void Input_Rating(int user_id,int item_id,int user_pref,int item_prior);
	void Input_Member(bool Type,int id);

	/*
	static unsigned int WINAPI Time_Interrupt(){
		SYSTEMTIME t;

		while(true){
			GetLocalTime(&t);
			if(t.wHour==03) C_Manager->Create_Cluster();
		}
	}

	static unsigned int WINAPI Rec_Model_Manager::Input_Rating(){
		while(true){
			if(Rating_Input_Queue.num==0);
			else{
				Rating_Point temp=Rating_Input_Queue.pop();
				R_Manager->Input_Rating(temp);
			}
		}
	}

	static unsigned int WINAPI Rec_Model_Manager::Input_Member(){
		while(true){
			if(Member_Input_Queue.num==0);
			else{
				Member temp=Member_Input_Queue.pop();
				if(temp.Type==1){
					num_of_user++;
				}
			}
		}
	}
	*/

private:
	Cluster_Manager* C_Manager;
	Member_Manager* M_Manager;
	Rating_Manager* R_Manager;

	int num_of_user;
	int num_of_item;

};

#endif RECOMMENDATION_MODEL_MANAGER_H