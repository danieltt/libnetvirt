:{
        "kind": {
            "term": "CloNeNode",
            "scheme": "http://schemas.ogf.org/occi/ocni",
            "class": "kind"
        },
        "occi.core.id": "CloNeNodeGreen",
        "occi.core.title": "Clone node for libnetvirt",
        "occi.core.summary": "CloNeNode with libnetvirt mixin created by Daniel",
        "mixins": [
            {
                "term": "libnetvirt",
                "scheme": "http://schemas.ogf.org/occi/ocni",
                "class": "mixin"
            }
        ],
        "links": [],
        "attributes": {
            "ocni.clonenode.availability":[
                {
                    "ocni.availability.start":"08:00",
                    "ocni.availability.end":"12:30"
                },
                {
                    "ocni.availability.start":"14:00",
                    "ocni.availability.end":"18:00"
                }
            ],
            "ocni.clonenode.location": "EU",
            "ocni.clonenode.state": "active",
            "ocni.libnetvirt.uuid" : "1",
            "ocni.libnetvirt.endpoint" : [
		{
	            "ocni.libnetvirt.endpoint.uuid" : "12",
	            "ocni.libnetvirt.endpoint.swid" : "1",
		    "ocni.libnetvirt.endpoint.port" : "2"
		},
		{
		    "ocni.libnetvirt.endpoint.uuid" : "21",
		    "ocni.libnetvirt.endpoint.swid" : "2",
		    "ocni.libnetvirt.endpoint.port" : "2",
		    "ocni.libnetvirt.endpoint.vlan" : "10"
		},
		{
		    "ocni.libnetvirt.endpoint.uuid" : "41",
		    "ocni.libnetvirt.endpoint.swid" : "4",
		    "ocni.libnetvirt.endpoint.port" : "1",
		    "ocni.libnetvirt.endpoint.vlan" : "20"
		}
            ],
            "ocni.libnetvirt.of_controller" : "192.168.56.1",
            "ocni.libnetvirt.of_controller_port" : "2000",
            "ocni.libnetvirt.constraint" : []
        }
}

