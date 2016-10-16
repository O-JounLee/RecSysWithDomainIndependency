#ifndef STRUCT_H
#define STRUCT_H

struct MemberVector{
	float Element[5];
};

/*
struct Pre_UserVector{
	int user_id;
	float Element[5];
};

typedef struct{
	int item_id;
	float Element[5];
}Pre_ItemVector;
*/

struct Cluster_Info{
	MemberVector mean;
	MemberVector s_deviation;
	float prob;
};

struct ItemEffect{
	int to_under40s;
	int to_40s;
	int to50s;
	int to_upto60s;
};

struct UserProfile{
	int age;
	bool hyperlipidemia;
	bool diabetesmellitus;
	bool hypertension;
};

struct ItemProperty{
	ItemEffect to_hyperlipidemia;
	ItemEffect to_diabetesmellitus;
	ItemEffect to_hypertension;
};

struct Rec_Content{
	int content_id;
};

struct Rating_Point{
	int user_id;
	int item_id;
	int user_pref;  //사용자 만족도
	int item_prior; //아이템 적중도
};

struct Member{
	bool Type;
	int id;
};

//Queue<int> Rec_Request_Queue;
//Queue<Rating_Point> Rating_Input_Queue;
//Queue<Member> Member_Input_Queue;

#endif STRUCT_H