/*******************************************************************************
 (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/
/* listeners_subscriber.cxx

   A subscription example

   This file is derived from code automatically generated by the rtiddsgen 
   command:

   rtiddsgen -language C++ -example <arch> listeners.idl

   Example subscription of type listeners automatically generated by 
   'rtiddsgen'. To test them follow these steps:

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
                          
       On Unix: 
       
       objs/<arch>/listeners_publisher <domain_id> 
       objs/<arch>/listeners_subscriber <domain_id> 
                            
       On Windows:
       
       objs\<arch>\listeners_publisher <domain_id>  
       objs\<arch>\listeners_subscriber <domain_id>   
              
       
modification history
------------ -------       
*/

#include <stdio.h>
#include <stdlib.h>
#ifdef RTI_VX653
#include <vThreadsData.h>
#endif
#include "listeners.h"
#include "listenersSupport.h"
#include "ndds/ndds_cpp.h"


class ParticipantListener : public DDSDomainParticipantListener {
public:
    virtual void on_requested_deadline_missed(
        DDSDataReader* /*reader*/,
	const DDS_RequestedDeadlineMissedStatus& /*status*/) 
    {
	printf("ParticipantListener: on_requested_deadline_missed()\n");
    }
    
    virtual void on_requested_incompatible_qos(
        DDSDataReader* /*reader*/,
	const DDS_RequestedIncompatibleQosStatus& /*status*/)
    {
	printf("ParticipantListener: on_requested_incompatible_qos()\n");
    }
    
    virtual void on_sample_rejected(
        DDSDataReader* /*reader*/,
	const DDS_SampleRejectedStatus& /*status*/) 
    {
	printf("ParticipantListener: on_sample_rejected()\n");
    }

    virtual void on_liveliness_changed(
        DDSDataReader* /*reader*/,
	const DDS_LivelinessChangedStatus& /*status*/) 
    {
        printf("ParticipantListener: on_liveliness_changed()\n");
    }
    
    virtual void on_sample_lost(
        DDSDataReader* /*reader*/,
	const DDS_SampleLostStatus& /*status*/) 
    {
	printf("ParticipantListener: on_sample_lost()\n");
    }

    virtual void on_subscription_matched(
        DDSDataReader* /*reader*/,
	const DDS_SubscriptionMatchedStatus& /*status*/) 
    {
	printf("ParticipantListener: on_subscription_matched()\n");
    }

    virtual void on_data_available(DDSDataReader* reader) 
    {
        printf("ParticipantListener: on_data_available()\n");
    }
    
    virtual void on_data_on_readers(DDSSubscriber *subscriber /*subscriber*/)
    {
	printf("ParticipantListener: on_data_on_readers()\n");

	/* notify_datareaders() only calls on_data_available for
	 * DataReaders with unread samples
	 */
	DDS_ReturnCode_t retcode = subscriber->notify_datareaders();
	if (retcode != DDS_RETCODE_OK) {
	    printf("notify_datareaders() error: %d\n", retcode);
	}
	
    }

    virtual void on_inconsistent_topic(
        DDSTopic *topic, 
	const DDS_InconsistentTopicStatus &status) 
    {
        printf("ParticipantListener: on_inconsistent_topic()\n");
    }
};


class SubscriberListener : public DDSSubscriberListener {
  public:
    virtual void on_requested_deadline_missed(
        DDSDataReader* /*reader*/,
        const DDS_RequestedDeadlineMissedStatus& /*status*/) 
    {
	printf("SubscriberListener: on_requested_deadline_missed()\n");
    }
    
    virtual void on_requested_incompatible_qos(
        DDSDataReader* /*reader*/,
        const DDS_RequestedIncompatibleQosStatus& /*status*/) 
    {
	printf("SubscriberListener: on_requested_incompatible_qos()\n");
    }
    
    virtual void on_sample_rejected(
        DDSDataReader* /*reader*/,
	const DDS_SampleRejectedStatus& /*status*/) 
    {
	printf("SubscriberListener: on_sample_rejected()\n");
    }

    virtual void on_liveliness_changed(
        DDSDataReader* /*reader*/,
	const DDS_LivelinessChangedStatus& /*status*/) 
    {
	printf("SubscriberListener: on_liveliness_changed()\n");
    }

    virtual void on_sample_lost(
        DDSDataReader* /*reader*/,
	const DDS_SampleLostStatus& /*status*/) 
    {
	printf("SubscriberListener: on_sample_lost()\n");
    }

    virtual void on_subscription_matched(
        DDSDataReader* /*reader*/,
	const DDS_SubscriptionMatchedStatus& /*status*/) 
    {
	printf("SubscriberListener: on_subscription_matched()\n");
    }

