//#include "Recommender.h"
//#include "Rule_Apllier.h"
//#include "structs.h"
//#include "stdafx.h"

#ifndef RECMMENDATION_MANAGER_H
#define RECMMENDATION_MANAGER_H

class Rec_Controller;
class Rule_Applier;
class Reccomender;

class Rec_Manager{
public:
	friend Rec_Controller;

	//friend Reccomender;
	//friend Rule_Applier;

	Rec_Manager();
	~Rec_Manager();

	int* get_Rec_list(int user_id,int* cur_Cluster_Table,float** cur_Rating_Table);

private:
	Reccomender* Rec_executer;
	Rule_Applier* Rule_applier;

	int num_of_user;
	int num_of_item;

};

#endif RECMMENDATION_MANAGER_H