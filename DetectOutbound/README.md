̽�⹤����������������MAC��ΪӲ��ID, ���ֻ��һ����������,ֱ���ã�����ͨ�������������������

ʹ�÷�ʽ����

    GCDetectOutbound* outbound = new GCDetectOutbound(this);
    connect(outbound,&GCDetectOutbound::sgLocalInterface,[=](QNetworkInterface face){
        QString hardid = GCDetectOutbound::faceID(face);
        qInfo()<<"get hardid"<<hardid;
        m_configSet->setValue("DeviceID",hardid);
    });

    QNetworkInterface face;
    if(outbound->tryObtainFace(face)){
        QString hardid = GCDetectOutbound::faceID(face);
        qInfo()<<face.hardwareAddress();
        qInfo()<<"get hardid"<<hardid;
        m_configSet->setValue("DeviceID",hardid);
    }
    else{
        //�õ���������������connect��
        outbound->init();
        outbound->start();
    }
