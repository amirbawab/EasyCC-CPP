class node {
    public static TYPE_INITIAL = "INITIAL";
    public static TYPE_NORMAL = "NORMAL";
    public static TYPE_FINAL = "FINAL";
    public id : number;
    public type : string;
    public token : string;
    public backtrack : boolean;
    public edges : edge[];

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
            label: this.id,
            group: this._getGroup()
        };
    }
}