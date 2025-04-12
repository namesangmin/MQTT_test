#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>

void on_message(struct mosquitto * mosq, void *userdata, const struct mosquitto_message *message){
    printf("Received message on topic '%s': %s\n",message->topic, (char*)message->payload);
}

int main(int argc, char ** argv){ // argc: 인자 개수, argv: 인자 배열(명령줄 인자자)
    struct mosquitto *mosq = NULL; // 클라이언트 객체를 가리킬 포인터

    if(argc !=3){ // 3개가 아니면 강제 종료 mqtt_sub myID, myTopic
        printf("illegal arguments.\n");
        printf("--------------------\n");
        printf("%s [ID] [Topic]\n",argv[0]);
        printf("- ex,\n");
        printf("- $ %s my_client_ID my_topic\n\n",argv[0]);
        exit(0);
    }

    mosquitto_lib_init();
    mosq = mosquitto_new(argv[1],true,NULL); // argv[1]: 클라이언트가 입력한 클라이언트 ID, true: 기존 세션 없이 새로 연결, NULL: 사용자 정의 데이터 안 씀씀
    
    mosquitto_message_callback_set(mosq,on_message); // 메세지 수신 시 콜백함수 설정
    // 콜백 함수란: 어떤 일이 생기면 어떤 함수를 호출함 -> 미리 함수를 등록해 놓음

    mosquitto_connect(mosq,"localhost",1883,60); // 브로커에 포트 1883으로 연결, 60: 초
    mosquitto_subscribe(mosq,NULL,argv[2],0); // 사용자가 입력한 토픽 (argv[2])을 Qos 0으로 구독
    mosquitto_loop_forever(mosq,-1,1); // 메세지를 계속 기다리며 처리, 내부적으로 select()를 이용해 네트워크 처리, -1: 무한대기, 1ㅣ 메세지 처리 실패시 자동 재연결 시도
    
    // 클라이언트 인스턴스 삭제
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}