/**
 * ID for the status of the server processing result
 */
#ifndef PROTOCOL_RESULTID_H_
#define PROTOCOL_RESULTID_H_

namespace Protocol {

enum class ResultId {
	Success,
	InvalidAction,
	InvalidClass,
	InvalidObject,
	InvalidMember,
	InvalidData,
	UnknownError,
};

}

#endif // PROTOCOL_RESULTID_H_