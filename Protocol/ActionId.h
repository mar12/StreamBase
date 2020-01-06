/**
 * ID for server action requested by the client
 */
#ifndef PROTOCOL_ACTIONID_H_
#define PROTOCOL_ACTIONID_H_

namespace Protocol {

enum class ActionId {
	SendRawData,
	RecvRawData,
	CreateObject,
	RemoveObject,
	ListClasses,
	ListObjects,
	ListAttribs,
	ListMethods,
	SetAttrib,
	GetAttrib,
	CallMethod,
};

}

#endif // PROTOCOL_ACTIONID_H_