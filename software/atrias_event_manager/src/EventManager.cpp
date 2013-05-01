#include "atrias_event_manager/EventManager.hpp"

namespace atrias {

namespace eventManager {

EventManager::EventManager(std::string const &name) :
	RTT::TaskContext(name),
	cmOut("cm_events_out"),
	guiOut("gui_events_out")
{
	this->addPort(cmOut);
	this->addPort(guiOut);

	// Lets make this an event port, so \a updateHook() will
	// get called when we receive a new event.
	this->addEventPort("eventsIn", this->eventsIn, boost::bind(&EventManager::eventInCallback, this, _1));

	// Connect eventsIn port w/ RTOps's output (through the ROS topic)
	auto policy = RTT::ConnPolicy::buffer(10);
	policy.transport = 3; // ROS == 3
	policy.name_id = "rt_events";
	this->eventsIn.createStream(policy);
	
	return;
}

void EventManager::sendCM(atrias_msgs::rt_ops_event &event) {
	cmOut.write(event);
}

void EventManager::sendGUI(atrias_msgs::rt_ops_event &event) {
	guiOut.write(event);
}

atrias_msgs::rt_ops_event EventManager::toNonRtEvent(atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>> &msg) {
	// The message we'll output
	atrias_msgs::rt_ops_event out;

	// Set event type
	out.event = msg.event;

	// Copy over metadata
	out.metadata.reserve(msg.metadata.size());
	for(auto i: msg.metadata)
		out.metadata.push_back(i);

	return out;
}

void EventManager::eventInCallback(RTT::base::PortInterface* portInterface) {
	// Read the event from the input port, convert to non-rt event
	atrias_msgs::rt_ops_event_<RTT::os::rt_allocator<uint8_t>> rt_event;
	this->eventsIn.read(rt_event);
	auto event = this->toNonRtEvent(rt_event);

	switch ((event::Event) event.event) {
		case event::Event::MISSED_DEADLINE: {
			auto metadata = shared::EventManip<>::readMetadata<event::MissedDeadlineMetadata<>>(event);
			log(RTT::Warning) << "[EventManager] Missed deadline of " << metadata.overshoot
			                  << "ns at location: " << metadata.location << RTT::endlog();
			sendGUI(event);
			break;
		}
		case event::Event::SAFETY:
		case event::Event::CONTROLLER_CUSTOM:
			sendGUI(event);
			break;

		default:
			// If we don't know what it is, then we should probably send it to both,
			// in case one of them does recognize it.
			sendCM(event);
			sendGUI(event);

			// Also, complain to the console
			log(RTT::Warning) << "[EventManager] Unrecognized event: " << RTT::endlog();
			log(RTT::Warning) << event << RTT::endlog();
			break;
	}
}

ORO_CREATE_COMPONENT(EventManager);

// End namespaces
}
}

// vim: noexpandtab
