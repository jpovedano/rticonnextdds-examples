/* listeners_subscriber.c

   A subscription example

   This file is derived from code automatically generated by the rtiddsgen 
   command:

   rtiddsgen -language C -example <arch> listeners.idl

   Example subscription of type listeners automatically generated by 
   'rtiddsgen'. To test them, follow these steps:

   (1) Compile this file and the example publication.

   (2) Start the subscription with the command
       objs/<arch>/listeners_subscriber <domain_id> <sample_count>

   (3) Start the publication with the command
       objs/<arch>/listeners_publisher <domain_id> <sample_count>

   (4) [Optional] Specify the list of discovery initial peers and 
       multicast receive addresses via an environment variable or a file 
       (in the current working directory) called NDDS_DISCOVERY_PEERS. 
       
   You can run any number of publishers and subscribers programs, and can 
   add and remove them dynamically from the domain.
              
                                   
   Example:
        
       To run the example application on domain <domain_id>:
                          
       On UNIX systems: 
       
       objs/<arch>/listeners_publisher <domain_id> 
       objs/<arch>/listeners_subscriber <domain_id> 
                            
       On Windows systems:
       
       objs\<arch>\listeners_publisher <domain_id>  
       objs\<arch>\listeners_subscriber <domain_id>   
       
       
modification history
------------ -------       
*/

#include <stdio.h>
#include <stdlib.h>
#include "ndds/ndds_c.h"
#include "listeners.h"
#include "listenersSupport.h"

/* -------------------------------------------------------
 * Participant Listener events 
 * ------------------------------------------------------- */

void 
ParticipantListener_on_requested_deadline_missed(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_RequestedDeadlineMissedStatus *status)
{
    printf("ParticipantListener: on_requested_deadline_missed()\n");
}

void ParticipantListener_on_requested_incompatible_qos(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_RequestedIncompatibleQosStatus *status)
{
    printf("ParticipantListener: on_requested_incompatible_qos()\n");
}

void ParticipantListener_on_sample_rejected(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SampleRejectedStatus *status)
{
    printf("ParticipantListener: on_sample_rejected()\n");
}

void ParticipantListener_on_liveliness_changed(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_LivelinessChangedStatus *status)
{
    printf("ParticipantListener: on_liveliness_changed()\n");
}

void ParticipantListener_on_sample_lost(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SampleLostStatus *status)
{
    printf("ParticipantListener: on_sample_lost()\n");
}

void ParticipantListener_on_subscription_matched(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SubscriptionMatchedStatus *status)
{
    printf("ParticipantListener: on_subscription_matched()\n");
}

void ParticipantListener_on_data_available(
    void* listener_data,
    DDS_DataReader* reader)
{
    printf("ParticipantListener: on_data_available()\n");
}

void ParticipantListener_on_data_on_readers(
    void* listener_data,
    DDS_Subscriber* sub)
{
    DDS_ReturnCode_t retcode;
    printf("ParticipantListener: on_data_on_readers()\n");
    
    /* notify_datareaders() only calls on_data_available for
     * DataReaders with unread samples
     */
    retcode = DDS_Subscriber_notify_datareaders(sub);
    if (retcode != DDS_RETCODE_OK) {
        printf("notify_datareaders() error: %d\n", retcode);
    }

}

void ParticipantListener_on_inconsistent_topic(
    void* listener_data,
    DDS_Topic* topic,
    const struct DDS_InconsistentTopicStatus *status)
{
    printf("ParticipantListener: on_inconsistent_topic()\n");
}


/* -------------------------------------------------------
 * Subscriber Listener events 
 * ------------------------------------------------------- */

void SubscriberListener_on_requested_deadline_missed(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_RequestedDeadlineMissedStatus *status)
{
    printf("SubscriberListener: on_requested_deadline_missed()\n");
}

void SubscriberListener_on_requested_incompatible_qos(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_RequestedIncompatibleQosStatus *status)
{
    printf("SubscriberListener: on_requested_incompatible_qos()\n");
}

void SubscriberListener_on_sample_rejected(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SampleRejectedStatus *status)
{
    printf("SubscriberListener: on_sample_rejected()\n");
}

void SubscriberListener_on_liveliness_changed(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_LivelinessChangedStatus *status)
{
    printf("SubscriberListener: on_liveliness_changed()\n");
}

void SubscriberListener_on_sample_lost(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SampleLostStatus *status)
{
    printf("SubscriberListener: on_sample_lost()\n");
}

void SubscriberListener_on_subscription_matched(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SubscriptionMatchedStatus *status)
{
    printf("SubscriberListener: on_subscription_matched()\n");
}