    virtual void on_data_available(DDSDataReader* reader) 
    {
        printf("SubscriberListener: on_data_available()\n");
    }

    virtual void on_data_on_readers(DDSSubscriber *sub /*subscriber*/) 
    {
	static int count = 0;
	printf("SubscriberListener: on_data_on_readers()\n");

	// notify_datareaders() only calls on_data_available for
	// DataReaders with unread samples
	DDS_ReturnCode_t retcode = sub->notify_datareaders();
	if (retcode != DDS_RETCODE_OK) {
	    printf("notify_datareaders() error: %d\n", retcode);
	}
	
	if (++count > 3) {
	    DDS_StatusMask newmask = DDS_STATUS_MASK_ALL;
	    // 'Unmask' DATA_ON_READERS status for listener
	    newmask &= ~DDS_DATA_ON_READERS_STATUS;
	    sub->set_listener(this, newmask);
	    printf("Unregistering SubscriberListener::on_data_on_readers()\n");
	}
    }
};


class ReaderListener : public DDSDataReaderListener {
  public:
    virtual void on_requested_deadline_missed(
        DDSDataReader* /*reader*/,
	const DDS_RequestedDeadlineMissedStatus& /*status*/) 
    {
	printf("ReaderListener: on_requested_deadline_missed()\n");
    }
    
    virtual void on_requested_incompatible_qos(
        DDSDataReader* /*reader*/,
	const DDS_RequestedIncompatibleQosStatus& /*status*/) 
    {
	printf("ReaderListener: on_requested_incompatible_qos()\n");
    }
    
    virtual void on_sample_rejected(
        DDSDataReader* /*reader*/,
	const DDS_SampleRejectedStatus& /*status*/) 
    {
	printf("ReaderListener: on_sample_rejected()\n");
    }

    virtual void on_liveliness_changed(
        DDSDataReader* /*reader*/,
	const DDS_LivelinessChangedStatus& status) 
    {
        printf("ReaderListener: on_liveliness_changed()\n");
        printf("  Alive writers: %d\n", status.alive_count);
    }

    virtual void on_sample_lost(
        DDSDataReader* /*reader*/,
	const DDS_SampleLostStatus& /*status*/) 
    {
	printf("ReaderListener: on_sample_lost()\n");
    }
    
    virtual void on_subscription_matched(
        DDSDataReader* /*reader*/,
	const DDS_SubscriptionMatchedStatus& /*status*/) 
    {
	printf("ReaderListener: on_subscription_matched()\n");
    }
    
    virtual void on_data_available(DDSDataReader* reader);
};

void ReaderListener::on_data_available(DDSDataReader* reader)
{
    listenersDataReader *listeners_reader = NULL;
    listenersSeq data_seq;
    DDS_SampleInfoSeq info_seq;
    DDS_ReturnCode_t retcode;
    int i;

    listeners_reader = listenersDataReader::narrow(reader);
    if (listeners_reader == NULL) {
        printf("DataReader narrow error\n");
        return;
    }

    retcode = listeners_reader->take(
        data_seq, info_seq, DDS_LENGTH_UNLIMITED,
        DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

    if (retcode == DDS_RETCODE_NO_DATA) {
        return;
    } else if (retcode != DDS_RETCODE_OK) {
        printf("take error %d\n", retcode);
        return;
    }

    for (i = 0; i < data_seq.length(); ++i) {
	/* If the reference we get is valid data, it means we have actual 
	 * data available, otherwise we got metadata */
        if (info_seq[i].valid_data) {
            listenersTypeSupport::print_data(&data_seq[i]);
        } else {
            printf("   Got metadata\n");
	}
    }

    retcode = listeners_reader->return_loan(data_seq, info_seq);
    if (retcode != DDS_RETCODE_OK) {
        printf("return loan error %d\n", retcode);
    }
}

/* Delete all entities */
static int subscriber_shutdown(
    DDSDomainParticipant *participant)
{
    DDS_ReturnCode_t retcode;
    int status = 0;

    if (participant != NULL) {
        retcode = participant->delete_contained_entities();
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_contained_entities error %d\n", retcode);
            status = -1;
        }

        retcode = DDSTheParticipantFactory->delete_participant(participant);
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_participant error %d\n", retcode);
            status = -1;
        }
    }

    /* RTI Connext provides the finalize_instance() method on
       domain participant factory for people who want to release memory used
       by the participant factory. Uncomment the following block of code for
       clean destruction of the singleton. */
    /*
      retcode = DDSDomainParticipantFactory::finalize_instance();
      if (retcode != DDS_RETCODE_OK) {
        printf("finalize_instance error %d\n", retcode);
        status = -1;
      }
    */
    return status;
}

