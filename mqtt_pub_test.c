#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

void on_publish(struct mosquitto *mosq, void * userdata, int mid){
    printf("Message with ID %d has been published.\n",mid);
}

int main(int argc, char** argv){
    struct mosquitto *mosq = NULL;
    int rc = 0;

    if(argc !=4){
        printf("illegal arguments.\n");
        printf("%s [ID] [Topic] [Message]\n",argv[0]);
        printf("- ex,\n");
        printf("- $ %s my_client_ID my_topic\n\n",argv[0]);
        exit(0);
    }

    mosquitto_lib_init();
    mosq = mosquitto_new(argv[1], true, NULL);
    mosquitto_publish_callback_set(mosq, on_publish);

    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if( rc != MOSQ_ERR_SUCCESS){
        fprintf(stderr,"Error: Could not connect to MQTT broker.\n");
        mosquitto_lib_cleanup();
        return 1;
    }

    int qos = 0;
    bool retain = false;
    
    rc = mosquitto_publish(mosq, NULL, argv[4], strlen(argv[3]), argv[3], qos, retain);
    if(rc != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "Error: Cou;d not publish message to topic %s.\n", argv[2]);
        mosquitto_lib_cleanup();
        return 1;
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    
    return 0;
}