void SubscriberListener_on_data_available(
    void* listener_data,
    DDS_DataReader* reader)
{
    printf("SubscriberListener: on_data_available()\n");
}

void SubscriberListener_on_data_on_readers(
    void* listener_data,
    DDS_Subscriber* sub)
{
    DDS_ReturnCode_t retcode;
    static int count = 0;

    printf("SubscriberListener: on_data_on_readers()\n");

    /* notify_datareaders() only calls on_data_available for
     * DataReaders with unread samples */
    retcode = DDS_Subscriber_notify_datareaders(sub);
    if (retcode != DDS_RETCODE_OK) {
        printf("notify_datareaders() error: %d\n", retcode);
    }

    if (++count > 3) {
        /* Stop receiving DATA_ON_READERS status */
        DDS_StatusMask newmask = DDS_REQUESTED_DEADLINE_MISSED_STATUS | DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS | 
            DDS_SAMPLE_REJECTED_STATUS | DDS_LIVELINESS_CHANGED_STATUS | DDS_SAMPLE_LOST_STATUS |
            DDS_SUBSCRIPTION_MATCHED_STATUS | DDS_DATA_AVAILABLE_STATUS;
        DDS_Subscriber_set_listener(sub, listener_data, newmask);
        printf("Unregistering SubscriberListener::on_data_on_readers()\n");
    }
}

/* -------------------------------------------------------
 * Reader Listener events 
 * ------------------------------------------------------- */

void ReaderListener_on_requested_deadline_missed(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_RequestedDeadlineMissedStatus *status)
{
    printf("ReaderListener: on_requested_deadline_missed()\n");
}

void ReaderListener_on_requested_incompatible_qos(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_RequestedIncompatibleQosStatus *status)
{
    printf("ReaderListener: on_requested_incompatible_qos()\n");
}

void ReaderListener_on_sample_rejected(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SampleRejectedStatus *status)
{
    printf("ReaderListener: on_sample_rejected()\n");
}

void ReaderListener_on_liveliness_changed(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_LivelinessChangedStatus *status)
{
    printf("ReaderListener: on_liveliness_changed()\n");
    printf("  Alive writers: %d\n", status->alive_count);
}

void ReaderListener_on_sample_lost(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SampleLostStatus *status)
{
    printf("ReaderListener: on_sample_lost()\n");
}

void ReaderListener_on_subscription_matched(
    void* listener_data,
    DDS_DataReader* reader,
    const struct DDS_SubscriptionMatchedStatus *status)
{
    printf("ReaderListener: on_subscription_matched()\n");
}

void ReaderListener_on_data_available(
    void* listener_data,
    DDS_DataReader* reader)
{
    listenersDataReader *listeners_reader = NULL;
    struct listenersSeq data_seq = DDS_SEQUENCE_INITIALIZER;
    struct DDS_SampleInfoSeq info_seq = DDS_SEQUENCE_INITIALIZER;
    DDS_ReturnCode_t retcode;
    int i;

    printf("ReaderListener: on_data_available()\n");

    listeners_reader = listenersDataReader_narrow(reader);
    if (listeners_reader == NULL) {
        printf("DataReader narrow error\n");
        return;
    }

    retcode = listenersDataReader_take(
        listeners_reader,
        &data_seq, &info_seq, DDS_LENGTH_UNLIMITED,
        DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);
    if (retcode == DDS_RETCODE_NO_DATA) {
        return;
    } else if (retcode != DDS_RETCODE_OK) {
        printf("take error %d\n", retcode);
        return;
    }

    for (i = 0; i < listenersSeq_get_length(&data_seq); ++i) {
	/* If the reference we get is valid data, it means we have actual 
	 * data available, otherwise we got metadata */
        if (DDS_SampleInfoSeq_get_reference(&info_seq, i)->valid_data) {
            listenersTypeSupport_print_data(
                listenersSeq_get_reference(&data_seq, i));
        } else {
            printf("   Got metadata\n");
        }
    }

    retcode = listenersDataReader_return_loan(
        listeners_reader,
        &data_seq, &info_seq);
    if (retcode != DDS_RETCODE_OK) {
        printf("return loan error %d\n", retcode);
    }
}


/* Delete all entities */
static int subscriber_shutdown(
    DDS_DomainParticipant *participant)
{
    DDS_ReturnCode_t retcode;
    int status = 0;

    if (participant != NULL) {
        retcode = DDS_DomainParticipant_delete_contained_entities(participant);
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_contained_entities error %d\n", retcode);
            status = -1;
        }

        retcode = DDS_DomainParticipantFactory_delete_participant(
            DDS_TheParticipantFactory, participant);
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_participant error %d\n", retcode);
            status = -1;
        }
    }

    /* RTI Connext provides the finalize_instance() method on
       domain participant factory for users who want to release memory used
       by the participant factory. Uncomment the following block of code for
       clean destruction of the singleton. */
