/*
 Libnetvirt - the network virtualization library
 Copyright (C) 2011  Daniel Turull <danieltt@kth.se>

 Libnetvirt is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Libnetvirt is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with libnetvirt.  If not, see <http://www.gnu.org/licenses/>.

 */

#include "libnetvirt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * This example should compile and run indifferently with libxml-1.8.8 +
 * and libxml2-2.1.0 +
 * Check the COMPAT comments below
 */

/*
 * COMPAT using xml-config --cflags to get the include path this will
 * work with both
 */
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "libnetvirt/libnetvirt.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

endpoint*
parseEndpoint(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur1) {
	endpoint* ret = NULL;
	xmlNodePtr cur = cur1;
	struct in_addr ip;
	int i;
	/*
	 * allocate the struct
	 */
	ret = (endpoint*) malloc(sizeof(endpoint));
	if (ret == NULL) {
		fprintf(stderr, "out of memory\n");
		return (NULL);
	}
	memset(ret, 0, sizeof(endpoint));

	/* We don't care what the top level element name is */
	ret->uuid = atoi((const char*) xmlGetProp(cur, (const xmlChar *) "uuid"));
	ret->vlan = 0xffff;
	ret->address=0;
	ret->mask=0;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "swId"))
				&& (cur->ns == ns)) {
			ret->swId = atoi((const char*) xmlNodeListGetString(doc,
					cur->xmlChildrenNode, 1));
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "port"))
				&& (cur->ns == ns)) {
			ret->port = atoi((const char*) xmlNodeListGetString(doc,
					cur->xmlChildrenNode, 1));
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "mpls"))
				&& (cur->ns == ns)) {
			ret->mpls = atoi((const char*) xmlNodeListGetString(doc,
					cur->xmlChildrenNode, 1));
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "vlan"))
				&& (cur->ns == ns)) {
			ret->vlan = atoi((const char*) xmlNodeListGetString(doc,
					cur->xmlChildrenNode, 1));
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "network")) && (cur->ns
				== ns)) {
			const char *network = (const char*) xmlNodeListGetString(doc,	cur->xmlChildrenNode, 1);
			char address[15];
			char *mask;

			for(i=0;i<strlen(network) && i < 15;i++){
				if(network[i]=='/'){
					address[i]=0;
					break;
				}
				address[i]=network[i];
			}
			if(i==strlen(network) && i == 15)
				break;

			mask = strndup(&network[i+1],strlen(network)-strlen(address)-1);
			printf("network %s mask %s\n",address, mask);
			ip.s_addr = ret->address;
			inet_aton(address, (struct in_addr*) &ret->address);
			printf("net %d\n",ret->address);



			ret->mask=atoi(mask);
			free(mask);
			free(network);

		}

		/*PE and CE addresses for L3 VPN*/
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "addressPE")) && (cur->ns
				== ns)) {
			const char *network = (const char*) xmlNodeListGetString(doc,	cur->xmlChildrenNode, 1);
			inet_aton(network, (struct in_addr*) &ret->address);
			free(network);
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "addressCE")) && (cur->ns
				== ns)) {
			const char *network = (const char*) xmlNodeListGetString(doc,	cur->xmlChildrenNode, 1);
			inet_aton(network, (struct in_addr*) &ret->addressEx);
			free(network);

		}
		/*TODO add extra optional fields*/

		cur = cur->next;

	}
	return (ret);
}

constraint* parseConstraint(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur) {
	constraint* ret = NULL;
	char* type;
	//xmlNodePtr cur = cur1;
	/*
	 * allocate the struct
	 */
	ret = (constraint*) malloc(sizeof(constraint));
	if (ret == NULL) {
		fprintf(stderr, "out of memory\n");
		return (NULL);
	}
	memset(ret, 0, sizeof(constraint));
	type = (const char*) xmlGetProp(cur, (const xmlChar *) "type");
	if (!strcasecmp(type, "minbw"))
		ret->type = LIBNETVIRT_CONSTRAINT_MINBW;
	else if (!strcasecmp(type, "maxbw"))
		ret->type = LIBNETVIRT_CONSTRAINT_MAXBW;
	ret->src = atoi((const char*) xmlGetProp(cur, (const xmlChar *) "src"));
	ret->dst = atoi((const char*) xmlGetProp(cur, (const xmlChar *) "dst"));
	ret->value = atoi((const char*) xmlGetProp(cur, (const xmlChar *) "value"));

	return ret;
}