extern "C" int subscriber_main(int domainId, int sample_count)
{
    DDSDomainParticipant *participant = NULL;
    DDSSubscriber *subscriber = NULL;
    DDSTopic *topic = NULL;
    DDSDataReader *reader = NULL;
    DDS_ReturnCode_t retcode;
    const char *type_name = NULL;
    int count = 0;
    DDS_Duration_t receive_period = {4,0};
    int status = 0;

    /* Create Particiant Listener */
    ParticipantListener *participant_listener = new ParticipantListener();
    if (participant_listener ==  NULL) {
        printf("participant listener instantiation error\n");
        subscriber_shutdown(participant);
        return -1;
    }

    /* We associate the participant_listener to the participant and set the 
     * status mask to get all the statuses */
    participant = 
	DDSTheParticipantFactory->create_participant(domainId, 
						     DDS_PARTICIPANT_QOS_DEFAULT, 
						     participant_listener /* listener */, 
						     DDS_STATUS_MASK_ALL /* get all statuses */);
    if (participant == NULL) {
        printf("create_participant error\n");
        subscriber_shutdown(participant);
	delete participant_listener;
        return -1;
    }

    /* Create Subscriber Listener */
    SubscriberListener *subscriber_listener = new SubscriberListener();
    if (subscriber_listener ==  NULL) {
        printf("subscriber listener instantiation error\n");
        subscriber_shutdown(participant);
	delete participant_listener;
        return -1;
    }

    /* Here we associate the subscriber listener to the subscriber and set the 
     * status mask to get all the statuses */
    subscriber = participant->create_subscriber(DDS_SUBSCRIBER_QOS_DEFAULT, 
						subscriber_listener /* listener */, 
						DDS_STATUS_MASK_ALL /* get all statuses */);
    if (subscriber == NULL) {
        printf("create_subscriber error\n");
        subscriber_shutdown(participant);
	delete participant_listener;
	delete subscriber_listener;
        return -1;
    }

    /* Register the type before creating the topic */
    type_name = listenersTypeSupport::get_type_name();
    retcode = listenersTypeSupport::register_type(
        participant, type_name);
    if (retcode != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode);
        subscriber_shutdown(participant);
	delete participant_listener;
	delete subscriber_listener;
        return -1;
    }

    /* To customize the topic QoS, use 
       the configuration file USER_QOS_PROFILES.xml */
    topic = participant->create_topic(
        "Example listeners",
        type_name, DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);
    if (topic == NULL) {
        printf("create_topic error\n");
        subscriber_shutdown(participant);
	delete participant_listener;
	delete subscriber_listener;
        return -1;
    }

    /* Create a data reader listener */
    ReaderListener *reader_listener = new ReaderListener();
    if (reader_listener ==  NULL) {
        printf("reader listener instantiation error\n");
        subscriber_shutdown(participant);
	delete participant_listener;
	delete subscriber_listener;
        return -1;
    }

    /* Here we associate the data reader listener to the reader.
     * We just listen for liveliness changed and data available,
     * since most specific listeners will get called */
    reader = 
	subscriber->create_datareader(topic, 
				      DDS_DATAREADER_QOS_DEFAULT, 
				      reader_listener /* listener */,
				      DDS_LIVELINESS_CHANGED_STATUS | 
				      DDS_DATA_AVAILABLE_STATUS  /* statuses */);
    if (reader == NULL) {
        printf("create_datareader error\n");
        subscriber_shutdown(participant);
	delete participant_listener;
	delete subscriber_listener;
        delete reader_listener;
        return -1;
    }

    /* Main loop */
    for (count=0; (sample_count == 0) || (count < sample_count); ++count) {

        NDDSUtility::sleep(receive_period);
    }

    /* Delete all entities */
    status = subscriber_shutdown(participant);
    delete reader_listener;
    delete subscriber_listener;
    delete participant_listener;

    return status;
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
    NDDSConfigLogger::get_instance()->
        set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API, 
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
    NDDSConfigLogger::get_instance()->
        set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API, 
                                  NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */
                                  
    return subscriber_main(domainId, sample_count);
}
#endif

#ifdef RTI_VX653
const unsigned char* __ctype = *(__ctypePtrGet());

extern "C" void usrAppInit ()
{
#ifdef  USER_APPL_INIT
    USER_APPL_INIT;         /* for backwards compatibility */
#endif
    
    /* add application specific code here */
    taskSpawn("sub", RTI_OSAPI_THREAD_PRIORITY_NORMAL, 0x8, 0x150000, (FUNCPTR)subscriber_main, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   
}
#endif

