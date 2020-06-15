#pragma once
#include<zinx.h>
#include<string>
#include<google/protobuf/message.h>
class GameMsg:
	public UserData
{
public:
	enum MSG_TYPE
	{
		MSG_TYPE_SYNCPID_LOGIN=1,		//���ID+�������
		MSG_TYPE_TALK=2,				//��������
		MSG_TYPE_POSITION=3,			//��λ��
		MSG_TYPE_BROADCAST=200,		//���ID,��������/��ʼλ��/����/��λ�� ���ⷢ�͵Ĺ㲥����
		MSG_TYPE_SYNCPID_LOGOFF=201,	//���ID���������
		MSG_TYPE_SYNCPLAYERS=202		//��Χ����ǵ�λ��
	}MsgType;
	google::protobuf::Message* poMessage;

public:
	/*���ݾ������Ϣ��������*/
	GameMsg(MSG_TYPE _type, google::protobuf::Message* _msg);
	/*�������л�����Ϣ��������*/
	GameMsg(MSG_TYPE _type, std::string _outData);

	/*����Ϣ���л�*/
	std::string GetSerialize();

	virtual ~GameMsg();

};