/*
    retcode = DDS_DomainParticipantFactory_finalize_instance();
    if (retcode != DDS_RETCODE_OK) {
        printf("finalize_instance error %d\n", retcode);
        status = -1;
    }
*/

    return status;
}

static int subscriber_main(int domainId, int sample_count)
{
    DDS_DomainParticipant *participant = NULL;
    DDS_Subscriber *subscriber = NULL;
    DDS_Topic *topic = NULL;
    struct DDS_DomainParticipantListener participant_listener =
        DDS_DomainParticipantListener_INITIALIZER;
    struct DDS_SubscriberListener subscriber_listener =
        DDS_SubscriberListener_INITIALIZER;
    struct DDS_DataReaderListener reader_listener =
        DDS_DataReaderListener_INITIALIZER;
    DDS_StatusMask mask;
    DDS_DataReader *reader = NULL;
    DDS_ReturnCode_t retcode;
    const char *type_name = NULL;
    int count = 0;
    struct DDS_Duration_t poll_period = {1,0};


   /* Set up participant listener */
    participant_listener.as_subscriberlistener.as_datareaderlistener.on_requested_deadline_missed =
        ParticipantListener_on_requested_deadline_missed;
    participant_listener.as_subscriberlistener.as_datareaderlistener.on_requested_incompatible_qos =
        ParticipantListener_on_requested_incompatible_qos;
    participant_listener.as_subscriberlistener.as_datareaderlistener.on_sample_rejected =
        ParticipantListener_on_sample_rejected;
    participant_listener.as_subscriberlistener.as_datareaderlistener.on_liveliness_changed =
        ParticipantListener_on_liveliness_changed;
    participant_listener.as_subscriberlistener.as_datareaderlistener.on_sample_lost =
        ParticipantListener_on_sample_lost;
    participant_listener.as_subscriberlistener.as_datareaderlistener.on_subscription_matched =
        ParticipantListener_on_subscription_matched;
    participant_listener.as_subscriberlistener.as_datareaderlistener.on_data_available =
        ParticipantListener_on_data_available;
    participant_listener.as_subscriberlistener.on_data_on_readers =
        ParticipantListener_on_data_on_readers;
    participant_listener.as_topiclistener.on_inconsistent_topic =
        ParticipantListener_on_inconsistent_topic;
    participant_listener.as_subscriberlistener.as_datareaderlistener.as_listener.listener_data = 
	&participant_listener;
    
    mask = DDS_REQUESTED_DEADLINE_MISSED_STATUS | 
	DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS | 
        DDS_SAMPLE_REJECTED_STATUS | 
	DDS_LIVELINESS_CHANGED_STATUS | 
	DDS_SAMPLE_LOST_STATUS |
        DDS_SUBSCRIPTION_MATCHED_STATUS | 
	DDS_DATA_AVAILABLE_STATUS | 
	DDS_DATA_ON_READERS_STATUS |
        DDS_INCONSISTENT_TOPIC_STATUS;

    /* We associate the participant_listener to the participant and set the 
     * status mask just defined */
    participant = 
	DDS_DomainParticipantFactory_create_participant(DDS_TheParticipantFactory, 
							domainId, 
							&DDS_PARTICIPANT_QOS_DEFAULT,
							&participant_listener /* listener */, 
							mask /* status mask */);
    if (participant == NULL) {
        printf("create_participant error\n");
        subscriber_shutdown(participant);
        return -1;
    }


    /* Setup subscriber listener */
    subscriber_listener.as_datareaderlistener.on_requested_deadline_missed =
        SubscriberListener_on_requested_deadline_missed;
    subscriber_listener.as_datareaderlistener.on_requested_incompatible_qos =
        SubscriberListener_on_requested_incompatible_qos;
    subscriber_listener.as_datareaderlistener.on_sample_rejected =
        SubscriberListener_on_sample_rejected;
    subscriber_listener.as_datareaderlistener.on_liveliness_changed =
        SubscriberListener_on_liveliness_changed;
    subscriber_listener.as_datareaderlistener.on_sample_lost =
        SubscriberListener_on_sample_lost;
    subscriber_listener.as_datareaderlistener.on_subscription_matched =
        SubscriberListener_on_subscription_matched;
    subscriber_listener.as_datareaderlistener.on_data_available =
        SubscriberListener_on_data_available;
    subscriber_listener.on_data_on_readers =
        SubscriberListener_on_data_on_readers;
    subscriber_listener.as_datareaderlistener.as_listener.listener_data = 
	&subscriber_listener;

    mask = DDS_REQUESTED_DEADLINE_MISSED_STATUS | 
	DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS | 
	DDS_SAMPLE_REJECTED_STATUS | 
	DDS_LIVELINESS_CHANGED_STATUS | 
	DDS_SAMPLE_LOST_STATUS |
        DDS_SUBSCRIPTION_MATCHED_STATUS | 
	DDS_DATA_AVAILABLE_STATUS | 
	DDS_DATA_ON_READERS_STATUS;

    /* Here we associate the subscriber listener to the subscriber and set the 
     * status mask to the mask we have just defined */
    subscriber = DDS_DomainParticipant_create_subscriber(participant, 
							 &DDS_SUBSCRIBER_QOS_DEFAULT, 
							 &subscriber_listener /* listener */,
							 mask /* status mask */);
    if (subscriber == NULL) {
        printf("create_subscriber error\n");
        subscriber_shutdown(participant);
        return -1;
    }


    /* Register the type before creating the topic */
    type_name = listenersTypeSupport_get_type_name();
    retcode = listenersTypeSupport_register_type(participant, type_name);
    if (retcode != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode);
        subscriber_shutdown(participant);
        return -1;
    }

    topic = DDS_DomainParticipant_create_topic(
        participant, "Example listeners",
        type_name, &DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);
    if (topic == NULL) {
        printf("create_topic error\n");
        subscriber_shutdown(participant);
        return -1;
    }

    /* Setup data reader listener */
    reader_listener.on_requested_deadline_missed  =
        ReaderListener_on_requested_deadline_missed;
    reader_listener.on_requested_incompatible_qos =
        ReaderListener_on_requested_incompatible_qos;
    reader_listener.on_sample_rejected =
        ReaderListener_on_sample_rejected;
    reader_listener.on_liveliness_changed =
        ReaderListener_on_liveliness_changed;
    reader_listener.on_sample_lost =
        ReaderListener_on_sample_lost;
    reader_listener.on_subscription_matched =
        ReaderListener_on_subscription_matched;
    reader_listener.on_data_available =
        ReaderListener_on_data_available;
    reader_listener.as_listener.listener_data = 
	&reader_listener;

    
    /* Just listen for liveliness changed and data available,
     * since most specific listeners will get called */
    mask = DDS_LIVELINESS_CHANGED_STATUS | 
	DDS_DATA_AVAILABLE_STATUS;
    
    /* In the data reader we change the status mask to the mask
     * we have just defined */
    reader = DDS_Subscriber_create_datareader(
        subscriber, DDS_Topic_as_topicdescription(topic),
        &DDS_DATAREADER_QOS_DEFAULT, &reader_listener, mask);
    if (reader == NULL) {
        printf("create_datareader error\n");
        subscriber_shutdown(participant);
        return -1;
    }

    /* Main loop */
    for (count=0; (sample_count == 0) || (count < sample_count); ++count) {
        NDDS_Utility_sleep(&poll_period);
    }

    /* Cleanup and delete all entities */ 
    return subscriber_shutdown(participant);
}

