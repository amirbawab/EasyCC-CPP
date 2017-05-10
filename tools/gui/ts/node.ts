class node {
    public static TYPE_INITIAL = "INITIAL";
    public static TYPE_NORMAL = "NORMAL";
    public static TYPE_FINAL = "FINAL";
    public id : number;
    public type : string;
    public token : string;
    public backtrack : boolean;
    public edges : edge[];

    private _getLabel() {
        if(this.type == node.TYPE_FINAL) {
            return this.id + " [" + this.token + "] B:" + (this.backtrack ? "yes" : "no");
        }
        return this.id;
    }

    private _getGroup() {
        if(this.type == node.TYPE_INITIAL) {
            return "initial";
        } else if(this.type == node.TYPE_NORMAL) {
            return "normal";
        } else if(this.type == node.TYPE_FINAL) {
            return this.backtrack ? "final_backtrack" : "final";
        }
        return "";
    }

    public object() : any {
        return {
            id: this.id,
            label: this._getLabel(),
            group: this._getGroup(),
            shape: 'box'
        };
    }

    public json() : any {
        let json = {};
        json["type"] = this.type;
        json["id"] = this.id;
        if(this.type == node.TYPE_FINAL) {
            json["token"] = this.token;
            json["backtrack"] = this.backtrack;
        }
        return json;
    }
}