static fnsDesc* parseFNSdoc(xmlDocPtr doc) {
	fnsDesc* ret;
	endpoint* curEp;
	constraint* curCons;
	xmlNsPtr ns;
	xmlNodePtr cur, cur1;
	int nEp, nCons;
	if (doc == NULL)
		return (NULL);
	/*
	 * Check the document is of the right kind
	 */
	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return (NULL);
	}
	ns = xmlSearchNsByHref(doc, cur,
			(const xmlChar *) "http://www.sail-project.eu/fns");
	if (ns == NULL) {
		fprintf(stderr, "document of the wrong type, FNS Namespace not found\n");
		xmlFreeDoc(doc);
		return (NULL);
	}
	if (xmlStrcmp(cur->name, (const xmlChar *) "description")) {
		fprintf(stderr, "document of the wrong type, root node != description");
		xmlFreeDoc(doc);
		return (NULL);
	}

	cur = cur->xmlChildrenNode;
	while (cur && xmlIsBlankNode(cur)) {
		cur = cur -> next;
	}
	if (cur == 0)
		return (NULL);

	if ((xmlStrcmp(cur->name, (const xmlChar *) "fns")) || (cur->ns != ns)) {
		xmlDocDump(stderr, doc);
		xmlFreeDoc(doc);
		return (NULL);
	}

	/* count how many endpoints and constraint we have in the request */
	nEp = 0;
	nCons = 0;
	cur1 = cur->xmlChildrenNode;
	while (cur1 != NULL) {
		/*endpoint */
		if ((!xmlStrcmp(cur1->name, (const xmlChar *) "endpoint")) && (cur1->ns
				== ns)) {
			nEp++;
		}
		if ((!xmlStrcmp(cur1->name, (const xmlChar *) "constraint"))
				&& (cur1->ns == ns)) {
			nCons++;
		}
		/*forwarding*/
		cur1 = cur1->next;
	}

	ret = (fnsDesc*) malloc(GET_FNS_SIZE(nEp,nCons));
	if (ret == NULL) {
		fprintf(stderr, "out of memory\n");
		xmlFreeDoc(doc);
		return (NULL);
	}

	memset(ret, 0, GET_FNS_SIZE(nEp,nCons));

	/*get name*/
	u_char* name = xmlGetProp(cur, (const xmlChar *) "name");
	size_t size_n = strlen((char*) name);
	if (size_n >= 20)
		size_n = 19;
	memcpy(ret->name, name, size_n);

	/* get uuid*/
	ret->uuid = atoi((const char*) xmlGetProp(cur, (const xmlChar *) "uuid"));

	ret->nEp = 0;
	ret->nCons = 0;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		/* endpoint */
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "endpoint")) && (cur->ns
				== ns)) {
			curEp = parseEndpoint(doc, ns, cur);

			if (curEp != NULL) {
				memcpy(GET_ENDPOINT(ret,ret->nEp), curEp, sizeof(endpoint));
				ret->nEp++;
				free(curEp);
			}
		}
		/* constraint */
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "constraint")) && (cur->ns
				== ns)) {
			curCons = parseConstraint(doc, ns, cur);
			if (curCons != NULL) {
				memcpy(GET_CONSTRAINT(ret,ret->nCons), curCons,
						sizeof(constraint));
				ret->nCons++;
				free(curCons);
			}
		}
		/*forwarding*/
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "forwarding")) && (cur->ns
				== ns)) {

			const char* forwarding = (const char*) xmlNodeListGetString(doc,
					cur->xmlChildrenNode, 1);
			if (!strcasecmp(forwarding, "L2"))
				ret->forwarding = LIBNETVIRT_FORWARDING_L2;
			else if (!strcasecmp(forwarding, "L3"))
				ret->forwarding = LIBNETVIRT_FORWARDING_L3;
		}
		cur = cur->next;
	}

	return (ret);
}

void printEndPoint(endpoint* ep) {
	printf("UUID: %ld\n", ep->uuid);
	printf("\t ID: %ld\n", ep->swId);
	printf("\t port: %d\n", ep->port);
	printf("\t vlan: %d\n", ep->vlan);
	printf("\t mpls: %d\n", ep->mpls);
//	if(ep->address)
		//printf("\t network: %s/%d\n", inet_ntoa((struct in_addr*) &ep->address),ep->mask);
}
void printConstraint(constraint* cons) {
	printf("Const Type: %d\n", cons->type);
	printf("\t src id: %ld\n", cons->src);
	printf("\t dst id: %ld\n", cons->dst);
	printf("\t value: %ld\n", cons->value);
}

void printFNS(fnsDesc* cur) {
	int i;
	if (cur->name != NULL)
		printf("name: %s\n", cur->name);
	printf("Uuid: %lu\n", cur->uuid);
	printf("Num Endpoints: %d\n", cur->nEp);
	printf("Num Constraints: %d\n", cur->nCons);
	printf("Forwarding: %d\n", cur->forwarding);
	for (i = 0; i < cur->nEp; i++) {
		printEndPoint((endpoint*) GET_ENDPOINT(cur, i));
	}
	for (i = 0; i < cur->nCons; i++) {
		printConstraint((constraint*) GET_CONSTRAINT(cur, i));
	}
}

fnsDesc* parse_fns_Mem(const char *content, int length) {
	fnsDesc* cur;
	xmlDocPtr doc; /* the resulting document tree */

	/* COMPAT: Do not genrate nodes for formatting spaces */
	LIBXML_TEST_VERSION
	xmlKeepBlanksDefault(0);

	doc = xmlReadMemory(content, length, "noname.xml", NULL, 0);
	if (doc == NULL) {
		fprintf(stderr, "Failed to parse document\n");
		return NULL;
	}
	cur = parseFNSdoc(doc);
	xmlFreeDoc(doc);
	/* Clean up everything else before quitting. */
	xmlCleanupParser();

	return cur;

}

fnsDesc* parse_fns(char* filename) {
	fnsDesc* cur;
	xmlDocPtr doc; /* the resulting document tree */
	/* COMPAT: Do not genrate nodes for formatting spaces */
	LIBXML_TEST_VERSION
	xmlKeepBlanksDefault(0);

	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Failed to parse document\n");
		return NULL;
	}
	cur = parseFNSdoc(doc);
	xmlFreeDoc(doc);
	/* Clean up everything else before quitting. */
	xmlCleanupParser();
	xmlMemoryDump();

	if (!cur)
		fprintf(stderr, "Error parsing file '%s'\n", filename);

	return cur;
}