#if defined(RTI_WINCE)
int wmain(int argc, wchar_t** argv)
{
    int domainId = 0;
    int sample_count = 0; /* infinite loop */
    
    if (argc >= 2) {
        domainId = _wtoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = _wtoi(argv[2]);
    }

    /* Uncomment this to turn on additional logging
    NDDS_Config_Logger_set_verbosity_by_category(
        NDDS_Config_Logger_get_instance(),
        NDDS_CONFIG_LOG_CATEGORY_API, 
        NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */
    
    return subscriber_main(domainId, sample_count);
}
#elif !(defined(RTI_VXWORKS) && !defined(__RTP__)) && !defined(RTI_PSOS)
int main(int argc, char *argv[])
{
    int domainId = 0;
    int sample_count = 0; /* infinite loop */

    if (argc >= 2) {
        domainId = atoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = atoi(argv[2]);
    }

    /* Uncomment this to turn on additional logging
    NDDS_Config_Logger_set_verbosity_by_category(
        NDDS_Config_Logger_get_instance(),
        NDDS_CONFIG_LOG_CATEGORY_API, 
        NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */
    
    return subscriber_main(domainId, sample_count);
}
#endif

#ifdef RTI_VX653
const unsigned char* __ctype = NULL;

void usrAppInit ()
{
#ifdef  USER_APPL_INIT
    USER_APPL_INIT;         /* for backwards compatibility */
#endif
    
    /* add application specific code here */
    taskSpawn("sub", RTI_OSAPI_THREAD_PRIORITY_NORMAL, 0x8, 0x150000, (FUNCPTR)subscriber_main, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   
}
#endif
