export namespace main {
	
	export class HotkeyConfig {
	    keys: string[];
	    action: string;
	    value: any;
	
	    static createFrom(source: any = {}) {
	        return new HotkeyConfig(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.keys = source["keys"];
	        this.action = source["action"];
	        this.value = source["value"];
	    }
	}

}

